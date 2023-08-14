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
parser.add_argument('--data_path', type=str, default='../../datasets/online_shopping_10_cats.csv', help='train set')
parser.add_argument('--word2vec_path', type=str, default='sgns.weibo.word.txt', help='word2vec model')

parser.add_argument('--word_dim', type=int, default=300, help='dim of word')
parser.add_argument('--seq_len', type=int, default=12, help='length of sequence')
parser.add_argument('--hidden_size', type=int, default=64, help='hidden size')
parser.add_argument('--num_classes', type=int, default=10, help='num classes')

parser.add_argument('--save_dir', type=str, default='./checkpoint', help='save dir')
parser.add_argument('--log_dir', type=str, default='./log', help='log dir')
parser.add_argument('--save_prefix', type=str, default='shopping_lstm_e', help='save prefix')

parser.add_argument('--lr_initial', type=float, default=1e-3, help='initial learning rate')
parser.add_argument('--weight_decay', type=float, default=2e-5, help='weight decay')

parser.add_argument('--batch_size', type=int, default=64, help='batch size')
parser.add_argument('--epoch_num', type=int, default=3, help='epoch num')

parser.add_argument('--checkpoint_frequency', type=int, default=1, help='checkpoint frequency')
parser.add_argument('--test_num', type=int, default=2, help='test num')

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
    log_txt_name = os.path.join(log_dir, datetime_now + '.txt')
    print('Now time is : ', datetime_now)

    writer = SummaryWriter(log_dir)

    if opt.anchor == 'rnn':
        opt.save_prefix = 'shopping_rnn_e'
        model = rnn.my_RNN(in_feature=opt.word_dim, out_feature=opt.num_classes,
                           hidden_size=opt.hidden_size, device=device)
    elif opt.anchor == 'gru':
        opt.save_prefix = 'shopping_gru_e'
        model = gru.my_GRU(in_feature=opt.word_dim, out_feature=opt.num_classes,
                           hidden_size=opt.hidden_size, device=device)
    elif opt.anchor == 'lstm':
        opt.save_prefix = 'shopping_lstm_e'
        model = lstm.my_LSTM(in_feature=opt.word_dim, out_feature=opt.num_classes,
                             hidden_size=opt.hidden_size, bidirectional=False, device=device)
    elif opt.anchor == 'bilstm':
        opt.save_prefix = 'shopping_bilstm_e'
        model = lstm.my_LSTM(in_feature=opt.word_dim, out_feature=opt.num_classes,
                             hidden_size=opt.hidden_size, bidirectional=True, device=device)
    else:
        raise Exception

    with open(log_txt_name, 'a') as f:
        f.write(opt.anchor + '\n')

    if torch.cuda.device_count() > 1:
        print('Use', torch.cuda.device_count(), 'GPUs!')
    model = torch.nn.DataParallel(model)

    optimizer = optim.Adam(filter(lambda p: p.requires_grad, model.parameters()), lr=opt.lr_initial,
                           betas=(0.9, 0.999), eps=1e-8, weight_decay=opt.weight_decay)

    scheduler = torch.optim.lr_scheduler.MultiStepLR(optimizer, milestones=[1, 2, 3], gamma=0.1)

    start_epoch = 1
    criterion = nn.CrossEntropyLoss().to(device)

    train_dataset = shoppint_dataset(txt_path=opt.data_path, word_path=opt.word2vec_path,
                                     padding_size=opt.seq_len, mode='train')
    train_dataloader = DataLoader(dataset=train_dataset, shuffle=True, batch_size=opt.batch_size)

    total_step = len(train_dataloader)
    print('Total step: {}'.format(total_step))

    epoch_num = opt.epoch_num
    print('===> Start Epoch {} End Epoch {}'.format(start_epoch, epoch_num))

    step = 0

    for epoch in range(start_epoch, epoch_num + 1):
        epoch_loss = 0.
        for i, (cls, seq) in enumerate(tqdm(train_dataloader, desc='Train'), 1):
            model.train()
            optimizer.zero_grad()
            seq = seq.to(device)
            cls = cls.to(device).long()
            pred = model(seq)
            pred = pred[:, -1, :]
            loss = criterion(pred, cls.detach())
            acc = torch.sum(torch.argmax(pred, dim=1) == cls) / opt.batch_size
            epoch_loss += loss.detach().item()
            batch_log = 'Epoch:[{}/{}] Batch: [{}/{}] loss = {:.4f} lr = {:.7f} acc = {:.3f}'.\
                format(epoch, epoch_num, i, total_step, loss.detach().item(), scheduler.get_last_lr()[0], acc)
            print(batch_log)
            loss.backward()
            optimizer.step()

            if i % (total_step // opt.test_num) == 0:
                step += 1
                acc = valid(model, opt.data_path, opt.word2vec_path, opt.seq_len, mode='valid', device=device)

                writer.add_scalar('accuracy', acc, step)
                writer.add_scalar('loss', epoch_loss * 2, step)

                valid_log = 'Epoch: {}\t Iter: {}\t Valid accuracy: {:.4f}%'.format(epoch, i, 100 * acc)
                print(valid_log)
                with open(log_txt_name, 'a') as f:
                    f.write(valid_log + '\n')
                    f.write(batch_log + '\n')

            if i == total_step:
                break
        if epoch % opt.checkpoint_frequency == 0:
            torch.save({'state_dict': model.state_dict()},
                       os.path.join(checkpoint_dir, opt.save_prefix + str(epoch) + '.pth'))
        scheduler.step()
    writer.close()


if __name__ == '__main__':
    main()
