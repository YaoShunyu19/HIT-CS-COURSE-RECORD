import numpy as np
import cv2 as cv


def distance(x1, y1, x2, y2):
    x1 = int(x1)
    y1 = int(y1)
    x2 = int(x2)
    y2 = int(y2)
    dis = ((x1 - x2) ** 2 + (y1 - y2) ** 2) ** 0.5
    return dis


def detect_fruit(img, minRadius, maxRadius, delta=50, width=7):
    circles = cv.HoughCircles(img, cv.HOUGH_GRADIENT, 1, 20,
                              param1=50, param2=30, minRadius=minRadius, maxRadius=maxRadius)
    circles = np.uint16(np.around(circles))

    circle_center = []
    center_copy = []

    for i in circles[0, :]:
        circle_center.append((i[0], i[1]))
        center_copy.append((i[0], i[1]))

    for i in range(len(circle_center)):
        for j in range(i + 1, len(circle_center)):
            x1, y1 = circle_center[i][0], circle_center[i][1]
            x2, y2 = circle_center[j][0], circle_center[j][1]
            if distance(x1, y1, x2, y2) < delta:
                center_copy.remove((circle_center[j][0], circle_center[j][1]))

    for i in range(len(center_copy)):
        cv.circle(img, center_copy[i], maxRadius, (0, 0, 0), width)

    return img, len(center_copy)


if __name__ == '__main__':
    img = cv.imread('tangerine&dates.jpg', 0)
    img1, orange_num = detect_fruit(img.copy(), 90, 100)
    img2, jujube_num = detect_fruit(img1.copy(), 40, 50)
    print('orange number: ' + str(orange_num))
    print('jujube number: ' + str(jujube_num))
    cv.imshow('img', img)
    cv.imshow('img1', img1)
    cv.imshow('img2', img2)
    cv.imwrite('orange.jpg', img1)
    cv.imwrite('orange_jujube.jpg', img2)
    denoise_img = cv.fastNlMeansDenoising(img, None, 10, 7, 21)
    cv.imshow('denoise', denoise_img)
    cv.imwrite('denoise_img.jpg', denoise_img)
    canny_img = cv.Canny(denoise_img, 100, 200)
    cv.imshow('canny_img', canny_img)
    cv.imwrite('canny_img.jpg', canny_img)
    cv.waitKey(0)
    cv.destroyAllWindows()
