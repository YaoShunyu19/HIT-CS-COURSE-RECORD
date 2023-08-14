import numpy as np
import matplotlib.pyplot as plt
from GenerateData import Generate_data
from scipy.stats import multivariate_normal
from k_means import k_means


def cal_Pz(X, p, mu, sigma):
    """
    计算p(zi=ck|xi,theta(t))的N*K矩阵
    :param X: 样本
    :param p: 各个隐变量的概率
    :param mu: 各个高斯分布的均值
    :param sigma: 各个高斯分布的协方差矩阵
    :return: p(zi=ck|xi,theta(t))的N*K矩阵
    """
    K = len(p)
    N = X.shape[0]
    P = np.mat(np.zeros(shape=(N, K)))
    for i in range(N):
        Denominator = 0
        for k in range(K):
            Denominator += p[k] * multivariate_normal.pdf(X[i], np.array(mu[k])[0], sigma[k], allow_singular=True)
        for k in range(K):
            P[i, k] = p[k] * multivariate_normal.pdf(X[i], np.array(mu[k])[0], sigma[k], allow_singular=True) / Denominator
    return P


def cal_Q(X, new_p, new_mu, new_sigma, p, mu, sigma):
    """
    计算Q(theta,theta(t))函数的值
    :param X: 数据
    :param new_p: 新的各个隐变量的概率
    :param new_mu: 新的各个高斯分布的均值
    :param new_sigma: 新的各个高斯分布的协方差矩阵
    :param p: 各个隐变量的概率
    :param mu: 各个高斯分布的均值
    :param sigma: 各个高斯分布的协方差矩阵
    :return: Q函数的值
    """
    K = len(new_p)
    N = X.shape[0]
    Q = 0
    P = cal_Pz(X, p, mu, sigma)
    for k in range(K):
        for i in range(N):
            Q += (np.log(new_p[k] + 1e-5) +
                  np.log(multivariate_normal.pdf(X[i], np.array(new_mu[k])[0], new_sigma[k], allow_singular=True) + 1e-5)) * P[i, k]
    return Q


def initialization(K, X):
    """
    随机初始化各个参数
    :param K: 聚类数目
    :param X: 样本
    :return: 随机初始化的参数
    """
    d = X.shape[1]
    p = np.random.random(K)
    psum = np.sum(p)
    p /= psum
    mu = []
    sigma = []
    for k in range(K):
        mu.append(np.mat(np.random.randn(d)))
        r = np.random.random(d)
        sigma.append(np.mat(np.diag(r)))
    return p, mu, sigma


def initialization_k_means(K, X):
    """
    使用k_means算法初始化参数
    :param K: 聚类数目
    :param X: 样本
    :return: 初始化的参数
    """
    X1 = k_means(X, K)
    N = X.shape[0]
    p = []
    mu = []
    sigma = []
    for i in range(K):
        p.append(X1[i].shape[0] / N)
        mu.append(np.mean(X1[i], axis=0))
        sigma.append(np.cov(X1[i].T))
    return p, mu, sigma


def GMM_EM(X, p, mu, sigma, max_epoch=100):
    """
    使用GM算法计算高斯混合模型的各个子高斯分布的参数
    :param max_epoch: 最大迭代轮数
    :param X: 数据
    :param p: 各个隐变量的概率初值
    :param mu: 各个高斯分布的均值初值
    :param sigma: 各个高斯分布的协方差初值
    :return: 高斯混合模型的各个子高斯分布的参数
    """
    Q = 0
    new_Q = 1
    N = X.shape[0]
    K = len(p)
    epoch = 0
    epoch_list = []
    Q_list = []
    new_p = []
    new_mu = []
    new_sigma = []
    while abs(new_Q - Q) > 1e-5 and epoch < max_epoch:  # 当两次迭代的Q值差距大于1e-5时继续迭代
        epoch += 1
        Q = new_Q
        new_p = []
        new_mu = []
        new_sigma = []
        P = cal_Pz(X, p, mu, sigma)
        for k in range(K):  # 根据各个参数的迭代公式计算下一轮的参数值
            temp = P.sum(axis=0)[0, k]
            new_p.append(temp / N)
            new_mu.append(np.multiply(1 / temp, P[:, k].T @ X))
            Numerator = 0
            for i in range(N):
                Numerator += np.multiply(P[i, k], (X[i] - mu[k]).T @ (X[i] - mu[k]))
            new_sigma.append(np.multiply(1 / temp, Numerator))
        new_Q = cal_Q(X, new_p, new_mu, new_sigma, p, mu, sigma)  # 计算新的Q值
        print(new_Q)
        epoch_list.append(epoch)
        Q_list.append(new_Q)
        p = new_p
        mu = new_mu
        sigma = new_sigma
    plt.plot(epoch_list, Q_list)  # 画出迭代轮数-Q函数值的变化曲线图
    return new_p, new_mu, new_sigma


if __name__ == '__main__':
    X = Generate_data()
    K = 3
    # p, mu, sigma = initialization(K, X)  # 初始化参数
    p, mu, sigma = initialization_k_means(K, X)  # 使用k-means算法初始化参数
    p, mu, sigma = GMM_EM(X, p, mu, sigma)  # 调用EM算法计算参数
    print(p)
    print(mu)
    print(sigma)
    plt.show()
