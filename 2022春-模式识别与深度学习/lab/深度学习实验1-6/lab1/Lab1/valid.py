import torch
from dataset import *
from tqdm import tqdm
from model import *


def valid(model, test_data_path, test_label_path):
    print('\n-----Valid-----')
    model.eval()
    test_dataset = mnist_dataset(data_path=test_data_path, label_path=test_label_path)
    pred_true = 0
    pred_sum = len(test_dataset)
    for i, (image, label) in enumerate(test_dataset, 1):
        image = torch.tensor(image).unsqueeze(0).flatten(1).type(torch.FloatTensor).detach().cuda()
        label = torch.tensor(label).long().detach().cuda()
        pred = torch.argmax(model(image), dim=1)
        print('valid: [{}/{}]'.format(i, pred_sum))
        if pred.item() == label.item():
            pred_true += 1
        if i == len(test_dataset):
            break
    acc = pred_true / pred_sum
    # print(acc)
    return acc


if __name__ == '__main__':
    net = MLP().cuda()
    net.load_state_dict(torch.load('./checkpoint/mlp_mnist_e8.pth')['state_dict'])
    acc = valid(net, '../../datasets/MNIST/t10k-images.idx3-ubyte', '../../datasets/MNIST/t10k-labels.idx1-ubyte')
    print(acc)
