import struct
import numpy as np


def readmfc(filename):
    file = open(filename, 'rb')
    nframes, frate, nbytes, feakind = struct.unpack('>IIHH', file.read(12))
    ndim = nbytes // 4
    ret = np.empty((nframes, ndim))
    for i in range(nframes):
        for j in range(ndim):
            mf = file.read(4)
            c = struct.unpack('>f', mf)
            ret[i][j] = c[0]
    file.close()
    return ret


def DWT(template, test):
    """
    计算两个序列的DWT距离
    :param template: 模板序列 shape:n*39
    :param test: 测试序列 shape:m*39
    :return: DWT距离
    """
    len_template = template.shape[0]
    len_test = test.shape[0]
    phi = np.empty((len_template + 1, len_test + 1))
    for i in range(1, len_template + 1):
        phi[i][0] = np.inf
    for j in range(1, len_test + 1):
        phi[0][j] = np.inf
    phi[0][0] = 0
    sum_w = 0
    for i in range(1, len_template + 1):
        for j in range(1, len_test + 1):
            d = distance(template[i - 1], test[j - 1])  # 欧氏距离
            a1 = phi[i - 1][j] + d
            a2 = phi[i][j - 1] + d
            a3 = phi[i - 1][j - 1] + 2 * d
            minimum = min(a1, a2, a3)
            phi[i][j] = minimum
            if minimum == a1:
                sum_w += 1
            elif minimum == a2:
                sum_w += 1
            else:
                sum_w += 2
    return phi[len_template][len_test] / sum_w


def distance(a, b):
    """
    计算两个向量的欧氏距离
    :param a: 向量1
    :param b: 向量2
    :return: 欧氏距离
    """
    return np.sqrt(np.sum(np.square(a - b)))


if __name__ == '__main__':
    PATH = 'D:\\2021秋\\视听觉信号处理\\听觉部分\\实验\\命令词识别实验3\\语料mfc\\'
    name = ['唱歌', '后退', '上台阶', '踢球', '右转', '直行', '左转']
    a = PATH + name[0] + '1.mfc'
    b = PATH + name[0] + '2.mfc'
    DWT(readmfc(a), readmfc(b))
    # exit(0)
    count = 0
    corrent = 0
    for c in range(len(name)):
        for i in range(2, 11):
            count += 1
            test_filename = PATH + name[c] + str(i) + '.mfc'
            print('test file: ' + name[c] + str(i) + '.mfc')
            min_dis_name = ''
            min_dis = np.inf
            for t in range(len(name)):
                template_filename = PATH + name[t] + '1.mfc'
                print('    template file: ' + name[t] + '1.mfc', end='')
                dwt_dis = DWT(readmfc(template_filename), readmfc(test_filename))
                print('  距离：' + str(round(dwt_dis, 4)))
                if dwt_dis < min_dis:
                    min_dis_name = name[t]
                    min_dis = dwt_dis
            print('    预测：' + min_dis_name + '    真实：' + name[c])
            if name[c] == min_dis_name:
                corrent += 1
    accuracy = float(corrent) / float(count)
    print('正确个数：' + str(corrent) + '  测试样本总数：' + str(count) + '  准确率：' + str(round(100 * accuracy, 4)) + '%')
