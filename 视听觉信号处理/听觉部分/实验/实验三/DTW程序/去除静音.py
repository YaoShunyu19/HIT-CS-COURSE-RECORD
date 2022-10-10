import wave
import math
import numpy as np
import os
import pylab as pl

params = ()


def readfile(filepath):
    f = wave.open(filepath, 'rb')
    global params
    params = f.getparams()
    str_data = f.readframes(params[3])
    f.close()
    wave_data = np.fromstring(str_data, dtype=np.short)
    return wave_data


def cal_energy(wave_data):
    step = 256
    wave_data_len = len(wave_data)
    frame_num = math.ceil(wave_data_len / step)
    energy = []
    for i in range(frame_num):
        frame = wave_data[np.arange(i * step, min(i * step + step, wave_data_len))]
        frame_mat = np.mat(frame)
        sum = np.longlong(frame_mat) @ np.longlong(frame_mat.T)
        energy.append(sum[0, 0])
    return energy


def write_ener(filename, ener):
    fw = open(filename, 'w')
    for i in range(len(ener)):
        m = str(ener[i]) + '\n'
        fw.writelines(m)
    fw.close()


def vad(wave_data, energy, energy_limit):
    wave_vad = []
    for i in range(len(energy) - 1):
        if energy[i] > energy_limit:
            wave_vad.extend(wave_data[i * 256:i * 256 + 255])
    return wave_vad


def write_vad(filename, wave_vad_final, framerate):
    fw = wave.open(filename, 'wb')
    fw.setnchannels(1)  # 配置声道数
    fw.setsampwidth(2)  # 配置量化位数
    fw.setframerate(framerate)  # 配置取样频率
    fw.writeframes(wave_vad_final.tostring())  # 转换为二进制数据写入文件
    fw.close()


if __name__ == '__main__':
    path = 'D:\\2021秋\\视听觉信号处理\\听觉部分\\实验\\命令词识别实验3\\语料\\'
    save_path = 'D:\\2021秋\\视听觉信号处理\\听觉部分\\实验\\命令词识别实验3\\去除静音的语料\\'
    for n, name in enumerate(os.listdir(path)):
        filepath = path + name
        print(filepath)
        wave_data = readfile(filepath)
        energy = cal_energy(wave_data)
        framerate = params[2]  # 采样频率
        enerlimit = 15000000  # 能量门限
        wave_vad = vad(wave_data, energy, enerlimit)
        wave_vad_final = np.array(wave_vad).astype(np.short)

        vad_filename = save_path + name
        write_vad(vad_filename, wave_vad_final, framerate)
