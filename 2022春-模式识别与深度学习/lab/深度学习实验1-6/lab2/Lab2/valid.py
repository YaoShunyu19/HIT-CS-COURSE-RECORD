import torch
from dataset import *
from tqdm import tqdm
from model import *


def valid(model, test_data_path, classes_info, mode):
    print('\n-----Valid-----')
    model.eval()
    transform = transforms.Compose([
        # transforms.CenterCrop([224, 224]),
        transforms.Resize([224, 224]),
        transforms.ToTensor()
    ])
    test_dataset = caltech_dataset(data_path=test_data_path, classes_info=classes_info, mode=mode, transform=transform)
    pred_true = 0
    pred_sum = len(test_dataset)
    for i, (image, label) in enumerate(test_dataset, 1):
        image = image.unsqueeze(0).type(torch.FloatTensor).detach().cuda()
        label = torch.tensor(label).long().detach().cuda()
        pred = torch.argmax(model(image), dim=1)
        # print(pred, label)
        print('valid: [{}/{}]'.format(i, pred_sum))
        if pred.item() == label.item():
            pred_true += 1
        if i == len(test_dataset):
            break
    acc = pred_true / pred_sum
    # print(acc)
    return acc


if __name__ == '__main__':
    net = AlexNet(output_num=101).cuda()
    net.load_state_dict(torch.load('./checkpoint/alexnet_caltech-101_e25.pth')['state_dict'])
    acc1 = valid(net, '../../datasets/caltech-101/101_ObjectCategories', 'classes.txt', 'valid')
    acc2 = valid(net, '../../datasets/caltech-101/101_ObjectCategories', 'classes.txt', 'test')
    print(acc1)
    print(acc2)
