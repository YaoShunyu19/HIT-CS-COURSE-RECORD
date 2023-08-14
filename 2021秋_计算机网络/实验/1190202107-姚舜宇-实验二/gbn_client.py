import socket
import gbn_si

"""
gbn cs架构 客户端
"""

# IP，端口号
address = ('127.0.0.1', 8888)

sender_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sender = gbn_si.gbn_sender(sender_socket, address=address)

data_list = []  # 存储数据
file = open('./client/data.jpg', 'rb')

while True:
    d = file.read(2048)
    if len(d) <= 0:
        break
    data_list.append(d)
file.close()
print('PKT的总数：' + str(len(data_list)))

index = 0
while True:
    while sender.next_seq < (sender.send_base + sender.size):
        if index >= len(data_list):
            break
        data = data_list[index]
        checksum = gbn_si.cal_checksum(data)
        if index < len(data_list) - 1:  # 不是最后一个数据包
            sender.pkt[sender.next_seq] = sender.make_pkt(sender.next_seq, data, checksum, False)
        else:  # 是最后一个数据包
            sender.pkt[sender.next_seq] = sender.make_pkt(sender.next_seq, data, checksum, True)
        sender.send_pkt(sender.pkt[sender.next_seq])  # 发送数据
        sender.next_seq = (sender.next_seq + 1) % 256
        index += 1
    sender.wait_for_ack()  # 每次发送数据后等待ack
    if index >= len(data_list):
        break
