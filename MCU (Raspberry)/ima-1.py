import numpy as np, cv2, sys

sys.path.append('../../../')
from hsapi import FaceDetector

WEBCAM = False  # Set to True if use Webcam
net = FaceDetector(zoom=True, verbose=2, thresh=0.55)
if WEBCAM: video_capture = cv2.VideoCapture(0)

try:
    while True:
        if WEBCAM:
            _, img = video_capture.read()
        else:
            img = None
        result = net.run(img)
        img = net.plot(result)
        img1 = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # 普通二值化处理 thresh1即为得到图像
        thre1 = 80
        ret, thresh1 = cv2.threshold(img1, thre1, 255, cv2.THRESH_BINARY)

        # 高斯模糊canny里面设置上下限阈值，canny为输出图像
        blur = cv2.GaussianBlur(img1, (3, 3), 0)
        canny = cv2.Canny(blur, 50, 200)

        # if
        cv2.namedWindow("Face Detector")
        cv2.namedWindow("二值化图像")
        cv2.namedWindow("高斯模糊图像")
        cv2.imshow("Face Detector", img1)
        cv2.imshow("二值化图像", thresh1)
        cv2.imshow("高斯模糊图像", canny)

        cv2.waitKey(1)
finally:
    net.quit()