import serial
import time

import numpy as np, cv2, sys

sys.path.append('../../../')
from hsapi import FaceDetector

WEBCAM = False  # Set to True if use Webcam
net = FaceDetector(zoom=True, verbose=2, thresh=0.55)
if WEBCAM: video_capture = cv2.VideoCapture(0)

# 打开串口
ser = serial.Serial("/dev/ttyAMA0", 115200)
#x是16进制数：F:0x46；B:0x42；R:0x52；L:0x49；
def send(x):
    ser.write(bytes([0x80]))
    ser.write(bytes([x]))
    ser.write(bytes([0x81]))
    ser.write(bytes([0x81]))



def receive():
    # 获得接收缓冲区字符
    count = ser.inWaiting()
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

def main():
    while True:
        if WEBCAM:
            _, img = video_capture.read()
        else:
            img = None
        result = net.run(img)
        img = net.plot(result)
        img1 = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # 普通二值化处理 thresh1即为得到图像
        thre1 = 127
        ret, thresh1 = cv2.threshold(img1, thre1, 255, cv2.THRESH_BINARY)

        # 高斯模糊canny里面设置上下限阈值，canny为输出图像
        blur = cv2.GaussianBlur(img1, (3, 3), 0)
        canny = cv2.Canny(blur, 50, 200)

        # if
        cv2.imshow("Face Detector", img1)
        cv2.imshow("二值化图像", thresh1)
        cv2.imshow("高斯模糊图像", thresh1)

        cv2.waitKey(1)

net.quit()




ser.close()