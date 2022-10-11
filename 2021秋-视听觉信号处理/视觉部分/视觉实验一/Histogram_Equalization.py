import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt


def Histogram_Equalization(img):
    img = img.copy()
    R, C = img.shape
    nj = np.zeros(shape=256)
    for i in range(R):
        for j in range(C):
            nj[img[i, j]] += 1
    pf = nj / (R * C)
    cf = np.zeros(shape=256)
    for i in range(1, 256):
        cf[i] = cf[i - 1] + pf[i]
    gj = np.zeros(shape=256)
    for i in range(256):
        gj[i] = int(np.floor(255 * cf[i] + 0.5))
    ret_img = img
    for i in range(R):
        for j in range(C):
            ret_img[i, j] = gj[img[i, j]]
    return ret_img


if __name__ == '__main__':
    img = cv.imread('./image/bg.jpg', 0)
    # plt.hist(img.flatten(), 256, [0, 256], color='r')
    he_img = Histogram_Equalization(img)
    # plt.hist(he_img.flatten(), 256, [0, 256], color='r')
    cv.imshow('he_img', he_img)
    cv.waitKey(0)
    cv.destroyAllWindows()
    cv.imwrite('image/he_bg.jpg', he_img)
    # plt.show()
