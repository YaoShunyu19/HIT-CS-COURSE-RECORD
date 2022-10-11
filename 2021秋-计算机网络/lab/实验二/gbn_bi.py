import socket
import time
import threading
import gbn_si

# 客户端配置
config_client = {
    'ADDRESS_SEND': ('127.0.0.1', 8888),
    'ADDRESS_RECEIVE': ('127.0.0.1', 8889),
    'DIR': './client'
}

# 服务器端配置
config_server = {
    'ADDRESS_SEND': ('127.0.0.1', 8889),
    'ADDRESS_RECEIVE': ('127.0.0.1', 8888),
    'DIR': './server'
}


def send(sender, dir):
    """
    发送数据包的方法
    :param sender: 发送端对象
    :param dir: 要发送的文件的目录
    :return: 无
    """
    file = open(dir + '/data.jpg', 'rb')

    dataList = []
    while True:
        data = file.read(2048)
        if len(data) <= 0:
            break
        dataList.append(data)
    file.close()
    print('PKT的总数' + str(len(dataList)))

    index = 0
    while True:
        while sender.next_seq < (sender.send_base + sender.size):
            if index >= len(dataList):
                break
            # 发送窗口未被占满
            data = dataList[index]
            checksum = gbn_si.cal_checksum(data)
            if index < len(dataList) - 1:
                sender.pkt[sender.next_seq] = sender.make_pkt(sender.next_seq, data, checksum,
                                                              if_stop=False)
            else:
                sender.pkt[sender.next_seq] = sender.make_pkt(sender.next_seq, data, checksum,
                                                              if_stop=True)
            print('发送了PKT' + str(index))
            sender.send_pkt(sender.pkt[sender.next_seq])
            sender.next_seq = (sender.next_seq + 1) % 256
            index += 1
        sender.wait_for_ack()
        if index >= len(dataList):
            break


def receive(receiver, dir):
    """
    接收数据包的方法
    :param receiver: 接收端对象
    :param dir: 接收文件的目录
    :return: 无
    """
    file = open(dir + '/gbn_bi_' + str(int(time.time())) + '.jpg', 'ab')
    while True:
        data, reset = receiver.wait_for_data()
        file.write(data)
        if reset:
            receiver.expect_seq = 0
            file.close()
            break


client_Receiver_Socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client_Receiver_Socket.bind(config_client['ADDRESS_RECEIVE'])
client_Receiver = gbn_si.gbn_receiver(client_Receiver_Socket)
# 客户端接收数据线程
thread1 = threading.Thread(target=receive, args=(client_Receiver, config_client['DIR']))
thread1.start()

server_Receiver_Socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_Receiver_Socket.bind(config_server['ADDRESS_RECEIVE'])
server_Receiver = gbn_si.gbn_receiver(server_Receiver_Socket)
# 服务器端接收数据线程
thread2 = threading.Thread(target=receive, args=(server_Receiver, config_server['DIR']))
thread2.start()

client_Sender_Socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# 客户端发送数据对象
client_Sender = gbn_si.gbn_sender(client_Sender_Socket, config_client['ADDRESS_SEND'])

server_Sender_Socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# 服务器端发送数据对象
server_Sender = gbn_si.gbn_sender(server_Sender_Socket, config_server['ADDRESS_SEND'])

send(client_Sender, config_client['DIR'])  # 客户端发送数据
send(server_Sender, config_server['DIR'])  # 服务器端发送数据
