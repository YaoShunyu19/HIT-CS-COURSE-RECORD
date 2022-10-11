import numpy as np
import cv2 as cv

if __name__ == '__main__':
    # 读取
    img = cv.imread('image/lena256color.jpg', 0)
    # 显示
    cv.imshow('lena', img)
    cv.waitKey(0)
    cv.destroyAllWindows()
    # 保存
    cv.imwrite('image/new_save.jpg', img)
