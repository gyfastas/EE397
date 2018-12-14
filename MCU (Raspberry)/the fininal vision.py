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
def blackline(imag, thresh_grey, image_width, image_height,flag,last):
    
    mid = np.array([[0,0]]*10)
    #黑线中心计算
    nb_of_midpoints = 0
    for i1 in range(1, 10):
        i = int((i1+75)*image_height / 90) - 1
        j = 0

        while ((imag[i, j] > thresh_grey) or (imag[i, j+3] > thresh_grey)) and (j < image_width-4):
            j=j+1
        j1=j
        while (imag[i, j1] < thresh_grey)and j1 < image_width-4:
            j1 = j1+1
        if j < image_width-4:
            mid[nb_of_midpoints] = [i,int((j+j1)/2)]
            imag[i,int((j+j1)/2)] = 255
            nb_of_midpoints = nb_of_midpoints +1
           
    #偏距离计算
    if nb_of_midpoints < 2:
        dist = -100
    else:
        dist = 0
        for ii in range(0, nb_of_midpoints-1):
            dist += (mid[ii, 1]) - int(image_width/2)
        dist = dist /nb_of_midpoints
    #print('in last {}'.format(last))   
    #前20行搜索
    if last <2:
        last = 0
        nb_of_midpoints = 0
        mid_turn = np.array([[0,0]]*10)
        for i1 in range(1, 10):
            i = int((i1+55)*image_height / 90) - 1
            j = 0

            while ((imag[i, j] > thresh_grey) or (imag[i, j+3] > thresh_grey)) and (j < image_width-4):
                j=j+1
            j1=j
            while (imag[i, j1] < thresh_grey)and j1 < image_width-4:
                j1 = j1+1
            if j < image_width-4:
                mid_turn[nb_of_midpoints] = [i,int((j+j1)/2)]
                imag[i,int((j+j1)/2)] = 255
                nb_of_midpoints = nb_of_midpoints +1
               
        #转弯判断
        if nb_of_midpoints > 2:
            midi = 0
            for ii in range(0, nb_of_midpoints-1):
                midi += mid_turn[ii, 1]
            midi = midi/nb_of_midpoints #judge the core of line location
            if (((mid_turn[0, 1]-mid_turn[nb_of_midpoints-1, 1])>40) and midi>int(image_width/2)):#right turn
                flag = 0
                last = 30
            elif(((mid_turn[0, 1]-mid_turn[nb_of_midpoints-1, 1])<-40) and midi<int(image_width/2)):#left turn
                flag = 1
    else :
        last = last -1
    #print('in flag {}'.format(flag))
    print('dist = {}'.format(dist))
    return flag,imag,dist,last

#图像读取分析
def imageanalyze(imgQueue,img,imgQueuegray,flag,last):
    #flag =1  #判断左转还是右转，0为右转
    thresh_grey = 100  # 灰度阈值
    thresh_angle = 25  # 角度阈值
    grey_image = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)#灰度化
    #图像缩小
    sp = grey_image.shape
    image_width = int(sp[1] * 0.8)
    image_height = int(sp[0] * 0.8)
    img2 = cv2.resize(grey_image, (image_width, image_height))
    thre1 = 100
    ret, binarized_image = cv2.threshold(img2, thre1, 255, cv2.THRESH_BINARY)
    queueLockgray.acquire()
    imgQueuegray.put(img2)
    queueLockgray.release()
    
    #图像分析
    flag,img3,dist,last = blackline(binarized_image, thresh_grey, image_width, image_height, flag,last)
    queueLock.acquire()
    imgQueue.put(img3)
    queueLock.release()
    mes = 0x53
    print('out flag {}'.format(flag))
    if dist == -100:

        if flag == 0:
            mes = 0x52#turn right
            print('flag change')
        elif flag ==1:
            mes = 0x4c
            
    if dist > thresh_angle:
       mes = 0x52#2 #R
    if dist < 0-thresh_angle and dist !=-100:
       mes = 0x4c#0x4c #L

    if dist <= thresh_angle and dist >= -thresh_angle:
       mes = 0x46#0x46#F
    return mes,flag,last  # , second

def motionControl(enable, imgQueue,imgQueuegray):
    
    camera = PiCamera()
    output = picamera.array.PiRGBArray(camera)
    camera.resolution = (320, 240) # 图像大小
    camera.framerate=30 # 帧
    flag = 1
    last = 0
    for frame in camera.capture_continuous(output, format="bgr", use_video_port=True):
        img = output.array
        mes,flag,last = imageanalyze(imgQueue,img,imgQueuegray,flag,last)
        output.truncate(0)
        send(0x53)
        send(mes)
        if enable.value == 0:
        	break

            #time.sleep(0.5)
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
            #camera.close()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        if ser != None:
            ser.close()
        camera.close()

