import math

import torch
import torch.nn as nn
from senet import SE


class MBConv(nn.Module):
    def __init__(self, channel, reduction):
        super(MBConv, self).__init__()
        self.bone = nn.Sequential(
            nn.Conv2d(channel, channel, 1),
            nn.BatchNorm2d(channel),
            nn.ReLU(),
            nn.Conv2d(channel, channel, kernel_size=3, padding=1, groups=channel),
            SE(channel, reduction),
            nn.Conv2d(channel, channel, 1),
            nn.BatchNorm2d(channel)
        )

    def forward(self, x):
        return x + self.bone(x)


class My_Improved(nn.Module):
    def __init__(self, in_channel=3, depths=[2, 2, 6, 2], out_channel=[64, 128, 256, 512], reduction=4, num_classes=10):
        super(My_Improved, self).__init__()
        self.downsample_layers = nn.ModuleList()
        stem = nn.Sequential(
            nn.Conv2d(in_channel, out_channel[0], kernel_size=4, stride=4),
            nn.BatchNorm2d(out_channel[0])
        )
        self.downsample_layers.append(stem)
        for i in range(3):
            downsample_layer = nn.Sequential(
                nn.BatchNorm2d(out_channel[i]),
                nn.Conv2d(out_channel[i], out_channel[i + 1], kernel_size=2, stride=2)
            )
            self.downsample_layers.append(downsample_layer)

        self.stages = nn.ModuleList()
        for i in range(4):
            stage = nn.Sequential(
                *[MBConv(out_channel[i], reduction) for _ in range(depths[i])]
            )
            self.stages.append(stage)

        self.head = nn.Linear(out_channel[-1], num_classes)
        self.apply(self._init_weights)

    def _init_weights(self, m):
        if isinstance(m, (nn.Conv2d, nn.Linear)):
            nn.init.kaiming_normal_(m.weight, a=math.sqrt(5))
            nn.init.constant_(m.bias, 0)

    def extract_features(self, x):
        for i in range(4):
            x = self.downsample_layers[i](x)
            x = self.stages[i](x)
        return x.mean([-2, -1])

    def forward(self, x):
        feature = self.extract_features(x)
        out = self.head(feature)
        return out


if __name__ == '__main__':
    mbconv = MBConv(64, 4)
    net = My_Improved()
    x = torch.rand((2, 3, 224, 224))
    y = net(x)
    print(y.shape)
