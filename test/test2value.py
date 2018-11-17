import numpy as np
import hsapi as hs
import cv2
device_list = hs.EnumerateDevices() # 获取所有已连接的角蜂鸟
device = hs.Device(device_list[0]) # 获取Device实例

device.OpenDevice() # 打开角蜂鸟设备

while(1):
    image  = device.getImage(True)

    gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)

    gray[gray>60] = 255
    gray[gray<60] = 0

    cv2.imshow('aaa',gray)

    cv2.waitKey(15)

cv2.destroyAllWindows()
