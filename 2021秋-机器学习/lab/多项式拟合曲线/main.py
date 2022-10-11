import numpy as np
import matplotlib.pyplot as plt


def Generate_data(min_x=0, max_x=10, num=10, scale=0.1, draw=True):
    """
    generate data  sin(2*pi*x)+Gaussian noise
    :param scale: the standard deviation of the Gaussian noise
    :param min_x: the minimum of x
    :param max_x: the maximum of x
    :param num: the number of data points
    :param draw: if you want to draw the points
    :return: X,Y both are num*1 matrix
    """
    _X = np.linspace(min_x, max_x, 1000)
    if draw is True:
        plt.plot(_X, np.sin(2 * np.pi * _X), c='k', label='sin(2*pi*x)')
    X = np.linspace(min_x, max_x, num)
    Y = np.sin(2 * np.pi * X)
    sigma = np.random.normal(loc=0, scale=scale, size=num)  # 生成高斯噪声
    Y += sigma
    if draw is True:
        plt.scatter(X, Y)
    X = np.mat(X).T
    Y = np.mat(Y).T
    return X, Y


def cal_Vandermonde_Matrix(X, m):
    """
    cal vandermonde matrix
    :param X: the x of data
    :param m: the max power of polynomial
    :return: vandermonde matrix
    """
    num = X.shape[0]
    Xm = np.mat(np.ones((m + 1, num)))
    t = X.T
    for i in range(1, m + 1):
        Xm[i] = t
        t = np.multiply(t, X.T)
    return Xm


def Polynomial_fit(X, Y, m=1, penalty_rate=float(0), draw=True):
    """
    use polynomial to fit the curve
    :param X: the x of data
    :param Y: the y of data
    :param m: the max power of polynomial
    :param penalty_rate: penalty rate for regularization loss function
    :param draw: if you want to draw the curve
    :return: a list of the parameters of the polynomial
    """
    Xm = cal_Vandermonde_Matrix(X, m)
    W = (Xm @ Xm.T + penalty_rate * np.mat(np.identity(m + 1))).I @ Xm @ Y  # 计算解析解
    loss = W.T @ Xm @ Xm.T @ W - 2 * W.T @ Xm @ Y + Y.T @ Y + penalty_rate * W.T @ W  # 计算损失函数的值
    print('loss: ' + str(np.array(loss.tolist()).flatten()[0]))
    w = np.array(W.tolist()).flatten()
    func = np.poly1d(w[::-1])
    x = np.linspace(min_x, max_x, 1000)
    if draw is True:
        plt.plot(x, func(x), c='g', label='Polynomial fit')
    return w


def Gradient_descent(X, Y, m=1, epoch=1000, lr=0.01, penalty_rate=float(0), draw=True):
    """
    use gradient descent to optimization  the curve
    :param X: the x of data
    :param Y: the y of data
    :param m: the max power of polynomial
    :param epoch: the number of iterations
    :param lr: the learning rate
    :param penalty_rate: penalty rate for regularization loss function
    :param draw: if you want to draw the curve
    :return: a list of the parameters of the polynomial
    """
    Xm = cal_Vandermonde_Matrix(X, m)
    W = np.mat(np.random.random(size=(m + 1, 1)))  # 随机初始化初始解
    old_loss = 0
    eplison = 1e-7
    for i in range(epoch):  # 迭代epoch轮进行训练
        W = W - lr * (2 * Xm @ Xm.T @ W - 2 * Xm @ Y + 2 * penalty_rate * W)  # 向梯度方向移动
        loss = W.T @ Xm @ Xm.T @ W - 2 * W.T @ Xm @ Y + Y.T @ Y + penalty_rate * W.T @ W  # 计算损失函数
        if abs(loss - old_loss) < eplison:
            break
        old_loss = loss
        print('epoch ' + str(i + 1) + '  loss: ' + str(np.array(loss.tolist()).flatten()[0]))
    w = np.array(W.tolist()).flatten()
    func = np.poly1d(w[::-1])
    x = np.linspace(min_x, max_x, 1000)
    if draw is True:
        plt.plot(x, func(x), c='b', label='Gradient descent')
    return w


