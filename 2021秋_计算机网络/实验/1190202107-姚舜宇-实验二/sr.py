import random
import socket
import struct
from cal_checksum import *

# 配置信息
config = {
    'TIME_OUT': 10,  # 超时时间
    'SIZE': 7,  # 窗口大小
    'LOSS_RATE': 0.05,  # 丢包率
    'BUFFER_SIZE': 4096  # 接收容量
}


class sr_sender:
    """
    SR发送方
    """
    def __init__(self, sender_socket, address, time_out=config['TIME_OUT'],
                 size=config['SIZE'], rate=config['LOSS_RATE']):
        self.sender_socket = sender_socket
        self.time_out = time_out
        self.address = address
        self.size = size
        self.rate = rate
        self.send_base = 0
        self.next_seq = 0
        self.pkt = [None] * 256
        self.ack = [False] * 256

    def send_pkt(self, pkt):
        """
        发送pkt
        :param pkt: 数据包
        :return: 无
        """
        if self.rate == 0 or random.randint(0, int(1 / self.rate)) != 1:
            # 不丢包
            print('发送了PKT' + str(pkt[0]))
            self.sender_socket.sendto(pkt, self.address)
        else:
            # 丢包
            print('PKT' + str(pkt[0]) + '丢失')

    def wait_for_ack(self):
        """
        等待ack确认
        :return: 是否收到ack
        """
        self.sender_socket.settimeout(self.time_out)
        time_out_count = 0
        while True:
            # 连续超时5次则终止
            if time_out_count >= 5:
                break
            try:
                ack, address = self.sender_socket.recvfrom(config['BUFFER_SIZE'])
                ack_seq, expect_seq = self.get_ack_info(ack)
                print('期待收到ACK' + str(expect_seq) + '  收到ACK' + str(ack_seq))

                self.send_base = max(self.send_base, (ack_seq + 1) % 256)
                if ack_seq in range(self.send_base, self.send_base + self.size):
                    self.ack[ack_seq] = True

                if ack_seq == self.send_base:
                    # 滑动窗口
                    while self.ack[self.send_base]:
                        self.send_base = (self.send_base + 1) % 256
                        # 新滑动进来的窗口单元需要初始化
                        self.ack[self.send_base + self.size] = False

                if self.send_base == self.next_seq:  # 已发送分组确认完毕
                    self.sender_socket.settimeout(None)
                    return True
            except socket.timeout:
                # 超时，重传分组
                time_out_count += 1
                print('超时，等待')
                for i in range(self.send_base, self.next_seq):
                    print('重传PKT' + str(i))
                    self.send_pkt(self.pkt[i])
                self.sender_socket.settimeout(self.time_out)
        return False

    def make_pkt(self, seq_num, data, checksum, if_stop):
        """
        制作pkt
        :param seq_num: 序号
        :param data: 数据
        :param checksum: 校验和
        :param if_stop: 是否是最后一个数据包
        :return: 数据包
        """
        flag = 1 if if_stop else 0
        return struct.pack('BBB', seq_num, flag, checksum) + data

    def get_ack_info(self, ack):
        """
        获得ack的信息
        :param ack: 确认消息
        :return: 序号，期望序号
        """
        ack_seq = ack[0]
        expect_seq = ack[1]
        return ack_seq, expect_seq


class sr_receiver:
    """
    SR接收方
    """
    def __init__(self, receiver_socket, time_out=config['TIME_OUT'], size=config['SIZE'], rate=config['LOSS_RATE']):
        self.receiver_socket = receiver_socket
        self.time_out = time_out
        self.size = size
        self.rate = rate
        self.recv_base = 0
        self.recvs = [None] * 256
        self.target = None

    def send_ack(self, ack):
        """
        发送ack
        :param ack: 确认消息
        :return: 无
        """
        if self.rate == 0 or random.randint(0, int(1 / self.rate)) != 1:
            # 不丢包
            self.receiver_socket.sendto(ack, self.target)
            print('发送ACK' + str(ack[0]))
        else:
            # 丢包
            print('ACK' + str(ack[0]) + '丢失')

    def wait_for_data(self):
        """
        等待发送方发送的数据包
        :return: 数据，是否是最后一个数据包
        """
        self.receiver_socket.settimeout(self.time_out)
        while True:
            try:
                pkt, address = self.receiver_socket.recvfrom(config['BUFFER_SIZE'])
                self.target = address
                seq_num, flag, checksum, data = self.get_pkt_info(pkt)
                print('接收到PKT' + str(seq_num))
                if seq_num in range(self.recv_base, self.recv_base + self.size) \
                        and cal_checksum(data) == checksum:
                    # 写入缓存，返回ACK
                    print('缓存PKT' + str(seq_num))
                    self.recvs[seq_num] = data
                    ack_pkt = self.make_ack(seq_num, seq_num)
                    self.send_ack(ack_pkt)

                    while self.recvs[self.recv_base] is not None:
                        # 滑动窗口并递交数据
                        self.recv_base = (self.recv_base + 1) % 256
                        self.recvs[self.recv_base + self.size] = None  # 新划入的单元要初始化

                    if flag:    # 最后一个数据包
                        return data, True
                    else:
                        return data, False
                else:
                    # 只返回ACK，不缓存
                    ack_pkt = self.make_ack(seq_num, seq_num)
                    self.send_ack(ack_pkt)
            except socket.timeout:
                # 超时
                return bytes('', encoding='utf-8'), False

    def make_ack(self, ack_seq, expect_seq):
        """
        制作ack
        :param ack_seq: 序号
        :param expect_seq: 期待序号
        :return: ack
        """
        return struct.pack('BB', ack_seq, expect_seq)

    def get_pkt_info(self, pkt):
        """
        获取pkt信息
        :param pkt: 数据包
        :return: 序号，是否是最后一个，校验和，数据
        """
        seq_num = pkt[0]
        flag = pkt[1]
        checksum = pkt[2]
        data = pkt[3:]
        return seq_num, flag, checksum, data
