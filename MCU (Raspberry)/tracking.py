import serial
import time

import numpy as np, cv2, sys

sys.path.append('../../../')
import hsapi as hs

device = hs.GetDevice()
device.OpenDevice()

# 打开串口
ser = serial.Serial("/dev/ttyAMA0", 115200)
#x是16进制数：F:0x46；B:0x42；R:0x52；L:0x49；
def send(x):
    #字符发送
    ser.write(bytes([0x80]))
    ser.write(bytes([x]))
    ser.write(bytes([0x81]))
    ser.write(bytes([0x81]))



def receive():
    # 获得接收缓冲区字符
    count = ser.inWaiting()
    data1=bytes([0x30])
    if count != 0:
        # 判断命令开始
        recv = ser.read(1)
        if recv == bytes([0x80]):
            #如果是，读入数据
            count1 = ser.inWaiting()
            while count1 != 0:
                data = ser.read(3)
                #判断结尾
                if data.endswith(bytes([0x81, 0x81])):
                    data1 = data[0]

    return data1
#黑线中心分析，输出偏差角tan
def blackline(imag, th, wi, hi):
    mid = [[0,0]]*10
    #黑线中心计算
    for i1 in range(1, 10):
        i = int(i1*hi) / 10 - 1
        j = 0
        while (imag[i, j] > th) and (j < wi) or (imag[i, j+3] > th):
            j=j+1
        j1=j
        while imag[i, j] < th and j < wi or imag[i, j+3] < th:
            j1 = j1+1
        mid[i1-1, :] = [i, int((j+j1)/2)]
    #偏差角计算
    tan = 0
    for ii in range(1, 5):
        tan += (mid(ii+4, 2) - mid(ii-1, 2))*10 / hi
    tan = tan / 5
    return tan



#图像读取分析
def imageanalyze():
    th = 50  # 灰度阈值
    th1 = 1  # 角度阈值
    img = device.GetImage(False)
    img1 = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)#灰度化
    #图像缩小
    sp = img1.shape
    wi = int(sp[1] * 0.2)
    hi = int(sp[0] * 0.2)
    img2 = cv2.resize(img1, (wi, hi))
    #图像分析
    tan = blackline(img2, th, wi, hi)
    # second = tan / 20
    if tan > th1:
        mes = 0x52
    if tan < -th1:
        mes = 0x49
    if tan < th1 and tan > -th1:
        mes = 0x46

    return mes  # , second


def main():
    while True:
        #判断是否开始
        sign =receive()
        if sign == '1':#bytes([0x01])
            while True:
                mes = imageanalyze()
                send(mes)
                sign1=receive()
                #判断是否结束
                if sign1 == '2':
                    break
                time.sleep(0.1)




net.quit()
if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        if ser != None:
            ser.close()