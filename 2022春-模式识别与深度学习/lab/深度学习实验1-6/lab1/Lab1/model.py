import torch
import torch.nn as nn


class MLP(nn.Module):
    def __init__(self, in_nums=784, out_nums=10, drop=0.2):
        super(MLP, self).__init__()
        self.mlp = nn.Sequential(
            nn.Linear(in_nums, in_nums * 2),
            nn.Dropout(drop),
            nn.ReLU(),
            nn.Linear(in_nums * 2, in_nums),
            nn.Dropout(drop),
            nn.ReLU(),
            nn.Linear(in_nums, in_nums // 2),
            nn.Dropout(drop),
            nn.ReLU(),
            nn.Linear(in_nums // 2, in_nums // 4),
            nn.Dropout(drop),
            nn.ReLU(),
            nn.Linear(in_nums // 4, out_nums)
        )

    def forward(self, x):
        ret = self.mlp(x)
        return ret


if __name__ == '__main__':
    mlp = MLP()
    x = torch.rand((1, 784))
    y = mlp(x)
    print(y.shape)
    print(y)
