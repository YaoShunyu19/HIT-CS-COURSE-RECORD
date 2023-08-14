import numpy as np
from pca import PCA_solve
import cv2 as cv


def compression(eta):
    """
    对人脸数据集进行压缩
    :param eta: 保留信息比例
    :return: 无
    """
    img0 = cv.imread('dataset/1.jpg', 0)
    X = np.mat(np.array(img0).flatten())
    for i in range(1, 38):
        file_path = 'dataset/' + str(i + 1) + '.jpg'
        img = cv.imread(file_path, 0)
        X = np.vstack((X, np.mat(np.array(img).flatten())))
    Y, B, m = PCA_solve(X, eta)
    x = Y @ B + m.T
    for i in range(38):
        img = x[i].reshape((48, 42))
        cv.imwrite('dataset_' + str(eta) + '/' + str(i + 1) + '.jpg', img)


def psnr(img1, img2):
    """
    计算峰值信噪比
    :param img1: 原图
    :param img2: 目的图像
    :return: 峰值信噪比
    """
    delta = img1 - img2
    mse = np.mean(np.multiply(delta, delta))
    ret = 20 * np.log10(255 / np.sqrt(mse))
    return ret


def cal_psnr():
    str2 = ''
    str3 = ''
    str4 = ''
    str5 = ''
    for i in range(0, 38):
        img1 = cv.imread('dataset/' + str(i + 1) + '.jpg', 0)
        img2 = cv.imread('dataset_0.95/' + str(i + 1) + '.jpg', 0)
        img3 = cv.imread('dataset_0.9/' + str(i + 1) + '.jpg', 0)
        img4 = cv.imread('dataset_0.85/' + str(i + 1) + '.jpg', 0)
        img5 = cv.imread('dataset_0.6/' + str(i + 1) + '.jpg', 0)
        ret2 = round(psnr(img1, img2), 4)
        ret3 = round(psnr(img1, img3), 4)
        ret4 = round(psnr(img1, img4), 4)
        ret5 = round(psnr(img1, img5), 4)
        str2 += str(i + 1) + '.jpg:  ' + str(ret2) + '\n'
        str3 += str(i + 1) + '.jpg:  ' + str(ret3) + '\n'
        str4 += str(i + 1) + '.jpg:  ' + str(ret4) + '\n'
        str5 += str(i + 1) + '.jpg:  ' + str(ret5) + '\n'

    f2 = open('dataset_0.95/psnr.txt', 'w')
    f2.write(str2)
    f3 = open('dataset_0.9/psnr.txt', 'w')
    f3.write(str3)
    f4 = open('dataset_0.85/psnr.txt', 'w')
    f4.write(str4)
    f5 = open('dataset_0.6/psnr.txt', 'w')
    f5.write(str5)


if __name__ == '__main__':
    compression(0.6)
    cal_psnr()
