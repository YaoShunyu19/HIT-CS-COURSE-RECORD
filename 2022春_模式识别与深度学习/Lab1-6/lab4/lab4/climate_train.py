import time

import torch
import torch.nn as nn
import os
import datetime
from models import rnn, gru, lstm
from shopping_valid import valid
from dataset import *
from torch.utils.data import DataLoader
import argparse
import torch.optim as optim
from tqdm import tqdm
from torch.utils.tensorboard import SummaryWriter

parser = argparse.ArgumentParser(description='lab4')
parser.add_argument('--gpu', type=str, default='0', help='gpu')
parser.add_argument('--anchor', type=str, default='gru', help='model')
parser.add_argument('--data_path', type=str, default='../../datasets/jena_climate_2009_2016.csv', help='train set')

parser.add_argument('--climate_dim', type=int, default=4, help='dim of climate')
parser.add_argument('--seq_len', type=int, default=720, help='length of sequence')
parser.add_argument('--hidden_size', type=int, default=64, help='hidden size')

parser.add_argument('--save_dir', type=str, default='./checkpoint', help='save dir')
parser.add_argument('--log_dir', type=str, default='./log', help='log dir')
parser.add_argument('--save_prefix', type=str, default='climate_bilstm_e', help='save prefix')

parser.add_argument('--lr_initial', type=float, default=1e-3, help='initial learning rate')
parser.add_argument('--weight_decay', type=float, default=2e-5, help='weight decay')

parser.add_argument('--batch_size', type=int, default=64, help='batch size')
parser.add_argument('--epoch_num', type=int, default=2, help='epoch num')

parser.add_argument('--checkpoint_frequency', type=int, default=1, help='checkpoint frequency')

opt = parser.parse_args()


def main():
    os.environ['CUDA_DEVICE_ORDER'] = 'PCI_BUS_ID'
    os.environ['CUDA_VISIBLE_DEVICES'] = opt.gpu
    device = torch.device('cuda') if opt.gpu == '0' else torch.device('cpu')
    # device = torch.device('cpu')

    checkpoint_dir = os.path.join(opt.save_dir)
    if not os.path.exists(checkpoint_dir):
        os.makedirs(checkpoint_dir)

    log_dir = os.path.join(opt.log_dir)
    if not os.path.exists(log_dir):
        os.makedirs(log_dir)

    datetime_now = datetime.datetime.now().isoformat()[:-7].replace(':', '-')
    print('Now time is : ', datetime_now)

    writer = SummaryWriter(log_dir)

    model = lstm.my_LSTM(in_feature=opt.climate_dim, out_feature=opt.climate_dim,
                         hidden_size=opt.hidden_size, bidirectional=True, device=device)

    if torch.cuda.device_count() > 1:
        print('Use', torch.cuda.device_count(), 'GPUs!')
    model = torch.nn.DataParallel(model)

    optimizer = optim.Adam(filter(lambda p: p.requires_grad, model.parameters()), lr=opt.lr_initial,
                           betas=(0.9, 0.999), eps=1e-8, weight_decay=opt.weight_decay)

    scheduler = torch.optim.lr_scheduler.MultiStepLR(optimizer, milestones=[1, 2, 3], gamma=0.1)

    start_epoch = 1
    criterion = nn.MSELoss().to(device)

    train_dataset = climate_dataset(data_path=opt.data_path, feature_dim=opt.climate_dim, mode='train')
    train_dataloader = DataLoader(dataset=train_dataset, shuffle=True, batch_size=opt.batch_size)
    total_step = len(train_dataloader)

    epoch_num = opt.epoch_num
    print('===> Start Epoch {} End Epoch {}'.format(start_epoch, epoch_num))

    for epoch in range(start_epoch, epoch_num + 1):
        epoch_loss = 0.
        for i, (x, y) in enumerate(tqdm(train_dataloader, desc='Train'), 1):
            model.train()
            optimizer.zero_grad()
            x = x.to(device)
            y = y.to(device)
            pred = model(x)
            pred = pred[:, -y.shape[1]:, :]
            loss = criterion(pred, y.detach())
            epoch_loss += loss.detach().item()
            batch_log = 'Epoch:[{}/{}] Batch: [{}/{}] loss = {:.4f} lr = {:.7f}'.\
                format(epoch, epoch_num, i, total_step, loss.detach().item(), scheduler.get_last_lr()[0])
            print(batch_log)
            loss.backward()
            optimizer.step()

            if i == total_step:
                break
        if epoch % opt.checkpoint_frequency == 0:
            torch.save({'state_dict': model.state_dict()},
                       os.path.join(checkpoint_dir, opt.save_prefix + str(epoch) + '.pth'))
        scheduler.step()
    writer.close()


if __name__ == '__main__':
    main()
