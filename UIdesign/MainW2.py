# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'Mainw1.ui'
#
# Created by: PyQt5 UI code generator 5.10.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(1097, 844)
        self.upButton = QtWidgets.QPushButton(Form)
        self.upButton.setGeometry(QtCore.QRect(770, 270, 93, 28))
        self.upButton.setObjectName("upButton")
        self.LeftButton = QtWidgets.QPushButton(Form)
        self.LeftButton.setGeometry(QtCore.QRect(670, 320, 93, 28))
        self.LeftButton.setObjectName("LeftButton")
        self.RightButton = QtWidgets.QPushButton(Form)
        self.RightButton.setGeometry(QtCore.QRect(890, 320, 93, 28))
        self.RightButton.setObjectName("RightButton")
        self.DownButton = QtWidgets.QPushButton(Form)
        self.DownButton.setGeometry(QtCore.QRect(770, 380, 93, 28))
        self.DownButton.setObjectName("DownButton")
        self.label = QtWidgets.QLabel(Form)
        self.label.setGeometry(QtCore.QRect(40, 40, 72, 15))
        self.label.setText("")
        self.label.setObjectName("label")
        self.horizontalSlider = QtWidgets.QSlider(Form)
        self.horizontalSlider.setGeometry(QtCore.QRect(30, 120, 541, 31))
        self.horizontalSlider.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider.setObjectName("horizontalSlider")
        self.horizontalSlider_2 = QtWidgets.QSlider(Form)
        self.horizontalSlider_2.setGeometry(QtCore.QRect(30, 200, 541, 31))
        self.horizontalSlider_2.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_2.setObjectName("horizontalSlider_2")
        self.horizontalSlider_3 = QtWidgets.QSlider(Form)
        self.horizontalSlider_3.setGeometry(QtCore.QRect(30, 270, 541, 31))
        self.horizontalSlider_3.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_3.setObjectName("horizontalSlider_3")
        self.horizontalSlider_4 = QtWidgets.QSlider(Form)
        self.horizontalSlider_4.setGeometry(QtCore.QRect(30, 340, 541, 31))
        self.horizontalSlider_4.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_4.setObjectName("horizontalSlider_4")
        self.horizontalSlider_5 = QtWidgets.QSlider(Form)
        self.horizontalSlider_5.setGeometry(QtCore.QRect(30, 420, 531, 31))
        self.horizontalSlider_5.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_5.setObjectName("horizontalSlider_5")
        self.lineEdit = QtWidgets.QLineEdit(Form)
        self.lineEdit.setGeometry(QtCore.QRect(90, 100, 113, 21))
        self.lineEdit.setObjectName("lineEdit")
        self.lineEdit_2 = QtWidgets.QLineEdit(Form)
        self.lineEdit_2.setGeometry(QtCore.QRect(90, 170, 113, 21))
        self.lineEdit_2.setObjectName("lineEdit_2")
        self.lineEdit_3 = QtWidgets.QLineEdit(Form)
        self.lineEdit_3.setGeometry(QtCore.QRect(90, 250, 113, 21))
        self.lineEdit_3.setObjectName("lineEdit_3")
        self.lineEdit_4 = QtWidgets.QLineEdit(Form)
        self.lineEdit_4.setGeometry(QtCore.QRect(90, 320, 113, 21))
        self.lineEdit_4.setObjectName("lineEdit_4")
        self.lineEdit_5 = QtWidgets.QLineEdit(Form)
        self.lineEdit_5.setGeometry(QtCore.QRect(90, 400, 113, 21))
        self.lineEdit_5.setObjectName("lineEdit_5")
        self.Connect = QtWidgets.QPushButton(Form)
        self.Connect.setGeometry(QtCore.QRect(650, 550, 93, 28))
        self.Connect.setObjectName("Connect")
        self.Disconnect = QtWidgets.QPushButton(Form)
        self.Disconnect.setGeometry(QtCore.QRect(790, 550, 93, 28))
        self.Disconnect.setObjectName("Disconnect")

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "Form"))
        self.upButton.setText(_translate("Form", "Up"))
        self.LeftButton.setText(_translate("Form", "Left"))
        self.RightButton.setText(_translate("Form", "Right"))
        self.DownButton.setText(_translate("Form", "Down"))
        self.Connect.setText(_translate("Form", "Connect"))
        self.Disconnect.setText(_translate("Form", "Disconnect"))

