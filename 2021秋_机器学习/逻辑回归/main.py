import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# the parameters
# 第一类数据的生成参数
num1 = 100
mean1 = [0, 0]
cov1 = [[1, 0.5], [0.5, 1]]
# 第二类数据的生成参数
num2 = 100
mean2 = [3, 3]
cov2 = [[1, 0.5], [0.5, 1]]
start = -2
end = 5

epoch = 1000
learning_rate = 0.000001  # 学习率
penalty_rate = 0.001  # 惩罚率
loss_list = []
epoch_list = []


def Generate_data(draw=True):
    """
    generate two types of data that obey Gaussian distribution
    :return:
    """
    data1 = np.random.multivariate_normal(mean1, cov1, num1)  # 生成高维高斯分布数据
    data1 = np.c_[data1, np.ones((num1, 1))]
    y1 = np.array([1] * num1)
    y1 = y1.reshape((num1, -1))
    data2 = np.random.multivariate_normal(mean2, cov2, num2)
    data2 = np.c_[data2, np.ones((num2, 1))]
    y2 = np.array([0] * num2)
    y2 = y2.reshape((num2, -1))
    if draw:  # 画出样本散点图
        plt.scatter(data1[:, 0], data1[:, 1], c='r')
        plt.scatter(data2[:, 0], data2[:, 1], c='g')
    # plt.grid()
    # plt.title('data')
    # plt.savefig('data.jpg')
    # plt.show()
    # exit(0)
    X = np.mat(np.vstack((data1, data2)))
    Y = np.mat(np.vstack((y1, y2)))
    return X, Y


def sigmoid(z):
    """
    sigmoid function
    :param z: input
    :return: sigmoid(z)
    """
    # sigmoid函数，当自变量过大或过小时，直接返回结果
    if z > 10:
        return 1 - 1e-5
    elif z < -10:
        return 1e-5
    return 1 / (1 + np.exp(- z))


def loss(X, Y, w, pr=float(0)):
    """
    calculate the loss function
    :param X: X
    :param Y: Y
    :param w: the parameters
    :param pr: penalty rate
    :return: the loss
    """
    res = 0
    for i in range(X.shape[0]):
        yi = np.array(Y[i]).flatten()[0]
        sig = np.array(sigmoid(w.T @ X[i].T)).flatten()[0]
        if yi == 1:
            res += - np.log(sig)
        else:
            res += - np.log(1 - sig)
    res += pr * np.array(w.T @ w).flatten()[0]
    return res


def grad(w, X, Y, pr):  # 计算梯度
    """
    calculate the gradient
    :param w: the parameters
    :param X: X
    :param Y: Y
    :param pr: penalty rate
    :return: the gradient matrix
    """
    sig = 1 / (1 + np.exp(- X @ w))
    g = X.T @ (sig - Y)
    return g + np.multiply(2 * pr, w)


def Hessian(w, X, pr):  # 计算海森矩阵
    """
    calculate the Hessian matrix
    :param w: the parameters
    :param X: X
    :param pr: penalty rate
    :return: Hessian matrix
    """
    Z = X @ w
    expZ = np.exp(Z)
    p = np.mat(np.ones((expZ.shape[0], expZ.shape[1]))) + expZ
    p = np.multiply(p, p)
    diff_sig = expZ / p
    diag = np.mat(np.identity(X.shape[0]))
    for j in range(0, diff_sig.shape[0]):
        diag[j, j] = diff_sig[j, 0]
    H = X.T @ diag @ X
    return H + np.multiply(2 * pr, w)


def Gradient_descent(X, Y, epoch=10000, lr=0.003, pr=0.001):
    """
    the gradient descent optimization
    :param X: X
    :param Y: Y
    :param epoch: epoch
    :param lr: learning rate
    :param pr: penalty rate
    :return: the parameters this algorithm learned
    """
    w = np.mat(np.random.random(size=(X.shape[1], 1)))
    eplison = 1e-7
    old_loss = 0
    epoch_list.clear()
    loss_list.clear()
    for i in range(epoch):
        w -= lr * grad(w, X, Y, pr)  # 迭代公式
        l = loss(X, Y, w, pr)
        loss_list.append(l)
        epoch_list.append(i)
        if abs(l - old_loss) < eplison:
            break
        old_loss = l
        print('epoch ' + str(i) + '  loss=' + str(l))
    return w


