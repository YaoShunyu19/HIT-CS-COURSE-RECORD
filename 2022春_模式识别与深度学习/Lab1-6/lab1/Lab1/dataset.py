import argparse
import struct
import numpy as np
import torch
import torch.utils.data
from matplotlib import pyplot as plt

parser = argparse.ArgumentParser(description='MNIST')
parser.add_argument('--train_data_path', type=str, default='../../datasets/MNIST/train-images.idx3-ubyte',
                    help='path to train set')
parser.add_argument('--train_label_path', type=str, default='../../datasets/MNIST/train-labels.idx1-ubyte',
                    help='path to train set')
parser.add_argument('--test_data_path', type=str, default='../../datasets/MNIST/t10k-images.idx3-ubyte',
                    help='path to train set')
parser.add_argument('--test_label_path', type=str, default='../../datasets/MNIST/t10k-labels.idx1-ubyte',
                    help='path to train set')

opt = parser.parse_args()


class mnist_dataset(torch.utils.data.Dataset):
    def __init__(self, data_path, label_path):
        super(mnist_dataset, self).__init__()
        self.bin_data = open(data_path, 'rb').read()
        self.bin_label = open(label_path, 'rb').read()
        self.data_offset = 0
        self.label_offset = 0
        data_fmt_header = '>iiii'
        label_fmt_header = '>ii'
        _, num_images, num_rows, num_cols = struct.unpack_from(data_fmt_header, self.bin_data, self.data_offset)
        print('num images: %d, shape: %d*%d' % (num_images, num_rows, num_cols))
        _, num_labels = struct.unpack_from(label_fmt_header, self.bin_label, self.label_offset)
        print('num labels: %d' % num_labels)
        assert num_images == num_labels
        self.num_images = num_images
        self.h = num_rows
        self.w = num_cols
        image_size = num_rows * num_cols
        self.data_offset += struct.calcsize(data_fmt_header)
        self.label_offset += struct.calcsize(label_fmt_header)
        self.data_fmt_image = '>' + str(image_size) + 'B'
        self.label_fmt_image = '>B'

    def __len__(self):
        return self.num_images

    def __getitem__(self, item):
        data_offset = self.data_offset + struct.calcsize(self.data_fmt_image) * item
        image = np.array(struct.unpack_from(self.data_fmt_image, self.bin_data, data_offset)).reshape((self.h, self.w))
        label_offset = self.label_offset + struct.calcsize(self.label_fmt_image) * item
        label = np.array(struct.unpack_from(self.label_fmt_image, self.bin_label, label_offset))[0]
        return image / 255., label


if __name__ == '__main__':
    train_dataset = mnist_dataset('../../datasets/MNIST/train-images.idx3-ubyte',
                                  '../../datasets/MNIST/train-labels.idx1-ubyte')
    for i, (image, label) in enumerate(train_dataset):
        print(image.shape)
        # print(image)
        print(label)
        print(i)
        # plt.imshow(image, cmap='gray')
        # plt.pause(0.000001)
        # plt.show()
