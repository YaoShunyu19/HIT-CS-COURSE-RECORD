import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from k_means import k_means
from GMM_EM import GMM_EM, initialization, initialization_k_means


def use_k_means(X, K):
    """
    使用k-means算法聚类鸢尾花数据集
    :param X: 数据
    :param K: 聚类数目
    :return: 无
    """
    ret_X = k_means(X, K)
    for i in range(len(ret_X)):
        print(ret_X[i].shape[0])


def use_GMM_EM(X, K):
    """
    调用EM算法计算鸢尾花数据集高斯混合模型的各个子模型的参数
    :param X: 数据
    :param K: 聚类数目
    :return: 无
    """
    p, mu, sigma = initialization_k_means(K, X)  # 初始化参数
    p, mu, sigma = GMM_EM(X, p, mu, sigma)  # 使用EM算法计算参数
    print(p)
    print(mu)
    print(sigma)
    plt.show()


if __name__ == '__main__':
    iris = pd.read_csv('iris.csv')
    iris = iris.drop(['class'], axis=1)
    iris = iris.drop(['sepal_length'], axis=1)
    iris = iris.drop(['sepal_width'], axis=1)
    X = np.mat(np.array(iris))
    permutation = np.random.permutation(X.shape[0])
    X = X[permutation, :]
    K = 3
    # use_k_means(X, K)
    use_GMM_EM(X, K)
