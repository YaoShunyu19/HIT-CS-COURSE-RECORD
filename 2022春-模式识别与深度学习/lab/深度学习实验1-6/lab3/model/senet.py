import torch
import torch.nn as nn


class SE(nn.Module):
    def __init__(self, channel, reduction=16):
        super(SE, self).__init__()
        self.avg_pool = nn.AdaptiveAvgPool2d(1)
        self.fc1 = nn.Linear(channel, channel // reduction)
        self.relu = nn.ReLU()
        self.fc2 = nn.Linear(channel // reduction, channel)
        self.sigmoid = nn.Sigmoid()

    def forward(self, x):
        # x: B, C, H, W
        out = self.avg_pool(x).flatten(1)
        out = self.sigmoid(self.fc2(self.relu(self.fc1(out))))
        out = out.unsqueeze(2).unsqueeze(2)
        return out * x


if __name__ == '__main__':
    se = SE(channel=64)
    x = torch.rand((2, 64, 56, 56))
    y = se(x)
    print(y.shape)
