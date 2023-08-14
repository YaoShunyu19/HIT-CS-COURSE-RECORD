import matplotlib.pyplot as plt
import numpy as np
import random
from GenerateData import Generate_data


# 簇的类
class cluster:
    def __init__(self, x):
        self.center = x  # 簇的中心
        self.X = 0

    def add(self, x):
        self.X = x
        self.center = self.X.mean(axis=0)


def cal_loss(Cluster_set):
    """
    计算损失函数的值
    :param Cluster_set: 簇的集合
    :return: 损失函数的值
    """
    l = 0
    for i in range(len(Cluster_set)):
        center = Cluster_set[i].center
        temp = Cluster_set[i].X - center
        temp = np.multiply(temp, temp)
        l += np.sum(temp)
    return l


def k_means(X, k):
    """
    k-means algorithm
    :param X: data
    :param k: the number of cluster
    :return: 每一个聚类里的数据
    """
    Cluster_set = []
    l = random.sample(range(X.shape[0]), k)
    for i in range(len(l)):
        Cluster_set.append(cluster(X[l[i]]))  # 随机选择k个聚类中心
    loss_list = []
    loss = 100
    new_loss = 200
    epoch_list = []
    epoch = 0
    while abs(new_loss - loss) > 1e-5:  # 当两次迭代的损失函数差距大于1e-5则继续循环
        epoch += 1
        loss = new_loss
        dis = 0
        for i in range(k):  # 计算每个样本到每个聚类中心的距离
            center = Cluster_set[i].center
            temp = X - center
            temp = np.multiply(temp, temp)
            temp = np.sum(temp, axis=1)
            if i == 0:
                dis = temp
            else:
                dis = np.hstack((dis, temp))
        classification = dis.argmin(axis=1)
        cluster_X = [0]*k
        flag_cluster_X = [0]*k
        for i in range(X.shape[0]):  # 将每个样本加入距离最短的聚类内并更新每个聚类中心
            cl = classification[i][0, 0]
            if flag_cluster_X[cl] == 0:
                cluster_X[cl] = X[i]
                flag_cluster_X[cl] = 1
            else:
                cluster_X[cl] = np.vstack((cluster_X[cl], X[i]))
        for i in range(k):
            Cluster_set[i].add(cluster_X[i])
        new_loss = cal_loss(Cluster_set)
        print('epoch: '+str(epoch)+'   loss: '+str(new_loss))
        loss_list.append(new_loss)
        epoch_list.append(epoch)
    plt.figure(2)
    plt.plot(epoch_list, loss_list, c='r')  # 画出迭代轮数-损失函数变化曲线
    plt.title('loss-epoch')
    plt.grid()
    plt.savefig('loss_epoch.jpg')
    X = []
    for i in range(len(Cluster_set)):
        X.append(Cluster_set[i].X)
    return X


if __name__ == '__main__':
    X = Generate_data()
    k = 3
    res = k_means(X, k)
    plt.ion()
    plt.figure(3)
    for i in range(k):
        plt.scatter(np.array(res[i])[:, 0], np.array(res[i])[:, 1], s=49, alpha=0.5)  # 画出k-means算法结果的散点图
    plt.grid()
    plt.title('k-means result')
    plt.savefig('k_means_result.jpg')
    plt.ioff()
    plt.show()
