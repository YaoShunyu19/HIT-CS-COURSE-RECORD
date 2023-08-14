import time
import torch
import numpy as np
import torch.nn as nn
import torch.optim as optim
from torch.autograd import Variable


class GRU_Cell(nn.Module):
    def __init__(self, in_feature, out_feature, hidden_size=0):
        super(GRU_Cell, self).__init__()
        self.in_feature = in_feature
        self.hidden_size = in_feature if hidden_size == 0 else hidden_size
        self.out_feature = out_feature

        self.w_xr = nn.Linear(in_feature, hidden_size)
        self.w_hr = nn.Linear(hidden_size, hidden_size)
        self.w_xz = nn.Linear(in_feature, hidden_size)
        self.w_hz = nn.Linear(hidden_size, hidden_size)
        self.w_xh = nn.Linear(in_feature, hidden_size)
        self.w_hh = nn.Linear(hidden_size, hidden_size)
        self.sigmoid = nn.Sigmoid()
        self.tanh = nn.Tanh()
        self.out = nn.Linear(hidden_size, out_feature)

    def forward(self, x, hidden):
        # x: batch_size, in_feature   hidden: batch_size, hidden_size
        Rt = self.sigmoid(self.w_xr(x) + self.w_hr(hidden))
        Zt = self.sigmoid(self.w_xz(x) + self.w_hz(hidden))
        Ht_ = self.tanh(self.w_xh(x) + self.w_hh(Rt * hidden))
        hidden = Zt * hidden + (1 - Zt) * Ht_
        out = self.out(hidden)
        # out: batch_size, out_feature
        return out, hidden


class my_GRU(nn.Module):
    def __init__(self, in_feature, out_feature, hidden_size=0, device=torch.device('cuda')):
        super(my_GRU, self).__init__()
        self.out_feature = out_feature
        self.hidden_size = in_feature if hidden_size == 0 else hidden_size
        self.device = device
        self.gru_cell = GRU_Cell(in_feature, out_feature, hidden_size).to(self.device)

    def forward(self, x, hidden_first=None):
        # x: batch_size, seq_len, in_feature   hidden: batch_size, hidden_size
        batch_size, seq_len, _ = x.shape
        x = x.permute((1, 0, 2))
        hidden = hidden_first if hidden_first is not None else Variable(torch.zeros(batch_size, self.hidden_size))
        hidden = hidden.to(self.device)
        out = torch.zeros((seq_len, batch_size, self.out_feature)).to(self.device)
        for i in range(seq_len):
            out[i], hidden = self.gru_cell(x[i], hidden)
        return out.permute((1, 0, 2))


if __name__ == '__main__':
    device = torch.device('cuda')
    rnn = my_GRU(300, 10, 64, device=device)
    # r = torch.nn.RNN(300, 10).cuda()
    x = torch.rand((32, 32, 300)).to(device)
    t1 = time.time()
    y = rnn(x, None)
    t2 = time.time()
    print(t2 - t1)
    print(y[:, -1, :].shape)