def Newton(X, Y, epoch=10000, lr=0.001, pr=0.001):
    """
    the newton optimization
    :param X: X
    :param Y: Y
    :param epoch: epoch
    :param lr: learning rate
    :param pr: penalty rate
    :return: the parameters this algorithm learned
    """
    w = np.mat(np.random.random(size=(X.shape[1], 1)))
    eplison = 1e-7
    epoch_list.clear()
    loss_list.clear()
    for i in range(epoch):
        g = grad(w, X, Y, pr)
        if np.sum(g.T @ g) < eplison:
            break
        w -= np.multiply(lr, Hessian(w, X, pr).I @ g)  # 迭代公式
        l = loss(X, Y, w, pr)
        loss_list.append(l)
        epoch_list.append(i)
        print('epoch ' + str(i) + '  loss=' + str(l))
    return w


def Conjugate_gradient(X, Y, epoch=10, pr=float(0)):
    """
    the conjugate gradient optimization
    :param X: X
    :param Y: Y
    :param epoch: epoch
    :param pr: penalty rate
    :return: the parameters this algorithm learned
    """
    w = np.mat(np.zeros((X.shape[1], 1)))
    g = grad(w, X, Y, pr)
    dk = rk = -g
    l = 0
    eplison = 1e-7
    epoch_list.clear()
    loss_list.clear()
    for i in range(epoch):
        H = Hessian(w, X, pr)
        ak = (dk.T @ rk).tolist()[0][0] / (dk.T @ H @ dk).tolist()[0][0]  # 计算步长
        new_w = w + np.multiply(ak, dk)  # 迭代公式
        new_loss = loss(X, Y, w, pr)
        if abs(new_loss - l) < eplison:
            w = new_w
            break
        l = new_loss
        loss_list.append(l)
        epoch_list.append(i)
        print('epoch ' + str(i) + '  loss=' + str(l))
        w = new_w
        rk = rk - np.multiply(ak, H @ dk)
        bk = (rk.T @ H @ dk).tolist()[0][0] / (dk.T @ H @ dk).tolist()[0][0]
        dk = rk - np.multiply(bk, dk)  # 计算方向
    return w


def draw_Classifier(w):  # 画出分类面
    """
    draw classifier curve
    :param w: the parameters
    :return: none
    """
    x = np.linspace(start, end, 1000)
    y = (-w[0] * x - w[2]) / w[1]
    plt.plot(x, y)


