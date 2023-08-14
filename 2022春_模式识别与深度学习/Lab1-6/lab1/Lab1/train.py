import torch
import torch.nn as nn
import os
import datetime
from model import *
from dataset import *
from torch.utils.data import DataLoader
import argparse
import torch.optim as optim
from valid import *
from tqdm import tqdm
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description='MNIST')
parser.add_argument('--gpu', type=str, default='0', help='gpu')
parser.add_argument('--train_data_path', type=str, default='../../datasets/MNIST/train-images.idx3-ubyte',
                    help='path to train set')
parser.add_argument('--train_label_path', type=str, default='../../datasets/MNIST/train-labels.idx1-ubyte',
                    help='path to train set')
parser.add_argument('--test_data_path', type=str, default='../../datasets/MNIST/t10k-images.idx3-ubyte',
                    help='path to train set')
parser.add_argument('--test_label_path', type=str, default='../../datasets/MNIST/t10k-labels.idx1-ubyte',
                    help='path to train set')
parser.add_argument('--save_dir', type=str, default='./checkpoint', help='save dir')
parser.add_argument('--log_dir', type=str, default='./log', help='log dir')
parser.add_argument('--save_prefix', type=str, default='mlp_mnist_e', help='save prefix')

parser.add_argument('--lr_initial', type=float, default=1e-4, help='initial learning rate')
parser.add_argument('--weight_decay', type=float, default=2e-5, help='weight decay')

parser.add_argument('--batch_size', type=int, default=32, help='batch size')
parser.add_argument('--epoch_num', type=int, default=8, help='epoch num')

parser.add_argument('--checkpoint_frequency', type=int, default=1, help='checkpoint frequency')
parser.add_argument('--test_num', type=int, default=2, help='test num')

opt = parser.parse_args()


def main():
    os.environ['CUDA_DEVICE_ORDER'] = 'PCI_BUS_ID'
    os.environ['CUDA_VISIBLE_DEVICES'] = opt.gpu
    device = 'cuda' if opt.gpu == '0' else 'cpu'

    checkpoint_dir = os.path.join(opt.save_dir)
    if not os.path.exists(checkpoint_dir):
        os.makedirs(checkpoint_dir)

    log_dir = os.path.join(opt.log_dir)
    if not os.path.exists(log_dir):
        os.makedirs(log_dir)

    datetime_now = datetime.datetime.now().isoformat()[:-7].replace(':', '-')
    log_txt_name = os.path.join(log_dir, datetime_now + '.txt')
    print('Now time is : ', datetime_now)

    model = MLP()

    if torch.cuda.device_count() > 1:
        print('Use', torch.cuda.device_count(), 'GPUs!')
    model = torch.nn.DataParallel(model).to(device)

    optimizer = optim.Adam(filter(lambda p: p.requires_grad, model.parameters()), lr=opt.lr_initial,
                           betas=(0.9, 0.999), eps=1e-8, weight_decay=opt.weight_decay)

    scheduler = torch.optim.lr_scheduler.MultiStepLR(optimizer, milestones=[2, 4, 6], gamma=0.2)

    start_epoch = 1
    criterion = nn.CrossEntropyLoss().to(device)

    train_dataset = mnist_dataset(data_path=opt.train_data_path, label_path=opt.train_label_path)
    train_dataloader = DataLoader(dataset=train_dataset, shuffle=True, batch_size=opt.batch_size)

    total_step = len(train_dataloader)
    print('Total step: {}'.format(total_step))

    epoch_num = opt.epoch_num
    print('===> Start Epoch {} End Epoch {}'.format(start_epoch, epoch_num))
    loss_list = []
    acc_list = []

    for epoch in range(start_epoch, epoch_num + 1):
        model.train()
        epoch_loss = 0.
        for i, (image, label) in enumerate(tqdm(train_dataloader, desc='Train'), 1):
            optimizer.zero_grad()
            image = image.cuda().flatten(1).type(torch.FloatTensor)
            label = label.cuda().long()
            pred = model(image)
            loss = criterion(pred, label.detach())
            acc = torch.sum(torch.argmax(pred, dim=1) == label) / opt.batch_size
            epoch_loss += loss.detach().item()
            batch_log = 'Epoch:[{}/{}] Batch: [{}/{}] loss = {:.4f} lr = {:.7f} acc = {:.3f}'.\
                format(epoch, epoch_num, i, total_step, loss.detach().item(), scheduler.get_last_lr()[0], acc)
            # print(batch_log)
            loss.backward()
            optimizer.step()

            if i % (total_step // opt.test_num) == 0:
                acc = valid(model, opt.test_data_path, opt.test_label_path)
                loss_list.append(epoch_loss * 2)
                acc_list.append(acc)
                valid_log = 'Epoch: {}\t Iter: {}\t Valid accuracy: {:.4f}%'.format(epoch, i, 100 * acc)
                print(valid_log)
                with open(log_txt_name, 'a') as f:
                    f.write(valid_log + '\n')
                    f.write(batch_log + '\n')

            if i == total_step:
                break

        if epoch % opt.checkpoint_frequency == 0:
            torch.save({'state_dict': model.module.state_dict()},
                       os.path.join(checkpoint_dir, opt.save_prefix + str(epoch) + '.pth'))
        scheduler.step()

    plt.plot([i for i in range(len(loss_list))], loss_list, c='r')
    plt.show()
    plt.plot([i for i in range(len(loss_list))], acc_list, c='g')
    plt.show()


if __name__ == '__main__':
    main()
