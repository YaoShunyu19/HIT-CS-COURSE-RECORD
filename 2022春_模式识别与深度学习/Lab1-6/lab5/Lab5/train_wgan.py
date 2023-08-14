import os.path

import torch
import torch.nn as nn
import argparse
import scipy.io as sio
import matplotlib.pyplot as plt
from dataset import *
from torch.utils.data import TensorDataset
from torch.utils.data import DataLoader

parser = argparse.ArgumentParser(description='wgan')
parser.add_argument('--anchor', type=str, default='wgan', help='model name')
parser.add_argument('--gpu', type=str, default='0', help='gpu')
parser.add_argument('--points_size', type=int, default=8196, help='points size')
parser.add_argument('--batch_size', type=int, default=2048, help='batch size')
parser.add_argument('--dim', type=int, default=2, help='dim of points')
parser.add_argument('--clip_value', type=float, default=0.01, help='clip value')
parser.add_argument('--distribution_type', type=str, default='gaussian', help='sample distribution')

parser.add_argument('--iteration_num', type=int, default=10000, help='iteration num')
parser.add_argument('--g_step_num', type=int, default=7, help='g_step_num')
parser.add_argument('--d_step_num', type=int, default=1, help='d_step_num')
parser.add_argument('--lr_g_initial', type=float, default=0.00005, help='initial learning rate of generator')
parser.add_argument('--lr_d_initial', type=float, default=0.00005, help='initial learning rate of discriminator')
parser.add_argument('--check_frequency', type=int, default=50, help='check frequency')

parser.add_argument('--data_path', type=str, default='points.mat', help='data path')
parser.add_argument('--save_dir', type=str, default='./checkpoint', help='save dir')

opt = parser.parse_args()


class Generator(nn.Module):
    def __init__(self, dim):
        super(Generator, self).__init__()

        self.net = nn.Sequential(
            nn.Linear(dim, 64),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Linear(64, 256),
            nn.BatchNorm1d(256, 0.8),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Linear(256, 256),
            nn.BatchNorm1d(256, 0.8),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Linear(256, dim)
        )

    def forward(self, x):
        y = self.net(x)
        return y


class Discriminator(nn.Module):
    def __init__(self, dim):
        super(Discriminator, self).__init__()
        self.net = nn.Sequential(
            nn.Linear(dim, 64),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Linear(64, 256),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Linear(256, 64),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Linear(64, 1)
        )

    def forward(self, x):
        y = self.net(x)
        return y


def main():
    if not os.path.exists(opt.save_dir):
        os.mkdir(opt.save_dir)
    if not os.path.exists(os.path.join(opt.save_dir, opt.anchor)):
        os.mkdir(os.path.join(opt.save_dir, opt.anchor))

    device = torch.device('cuda') if opt.gpu == '0' and torch.cuda.is_available() else torch.device('cpu')

    standard_points = get_points(file_path=opt.data_path)
    opt.points_size = standard_points.shape[0]
    opt.dim = standard_points.shape[1]
    fixed_noise = torch.randn((opt.points_size, opt.dim), device=device)  # 用于测试，来生成给定分布

    generator = Generator(dim=opt.dim).to(device)
    discriminator = Discriminator(dim=opt.dim).to(device)

    train_ds = TensorDataset(standard_points)
    train_ld = DataLoader(train_ds, batch_size=opt.batch_size, shuffle=True)

    criterion = nn.BCELoss()

    optimizer_g = torch.optim.Adam(generator.parameters(), lr=opt.lr_g_initial)
    optimizer_d = torch.optim.Adam(discriminator.parameters(), lr=opt.lr_g_initial)

    scheduler_g = torch.optim.lr_scheduler.MultiStepLR(optimizer_g, milestones=[1000, 2000, 3000], gamma=0.1)
    scheduler_d = torch.optim.lr_scheduler.MultiStepLR(optimizer_d, milestones=[1000, 2000, 3000], gamma=0.1)

    g_losses = []
    d_losses = []

    plt.ion()
    save_fig_step = 0

    for iteration in range(opt.iteration_num):

        g_loss = 0.
        d_loss = 0.

        for g_step in range(opt.g_step_num):
            noise = torch.randn((opt.batch_size, opt.dim), device=device)
            fake = generator(noise)
            output = discriminator(fake)

            optimizer_g.zero_grad()

            # g_loss = criterion(output, torch.ones_like(output))
            g_loss = -torch.mean(output)
            g_loss.backward()
            g_losses.append(g_loss.item())

            optimizer_g.step()

        for d_step in range(opt.d_step_num):

            for i, data in enumerate(train_ld):
                optimizer_d.zero_grad()
                real = data[0].to(device)
                output = discriminator(real)
                # loss_real = criterion(output, torch.ones_like(output))
                loss_real = -torch.mean(output)

                noise = torch.randn(opt.batch_size, 2, device=device)
                fake = generator(noise)
                output = discriminator(fake.detach())
                # loss_fake = criterion(output, torch.zeros_like(output))
                loss_fake = torch.mean(output)

                d_loss = loss_real + loss_fake
                d_loss.backward()
                d_losses.append(d_loss.item())

                optimizer_d.step()

                for p in discriminator.parameters():
                    p.data.clamp_(-opt.clip_value, opt.clip_value)

        scheduler_g.step()
        scheduler_d.step()

        if iteration % opt.check_frequency == 0:
            print('iteration {:d}  g_loss={:.4f}  d_loss={:.4f}'.format(iteration, g_loss, d_loss))
            with torch.no_grad():
                fake = generator(fixed_noise).detach().cpu()
                plt.cla()
                plt.scatter(standard_points[:, 0], standard_points[:, 1], c='k')
                plt.scatter(fake[:, 0], fake[:, 1], c='r')
                plt.xlim(-0.5, 1.6)
                plt.ylim(-0.2, 1)
                plt.grid()
                plt.draw()
                plt.savefig(os.path.join(opt.save_dir, opt.anchor, str(save_fig_step) + '.jpg'))
                torch.save(generator, os.path.join(opt.save_dir, 'generator_' + opt.anchor))
                save_fig_step += 1
                plt.pause(0.01)

    plt.ioff()
    plt.show()
    plt.figure()
    plt.plot(g_losses)
    plt.show()
    plt.figure()
    plt.plot(d_losses)
    plt.show()


if __name__ == '__main__':
    main()
    exit(0)
