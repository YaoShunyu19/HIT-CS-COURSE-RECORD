import numpy as np
import matplotlib.pyplot as plt


def Generate_data(draw=True):
    """
    generate 3 groups of gaussian data
    :param draw: draw the 2D scatter picture
    :return: data
    """
    # 第一组高斯分布数据的参数
    num1 = 50
    mean1 = [3, 0]
    cov1 = [[1.5, 0], [0, 1]]
    data1 = np.random.multivariate_normal(mean1, cov1, num1)
    # 第二组高斯分布数据的参数
    num2 = 75
    mean2 = [-3, 0]
    cov2 = [[1.5, 0], [0, 2]]
    data2 = np.random.multivariate_normal(mean2, cov2, num2)
    # 第三组高斯分布数据的参数
    num3 = 100
    mean3 = [0, 6]
    cov3 = [[2, 1.5], [1.5, 2]]
    data3 = np.random.multivariate_normal(mean3, cov3, num3)
    if draw:
        plt.figure(1)
        # 画出散点图
        plt.scatter(data1[:, 0], data1[:, 1], s=49, c='r', alpha=0.5)
        plt.scatter(data2[:, 0], data2[:, 1], s=49, c='g', alpha=0.5)
        plt.scatter(data3[:, 0], data3[:, 1], s=49, c='b', alpha=0.5)
        plt.grid()
        plt.title('three groups of data')
        plt.savefig('data.jpg')
    X = np.vstack((np.vstack((np.mat(data1), np.mat(data2))), np.mat(data3)))
    permutation = np.random.permutation(X.shape[0])
    shuffled_X = X[permutation, :]  # 将数据顺序打乱
    return shuffled_X


if __name__ == '__main__':
    plt.ion()
    X = Generate_data()
    plt.ioff()
    plt.show()
