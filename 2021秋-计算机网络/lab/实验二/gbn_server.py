import socket
import gbn_si
import time

"""
gbn cs架构 服务器端
"""

address = ('', 8888)  # 端口号

receiver_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
receiver_socket.bind(address)
receiver = gbn_si.gbn_receiver(receiver_socket)

file = open('./server/gbn_' + str(int(time.time())) + '.jpg', 'ab')

while True:
    data, reset = receiver.wait_for_data()  # 等待客户端发送的数据包
    file.write(data)
    if reset:  # 发送结束
        receiver.expect_seq = 0
        file.close()
        break
