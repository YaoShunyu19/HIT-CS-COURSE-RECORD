import numpy as np
from pca import PCA_solve
import matplotlib.pyplot as plt


def Generate_Gaussian_data():
    """
    生成一组高斯分布的数据
    :return: 高斯数据矩阵
    """
    num = 500
    mean = [1, 2, 3]
    cov = [[5, 0, 0], [0, 5, 0], [0, 0, 2.5]]
    data = np.random.multivariate_normal(mean, cov, num)
    return np.mat(data)


def Generate_Spiral_data():
    """
    生成一组螺旋线数据
    :return: 螺旋线数据矩阵
    """
    num = 500
    theta = np.linspace(-5*np.pi, 5*np.pi, num)
    z = np.linspace(-1, 1, num)
    sigma = np.random.normal(loc=0, scale=0.2, size=num)
    z += sigma
    r = 5
    x = r * np.sin(theta)
    y = r * np.cos(theta)
    X = np.mat(x)
    X = np.vstack((X, np.mat(y)))
    X = np.vstack((X, np.mat(z)))
    return X.T


def rotate(X, axis, theta):
    """
    对数据X沿着某一个轴进行旋转一定的角度
    :param X: 数据矩阵
    :param axis: 轴
    :param theta: 旋转角度
    :return: 旋转之后的数据矩阵
    """
    if axis == 'x':
        rotate_matrix = np.mat([[1, 0, 0], [0, np.cos(theta), np.sin(theta)], [0, -np.sin(theta), np.cos(theta)]])
        return X @ rotate_matrix.T
    elif axis == 'y':
        rotate_matrix = np.mat([[np.cos(theta), 0, -np.sin(theta)], [0, 1, 0], [np.sin(theta), 0, np.cos(theta)]])
        return X @ rotate_matrix.T
    elif axis == 'z':
        rotate_matrix = np.mat([[np.cos(theta), np.sin(theta), 0], [-np.sin(theta), np.cos(theta), 0], [0, 0, 1]])
        return X @ rotate_matrix.T
    else:
        print('wrong')
        exit(0)


def show3D(X):
    """
    将数据在三维空间中绘制出来
    :param X: 数据
    :return: 无
    """
    plt.figure(1)
    ax1 = plt.axes(projection='3d')
    ax1.scatter3D(np.array(X)[:, 0], np.array(X)[:, 1], np.array(X)[:, 2], c='b', marker='o', s=49, alpha=0.5)
    ax1.set_xlim(-6, 6)
    ax1.set_ylim(-5, 9)
    ax1.set_zlim(-3, 8)
    ax1.set_xlabel('X label')
    ax1.set_ylabel('Y label')
    ax1.set_zlabel('Z label')


def show2D(Y):
    """
    将数据在二维空间中绘制出来
    :param Y: 数据
    :return: 无
    """
    plt.figure(2)
    plt.scatter(np.array(Y)[:, 0], np.array(Y)[:, 1], c='r', marker='o', s=36, alpha=0.5)


if __name__ == '__main__':
    X = Generate_Gaussian_data()
    # X = Generate_Spiral_data()  # 生成三维的数据
    X = rotate(X, 'y', np.pi / 6)
    Y, B, m = PCA_solve(X, k=2)  # 把三维的数据降到二维
    plt.ion()
    show3D(X)
    show2D(Y)

    plt.ioff()
    plt.show()
