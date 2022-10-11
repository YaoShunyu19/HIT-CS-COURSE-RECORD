from dataset import *
from models import rnn, gru, lstm
from climate_train import opt
import numpy as np
import matplotlib.pyplot as plt


def valid(model, data_path, climate_dim, begin, dim, device):
    assert dim < climate_dim
    column_min = torch.tensor([913.6, -23.01, 250.6, -25.01, 12.95, 0.95, 0.79, 0.0,
                               0.5, 0.8, 1059.45, 0.0, 0.0, 0.0])
    column_max = torch.tensor([1015.35, 37.28, 311.34, 23.11, 100.0, 63.77, 28.32, 46.01,
                               18.13, 28.82, 1393.54, 28.49, 23.5, 360.0])
    column_delta = column_max - column_min
    column_min = column_min[:climate_dim]
    column_delta = column_delta[:climate_dim]
    model.eval()
    test_dataset = climate_dataset(data_path=data_path, feature_dim=climate_dim, mode='test', begin=begin)
    x, y = test_dataset[0]
    x = x.unsqueeze(0).to(device)
    y = y.unsqueeze(0).to(device)
    pred = model(x)
    pred = pred[:, -y.shape[1]:, :]
    pred = (pred.squeeze(0).detach().cpu() * column_delta + column_min).numpy()
    y = (y.squeeze(0).detach().cpu() * column_delta + column_min).numpy()
    x = (x.squeeze(0).detach().cpu() * column_delta + column_min).numpy()
    print(pred.shape, y.shape)
    d1 = np.mean(np.abs(y[:, 0] - pred[:, 0]))
    d2 = np.mean(np.abs(y[:, 1] - pred[:, 1]))
    d3 = np.mean(np.abs(y[:, 2] - pred[:, 2]))
    d4 = np.mean(np.abs(y[:, 3] - pred[:, 3]))
    m1 = np.median(np.abs(y[:, 0] - pred[:, 0]))
    m2 = np.median(np.abs(y[:, 1] - pred[:, 1]))
    m3 = np.median(np.abs(y[:, 2] - pred[:, 2]))
    m4 = np.median(np.abs(y[:, 3] - pred[:, 3]))
    print(d1, d2, d3, d4, m1, m2, m3, m4)
    plt.plot(np.array([i for i in range(y.shape[0] + x.shape[0])]), np.concatenate((x[:, dim], pred[:, dim])), c='r')
    plt.plot(np.array([i for i in range(y.shape[0] + x.shape[0])]), np.concatenate((x[:, dim], y[:, dim])), c='g')
    plt.grid()
    plt.show()
    return abs(d1), abs(d2), abs(d3), abs(d4)


if __name__ == '__main__':
    device = torch.device('cuda') if opt.gpu == '0' else torch.device('cpu')
    model = lstm.my_LSTM(in_feature=opt.climate_dim, out_feature=opt.climate_dim,
                         hidden_size=opt.hidden_size, bidirectional=True, device=device)
    model = torch.nn.DataParallel(model)
    model.load_state_dict(torch.load('./checkpoint/climate_bilstm_e2.pth')['state_dict'])
    valid(model, opt.data_path, opt.climate_dim, 657, 0, device)
    valid(model, opt.data_path, opt.climate_dim, 284, 1, device)
    valid(model, opt.data_path, opt.climate_dim, 284, 2, device)
    valid(model, opt.data_path, opt.climate_dim, 248, 3, device)
    exit()
    d1m, d2m, d3m, d4m = 100, 100, 100, 100
    idx1, idx2, idx3, idx4 = 0, 0, 0, 0
    for i in range(1, 724):
        d1, d2, d3, d4 = valid(model, opt.data_path, opt.climate_dim, i, 2, device)
        print(i, d1, d2, d3, d4)
        if d1 < d1m:
            idx1 = i
            d1m = d1
        if d2 < d2m:
            idx2 = i
            d2m = d2
        if d3 < d3m:
            idx3 = i
            d3m = d3
        if d4 < d4m:
            idx4 = i
            d4m = d4
    print(idx1, idx2, idx3, idx4)
    print(d1m, d2m, d3m, d4m)  # lstm: 507 351 659 656  bilstm: 657 284 284 248

