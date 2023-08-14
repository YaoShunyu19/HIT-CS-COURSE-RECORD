import extmem
from extmem import disk_dir, tuple_num_per_blk, tuple_num_R, tuple_num_S, Buffer
from math import ceil, floor

numAllBlk = 8
select_dir = './disk/select/'
project_dir = './disk/project/'
nlj_dir = './disk/join/nlj/'
hash_temp_dir, hash_res_dir = './disk/join/hash/hash/', './disk/join/hash/res/'
sort_temp_dir, sort_res_dir = './disk/join/sort/sort/', './disk/join/sort/res/'


def select(buffer):
    extmem.dropAllBlock(select_dir)
    items = [('r', tuple_num_R // tuple_num_per_blk, 40), ('s', tuple_num_S // tuple_num_per_blk, 60)]
    buffer.numIO, count, res = 0, 0, []
    for item in items:
        for disk_idx in range(item[1]):  # item[1]表示关系占用的物理磁盘块数
            index = buffer.readBlockFromDisk('%s%s%d.blk' % (disk_dir, item[0], disk_idx))
            for data in buffer.data[index]:
                data1, data2 = data.split()
                if int(data1) == item[2]:
                    res.append(data)
                    if len(res) == tuple_num_per_blk:  # 到达block内容上限，写入磁盘
                        buffer.writeDataToBlockToDisk(res, '%s%s%d.blk' % (select_dir, item[0], count))
                        res, count = [], count + 1
            buffer.freeBlockInBuffer(0)
        if res:  # 将buffer内剩余的数据写入磁盘
            buffer.writeDataToBlockToDisk(res, '%s%s%d.blk' % (select_dir, item[0], count))
        res, count = [], 0


def project(buffer):
    extmem.dropAllBlock(project_dir)
    buffer.numIO, count, res = 0, 0, []
    all_res = set()  # 用于去重
    for disk_idx in range(tuple_num_R // tuple_num_per_blk):
        index = buffer.readBlockFromDisk('%sr%d.blk' % (disk_dir, disk_idx))  # 加载磁盘块内容到缓冲区中
        for data in buffer.data[index]:
            if data.split()[0] not in all_res:
                res.append(data.split()[0])
                all_res.add(data.split()[0])
                if len(res) == tuple_num_per_blk * 2:  # 每个元组只保存4字节，故每个block的容量是2倍的tuple数量
                    buffer.writeDataToBlockToDisk(res, '%sr%d.blk' % (project_dir, count))
                    res, count = [], count + 1
        buffer.freeBlockInBuffer(0)
    if res:  # 将buffer内剩余的数据写入磁盘
        buffer.writeDataToBlockToDisk(res, '%sr%d.blk' % (project_dir, count))


def nested_loop_join(buffer):
    extmem.dropAllBlock(nlj_dir)
    buffer.numIO, count, res = 0, 0, []
    # R-S二重循环
    for outer_idx in range(ceil((tuple_num_R // tuple_num_per_blk) / (tuple_num_per_blk - 2))):  # 外层：R
        # R的数据最多占用6个块，一个块给S，一个块来写入
        start = outer_idx * (tuple_num_per_blk - 2)
        end = min((outer_idx + 1) * (tuple_num_per_blk - 2), tuple_num_R // tuple_num_per_blk)
        outer_data = [buffer.data[buffer.readBlockFromDisk('%sr%d.blk' % (disk_dir, idx))] for idx in range(start, end)]
        # 把R在磁盘中的数据写入buffer

        for inner_idx in range(tuple_num_S // tuple_num_per_blk):  # 内层：S
            inner_data = buffer.data[buffer.readBlockFromDisk('%ss%d.blk' % (disk_dir, inner_idx))]

            for outer_lst in outer_data:  # 内存中执行连接操作
                for outer_item in outer_lst:
                    r_a, r_b = outer_item.split()
                    for inner_item in inner_data:
                        s_c, s_d = inner_item.split()
                        if r_a == s_c:
                            res.append('%s %s' % (outer_item, inner_item))
                            if len(res) == int(tuple_num_per_blk / 2):  # 每个block只能保存3行数据
                                buffer.writeDataToBlockToDisk(res, '%srs%d.blk' % (nlj_dir, count))
                                res, count = [], count + 1
            buffer.freeBlockInBuffer(len(outer_data))
        buffer.data_occupy = [False] * tuple_num_per_blk
    if res:  # 将buffer内剩余的数据写入磁盘
        buffer.writeDataToBlockToDisk(res, '%srs%d.blk' % (nlj_dir, count))


def hash_join(buffer):  # 划分数据桶：划分为B(buffer)-1个数据桶，缓冲区的B(buffer)-1块用作数据桶暂存，1块用作数据输入
    # R.A是7的倍数放一组，%7=1放一组。。。S.C同理
    buffer.__init__(numAllBlk)
    extmem.dropAllBlock(hash_temp_dir)
    # 对关系R进行hash操作，将缓存中的numAllBlk-1块作为Hash桶，1块作为输出
    hash_num = numAllBlk - 1
    all_data = [('r', tuple_num_R // tuple_num_per_blk, [[] for _ in range(hash_num)]),  # [2]保存addr
                ('s', tuple_num_S // tuple_num_per_blk, [[] for _ in range(hash_num)])]
    hash_blk = [[] for _ in range(hash_num)]
    # 构建hash表，写入磁盘
    for item in all_data:  # R和S
        for idx in range(item[1]):  # 每个磁盘块的数据
            blk_data = buffer.data[buffer.readBlockFromDisk('%s%s%d.blk' % (disk_dir, item[0], idx))]
            for data in blk_data:  # 某一个磁盘块中的每个元组
                hash_idx = int(data.split()[0]) % hash_num  # hash函数：取模
                hash_blk[hash_idx].append(data)
                if len(hash_blk[hash_idx]) == tuple_num_per_blk:  # hash桶已满，需要输出
                    addr = '%s%s%d_%d.blk' % (hash_temp_dir, item[0], hash_idx, len(item[2][hash_idx]))
                    buffer.writeDataToBlockToDisk(hash_blk[hash_idx], addr)
                    item[2][hash_idx].append(addr)
                    hash_blk[hash_idx] = []
            buffer.freeBlockInBuffer(0)  # 用缓冲区的一个快来读数据，每遍历一个磁盘块都要清空一次
        for idx in range(hash_num):  # 把剩下没满的块写入磁盘
            if hash_blk[idx]:
                addr = '%s%s%d_%d.blk' % (hash_temp_dir, item[0], idx, len(item[2][idx]))
                buffer.writeDataToBlockToDisk(hash_blk[idx], addr)
                item[2][idx].append(addr)
                hash_blk[idx] = []

    # 进行连接操作，将缓存中的numAllBlk-2块保存第i个桶的内容，1块作为输出，1块保存另外1个关系
    # 循环B(buffer)-1次，对每个桶执行连接操作；将缓冲区中的B(buffer-2)块作为关系R的输入块，1块作为关系S的输入块，1块作为输出块
    res, count, buffer.data_occupy = [], 0, [False] * numAllBlk
    extmem.dropAllBlock(hash_res_dir)
    for idx in range(hash_num):
        r_buffer_data, s_buffer_data, flag = [], [], False
        for addr in all_data[0][2][idx]:
            r_buffer_data.extend(buffer.data[buffer.readBlockFromDisk(addr)])
        for addr in all_data[1][2][idx]:  # 将S的第i个hash桶内容逐个加入
            s_buffer_data = buffer.data[buffer.readBlockFromDisk(addr)]
            for r_data in r_buffer_data:
                for s_data in s_buffer_data:
                    if r_data.split()[0] == s_data.split()[0]:  # 连接判断
                        res.append('%s %s' % (r_data, s_data))
                        if len(res) == int(tuple_num_per_blk / 2):
                            buffer.writeDataToBlockToDisk(res, '%srs%d.blk' % (hash_res_dir, count))
                            res, count = [], count + 1
            buffer.freeBlockInBuffer(ceil(len(r_buffer_data) / tuple_num_per_blk))
        buffer.data_occupy = [False] * numAllBlk
    if res:  # 将buffer内剩余的数据写入磁盘
        buffer.writeDataToBlockToDisk(res, '%srs%d.blk' % (hash_res_dir, count))  # 将结果磁盘上的剩余数据写入磁盘


def sort_merge_join(buffer):
    res, buffer.numIO = [], 0
    all_data = [('r', tuple_num_R // tuple_num_per_blk), ('s', tuple_num_S // tuple_num_per_blk)]
    for data in all_data:
        num = floor(data[1] / buffer.numAllBlk) + 1  # 将待排序磁盘块划分为num个集合  3
        # 块内排序：缓冲区B(buffer)-1块作为数据输入，1块作为数据输出；故写入磁盘的结果中每B(buffer)-1块是有序的
        # 结果保存在./disk/relation中，r0-r6有序  r7-r13有序  r14-r15有序，整体无序。S同理
        for idx in range(num):  # 缓冲区的7块放置待排序数据，1块放置排序输出数据
            stop, blk_data = ((idx + 1) * (buffer.numAllBlk - 1)), []
            for idy in range(idx * (buffer.numAllBlk - 1), stop if stop < data[1] else data[1]):
                # 加载磁盘块内容到缓冲区中
                blk_data.extend(buffer.data[buffer.readBlockFromDisk('%s%s%d.blk' % (disk_dir, data[0], idy))])
            blk_data = sorted(blk_data, key=lambda item1: int(item1.split()[0]))
            for idy in range(int(len(blk_data) / tuple_num_per_blk)):
                buffer.writeDataToBlockToDisk(blk_data[idy * tuple_num_per_blk:(idy + 1) * tuple_num_per_blk],
                                              '%s%s%d.blk' % (disk_dir, data[0], idx * tuple_num_per_blk + idy))
                buffer.freeBlockInBuffer(idy)

        # 块间排序:缓冲区1块作为整体有序数据输出块，其余块作为输入块，依次取有序磁盘块中的1块，不断选取其中最小的元组
        count, blk_data, sorted_blk = 0, [[]] * num, []  # count表示已写入磁盘块数
        idx_lst = [idx * (buffer.numAllBlk - 1) for idx in range(num)]  # 保存num个索引，用于指向磁盘所在位置
        while True:
            for idx, item in enumerate(blk_data):
                if not item:
                    buffer.freeBlockInBuffer(idx)
                    if idx_lst[idx] < min((idx + 1) * (buffer.numAllBlk - 1), data[1]):  # 缓冲区待归并数据已被取空
                        blk_data[idx] = buffer.data[
                            buffer.readBlockFromDisk('%s%s%d.blk' % (disk_dir, data[0], idx_lst[idx]))]
                        idx_lst[idx] += 1
            flag = True if len(list(filter(None, blk_data))) else False  # 有剩余数据则为True
            if count == data[1] and not flag:  # 数据已经遍历完毕且缓冲区中无剩余数据
                break
            elif flag:  # 缓冲区中有剩余数据
                index, digit = 0, 1e5  # 找到最小的一个元素
                for idx in range(num):
                    if blk_data[idx] and int(blk_data[idx][0].split()[0]) < digit:
                        index, digit = idx, int(blk_data[idx][0].split()[0])
                sorted_blk.append(blk_data[index][0])  # 加入到输出缓冲区
                blk_data[index].pop(0)
                if len(sorted_blk) == tuple_num_per_blk:
                    buffer.writeDataToBlockToDisk(sorted_blk, '%s%s%d.blk' % (sort_temp_dir, data[0], count))
                    count, sorted_blk = count + 1, []

    # 执行连接算法:置2个游标不断滑动，若从R中取到的数据较小，则滑动R的游标；若较大，则滑动S的游标；
    # 否则，输出该结果，并生成2个临时游标，临时滑动关系R或S的数据直到两者数据不相同。最终原游标分别滑动一个元素
    extmem.dropAllBlock(sort_res_dir)  # 删除文件夹下的所有模拟磁盘文件
    r_idx, s_idx, count, res = 0, 0, 0, []
    r_data = buffer.data[buffer.readBlockFromDisk('%sr0.blk' % sort_temp_dir)]
    s_data = buffer.data[buffer.readBlockFromDisk('%ss0.blk' % sort_temp_dir)]
    while r_idx < tuple_num_R // tuple_num_per_blk * tuple_num_per_blk \
            and s_idx < tuple_num_S // tuple_num_per_blk * tuple_num_per_blk:
        data1 = int(r_data[r_idx % tuple_num_per_blk].split()[0])
        data2 = int(s_data[s_idx % tuple_num_per_blk].split()[0])
        if data1 == data2:  # 先记录原位置，然后向右滑动
            res.append('%s %s' % (r_data[r_idx % tuple_num_per_blk], s_data[s_idx % tuple_num_per_blk]))
            if len(res) == floor(tuple_num_per_blk / 2):  # 结果缓冲区块已满
                buffer.writeDataToBlockToDisk(res, '%srs%d.blk' % (sort_res_dir, count))
                res, count = [], count + 1
            idx_temp = s_idx + 1  # S变量临时向右滑动
            while idx_temp < tuple_num_S // tuple_num_per_blk * tuple_num_per_blk:
                if not idx_temp % tuple_num_per_blk:  # 扫描到结尾，更新s_data
                    buffer.freeBlockInBuffer(1)
                    s_data = buffer.data[
                        buffer.readBlockFromDisk('%ss%d.blk' % (sort_temp_dir, floor(idx_temp / tuple_num_per_blk)))]
                if data1 == int(s_data[idx_temp % tuple_num_per_blk].split()[0]):  # 如果相等则继续右滑
                    res.append('%s %s' % (r_data[r_idx % tuple_num_per_blk], s_data[idx_temp % tuple_num_per_blk]))
                    idx_temp += 1  # 继续滑动
                    if len(res) == int(tuple_num_per_blk / 2):  # 结果缓冲区块已满
                        buffer.writeDataToBlockToDisk(res, '%srs%d.blk' % (sort_res_dir, count))
                        res, count = [], count + 1
                else:  # 不相等就退出循环
                    break
            if floor(idx_temp / tuple_num_per_blk) > floor(s_idx / tuple_num_per_blk):  # 如果关系S临时滑动到了新的一块
                buffer.freeBlockInBuffer(1)
                s_data = buffer.data[
                    buffer.readBlockFromDisk('%ss%d.blk' % (sort_temp_dir, floor(s_idx / tuple_num_per_blk)))]
            idx_temp = r_idx + 1  # 关系R临时向右滑动
            while idx_temp < tuple_num_R // tuple_num_per_blk * tuple_num_per_blk:
                if not idx_temp % tuple_num_per_blk:
                    buffer.freeBlockInBuffer(0)
                    r_data = buffer.data[
                        buffer.readBlockFromDisk('%sr%d.blk' % (sort_temp_dir, floor(idx_temp / tuple_num_per_blk)))]
                if int(r_data[idx_temp % tuple_num_per_blk].split()[0]) == data2:
                    res.append('%s %s' % (r_data[idx_temp % tuple_num_per_blk], s_data[s_idx % tuple_num_per_blk]))
                    idx_temp += 1
                    if len(res) == int(tuple_num_per_blk / 2):
                        buffer.writeDataToBlockToDisk(res, '%srs%d.blk' % (sort_res_dir, count))
                        res, count = [], count + 1
                else:
                    break
            if floor(idx_temp / tuple_num_per_blk) > floor(r_idx / tuple_num_per_blk):  # 如果关系R临时滑动到了新的一块
                buffer.freeBlockInBuffer(0)
                r_data = buffer.data[
                    buffer.readBlockFromDisk('%sr%d.blk' % (sort_temp_dir, floor(r_idx / tuple_num_per_blk)))]
            r_idx, s_idx = r_idx + 1, s_idx + 1  # R和S向右滑动
            # 滑动到新的一块
            if not r_idx % tuple_num_per_blk and r_idx < tuple_num_R // tuple_num_per_blk * tuple_num_per_blk:
                buffer.freeBlockInBuffer(0)
                r_data = buffer.data[
                    buffer.readBlockFromDisk('%sr%d.blk' % (sort_temp_dir, floor(r_idx / tuple_num_per_blk)))]
            if not s_idx % tuple_num_per_blk and s_idx < tuple_num_S // tuple_num_per_blk * tuple_num_per_blk:
                buffer.freeBlockInBuffer(1)
                s_data = buffer.data[
                    buffer.readBlockFromDisk('%ss%d.blk' % (sort_temp_dir, floor(s_idx / tuple_num_per_blk)))]
        elif data1 > data2:
            s_idx += 1
            if not s_idx % tuple_num_per_blk and s_idx < tuple_num_S // tuple_num_per_blk * tuple_num_per_blk:
                buffer.freeBlockInBuffer(1)
                s_data = buffer.data[
                    buffer.readBlockFromDisk('%ss%d.blk' % (sort_temp_dir, floor(s_idx / tuple_num_per_blk)))]
        else:  # data2 > data1
            r_idx += 1
            if not r_idx % tuple_num_per_blk and r_idx < tuple_num_R // tuple_num_per_blk * tuple_num_per_blk:
                buffer.freeBlockInBuffer(0)
                r_data = buffer.data[
                    buffer.readBlockFromDisk('%sr%d.blk' % (sort_temp_dir, floor(r_idx / tuple_num_per_blk)))]
    if res:
        buffer.writeDataToBlockToDisk(res, '%srs%d.blk' % (sort_res_dir, count))  # 将结果磁盘上的剩余数据写入磁盘


if __name__ == '__main__':
    buffer = Buffer(numAllBlk)
    select(buffer)
    # print('关系选择的磁盘IO次数为：%d' % buffer.numIO)
    project(buffer)
    # print('关系投影的磁盘IO次数为：%d' % buffer.numIO)
    nested_loop_join(buffer)
    # print('nest-loop-join算法的磁盘IO次数为：%d' % buffer.numIO)
    hash_join(buffer)
    # print('hash-join算法的磁盘IO次数为：%d' % buffer.numIO)
    sort_merge_join(buffer)
    # print('sort-merge-join算法的磁盘IO次数为：%d' % buffer.numIO)
