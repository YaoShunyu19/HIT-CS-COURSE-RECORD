import torch
from dataset import *
from tqdm import tqdm
from models import rnn, gru, lstm


def valid(model, test_data_path, word2vec_path, padding_size, mode, device):
    print('\n-----Valid-----')
    eps = 1e-5
    model.eval()
    test_dataset = shoppint_dataset(txt_path=test_data_path, word_path=word2vec_path,
                                    padding_size=padding_size, mode=mode)
    pred_sum = len(test_dataset)

    confusion_matrix = torch.zeros((model.module.out_feature, model.module.out_feature))

    for i, (cls, seq) in enumerate(test_dataset, 1):
        seq = seq.unsqueeze(0).type(torch.FloatTensor).detach().to(device)
        cls = torch.tensor(cls).long().detach().to(device)
        pred = model(seq)
        pred = torch.argmax(pred[:, -1, :], dim=1)
        if i % 100 == 0:
            print('valid: [{}/{}]'.format(i, pred_sum))
        confusion_matrix[cls.item(), pred.item()] += 1
        if i == len(test_dataset):
            break
    accuracy = torch.trace(confusion_matrix) / torch.sum(confusion_matrix)
    precision = torch.mean(torch.tensor([confusion_matrix[i, i] / (torch.sum(confusion_matrix[:, i]) + eps)
                                         for i in range(model.module.out_feature)]))
    recall = torch.mean(torch.tensor([confusion_matrix[i, i] / (torch.sum(confusion_matrix[i, :]) + eps)
                                      for i in range(model.module.out_feature)]))
    F1 = 2 * precision * recall / (precision + recall)
    print('Accuracy: {:.4f},  Precision: {:.4f},  Recall: {:.4f},  F1: {:.4f}'.format(accuracy, precision, recall, F1))
    return accuracy


if __name__ == '__main__':
    from shopping_train import opt

    device = torch.device('cuda') if opt.gpu == '0' else torch.device('cpu')
    model = lstm.my_LSTM(in_feature=opt.word_dim, out_feature=opt.num_classes,
                         hidden_size=opt.hidden_size, bidirectional=True, device=device)
    model = torch.nn.DataParallel(model)
    model.load_state_dict(torch.load('./checkpoint/shopping_bilstm_e3.pth')['state_dict'])
    acc1 = valid(model, opt.data_path, opt.word2vec_path, opt.seq_len, mode='test', device=device)
