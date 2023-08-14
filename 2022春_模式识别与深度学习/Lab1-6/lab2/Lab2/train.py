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
from torch.utils.tensorboard import SummaryWriter

parser = argparse.ArgumentParser(description='caltech')
parser.add_argument('--gpu', type=str, default='0', help='gpu')
parser.add_argument('--data_path', type=str, default='../../datasets/caltech-101/101_ObjectCategories',
                    help='path to train set')
parser.add_argument('--classes_info_path', type=str, default='classes.txt', help='path to classes info txt')
parser.add_argument('--train_percent', type=float, default=0.8, help='train data percent')
parser.add_argument('--valid_percent', type=float, default=0.1, help='train data percent')
parser.add_argument('--test_percent', type=float, default=0.1, help='train data percent')

parser.add_argument('--save_dir', type=str, default='./checkpoint', help='save dir')
parser.add_argument('--log_dir', type=str, default='./log', help='log dir')
parser.add_argument('--save_prefix', type=str, default='alexnet_caltech-101_e', help='save prefix')

parser.add_argument('--lr_initial', type=float, default=1e-4, help='initial learning rate')
parser.add_argument('--weight_decay', type=float, default=2e-5, help='weight decay')

parser.add_argument('--batch_size', type=int, default=32, help='batch size')
parser.add_argument('--epoch_num', type=int, default=50, help='epoch num')

parser.add_argument('--checkpoint_frequency', type=int, default=3, help='checkpoint frequency')
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

    writer = SummaryWriter(log_dir)

    model = AlexNet(output_num=101)

    if torch.cuda.device_count() > 1:
        print('Use', torch.cuda.device_count(), 'GPUs!')
    model = torch.nn.DataParallel(model).to(device)

    optimizer = optim.Adam(filter(lambda p: p.requires_grad, model.parameters()), lr=opt.lr_initial,
                           betas=(0.9, 0.999), eps=1e-8, weight_decay=opt.weight_decay)

    scheduler = torch.optim.lr_scheduler.MultiStepLR(optimizer, milestones=[12, 24, 36], gamma=0.1)

    start_epoch = 1
    criterion = nn.CrossEntropyLoss().to(device)

    transform = transforms.Compose([
        # transforms.CenterCrop([224, 224]),
        transforms.Resize([224, 224]),
        transforms.RandomHorizontalFlip(0.5),
        transforms.ToTensor()
    ])
    train_dataset = caltech_dataset(data_path=opt.data_path, classes_info=opt.classes_info_path, transform=transform)
    train_dataloader = DataLoader(dataset=train_dataset, shuffle=True, batch_size=opt.batch_size)

    total_step = len(train_dataloader)
    print('Total step: {}'.format(total_step))

    epoch_num = opt.epoch_num
    print('===> Start Epoch {} End Epoch {}'.format(start_epoch, epoch_num))

    step = 0

    for epoch in range(start_epoch, epoch_num + 1):
        model.train()
        epoch_loss = 0.
        for i, (image, label) in enumerate(tqdm(train_dataloader, desc='Train'), 1):
            optimizer.zero_grad()
            image = image.cuda()
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
                step += 1
                acc = valid(model, opt.data_path, opt.classes_info_path, mode='valid')

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
            torch.save({'state_dict': model.module.state_dict()},
                       os.path.join(checkpoint_dir, opt.save_prefix + str(epoch) + '.pth'))
        scheduler.step()


if __name__ == '__main__':
    main()
