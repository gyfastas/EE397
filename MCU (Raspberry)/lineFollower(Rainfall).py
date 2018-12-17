#!/usr/bin/env
# -*- coding: utf-8 -*-

# Copyright (C) 2018 rainfall1998. All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import math, time
import serial
import numpy as np
import multiprocess as mp
from picamera import PiCamera
import picamera.array

queueLock = mp.Lock()

# Get serial device and config it
ser = serial.Serial("/dev/ttyAMA0", 115200)

# Mapping:
# F:0x46 B:0x42 R:0x52 L:0x49
def sendCommand(x):
    ser.write(bytes([0x80]))
    ser.write(bytes([x]))
    ser.write(bytes([0x81]))
    ser.write(bytes([0x81]))

def receive():
    # Check if receive a frame
    count = ser.inWaiting()
    command = bytes([0x00])
    if count != 0:
        # Start reading command when receive a correct frame header (0x80)
        recv = ser.read(1)
        if recv == bytes([0x80]):
            length_of_frame = ser.inWaiting()
            # Get a frame (0x80 [one byte command] 0x81 0x81)
            if length_of_frame >= 3:
                frame = ser.read(3)
                # Check if the frame is valid (i.e. it ends with double [0x81])
                if frame.endswith(bytes([0x81, 0x81])):
                    command = frame[0]
    return command
 
def blackline(image, thresh_grey, image_width, image_height, flag, last):
    
    mid = np.array([[0,0]]*10)
    # Calculate the mid point of the black line
    nb_of_midpoints = 0
    for index in range(1, 10):
        i = int((index+75)*image_height/90)-1
        j = 0

        while ((image[i, j] > thresh_grey) or (image[i, j+3] > thresh_grey)) and (j < image_width-4):
            j=j+1
        j1=j
        while (image[i, j1] < thresh_grey)and j1 < image_width-4:
            j1 = j1+1
        if j < image_width-4:
            mid[nb_of_midpoints] = [i,int((j+j1)/2)]
            image[i,int((j+j1)/2)] = 255
            nb_of_midpoints = nb_of_midpoints +1
           
    # Calculate the distance between the black line and the center line
    if nb_of_midpoints < 2:
        dist = -100  # Regard as lose line
    else:
        dist = 0
        for index in range(0, nb_of_midpoints-1):
            dist += (mid[index, 1]) - int(image_width/2)
        dist = dist /nb_of_midpoints

    # Search in the first 10 rows
    if last < 2:
        last = 0
        nb_of_midpoints = 0
        mid_turn = np.array([[0,0]]*10)
        for index in range(1, 10):
            i = int((index+55)*image_height / 90) - 1
            j = 0

            while ((image[i, j] > thresh_grey) or (image[i, j+3] > thresh_grey)) and (j < image_width-4):
                j=j+1
            j1=j
            while (image[i, j1] < thresh_grey)and j1 < image_width-4:
                j1 = j1+1
            if j < image_width-4:
                mid_turn[nb_of_midpoints] = [i,int((j+j1)/2)]
                image[i,int((j+j1)/2)] = 255
                nb_of_midpoints = nb_of_midpoints +1
               
        # Judge the situation of turn
        if nb_of_midpoints > 2:
            midi = 0
            for index in range(0, nb_of_midpoints-1):
                midi += mid_turn[index, 1]
            midi = midi/nb_of_midpoints # Judge the core of line location
            if (((mid_turn[0, 1]-mid_turn[nb_of_midpoints-1, 1])>40) and midi>int(image_width/2)): # right turn
                flag = 0
                last = 30
            elif(((mid_turn[0, 1]-mid_turn[nb_of_midpoints-1, 1])<-40) and midi<int(image_width/2)): # left turn
                flag = 1
    else:
        last = last -1

    print('dist = {}'.format(dist))

    return flag, image, dist, last


def imageAnalyze(image,imgQueue,imgQueueBin,flag,last):

    thresh_grey = 100  # thresh for BGR2GRAY
    thresh_angle = 25  # thresh for angle

    # Convert to grayscale.
    grey_image = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)   
    # Binarize, remove noise, invert         
    __, bin_image = cv2.threshold(grey_image,thresh_grey,255,cv2.THRESH_BINARY) 
    # Resize the image
    sp = grey_image.shape
    image_width = int(sp[1] * 0.8)
    image_height = int(sp[0] * 0.8)
    bin_image = cv2.resize(bin_image, (image_width, image_height))
    
    # Analyze the binary image
    flag,image,dist,last = blackline(bin_image, thresh_grey, image_width, image_height, flag, last)

    queueLock.acquire()
    imgQueue.put(image)
    imgQueueBin.put(bin_image)
    queueLock.release()

    # 0x53 - stop 0x46 - forward
    # 0x4c - left 0x52 - right
    mes = 0x53
    print('out flag {}'.format(flag))
    if dist == -100:    # if lose line
        mes = 0x52 if flag == 0 else 0x4c
    elif np.abs(dist) < thresh_angle:
        mes = 0x46
    else:
        mes = 0x52 if dist > thresh_angle else 0x4c

    return mes, flag, last

def motionControl(enable, imgQueue, imgQueueBin):

    # Get PiCamera device and config it
    camera = PiCamera()
    output = picamera.array.PiRGBArray(camera)
    camera.resolution = (320, 240) # Set resolution
    camera.framerate = 30          # Set framerate

    # Declare 2 variables to record the situation of right angle turn
    flag = 1  # 0-right, 1-left
    last = 0

    # Start a camera capture loop
    for frame in camera.capture_continuous(output, format="bgr", use_video_port=True):
        img = output.array
        mes,flag,last = imageAnalyze(img.copy(),imgQueue,imgQueueBin,flag,last)
        output.truncate(0)
        sendCommand(0x53)
        sendCommand(mes)
        if enable.value == 0:
            break
    camera.close()
    print('Quit motionControl')

def displayImage(enable, imgQueue, imgQueueBin):

    while enable.value == 1:

        img = None
        img_bin = None

        queueLock.acquire()
        if not imgQueue.empty():
            img = imgQueue.get()
        if not imgQueueBin.empty():
            img_bin = imgQueueBin.get()
        queueLock.release()

        if img is not None:
            cv2.imshow('Image', img)
            cv2.waitKey(1)
        if img_bin is not None:
            cv2.imshow('Image (bin)', img_bin)
            cv2.waitKey(1)

    print('Quit displayImage')
    cv2.destroyAllWindows()

def main():

    enable = mp.Value('i', 0)
    imgQueue = mp.Queue(0)
    imgQueueBin = mp.Queue(0)

    while True:
        sign = receive()
        if sign == 1:
            print('connect')
            enable.value = 1
            mp.Process(target=motionControl, args=(enable,imgQueue,imgQueueBin)).start()
            mp.Process(target=displayImage, args=(enable,imgQueue,imgQueueBin)).start()
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
