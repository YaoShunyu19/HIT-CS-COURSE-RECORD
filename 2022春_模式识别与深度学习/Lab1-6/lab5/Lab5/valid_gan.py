import torch
import matplotlib.pyplot as plt
from train_gan import Generator

if __name__ == '__main__':
    noise = torch.randn((8191, 2)).cuda()
    generator = torch.load('./checkpoint/generator_gan_rmsprop').cuda()
    fake = generator(noise).detach().cpu().numpy()
    plt.scatter(fake[:, 0], fake[:, 1])
    plt.show()
