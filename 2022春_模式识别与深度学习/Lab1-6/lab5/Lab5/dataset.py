import random
import time
import scipy.io as sio
import numpy as np
import random
import matplotlib.pyplot as plt
import torch


def get_points(file_path):
    points = sio.loadmat(file_path)
    return torch.from_numpy(points['xx']).float()


def get_samples(points, batch_size, sample_num):
    sample_points = torch.zeros((batch_size, sample_num, points.shape[1]), dtype=torch.float32)
    for i in range(batch_size):
        sample_idx = random.sample(range(points.shape[0]), sample_num)
        sample_points[i, :, :] = points[sample_idx, :].float()
    return sample_points  # batch_size, sample_num, dim(2)


def get_gaussian_distribution(batch_size, sample_num, dim, mean=0.5, std=0.5):
    return torch.from_numpy(np.random.normal(loc=mean, scale=std, size=(batch_size, sample_num, dim))).float()


def get_uniform_distribution(batch_size, sample_num, dim, low=0, high=1):
    return torch.from_numpy(np.random.uniform(low=low, high=high, size=(batch_size, sample_num, dim))).float()


if __name__ == '__main__':
    # a = get_uniform_distribution(2, 3, 2)
    # print(a)
    # exit()

    points = get_points('points.mat')
    t1 = time.time()
    a = get_samples(points, 100, 500)
    print(time.time() - t1)
    print(a.shape)
    plt.scatter(points[:, 0], points[:, 1], c='k')
    plt.show()