def train(method, dataset='my', lr=learning_rate, pr=penalty_rate, draw=True):
    """
    train a logistic regression model
    :param method: the optimization algorithm
    :param dataset: dataset
    :param lr: learning rate
    :param pr: penalty rate
    :param draw: if you want to draw a picture
    :return: the parameters this model learned
    """
    if dataset == 'my':
        X, Y = Generate_data()  # 生成画图的数据
        # X, Y = Generate_data(False)  # 生成不画图的数据
    else:  # 读入文件进行训练
        data = pd.read_csv(dataset)
        data_num = int(data.shape[0] * 0.75)
        dim = data.shape[1]
        X = np.mat(np.ones((data_num, dim)))
        Y = np.mat(np.ones((data_num, 1)))
        for i in range(data_num):
            for j in range(dim - 1):
                X[i, j] = data[data.columns[j]][i]
            Y[i] = data[data.columns[dim - 1]][i]
        if draw and dim == 3:
            x1 = []
            x2 = []
            y1 = []
            y2 = []
            for i in range(data_num):
                if Y[i] == 1:
                    x1.append(X[i, 0])
                    y1.append(X[i, 1])
                else:
                    x2.append(X[i, 0])
                    y2.append(X[i, 1])
            global start
            start = min(x1[:]) if min(x1[:]) < min(x2[:]) else min(x2[:])
            global end
            end = max(x1[:]) if max(x1[:]) < max(x2[:]) else max(x2[:])
            plt.scatter(x1, y1)
            plt.scatter(x2, y2)
    w = 0
    if method == 'gd':
        w = Gradient_descent(X, Y, epoch=epoch, lr=lr, pr=pr)  # 使用梯度下降法优化
        method = 'gradient descent'
    elif method == 'nt':
        w = Newton(X, Y, epoch=epoch, lr=lr, pr=pr)  # 使用牛顿法优化
        method = 'newton'
    elif method == 'cg':
        w = Conjugate_gradient(X, Y, epoch=epoch, pr=pr)  # 使用共轭梯度法优化
        method = 'conjugate gradient'
    else:
        print('wrong')
        exit(0)
    w = np.array(w).flatten()
    f = open('parameter.txt', 'a')
    f.write(str(method) + ', epoch=' + str(epoch) + ' lr=' + str(lr) + ' pr=' + str(
        pr) + ' w=' + str(w) + '\n')
    if not draw:
        return w

    draw_Classifier(w)
    plt.grid()
    plt.title(str(method) + ', lr=' + str(lr) + ' pr=' + str(pr))
    plt.savefig(str(method) + '.jpg')
    plt.show()

    plt.plot(epoch_list, loss_list, label='lr=' + str(lr) + ' pr=' + str(pr))  # 画出损失函数随迭代次数变化的曲线

    plt.grid()
    plt.xlabel('epoch')
    plt.ylabel('loss')
    plt.title('loss-epoch')
    plt.show()
    return w


def test(w, dataset='my'):
    """
    use test set data to test the model
    :param w: the parameters
    :param dataset: the dataset
    :return: none
    """
    w = np.mat(w).T
    cnt = 0
    if dataset == 'my':
        data_num = num1 + num2
        X, Y = Generate_data()
        for i in range(X.shape[0]):
            p1 = sigmoid(X[i] @ w)
            if p1 > 0.5 and Y[i] == 1:
                cnt += 1
            if p1 < 0.5 and Y[i] == 0:
                cnt += 1
    else:
        data = pd.read_csv(dataset)
        data_sum_num = data.shape[0]
        data_num = int(data.shape[0] * 0.25)
        dim = data.shape[1]
        X = np.mat(np.ones((data_num, dim)))
        Y = np.mat(np.ones((data_num, 1)))
        for i in range(data_num):
            for j in range(dim - 1):
                X[i, j] = data[data.columns[j]][data_sum_num - data_num + i]
            Y[i] = data[data.columns[dim - 1]][data_sum_num - data_num + i]
        for i in range(data_num):
            p1 = sigmoid(X[i] @ w)
            if p1 > 0.5 and Y[i] == 1:
                cnt += 1
            if p1 < 0.5 and Y[i] == 0:
                cnt += 1

    print('correctness: ' + str(100 * cnt / data_num) + '%')


# w = train('gd', dataset='my', lr=0.004, pr=0.001)
# w = train('nt', dataset='my', lr=0.015, pr=0)
# w = train('cg', dataset='my', pr=0, draw=True)


# for i in range(5):
#     train('cg', lr=0, pr=round(0.002*(i+1), 4))
# plt.grid()
# plt.legend()
# plt.show()

# test(w)


# 未删减维度的数据集
# w = train('gd', dataset='white_wine.csv', draw=False)
# w = [-0.6571559, 0.80784953, 0.38885334, 0.10746713, 0.80613137, 0.05122623,
#      -0.02438892, 0.52578626, 0.04262576, 0.47074137, 0.32694792, 0.85998691]
# test(w, 'white_wine.csv')


# 删减维度的数据集
# w = train('gd', dataset='white_wine_dr.csv')
# test(w, 'white_wine_dr.csv')
