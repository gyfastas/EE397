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
import sys
class Ui_Form(QWidget):
    def __init__(self):
        super(Ui_Form, self).__init__()
        self.isConnect = False
        self.setupUi()
        self.slotInit()
        self.sock = QTcpSocket()
    def setupUi(self):
        self.resize(1097, 844)
        self.upButton = QtWidgets.QPushButton(self)
        self.upButton.setGeometry(QtCore.QRect(770, 270, 93, 28))
        self.upButton.setObjectName("upButton")
        self.LeftButton = QtWidgets.QPushButton(self)
        self.LeftButton.setGeometry(QtCore.QRect(670, 320, 93, 28))
        self.LeftButton.setObjectName("LeftButton")
        self.RightButton = QtWidgets.QPushButton(self)
        self.RightButton.setGeometry(QtCore.QRect(890, 320, 93, 28))
        self.RightButton.setObjectName("RightButton")
        self.DownButton = QtWidgets.QPushButton(self)
        self.DownButton.setGeometry(QtCore.QRect(770, 380, 93, 28))
        self.DownButton.setObjectName("DownButton")
        self.label = QtWidgets.QLabel(self)
        self.label.setGeometry(QtCore.QRect(40, 40, 72, 15))
        self.label.setText("")
        self.label.setObjectName("label")
        self.horizontalSlider = QtWidgets.QSlider(self)
        self.horizontalSlider.setGeometry(QtCore.QRect(30, 120, 541, 31))
        self.horizontalSlider.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider.setObjectName("horizontalSlider")
        self.horizontalSlider_2 = QtWidgets.QSlider(self)
        self.horizontalSlider_2.setGeometry(QtCore.QRect(30, 200, 541, 31))
        self.horizontalSlider_2.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_2.setObjectName("horizontalSlider_2")
        self.horizontalSlider_3 = QtWidgets.QSlider(self)
        self.horizontalSlider_3.setGeometry(QtCore.QRect(30, 270, 541, 31))
        self.horizontalSlider_3.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_3.setObjectName("horizontalSlider_3")
        self.horizontalSlider_4 = QtWidgets.QSlider(self)
        self.horizontalSlider_4.setGeometry(QtCore.QRect(30, 340, 541, 31))
        self.horizontalSlider_4.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_4.setObjectName("horizontalSlider_4")
        self.horizontalSlider_5 = QtWidgets.QSlider(self)
        self.horizontalSlider_5.setGeometry(QtCore.QRect(30, 420, 531, 31))
        self.horizontalSlider_5.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_5.setObjectName("horizontalSlider_5")
        self.lineEdit = QtWidgets.QLineEdit(self)
        self.lineEdit.setGeometry(QtCore.QRect(90, 100, 113, 21))
        self.lineEdit.setObjectName("lineEdit")
        self.lineEdit_2 = QtWidgets.QLineEdit(self)
        self.lineEdit_2.setGeometry(QtCore.QRect(90, 170, 113, 21))
        self.lineEdit_2.setObjectName("lineEdit_2")
        self.lineEdit_3 = QtWidgets.QLineEdit(self)
        self.lineEdit_3.setGeometry(QtCore.QRect(90, 250, 113, 21))
        self.lineEdit_3.setObjectName("lineEdit_3")
        self.lineEdit_4 = QtWidgets.QLineEdit(self)
        self.lineEdit_4.setGeometry(QtCore.QRect(90, 320, 113, 21))
        self.lineEdit_4.setObjectName("lineEdit_4")
        self.lineEdit_5 = QtWidgets.QLineEdit(self)
        self.lineEdit_5.setGeometry(QtCore.QRect(90, 400, 113, 21))
        self.lineEdit_5.setObjectName("lineEdit_5")
        self.Connect = QtWidgets.QPushButton(self)
        self.Connect.setGeometry(QtCore.QRect(650, 550, 93, 28))
        self.Connect.setObjectName("Connect")
        self.Connect.setText('Connect')
        self.Disconnect = QtWidgets.QPushButton(self)
        self.Disconnect.setGeometry(QtCore.QRect(790, 550, 93, 28))
        self.Disconnect.setObjectName("Disconnect")

    #初始化信号槽
    def slotInit(self):
        #连接服务器按钮
        self.Connect.clicked.connect(self.Sockconnect)
        self.Disconnect.clicked.connect(self.Sockdisconnect)
        #滑动条控制
        # self.horizontalSlider.valueChanged.connect(self.K1change)
        # self.horizontalSlider_2.valueChanged.connect(self.K2change)
        # self.horizontalSlider_3.valueChanged.connect(self.K3change)
        # self.horizontalSlider_4.valueChanged.connect(self.K4change)
        # self.horizontalSlider_5.valueChanged.connect(self.K5change)
    #connect TCP server =101.132.151.237
    def Sockconnect(self):
        if self.isConnect ==True:
            return False
        else:
            print('ready ip')
            IP ='101.132.151.237'
            Port = 81

            self.sock.connectToHost(IP,Port)
            print('connect on')
            if not self.sock.waitForConnected(2500):
                print('not connect')
                return False

            self.sock.connected.connect(self.on_socket_connected)
            self.sock.disconnected.connect(self.on_socket_disconnected)
            self.sock.readyRead.connect(self.on_socket_receive)
            self.sock.bytesWritten.connect(self.on_socket_transmit)

            print('Connect ok')
            return True
    def on_sokect_connected(self):
        user ='bala_control 2222'
        for i in user:
            self.sock.writeData()
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