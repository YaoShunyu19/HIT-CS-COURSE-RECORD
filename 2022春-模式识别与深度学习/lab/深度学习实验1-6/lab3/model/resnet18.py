import torch
import torch.nn as nn
from Lab3.model.senet import SE


class ResBlock(nn.Module):
    def __init__(self, in_channel, out_channel, stride=1, se=False, reduction=16):
        super(ResBlock, self).__init__()
        self.conv = nn.Sequential(
            nn.Conv2d(in_channel, out_channel, kernel_size=3, stride=stride, padding=1),
            nn.BatchNorm2d(out_channel),
            nn.ReLU(inplace=True),
            nn.Conv2d(out_channel, out_channel, kernel_size=3, padding=1),
            nn.BatchNorm2d(out_channel)
        )
        if se:
            self.se_layer = SE(channel=out_channel, reduction=reduction)
        else:
            self.se_layer = nn.Identity()
        if in_channel != out_channel:
            self.short_cut = nn.Sequential(
                nn.Conv2d(in_channel, out_channel, kernel_size=1, stride=2),
                nn.BatchNorm2d(out_channel)
            )
        else:
            self.short_cut = nn.Sequential()

    def forward(self, x):
        # print(self.conv(x).shape)
        # print(self.short_cut(x).shape)
        return self.se_layer(self.conv(x)) + self.short_cut(x)


class ResNet18(nn.Module):
    def __init__(self, class_num, se=False):
        super(ResNet18, self).__init__()
        self.residual_layer_nums = [2, 2, 2, 2]
        self.out_channels = [64, 64, 128, 256, 512]
        self.se = se
        self.in_proj = nn.Sequential(
            nn.Conv2d(3, 64, kernel_size=7, stride=2, padding=3),
            nn.BatchNorm2d(64),
            nn.ReLU()
        )
        self.residual_layer = []
        self.residual_layer.append(nn.MaxPool2d(kernel_size=3, stride=2, padding=1))
        for i in range(len(self.residual_layer_nums)):
            layer_num = self.residual_layer_nums[i]
            in_channel = self.out_channels[i]
            out_channel = self.out_channels[i + 1]
            self.residual_layer.append(self.make_layer(ResBlock, layer_num, in_channel, out_channel))
        self.residual_layer = nn.Sequential(*self.residual_layer)
        self.avg_pool = nn.AdaptiveAvgPool2d(1)
        self.fc = nn.Linear(512, class_num)

    def make_layer(self, block, layer_num, in_channel, out_channel):
        layer = []
        for i in range(layer_num):
            layer.append(block(in_channel, out_channel, stride=2 if in_channel != out_channel and i == 0 else 1,
                               se=self.se))
            in_channel = out_channel
        return nn.Sequential(*layer)

    def forward(self, x):
        out = self.in_proj(x)  # B, 64, 112, 112
        out = self.residual_layer(out)  # B, 512, 7, 7
        out = self.avg_pool(out).flatten(1)  # B, 512
        out = self.fc(out)  # B, class_num
        return out


class My_ResBlock(nn.Module):
    def __init__(self, in_channel, out_channel, stride=1, se=False, reduction=16):
        super(My_ResBlock, self).__init__()
        self.conv = nn.Sequential(
            nn.Conv2d(in_channel, out_channel, kernel_size=3, stride=stride, padding=1),
            nn.BatchNorm2d(out_channel),
            nn.ReLU(inplace=True),
            nn.Conv2d(out_channel, out_channel, kernel_size=3, padding=1),
            nn.BatchNorm2d(out_channel)
        )
        if se:
            self.se_layer = SE(channel=out_channel, reduction=reduction)
        else:
            self.se_layer = nn.Identity()
        if in_channel != out_channel:
            self.short_cut = nn.Sequential(
                nn.Conv2d(in_channel, out_channel, kernel_size=1, stride=2),
                nn.BatchNorm2d(out_channel)
            )
        else:
            self.short_cut = nn.Sequential()

    def forward(self, x):
        # print(self.conv(x).shape)
        # print(self.short_cut(x).shape)
        return self.se_layer(self.conv(x)) + self.short_cut(x)


class My_ResNet18(nn.Module):
    def __init__(self, class_num, se=False):
        super(My_ResNet18, self).__init__()
        self.residual_layer_nums = [2, 2, 6, 2]
        self.out_channels = [64, 64, 128, 256, 512]
        self.se = se
        self.in_proj = nn.Sequential(
            nn.Conv2d(3, 64, kernel_size=7, stride=2, padding=3),
            nn.BatchNorm2d(64),
            nn.ReLU()
        )
        self.residual_layer = []
        self.residual_layer.append(nn.MaxPool2d(kernel_size=3, stride=2, padding=1))
        for i in range(len(self.residual_layer_nums)):
            layer_num = self.residual_layer_nums[i]
            in_channel = self.out_channels[i]
            out_channel = self.out_channels[i + 1]
            self.residual_layer.append(self.make_layer(My_ResBlock, layer_num, in_channel, out_channel))
        self.residual_layer = nn.Sequential(*self.residual_layer)
        self.avg_pool = nn.AdaptiveAvgPool2d(1)
        self.fc = nn.Linear(512, class_num)

    def make_layer(self, block, layer_num, in_channel, out_channel):
        layer = []
        for i in range(layer_num):
            layer.append(block(in_channel, out_channel, stride=2 if in_channel != out_channel and i == 0 else 1,
                               se=self.se))
            in_channel = out_channel
        return nn.Sequential(*layer)

    def forward(self, x):
        out = self.in_proj(x)  # B, 64, 112, 112
        out = self.residual_layer(out)  # B, 512, 7, 7
        out = self.avg_pool(out).flatten(1)  # B, 512
        out = self.fc(out)  # B, class_num
        return out


if __name__ == '__main__':
    net = My_ResNet18(class_num=2, se=True)
    # print(net)
    x = torch.rand((2, 3, 224, 224))
    y = net(x)
    print(y)
