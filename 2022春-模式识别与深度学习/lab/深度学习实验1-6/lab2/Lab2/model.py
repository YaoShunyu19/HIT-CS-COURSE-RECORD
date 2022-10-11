import torch
import torch.nn as nn
from torch.nn import init


class AlexNet(nn.Module):
    def __init__(self, output_num):
        super(AlexNet, self).__init__()
        self.extract_features = nn.Sequential(
            nn.Conv2d(3, 96, 11, 4, 1),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=3, stride=2),
            nn.LocalResponseNorm(size=5),
            nn.Conv2d(96, 256, 5, 1, 2),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=3, stride=2),
            nn.LocalResponseNorm(size=5),
            nn.Conv2d(256, 384, 3, 1, 1),
            nn.ReLU(),
            nn.Conv2d(384, 384, 3, 1, 1),
            nn.ReLU(),
            nn.Conv2d(384, 256, 3, 1, 1),
            nn.ReLU(),
            nn.MaxPool2d(3, 2),
        )
        self.head = nn.Sequential(
            nn.Linear(6400, 4096),
            nn.ReLU(),
            nn.Dropout(p=0.8),
            nn.Linear(4096, 4096),
            nn.ReLU(),
            nn.Dropout(p=0.8),
            nn.Linear(4096, output_num)
        )

    def _initialize_weights(self, x):
        for m in x():
            if isinstance(m, nn.Conv2d):
                init.orthogonal_(m.weight)
                print('init weight')
                if m.bias is not None:
                    init.constant_(m.bias, 1)

    def forward(self, x):
        features = self.extract_features(x)  # B, 256, 5, 5
        features = features.flatten(1)  # B, 6400
        out = self.head(features)
        return out


if __name__ == '__main__':
    alex = AlexNet(10)
    x = torch.rand((1, 3, 224, 224))
    y = alex(x)
    print(y.shape)