def Conjugate_gradient(X, Y, m=1, epoch=10, penalty_rate=float(0), draw=True):
    """
    use conjugate descent to optimization  the curve
    :param X: the x of data
    :param Y: the y of data
    :param m: the max power of polynomial
    :param epoch: the number of iterations
    :param penalty_rate: penalty rate for regularization loss function
    :param draw: if you want to draw the curve
    :return: a list of the parameters of the polynomial
    """
    Xm = cal_Vandermonde_Matrix(X, m)
    W = np.mat(np.random.random(size=(m + 1, 1)))  # 随机初始化初始解
    A = 2 * Xm @ Xm.T + 2 * penalty_rate * np.mat(np.identity(m + 1))
    B = 2 * Xm @ Y
    grad = A @ W - B
    p = -grad
    old_loss = 0
    eplison = 1e-7
    for i in range(epoch):
        a = (grad.T @ grad).tolist()[0][0] / (p.T @ A @ p).tolist()[0][0]  # 计算最佳步长
        W = W + a * p
        grad1 = grad + a * A @ p
        beta = (grad1.T @ grad1).tolist()[0][0] / (grad.T @ grad).tolist()[0][0]
        grad = grad1
        p = -grad + beta * p  # 计算最佳方向
        loss = W.T @ Xm @ Xm.T @ W - 2 * W.T @ Xm @ Y + Y.T @ Y + penalty_rate * W.T @ W  # 计算损失函数
        if abs(loss - old_loss) < eplison:
            break
        old_loss = loss
        print('epoch ' + str(i + 1) + '  loss: ' + str(np.array(loss.tolist()).flatten()[0]))
    w = np.array(W.tolist()).flatten()
    func = np.poly1d(w[::-1])
    x = np.linspace(min_x, max_x, 1000)
    if draw is True:
        plt.plot(x, func(x), c='r', label='Conjugate gradient, penalty-rate=' + str(penalty_rate))
    return w


def trainModel(method):
    """
    use train set to train a model
    :param method: optimization method
    :return: parameters
    """
    w = []
    if method == 'polynomial_fit':
        w = Polynomial_fit(X, Y, m=m, penalty_rate=0.001)  # 使用多项式拟合
    elif method == 'gradient_descent':
        w = Gradient_descent(X, Y, m=m, epoch=20000, lr=0.002, penalty_rate=0.005)  # 使用梯度下降法优化
    elif method == 'Conjugate_gradient':
        w = Conjugate_gradient(X, Y, m=m, epoch=70, penalty_rate=0.001)  # 使用随机梯度下降法优化
    else:
        print('wrong')
        exit(0)
    return w


def testModel(method):
    """
    use test set to test the model
    :param method: different optimization method
    :return: none
    """
    X_test, Y_test = Generate_data(min_x=min_x, max_x=max_x, num=20, scale=0.1, draw=False)  # 测试集
    prlist = []
    losslist = []
    for i in range(1, 500):
        pr = 0.00001 * i
        prlist.append(pr)
        w = 0
        if method == 'polynomial_fit':
            w = Polynomial_fit(X, Y, m=m, penalty_rate=pr, draw=False)
        elif method == 'gradient_descent':
            w = Gradient_descent(X, Y, m=m, epoch=20000, lr=0.005, penalty_rate=pr, draw=False)
        elif method == 'Conjugate_gradient':
            w = Conjugate_gradient(X, Y, m=m, epoch=30, penalty_rate=pr, draw=False)
        else:
            print('wrong')
            exit(0)
        func = np.poly1d(w[::-1])
        Y_pre = np.mat(func(np.array(X_test)))
        l = Y_test - Y_pre
        loss = l.T @ l
        loss = loss.tolist()[0][0]
        losslist.append(loss)
    plt.plot(prlist, losslist)
    plt.show()


min_x = 0  # x的最小值
max_x = 1  # x的最大值
num_data = 75  # 样本数目
m = 50  # 拟合多项式的阶数
X, Y = Generate_data(min_x=min_x, max_x=max_x, num=num_data, scale=0.1)  # 生成样本点

# 训练模型
# trainModel('polynomial_fit')
# trainModel('gradient_descent')
trainModel('Conjugate_gradient')

plt.title('polynomial fit curve')
plt.legend()
plt.grid()
plt.savefig('result.jpg')
plt.show()

# 测试模型
# testModel('polynomial_fit')
