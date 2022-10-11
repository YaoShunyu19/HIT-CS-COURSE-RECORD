import time
import torch
import numpy as np
import torch.nn as nn
import torch.optim as optim
from torch.autograd import Variable


class LSTM_Cell(nn.Module):
    def __init__(self, in_feature, out_feature, hidden_size=0, bidirectional=False):
        super(LSTM_Cell, self).__init__()
        self.in_feature = in_feature
        self.hidden_size = in_feature if hidden_size == 0 else hidden_size
        self.out_feature = out_feature
        self.bidirectional = bidirectional
        self.w_f = nn.Linear(in_feature + hidden_size, hidden_size)
        self.w_i = nn.Linear(in_feature + hidden_size, hidden_size)
        self.w_c = nn.Linear(in_feature + hidden_size, hidden_size)
        self.w_o = nn.Linear(in_feature + hidden_size, hidden_size)
        self.sigmoid = nn.Sigmoid()
        self.tanh = nn.Tanh()
        if self.bidirectional:
            pass
        else:
            self.out = nn.Linear(hidden_size, out_feature)

    def forward(self, x, hidden, C_t_1):
        # x: batch_size, in_feature   hidden: batch_size, hidden_size
        x_cat = torch.cat((x, hidden), dim=1)
        f_t = self.sigmoid(self.w_f(x_cat))
        i_t = self.sigmoid(self.w_i(x_cat))
        C_t_ = self.tanh(self.w_c(x_cat))
        C_t = f_t * C_t_1 + i_t * C_t_
        o_t = self.sigmoid(self.w_o(x_cat))
        hidden = o_t * self.tanh(C_t)
        out = self.out(hidden) if not self.bidirectional else None
        # out: batch_size, out_feature
        return out, hidden, C_t


class my_LSTM(nn.Module):
    def __init__(self, in_feature, out_feature, hidden_size=0, bidirectional=False, device=torch.device('cuda')):
        super(my_LSTM, self).__init__()
        self.out_feature = out_feature
        self.hidden_size = in_feature if hidden_size == 0 else hidden_size
        self.bidirectional = bidirectional
        self.device = device
        if self.bidirectional:
            self.lstm_cell_1 = LSTM_Cell(in_feature, out_feature, hidden_size, bidirectional=True).to(self.device)
            self.lstm_cell_2 = LSTM_Cell(in_feature, out_feature, hidden_size, bidirectional=True).to(self.device)
            self.out = nn.Linear(2 * hidden_size, out_feature).to(self.device)
        else:
            self.lstm_cell = LSTM_Cell(in_feature, out_feature, hidden_size, bidirectional=False).to(self.device)

    def forward(self, x, hidden_first=None, C_first=None):
        # x: batch_size, seq_len, in_feature   hidden: batch_size, hidden_size
        batch_size, seq_len, _ = x.shape
        x = x.permute((1, 0, 2))
        hidden = hidden_first if hidden_first is not None else Variable(torch.zeros(batch_size, self.hidden_size))
        hidden = hidden.to(self.device)
        hidden1, hidden2 = hidden, hidden
        C_t = C_first if C_first is not None else Variable(torch.zeros(batch_size, self.hidden_size))
        C_t = C_t.to(self.device)
        C_t1, C_t2 = C_t, C_t
        out = torch.zeros((seq_len, batch_size, self.out_feature)).to(self.device)
        if self.bidirectional:
            for i in range(seq_len):
                _, hidden1, C_t1 = self.lstm_cell_1(x[i], hidden1, C_t1)
                _, hidden2, C_t2 = self.lstm_cell_2(x[seq_len - i - 1], hidden2, C_t2)
                out[i] = self.out(torch.cat((hidden1, hidden2), dim=1))
        else:
            for i in range(seq_len):
                out[i], hidden, C_t = self.lstm_cell(x[i], hidden, C_t)
        return out.permute((1, 0, 2))


if __name__ == '__main__':
    device = torch.device('cuda')
    rnn = my_LSTM(300, 10, 64, bidirectional=True, device=device)
    # r = torch.nn.RNN(300, 10).cuda()
    x = torch.rand((32, 32, 300)).to(device)
    t1 = time.time()
    y = rnn(x, None)
    t2 = time.time()
    print(t2 - t1)
    print(y[:, -1, :].shape)
