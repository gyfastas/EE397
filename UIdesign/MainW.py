# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'Mainw1.ui'
#
# Created by: PyQt5 UI code generator 5.10.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtNetwork import *
import socket
import sys
class Ui_Form(QWidget):
    def __init__(self):
        super(Ui_Form, self).__init__()
        self.isConnect = False
        self.setupUi()
        self.slotInit()
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    def setupUi(self):
        self.resize(1097, 844)
        self.upButton = QtWidgets.QPushButton(self)
        self.upButton.setGeometry(QtCore.QRect(770, 270, 93, 28))
        self.upButton.setObjectName("upButton")
        self.upButton.setText("加速")
        self.LeftButton = QtWidgets.QPushButton(self)
        self.LeftButton.setGeometry(QtCore.QRect(670, 320, 93, 28))
        self.LeftButton.setObjectName("LeftButton")
        self.LeftButton.setText("左转")
        self.RightButton = QtWidgets.QPushButton(self)
        self.RightButton.setGeometry(QtCore.QRect(890, 320, 93, 28))
        self.RightButton.setObjectName("RightButton")
        self.RightButton.setText("右转")
        self.DownButton = QtWidgets.QPushButton(self)
        self.DownButton.setGeometry(QtCore.QRect(770, 380, 93, 28))
        self.DownButton.setObjectName("DownButton")
        self.DownButton.setText("后退")
        self.label = QtWidgets.QLabel(self)
        self.label.setGeometry(QtCore.QRect(40, 40, 72, 15))
        self.label.setText("")
        self.label.setObjectName("label")
        self.BKPSlider = QtWidgets.QSlider(self)
        self.BKPSlider.setGeometry(QtCore.QRect(30, 120, 541, 31))
        self.BKPSlider.setOrientation(QtCore.Qt.Horizontal)
        self.BKPSlider.setObjectName("BKPSlider")
        self.BKDSlider = QtWidgets.QSlider(self)
        self.BKDSlider.setGeometry(QtCore.QRect(30, 200, 541, 31))
        self.BKDSlider.setOrientation(QtCore.Qt.Horizontal)
        self.BKDSlider.setObjectName("BKD")
        self.VKPSlider = QtWidgets.QSlider(self)
        self.VKPlider.setGeometry(QtCore.QRect(30, 270, 541, 31))
        self.VKPSlider.setOrientation(QtCore.Qt.Horizontal)
        self.VKPSlider.setObjectName("VKPSlider")
        self.VKISlider = QtWidgets.QSlider(self)
        self.VKISlider.setGeometry(QtCore.QRect(30, 340, 541, 31))
        self.VKISlider.setOrientation(QtCore.Qt.Horizontal)
        self.VKISlider.setObjectName("VKISlider")
        self.VKDSlider = QtWidgets.QSlider(self)
        self.VKDSlider.setGeometry(QtCore.QRect(30, 420, 531, 31))
        self.VKDSlider.setOrientation(QtCore.Qt.Horizontal)
        self.VKDSlider.setObjectName("VKDSlider")
        self.BKPEdit = QtWidgets.QLineEdit(self)
        self.BKPEdit.setGeometry(QtCore.QRect(90, 100, 113, 21))
        self.BKPEdit.setObjectName("BKPEdit")
        self.BKDEdit = QtWidgets.QLineEdit(self)
        self.BKDEdit.setGeometry(QtCore.QRect(90, 170, 113, 21))
        self.BKDEdit.setObjectName("BKDEdit")
        self.VKPEdit = QtWidgets.QLineEdit(self)
        self.VKPEdit.setGeometry(QtCore.QRect(90, 250, 113, 21))
        self.VKPEdit.setObjectName("VKPEdit")
        self.VKIEdit = QtWidgets.QLineEdit(self)
        self.VKIEdit.setGeometry(QtCore.QRect(90, 320, 113, 21))
        self.VKIEdit.setObjectName("VKIEdit")
        self.VKDEdit = QtWidgets.QLineEdit(self)
        self.VKDEdit.setGeometry(QtCore.QRect(90, 400, 113, 21))
        self.VKDEdit.setObjectName("VKDEdit")
        self.ConnectButton = QtWidgets.QPushButton(self)
        self.ConnectButton.setGeometry(QtCore.QRect(650, 550, 93, 28))
        self.ConnectButton.setObjectName("ConnectButton")
        self.ConnectButton.setText('Connect')
        self.DisconnectButton = QtWidgets.QPushButton(self)
        self.DisconnectButton.setGeometry(QtCore.QRect(790, 550, 93, 28))
        self.DisconnectButton.setObjectName("DisconnectButton")
        self.DisconnectButton.setText("DisconnectButton")

    #初始化信号槽
    def slotInit(self):
        #连接服务器按钮
        self.Connect.clicked.connect(self.Sockconnect)
        self.Disconnect.clicked.connect(self.Sockdisconnect)
        #滑动条控制
        # self.horizontalSlider.valueChanged.connect(self.K1change)
        # self.horizontalSlider_2.valueChanged.connect(self.K2change)
        # self.BKISlider.valueChanged.connect(self.K3change)
        # self.BKDSlider.valueChanged.connect(self.K4change)
        # self.horizontalSlider_5.valueChanged.connect(self.K5change)

    def Send_Value(self,method):
        pass
        #send value to server according to method

    #connect TCP server =101.132.151.237

    def Sockconnect(self):
        if self.isConnect ==True:
            return False
        else:
            print('ready ip')
            IP ='101.132.151.237'
            Port = 81

            self.sock.connect(IP,Port)
            print('connect on')
            if not self.sock.waitForConnected(2500):
                print('not connect')
                return False

            self.sock.connected.connect(self.on_socket_connected)
            # self.sock.disconnected.connect(self.on_socket_disconnected)
            # self.sock.readyRead.connect(self.on_socket_receive)
            # self.sock.bytesWritten.connect(self.on_socket_transmit)
            print('Connect ok')
            return True
    def on_sokect_connected(self):
        user ='bala_control 2222'
        self.sock.send(user)
    def Sockdisconnect(self):
        if self.isConnect==False:
            return
        else:
            self.sock.disconnect()

    def K1change(self):
        Pos = self.horizontalSlider.value()


app = QtWidgets.QApplication(sys.argv)
ui = Ui_Form()
ui.show()
sys.exit(app.exec_())