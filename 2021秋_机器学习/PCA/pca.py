import numpy as np


def cal_cov_matrix(X):
    """
    计算X的协方差矩阵
    :param X: 样本矩阵N*M，N个样本，每个样本M维
    :return: X的协方差矩阵
    """
    n = X.shape[0]
    H = np.mat(np.identity(n)) - np.multiply(1 / n, np.mat(np.ones(n)).T @ np.mat(np.ones(n)))
    return np.multiply(1 / n, X.T @ H @ X)


def PCA_solve(X, eta=float(1), k=0):
    """
    PCA求解
    :param X: 样本矩阵
    :param eta: 保留信息比例
    :param k: 保留特征值数
    :return: 降维后中心化的样本 （N*降过之后的维度），变换矩阵，原样本均值
    """
    mean = np.multiply(1 / X.shape[0], X.T @ np.mat(np.ones(X.shape[0])).T)
    X = X - mean.T
    S = cal_cov_matrix(X)
    eigval, eigvec = np.linalg.eig(S)
    sorted_indices = np.argsort(eigval)
    eigvec = eigvec[:, sorted_indices[:-X.shape[1] - 1:-1]]
    eigval = sorted(eigval, reverse=True)
    cnt = 0
    sum_eigval = np.sum(np.array(eigval))
    pre_sum = 0
    if 1 > eta > 0 and k == 0:
        for i in range(len(eigval)):
            pre_sum += eigval[i]
            if pre_sum / sum_eigval > eta:
                cnt = i + 1
                break
    pre_sum = 0
    if k > 0:
        cnt = k
    for i in range(cnt):
        pre_sum += eigval[i]
    print('维度：'+str(cnt))
    print('信息保留百分比：' + str(np.real(pre_sum / sum_eigval)))
    B = eigvec.T[0]
    for i in range(1, cnt):
        B = np.vstack((B, eigvec.T[i]))
    B = np.real(B)
    return X @ B.T, B, mean
