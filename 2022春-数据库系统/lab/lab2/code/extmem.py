import os
from random import randint

disk_dir = 'disk/relation/'
tuple_num_per_blk = 7
tuple_num_R = 112
tuple_num_S = 224


class Buffer:
    def __init__(self, numAllBlk=8):
        self.numIO = 0  # IO次数
        self.numAllBlk = numAllBlk  # buffer最多存放8个block
        self.numFreeBlk = numAllBlk  # 当前空闲的block数
        self.data_occupy = [False] * self.numAllBlk  # 记录每个block是否空闲
        self.data = [[]] * self.numAllBlk  # 数据

    def getNewBlockInBuffer(self):  # 从buffer中返回一个空闲块
        for idx, flag in enumerate(self.data_occupy):
            if not flag:
                self.data_occupy[idx] = True
                self.numFreeBlk -= 1
                return idx
        return -1

    def freeBlockInBuffer(self, index):  # 释放buffer中某一个block的空间
        flag = self.data_occupy[index]
        if flag:
            self.numFreeBlk += 1
            self.data_occupy[index] = False
        return flag

    def readBlockFromDisk(self, path):  # 从磁盘中读入一个块写入buffer
        index = self.getNewBlockInBuffer()
        if index != -1:
            with open(path) as f:
                self.data_occupy[index] = True
                self.data[index] = f.read().split('\n')
                self.numIO += 1
        return index

    def writeBlockToDisk(self, index, path):  # 将buffer中的某一个块的内容写入磁盘
        with open(path, 'w') as f:
            self.numIO += 1
            self.numFreeBlk += 1
            self.data_occupy[index] = False
            f.write('\n'.join(self.data[index]))
            return True

    def writeDataToBlockToDisk(self, data, path):  # 将cpu处理完的数据写入磁盘
        index = self.getNewBlockInBuffer()
        if index != -1:
            self.data[index] = data
            self.writeBlockToDisk(index, path)
            return True
        return False


def dropBlockOnDisk(path):  # 删除磁盘中的所有块
    if os.path.exists(path):
        os.remove(path)
        return True
    return False


def dropAllBlock(BlockDir):  # 删除块目录中的所有块
    for f in os.listdir(BlockDir):
        os.remove(BlockDir + f)


def generateData():  # 生成数据
    dropAllBlock(disk_dir)
    dataR, dataS = set(), set()
    while len(dataR) < tuple_num_R:
        dataR.add((randint(1, 40), randint(1, 1000)))
    while len(dataS) < tuple_num_S:
        dataS.add((randint(20, 60), randint(1, 1000)))
    return list(dataR), list(dataS)


def writeDataToDisk(dataR, dataS):  # 将数据写入磁盘
    all_data = [('r', tuple_num_R // tuple_num_per_blk, dataR), ('s', tuple_num_S // tuple_num_per_blk, dataS)]
    for data in all_data:
        for idx in range(data[1]):
            with open('%s%s%d.blk' % (disk_dir, data[0], idx), 'w') as f:
                blk_data = ['%d %d' % item for item in data[2][idx * tuple_num_per_blk:(idx + 1) * tuple_num_per_blk]]
                f.write('\n'.join(blk_data))


if __name__ == '__main__':
    dataR, dataS = generateData()
    writeDataToDisk(dataR, dataS)
