import math
import re
import time
import jieba
import numpy as np
import pandas as pd
import torch
import torch.nn as nn
import torch.utils.data
import word2vec
from torch.utils.data import DataLoader


class shoppint_dataset(torch.utils.data.Dataset):
    def __init__(self, txt_path, word_path, padding_size=32, mode='train'):
        super(shoppint_dataset, self).__init__()
        self.df = pd.read_csv(txt_path)[['cat', 'review']]
        self.classes = sorted(list(set(self.df['cat'])))
        self.mode = mode
        assert self.mode == 'train' or self.mode == 'valid' or self.mode == 'test'
        embedding_model = word2vec.load(word_path)
        vocab = embedding_model.vocab  # 词表
        self.vocab_dict = embedding_model.vocab_hash  # 词表对应的编号
        self.vocab_dict.update({'<UNK>': len(vocab), '<PAD>': len(vocab) + 1})  # 补充未登录词及PAD
        # vocab = np.append(np.append(vocab, '<UNK>'), '<PAD>')  # 补充未登录词
        vectors = embedding_model.vectors  # vocab_size * embedding_size大小的矩阵
        vectors = torch.Tensor(np.append(np.append(vectors, vectors.mean(axis=0).reshape(1, -1),
                                                   axis=0), vectors.mean(axis=0).reshape(1, -1), axis=0))
        self.padding_size = padding_size
        self.Embedding = nn.Embedding.from_pretrained(vectors, freeze=False, sparse=True)

    def __len__(self):
        if self.mode == 'train':
            return 37665
        if self.mode == 'valid':
            return 12555
        return 12554

    def __getitem__(self, item):
        item = item + 1
        if self.mode == 'train':
            idx = (item - 1) // 3 * 5 + (item % 3 if item % 3 != 0 else 3) - 1
        elif self.mode == 'valid':
            idx = (item - 1) * 5 + 3
        else:
            idx = item * 5 - 1
        cls, data = self.df.values[idx]
        cls = self.classes.index(cls)
        rule = re.compile(u'[^a-zA-Z0-9\u4E00-\u9FA5]')
        try:
            data = rule.sub('', data)
        except Exception:
            data = ''
        data = list(jieba.cut(data))
        if len(data) < self.padding_size:
            data += ['<PAD>'] * (self.padding_size - len(data))
        if len(data) > self.padding_size:
            data = data[:self.padding_size]
        assert len(data) == self.padding_size
        # print(data)
        word_id = [self.vocab_dict.get(word, self.vocab_dict.get('<UNK>')) for word in data]
        data = self.Embedding(torch.tensor(word_id))
        # print(cls, data.shape)
        return cls, data


class climate_dataset(torch.utils.data.Dataset):
    def __init__(self, data_path, in_dim=720, out_dim=288, feature_dim=1, gap=144, mode='train', begin=None):
        super(climate_dataset, self).__init__()
        self.df = pd.read_csv(data_path)
        self.in_dim = in_dim
        self.out_dim = out_dim
        self.feature_dim = feature_dim
        self.gap = gap
        self.mode = mode
        assert mode == 'train' or mode == 'test'
        if self.mode == 'test':
            self.begin = begin
            assert begin is not None and self.begin <= len(self)
        self.column_min = torch.tensor([913.6, -23.01, 250.6, -25.01, 12.95, 0.95, 0.79, 0.0,
                                        0.5, 0.8, 1059.45, 0.0, 0.0, 0.0])
        column_max = torch.tensor([1015.35, 37.28, 311.34, 23.11, 100.0, 63.77, 28.32, 46.01,
                                   18.13, 28.82, 1393.54, 28.49, 23.5, 360.0])
        self.column_delta = column_max - self.column_min
        self.column_min = self.column_min[:self.feature_dim]
        self.column_delta = self.column_delta[:self.feature_dim]

    def __len__(self):
        if self.mode == 'train':
            return 365 * 6 - 3  # 2187
        else:
            return 725

    def __getitem__(self, item):
        if self.mode == 'train':
            begin = item * self.gap
        else:
            begin = 2191 * 144 + self.begin * self.gap
        data = torch.tensor(self.df.values[begin:begin + self.in_dim + self.out_dim,
                            1:1 + self.feature_dim].astype(float), dtype=torch.float)
        data = (data - self.column_min) / self.column_delta
        x = data[:self.in_dim, :]
        l = data[self.in_dim:, :]
        return x, l


if __name__ == '__main__':
    # dataset = shoppint_dataset(txt_path='../../datasets/online_shopping_10_cats.csv',
    #                            word_path='sgns.weibo.word.txt', mode='valid')
    # train_dataloader = DataLoader(dataset=dataset, shuffle=True, batch_size=32)
    # for cls, da in train_dataloader:
    #     print(da.shape)
    #     time.sleep(0.1)

    df = pd.read_csv('../../datasets/jena_climate_2009_2016.csv')
    print(df.head())
    print(df.columns)
    print(df.shape)
    # print(df.values[315731])
    # print(df['p (mbar)'].min())
    import numpy as np

    a = torch.tensor(df.values[0:2, 1:].astype(float))
    print(a.shape)
    column_min = torch.tensor([913.6, -23.01, 250.6, -25.01, 12.95, 0.95, 0.79, 0.0,
                               0.5, 0.8, 1059.45, 0.0, 0.0, 0.0])
    print(a / column_min)

    climate = climate_dataset('../../datasets/jena_climate_2009_2016.csv', mode='test', begin=1)
    for d in climate:
        data, label = d
        print(data.shape, label.shape)

    exit(0)
