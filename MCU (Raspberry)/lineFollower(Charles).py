#!/usr/bin/env
# -*- coding: utf-8 -*-

# Copyright (C) 2018 Charles. All rights reserved.
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

def processImage(image,state_buff):

    def lineAngle(point1, point2):
        angle = round(math.atan2((point2[1] - point1[1]), -(point2[0] - point1[0]))*180/math.pi)
        return angle

    # Resolution of the camera image.
    RESOLUTION_X, RESOLUTION_Y = image.shape[1], image.shape[0]
    # The range of y axis (i.e. how many rows) to consider.
    SCANRANGE_Y = int(RESOLUTION_Y * 0.3)

    # Convert to grayscale.
    grey_image = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)   
    # Blur the grayscale image
    grey_image = cv2.GaussianBlur(grey_image,(5,5),0)  
    # Binarize, remove noise, invert         
    __,grey_image = cv2.threshold(grey_image,100,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU+cv2.THRESH_BINARY_INV) 

    # Trace a midline at the bottom of the frame
    midpoints_list = []
    for row_index in range(RESOLUTION_Y-1,RESOLUTION_Y-SCANRANGE_Y-1,-1):
        row = grey_image[row_index,:]
        for index in range(0,RESOLUTION_X):
            if row[index] != 0:
                break
        left_bound = index
        for index in range(0,RESOLUTION_X):
            if row[RESOLUTION_X-index-1] != 0:
                break
        right_bound = RESOLUTION_X-index-1

        # If no line, terminate
        if left_bound >= right_bound:
            break

        midpoint = (left_bound + right_bound) // 2
        midpoints_list.append(midpoint)

    # Ignore all information outside the scan range
    grey_image[:RESOLUTION_Y-len(midpoints_list),:] = 0

    # If cannot find any mid points, regard as lose line
    if (grey_image == 0).all():
        lose_flag = 1
        right_angle = 0
        angle = 0
        side_dist = 0
    else:
        lose_flag = 0

        # Calculate gradient
        grad = np.gradient(midpoints_list)

        # Check whether there is a right angle turn (with huge gradient)
        if np.amin(grad) < -10:
            right_angle = -1
        elif np.amax(grad) > +10:
            right_angle = +1
        else:
            right_angle = 0

        # Calculate the line angle using the first and the last mid points
        first_midpoint = (RESOLUTION_Y-1, midpoints_list[0])
        last_midpoint = (RESOLUTION_Y-len(midpoints_list)-1, midpoints_list[-1])
        angle = lineAngle(first_midpoint, last_midpoint)

        # Calculate the distance between the line and the mid line
        core_midpoint_x = np.mean(midpoints_list)
        side_dist = int(core_midpoint_x - RESOLUTION_X // 2)
        
        # Mark the mid line on the original image
        for r,c in enumerate(midpoints_list):
            image[RESOLUTION_Y - r - 1][c] = 255

    print('Lose: {}, Right_Angle: {}, Angle: {}, Side: {}'.format(lose_flag, right_angle, angle, side_dist))

    # The buffer only store the state of the last 10 frames
    if len(state_buff) == 10:
        state_buff = state_buff[1:]
    buff.append({'Lose':lose_flag, 'Right':right_angle, 'Angle':angle, 'Side': side_dist})

    return buff[-1], image, grey_image

def getCommand(state):

    # 0x53 - stop 0x46 - forward
    # 0x4c - left 0x52 - right
    mes = 0x53
    if np.abs(state['Angle']) < 20:
        mes = 0x46
        if np.abs(state['Side']) > 100:
            mes = 0x4c if state['Side'] < 0 else 0x52 
    else:
        mes = 0x4c if state['Angle'] < 0 else 0x52
    return mes

def imageAnalyze(img,imgQueue,imgQueueBin,state_buff):

    state, img, bin_img, state_buff = processImage(img, state_buff)

    queueLock.acquire()
    imgQueue.put(img)
    imgQueueBin.put(bin_img)
    queueLock.release()

    mes = 0x53
    if state['Lose'] == 1:  # if lose line
        mes = None
        while len(state_buff) != 0:
            if state_buff[-1]['Lose']:
                state_buff = state_buff[:-1]
            else:
                mes = getCommand(state_buff[-1])
        if mes is None: # buffer empty, rotate to get line
            mes = 0x52
    else:
        mes = getCommand(state)

    return mes

def motionControl(enable, imgQueue, imgQueueBin):

    # Get PiCamera device and config it
    camera = PiCamera()
    output = picamera.array.PiRGBArray(camera)
    camera.resolution = (320, 240) # Set resolution
    camera.framerate = 30          # Set framerate

    # Declare a list as a buffer to store the state of the frames
    state_buff = []
    # Start a camera capture loop
    for frame in camera.capture_continuous(output, format="bgr", use_video_port=True):
        img = output.array
        mes = imageAnalyze(img.copy(),imgQueue,imgQueueBin,state_buff)
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
