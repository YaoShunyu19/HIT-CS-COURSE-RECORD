import time
import torch
import torch.nn as nn
from torch.autograd import Variable


class RNN_Cell(nn.Module):
    def __init__(self, in_feature, out_feature, hidden_size=0, nonlinearity='tanh'):
        super(RNN_Cell, self).__init__()
        self.in_feature = in_feature
        self.hidden_size = in_feature if hidden_size == 0 else hidden_size
        self.out_feature = out_feature
        self.in2hidden = nn.Linear(self.in_feature + self.hidden_size, self.hidden_size)
        self.hidden2out = nn.Linear(self.hidden_size, self.out_feature)
        if nonlinearity == 'relu':
            self.nonlinearity = nn.ReLU()
        else:
            self.nonlinearity = nn.Tanh()

    def forward(self, x, hidden):
        # x: batch_size, in_feature   hidden: batch_size, hidden_size
        x_cat = torch.cat((x, hidden), 1)
        hidden = self.nonlinearity(self.in2hidden(x_cat))
        out = self.hidden2out(hidden)
        # out: batch_size, out_feature
        return out, hidden


class my_RNN(nn.Module):
    def __init__(self, in_feature, out_feature, hidden_size=0, nonlinearity='tanh', device=torch.device('cuda')):
        super(my_RNN, self).__init__()
        self.out_feature = out_feature
        self.hidden_size = in_feature if hidden_size == 0 else hidden_size
        self.device = device
        self.rnn_cell = RNN_Cell(in_feature, out_feature, hidden_size, nonlinearity).to(self.device)

    def forward(self, x, hidden_first=None):
        # x: batch_size, seq_len, in_feature   hidden: batch_size, hidden_size
        batch_size, seq_len, _ = x.shape
        x = x.permute((1, 0, 2))
        hidden = hidden_first if hidden_first is not None else Variable(torch.zeros(batch_size, self.hidden_size))
        hidden = hidden.to(self.device)
        out = torch.zeros((seq_len, batch_size, self.out_feature)).to(self.device)
        for i in range(seq_len):
            out[i], hidden = self.rnn_cell(x[i], hidden)
        return out.permute((1, 0, 2))


if __name__ == '__main__':
    device = torch.device('cuda')
    rnn = my_RNN(300, 10, 64, device=device)
    # r = torch.nn.RNN(300, 10).cuda()
    x = torch.rand((32, 32, 300)).to(device)
    t1 = time.time()
    y = rnn(x, None)
    t2 = time.time()
    print(t2 - t1)
    print(y[:, -1, :].shape)
