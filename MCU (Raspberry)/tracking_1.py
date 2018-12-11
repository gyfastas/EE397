import serial
import time
import multiprocess as mp
#import queue
from picamera import PiCamera
import picamera.array
import numpy as np, cv2, sys

# camera = PiCamera()
# output = picamera.array.PiRGBArray(camera)
# camera.resolution = (320, 240) # 图像大小
# camera.framerate=10 # 帧
# 定义sobel算子
sobel_r = np.array([[0,0,0],[-1,0,1],[0,0,0]])
sobel_l = np.array([[0,0,0],[1,0,-1],[0,0,0]])

'''
垂直边缘检测
'''
def edgeDetection(img):
    '''

    :param img: 二值化之后的图像
    :return: 边缘检测图像
    '''
    edge_L = cv2.filter2D(img,sobel_l)
    edge_R = cv2.filter2D(img,sobel_r)
    edge = edge_L + edge_R

    return edge



queueLock = mp.Lock()
queueLockgray = mp.Lock()

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
    data1 = bytes([0x30])
    if count != 0:
        # 判断命令开始
        recv = ser.read(1)
        if recv == bytes([0x80]):
            #如果是，读入数据
            count1 = ser.inWaiting()
            if count1 != 0:
                data = ser.read(3)
                #判断结尾
                if data.endswith(bytes([0x81, 0x81])):
                    data1 = data[0]
                    #print(data1)
    return data1
 
#黑线中心分析，输出偏差角tan
def blackline(imag, th, wi, hi):
    '''

    :param imag: 图像矩阵
    :param th:   角度阈值
    :param wi:   图像的宽度
    :param hi:   图像的高度
    :return: imag: 标注完点的图像
             tan: 计算的角度
    '''
    mid = np.array([[0,0]]*10)
    #黑线中心计算
    i2 = 0
    for i1 in range(1, 10):
        i = int((i1+70)*hi / 90) - 1
        j = 0

        while ((imag[i, j] > th) or (imag[i, j+3] > th)) and (j < wi- 3):
            j=j+1
        j1=j
        while (imag[i, j1] < th)and j1 < wi-3:
            j1 = j1+1
        if j < wi-3:
            mid[i2] = [i,int((j+j1)/2)]
            imag[i,int((j+j1)/2)] = 255
            i2 = i2 +1
            
    #偏差角计算
    if i2 < 2:
        tan = -100
    else:
        tan = 0
        m = int(i2/2)
        for ii in range(0, m):
            tan += (mid[ii+m, 1] + mid[ii, 1]) - 2*int(wi/2)
        tan = tan / m /2
    print('tan=')
    print(tan)
    return imag,tan

#图像读取分析
def imageanalyze(imgQueue,img,imgQueuegray):
    th = 80  # 灰度阈值
    th1 = 20  # 角度阈值
    img1 = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)#灰度化
    #图像缩小
    sp = img1.shape
    wi = int(sp[1] * 0.8)
    hi = int(sp[0] * 0.8)
    img2 = cv2.resize(img1, (wi, hi))
    thre1 = 80
    ret, thresh1 = cv2.threshold(img2, thre1, 255, cv2.THRESH_BINARY)
    queueLockgray.acquire()
    imgQueuegray.put(thresh1)
    queueLockgray.release()
    
    #图像分析
    [img3,tan] = blackline(img2, th, wi, hi)
    queueLock.acquire()
    imgQueue.put(img3)
    queueLock.release()
    mes = 0x53
    if tan > th1:
       mes = 0x4c#2 #R
    if tan < 0-th1:
       mes = 0x52#0x4c #L

    if tan <= th1 and tan >= -th1:
       mes = 0x46#0x46#F
    return mes  # , second

def motionControl(enable, imgQueue,imgQueuegray):
    
    camera = PiCamera()
    output = picamera.array.PiRGBArray(camera)
    camera.resolution = (320, 240) # 图像大小
    camera.framerate=40 # 帧
    while enable.value == 1:
        for frame in camera.capture_continuous(output, format="bgr", use_video_port=True):
            img = output.array
            mes = imageanalyze(imgQueue,img,imgQueuegray)
            output.truncate(0)
            send(0x53)
            send(mes)
    # except:
    #     enable.value = 0
    #     print('control')
    #     send(0x45) # some wrong with device, send error message and quit the thread
    camera.close()
    print('Quit motionControl')

def displayImage(enable, imgQueue,imgQueuegray):
    while enable.value == 1:
        img = None
        img2 = None
        queueLock.acquire()
        if not imgQueue.empty():
            img = imgQueue.get()
            queueLock.release()
        else:
            queueLock.release()
        if img is not None:
            cv2.imshow('Tracking', img)
            cv2.waitKey(1)
        #二值化图
        queueLockgray.acquire()
        if not imgQueuegray.empty():
            img2 = imgQueuegray.get()
            queueLockgray.release()
        else:
            queueLockgray.release()
        if img2 is not None:
            cv2.imshow('Trackinggray', img2)
            cv2.waitKey(1)
    print('Quit displayImage')
    cv2.destroyAllWindows()

def main():
    enable = mp.Value('i', 0)
    imgQueue = mp.Queue(0)
    imgQueuegray = mp.Queue(0)
    while True:
        #判断是否开始
        sign = receive()
        if sign == 1:#bytes([0x01])
            print('connect')
            # try:
            enable.value = 1
            mp.Process(target=motionControl, args=(enable, imgQueue,imgQueuegray)).start()
            mp.Process(target=displayImage, args=(enable, imgQueue,imgQueuegray)).start()
            # except:
            #     enable.value = 0
            #     print('main')
            #     send(0x45)   # device not found, send error message
        elif sign == 2:
            print('disconnect')
            enable.value = 0

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        if ser != None:
            ser.close()
        camera.close()

