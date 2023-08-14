import numpy as np
import cv2 as cv


def MedianFilter(img, size=(3, 3)):
    img = img.copy()
    r, c = size
    R = img.shape[0]
    C = img.shape[1]
    ret_img = img
    for i in range(r // 2, R - r // 2):
        for j in range(c // 2, C - c // 2):
            k = img[i - r // 2:i + r // 2 + 1, j - c // 2:j + c // 2 + 1]
            ret_img[i, j] = np.median(k)
    return ret_img


def MeanFilter(img, size=(3, 3)):
    img = img.copy()
    r, c = size
    R = img.shape[0]
    C = img.shape[1]
    ret_img = img
    for i in range(r // 2, R - r // 2):
        for j in range(c // 2, C - c // 2):
            k = img[i - r // 2:i + r // 2 + 1, j - c // 2:j + c // 2 + 1]
            ret_img[i, j] = np.mean(k)
    return ret_img


if __name__ == '__main__':
    img = cv.imread('image/psNoiseLena.jpg', 0)
    median_img = MedianFilter(img)
    mean_img = MeanFilter(img)
    cv.imshow('median_lena', median_img)
    cv.imshow('mean_lena', mean_img)
    cv.waitKey(0)
    cv.destroyAllWindows()
    cv.imwrite('./image/psNoise_median_lena.jpg', median_img)
    cv.imwrite('./image/psNoise_mean_lena.jpg', mean_img)
