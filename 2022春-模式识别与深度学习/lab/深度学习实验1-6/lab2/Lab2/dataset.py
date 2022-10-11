import argparse
import random
import time

from PIL import Image
import numpy as np
import torch
import torch.utils.data
import os
from torchvision.transforms import transforms

parser = argparse.ArgumentParser(description='caltech')
parser.add_argument('--data_path', type=str, default='../../datasets/caltech-101/101_ObjectCategories',
                    help='path to train set')
parser.add_argument('--classes_info_path', type=str, default='classes.txt', help='path to classes info txt')
parser.add_argument('--train_percent', type=float, default=0.8, help='train data percent')
parser.add_argument('--valid_percent', type=float, default=0.1, help='train data percent')
parser.add_argument('--test_percent', type=float, default=0.1, help='train data percent')
opt = parser.parse_args()


class caltech_dataset(torch.utils.data.Dataset):
    def __init__(self, data_path, classes_info, transform, percent=(0.8, 0.1, 0.1), mode='train'):
        super(caltech_dataset, self).__init__()
        self.root_dir = data_path
        self.classes_info = np.loadtxt(classes_info, str)
        self.p = []
        for i in self.classes_info:
            self.p.append(int(i.split(',')[-1]))
        self.p = [i / sum(self.p) for i in self.p]
        self.transform = transform
        self.percent = percent
        self.mode = mode
        assert mode == 'train' or mode == 'valid' or mode == 'test'
        assert percent[0] + percent[1] + percent[2] == 1.
        # print(self.root_dir)
        # with open(classes_info, 'w') as cf:
        #     for _, d, _ in os.walk(self.root_dir):
        #         print(d)
        #         i = 0
        #         for c in d:
        #             i += 1
        #             c_path = os.path.join(self.root_dir, c)
        #             for _, _, im in os.walk(c_path):
        #                 cf.write(c + ',' + str(i) + ',' + str(len(im)) + '\n')
        #                 break
        #         break

    def __len__(self):
        image_num = 0
        if self.mode == 'train':
            for c in self.classes_info:
                image_num += int(np.floor(int(c.split(',')[-1]) * self.percent[0]))
        elif self.mode == 'valid':
            for c in self.classes_info:
                image_num += int(np.floor(int(c.split(',')[-1]) * self.percent[1]))
        elif self.mode == 'test':
            for c in self.classes_info:
                image_num += int(c.split(',')[-1]) - int(np.floor(int(c.split(',')[-1]) * self.percent[0])) -\
                             int(np.floor(int(c.split(',')[-1]) * self.percent[1]))
        return image_num

    def __getitem__(self, item):
        cls = item % len(self.classes_info)  # 0~101
        cls = np.random.choice(a=np.arange(len(self.classes_info)), size=1, replace=False, p=self.p)[0]
        cls_img_num = int(self.classes_info[cls].split(',')[-1])
        cls_name = self.classes_info[cls].split(',')[0]
        if self.mode == 'train':
            start = 1
            end = int(self.percent[0] * cls_img_num)
        elif self.mode == 'valid':
            start = int(self.percent[0] * cls_img_num) + 1
            end = start + int(self.percent[1] * cls_img_num)
        else:
            start = int(self.percent[0] * cls_img_num) + 1 + int(self.percent[1] * cls_img_num)
            end = cls_img_num
        img_name = 'image_' + '%04d' % random.randint(start, end) + '.jpg'
        image_path = os.path.join(self.root_dir, cls_name, img_name)
        # print(cls, image_path)
        image = Image.open(image_path).convert('RGB')
        if self.transform:
            image = self.transform(image)
        return image, cls


if __name__ == '__main__':
    transform = transforms.Compose([
        transforms.Resize([224, 224]),
        transforms.RandomHorizontalFlip(0.5),
        transforms.ToTensor()
    ])
    d = caltech_dataset(opt.data_path, opt.classes_info_path, transform, mode='train')
    print(len(d))
    # exit()
    for data in d:
        image, label = data
        print(image.shape, label)
        # print(image)
        time.sleep(0.5)
