import serial
import time
import multiprocess as mp
# import queue

import numpy as np, cv2, sys

sys.path.append('../../../')
import hsapi as hs

device = hs.GetDevice()
enable = False
queueLock = mp.Lock()
imgQueue = mp.Queue(0)

# 打开串口
ser = serial.Serial("/dev/ttyAMA0", 115200)


# x是16进制数：F:0x46；B:0x42；R:0x52；L:0x49；
def send(x):
    # 字符发送
    ser.write(bytes([0x80]))
    ser.write(bytes([x]))
    ser.write(bytes([0x81]))
    ser.write(bytes([0x81]))


def receive():
    # 获得接收缓冲区字符
    count = ser.inWaiting()
    data1 = bytes([0x30])
    if count != 0:
        # 判断命令开始
        recv = ser.read(1)
        if recv == bytes([0x80]):
            # 如果是，读入数据
            count1 = ser.inWaiting()
            if count1 != 0:
                data = ser.read(3)
                # 判断结尾
                if data.endswith(bytes([0x81, 0x81])):
                    data1 = data[0]
                    # print(data1)
    return data1


# 黑线中心分析，输出偏差角tan
def blackline(imag, th, wi, hi):
    mid = np.array([[0, 0]] * 10)
    # 黑线中心计算
    i2 = 0
    for i1 in range(1, 10):
        i = int((i1 + 10) * hi / 30) - 1
        j = 0

        while ((imag[i, j] > th) or (imag[i, j + 3] > th)) and (j < wi - 3):
            j = j + 1
            
        j1 = j
        
        while (imag[i, j1] < th)  and j1 < wi - 3:
            j1 = j1 + 1
        #print(j1)
        print('j1 is ok')
        if j < wi - 3:
            mid[i2] = [i, int((j + j1) / 2)]
            i2 = i2 + 1

    print('--')

    # 偏差角计算
    if i2 < 2:
        tan = -100
    else:
        tan = 0
        m = int(i2 / 2)
        print('m=')
        print(m)
        for ii in range(0, m):
            tan += (mid[ii + m, 1] - mid[ii, 1]) * 30 / hi
        tan = tan / m
    print(tan)
    return tan


# 图像读取分析
def imageanalyze():
    global device
    th = 50  # 灰度阈值
    th1 = 10  # 角度阈值
    img = device.GetImage(True)
    img1 = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # 灰度化
    # 图像缩小
    sp = img1.shape
    wi = int(sp[1] * 0.8)
    hi = int(sp[0] * 0.8)
    img2 = cv2.resize(img1, (wi, hi))
    queueLock.acquire()
    imgQueue.put(img2)
    queueLock.release()
    # 图像分析
    tan = blackline(img2, th, wi, hi)

    mes = 0x52
    # second = tan / 20
    if tan > th1:
        mes = 0x52  # R
    if tan <  - th1:
        mes = 0x4c  # L
    if tan <= th1 and tan >= -th1:
        mes = 0x46  # F

    return mes  # , second


def motionControl():
    global device, enable
    try:
        while enable:
            mes = imageanalyze()
            send(0x53)
            send(mes)
            # time.sleep(0.5)
        device.CloseDevice()
    except:
        enable = False
       
        send(0x45)  # some wrong with device, send error message and quit the thread


def displayImage():
    global enable
    while enable:
        img = None
        queueLock.acquire()
        if not imgQueue.empty():
            img = imgQueue.get()
            queueLock.release()
        else:
            queueLock.release()
        if img is not None:
            cv2.imshow('Tracking', img)
            cv2.waitKey(1)
        #print(enable)
    cv2.destroyAllWindows()


def main():
    global device, enable
    while True:
        # 判断是否开始
        sign = receive()
        if sign == 1:  # bytes([0x01])
            print('connect')
            try:
                device.OpenDevice()
                enable = True
                mp.Process(target=motionControl).start()
                mp.Process(target=displayImage).start()
            except:
                enable = False
                device = hs.GetDevice()
                print('main')
                send(0x45)  # device not found, send error message
        elif sign == 2:
            print('disconnect')
            enable = False


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        if ser != None:
            ser.close()
        device.CloseDevice()

