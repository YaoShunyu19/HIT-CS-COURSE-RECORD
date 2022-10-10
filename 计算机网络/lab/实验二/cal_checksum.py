def cal_checksum(data):
    """
    计算校验和chechsum
    :param data: 数据
    :return: 校验和
    """
    length = len(str(data))
    checksum = 0
    for i in range(0, length):
        # 小端序，不考虑符号位
        checksum += int.from_bytes(bytes(str(data)[i], encoding='utf-8'), byteorder='little', signed=False)
        checksum &= 0xFF
    return checksum
