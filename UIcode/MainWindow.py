# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'Mainw1.ui'
#
# Created by: PyQt5 UI code generator 5.10.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtNetwork import *
class Ui_Form(QWidget):
    def __init__(self):

        self.isConnect = False
        self.setupUi()
        #连接服务器
        self.connectSock()
        #初始化信号槽
        self.slotInit()
    def connectSock(self):
        if self.isConnect ==True:
            return
        else:
            self.sock = QTcpSocket()
            IP =''
            Port =
            self.sock.connectToHost(IP,Port)

            if not self.sock.waitForConnected(2500):
                msg = self.sock.errorString()
                self.add_message(msg + '\n')
                QMessageBox.critical(self, "Error", msg)
                return False

            self.sock.connected.connect(self.on_socket_connected)
            self.sock.disconnected.connect(self.on_socket_disconnected)
            self.sock.readyRead.connect(self.on_socket_receive)
            self.sock.bytesWritten.connect(self.on_socket_transmit)

            self.add_message("Connected to {} on port {}\n".format(IP, port))

            self.isConnectedToServer = True
            self.lineEdit.setEnabled(True)
            self.pushButton.setEnabled(True)
            return True

    def setupUi(self):
        self.resize(1097, 844)
        self.pushButton = QtWidgets.QPushButton(self)
        self.pushButton.setGeometry(QtCore.QRect(770, 300, 93, 28))
        self.pushButton.setObjectName("pushButton")
        self.pushButton_2 = QtWidgets.QPushButton(self)
        self.pushButton_2.setGeometry(QtCore.QRect(680, 330, 93, 28))
        self.pushButton_2.setObjectName("pushButton_2")
        self.pushButton_3 = QtWidgets.QPushButton(self)
        self.pushButton_3.setGeometry(QtCore.QRect(860, 330, 93, 28))
        self.pushButton_3.setObjectName("pushButton_3")
        self.pushButton_4 = QtWidgets.QPushButton(self)
        self.pushButton_4.setGeometry(QtCore.QRect(770, 350, 93, 28))
        self.pushButton_4.setObjectName("pushButton_4")
        self.label = QtWidgets.QLabel(self)
        self.label.setGeometry(QtCore.QRect(40, 40, 72, 15))
        self.label.setText("")
        self.label.setObjectName("label")
        self.horizontalSlider = QtWidgets.QSlider(self)
        self.horizontalSlider.setGeometry(QtCore.QRect(30, 120, 251, 31))
        self.horizontalSlider.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider.setObjectName("horizontalSlider")
        self.horizontalSlider_2 = QtWidgets.QSlider(self)
        self.horizontalSlider_2.setGeometry(QtCore.QRect(30, 200, 251, 31))
        self.horizontalSlider_2.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_2.setObjectName("horizontalSlider_2")
        self.horizontalSlider_3 = QtWidgets.QSlider(self)
        self.horizontalSlider_3.setGeometry(QtCore.QRect(30, 270, 251, 31))
        self.horizontalSlider_3.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_3.setObjectName("horizontalSlider_3")
        self.horizontalSlider_4 = QtWidgets.QSlider(self)
        self.horizontalSlider_4.setGeometry(QtCore.QRect(30, 340, 251, 31))
        self.horizontalSlider_4.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_4.setObjectName("horizontalSlider_4")
        self.horizontalSlider_5 = QtWidgets.QSlider(self)
        self.horizontalSlider_5.setGeometry(QtCore.QRect(30, 420, 251, 31))
        self.horizontalSlider_5.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_5.setObjectName("horizontalSlider_5")

        self.retranslateUi(self)

    def retranslateUi(self):
        _translate = QtCore.QCoreApplication.translate
        self.setWindowTitle(_translate("智能小车", "智能小车"))
        self.pushButton.setText(_translate("Form", "Up"))
        self.pushButton_2.setText(_translate("Form", "Left"))
        self.pushButton_3.setText(_translate("Form", "Right"))
        self.pushButton_4.setText(_translate("Form", "Down"))


    def slotInit(self):
