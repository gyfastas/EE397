import numpy as np
import cv2




def findDirection(img,hrate = 0.2,search_size = 5):
    '''

    :param img: 输入图像矩阵
    :param hrate:   图像高度区域占比
    :param search_size: 搜索窗的大小
    :return: tan: 角度
    '''
    img1 = img.copy()

    height,width = img.shape
    h = int(height*(1-hrate))
    img = img[h:height-1][:]
    #在列上进行扫描，寻找黑线
    search_window = np.zeros([search_size,search_size])
    search_window[search_size/2][:] = 1/search_size
    tempimg1 = cv2.filter2D(img,-1,search_window)
    tempimg1[tempimg1>0] = 255

    linewhere = np.where(tempimg1 == 0)

    #计算线的中心点
    line_mid_x = int(np.mean(linewhere[0]))
    line_mid_y = int(np.mean(linewhere[1]))

    #计算线的偏角
    '''
    思路:计算中心点竖直方向/水平方向黑点数量比例来计算夹角
    '''
    sample_num_x = np.sum(tempimg1[0:line_mid_x][line_mid_y]==0)
    sample_num_y = np.sum(tempimg1[line_mid_x][line_mid_y:width]==0)

    tan = sample_num_x/sample_num_y

    #线心与图像中心点的距离
    dis_mid_x = line_mid_x - height/2
    dis_mid_y = line_mid_y - width/2



