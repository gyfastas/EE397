# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'Mainw_2.ui'
#
# Created by: PyQt5 UI code generator 5.10.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(1069, 788)
        self.upButton = QtWidgets.QPushButton(Form)
        self.upButton.setGeometry(QtCore.QRect(750, 490, 93, 28))
        self.upButton.setObjectName("upButton")
        self.leftButton = QtWidgets.QPushButton(Form)
        self.leftButton.setGeometry(QtCore.QRect(650, 530, 93, 28))
        self.leftButton.setObjectName("leftButton")
        self.rightButton = QtWidgets.QPushButton(Form)
        self.rightButton.setGeometry(QtCore.QRect(850, 530, 93, 28))
        self.rightButton.setObjectName("rightButton")
        self.downButton = QtWidgets.QPushButton(Form)
        self.downButton.setGeometry(QtCore.QRect(750, 570, 93, 28))
        self.downButton.setObjectName("downButton")
        self.Connect = QtWidgets.QPushButton(Form)
        self.Connect.setGeometry(QtCore.QRect(650, 680, 93, 28))
        self.Connect.setObjectName("Connect")
        self.disConnet = QtWidgets.QPushButton(Form)
        self.disConnet.setGeometry(QtCore.QRect(870, 680, 93, 28))
        self.disConnet.setObjectName("disConnet")
        self.verticalLayoutWidget = QtWidgets.QWidget(Form)
        self.verticalLayoutWidget.setGeometry(QtCore.QRect(620, 20, 431, 390))
        self.verticalLayoutWidget.setObjectName("verticalLayoutWidget")
        self.controlLayout = QtWidgets.QVBoxLayout(self.verticalLayoutWidget)
        self.controlLayout.setContentsMargins(0, 0, 0, 0)
        self.controlLayout.setObjectName("controlLayout")
        self.label = QtWidgets.QLabel(self.verticalLayoutWidget)
        self.label.setObjectName("label")
        self.controlLayout.addWidget(self.label)
        self.BKPEdit = QtWidgets.QLineEdit(self.verticalLayoutWidget)
        self.BKPEdit.setObjectName("BKPEdit")
        self.controlLayout.addWidget(self.BKPEdit)
        self.BKPSlider = QtWidgets.QSlider(self.verticalLayoutWidget)
        self.BKPSlider.setOrientation(QtCore.Qt.Horizontal)
        self.BKPSlider.setObjectName("BKPSlider")
        self.controlLayout.addWidget(self.BKPSlider)
        self.label_2 = QtWidgets.QLabel(self.verticalLayoutWidget)
        self.label_2.setObjectName("label_2")
        self.controlLayout.addWidget(self.label_2)
        self.BKDEdit = QtWidgets.QLineEdit(self.verticalLayoutWidget)
        self.BKDEdit.setObjectName("BKDEdit")
        self.controlLayout.addWidget(self.BKDEdit)
        self.BKDSlider = QtWidgets.QSlider(self.verticalLayoutWidget)
        self.BKDSlider.setOrientation(QtCore.Qt.Horizontal)
        self.BKDSlider.setObjectName("BKDSlider")
        self.controlLayout.addWidget(self.BKDSlider)
        self.label_3 = QtWidgets.QLabel(self.verticalLayoutWidget)
        self.label_3.setObjectName("label_3")
        self.controlLayout.addWidget(self.label_3)
        self.VKPEdit = QtWidgets.QLineEdit(self.verticalLayoutWidget)
        self.VKPEdit.setObjectName("VKPEdit")
        self.controlLayout.addWidget(self.VKPEdit)
        self.VKPSlider = QtWidgets.QSlider(self.verticalLayoutWidget)
        self.VKPSlider.setOrientation(QtCore.Qt.Horizontal)
        self.VKPSlider.setObjectName("VKPSlider")
        self.controlLayout.addWidget(self.VKPSlider)
        self.label_4 = QtWidgets.QLabel(self.verticalLayoutWidget)
        self.label_4.setObjectName("label_4")
        self.controlLayout.addWidget(self.label_4)
        self.VKIEdit = QtWidgets.QLineEdit(self.verticalLayoutWidget)
        self.VKIEdit.setObjectName("VKIEdit")
        self.controlLayout.addWidget(self.VKIEdit)
        self.VKISlider = QtWidgets.QSlider(self.verticalLayoutWidget)
        self.VKISlider.setOrientation(QtCore.Qt.Horizontal)
        self.VKISlider.setObjectName("VKISlider")
        self.controlLayout.addWidget(self.VKISlider)
        self.label_5 = QtWidgets.QLabel(self.verticalLayoutWidget)
        self.label_5.setObjectName("label_5")
        self.controlLayout.addWidget(self.label_5)
        self.VKDEdit = QtWidgets.QLineEdit(self.verticalLayoutWidget)
        self.VKDEdit.setObjectName("VKDEdit")
        self.controlLayout.addWidget(self.VKDEdit)
        self.VKDSlider = QtWidgets.QSlider(self.verticalLayoutWidget)
        self.VKDSlider.setOrientation(QtCore.Qt.Horizontal)
        self.VKDSlider.setObjectName("VKDSlider")
        self.controlLayout.addWidget(self.VKDSlider)
        self.verticalLayoutWidget_2 = QtWidgets.QWidget(Form)
        self.verticalLayoutWidget_2.setGeometry(QtCore.QRect(10, 20, 591, 251))
        self.verticalLayoutWidget_2.setObjectName("verticalLayoutWidget_2")
        self.commandLayout = QtWidgets.QVBoxLayout(self.verticalLayoutWidget_2)
        self.commandLayout.setContentsMargins(0, 0, 0, 0)
        self.commandLayout.setObjectName("commandLayout")
        self.commandBrowser = QtWidgets.QTextBrowser(self.verticalLayoutWidget_2)
        self.commandBrowser.setObjectName("commandBrowser")
        self.commandLayout.addWidget(self.commandBrowser)
        self.CommandEdit = QtWidgets.QLineEdit(self.verticalLayoutWidget_2)
        self.CommandEdit.setText("")
        self.CommandEdit.setObjectName("CommandEdit")
        self.commandLayout.addWidget(self.CommandEdit)
        self.formLayoutWidget = QtWidgets.QWidget(Form)
        self.formLayoutWidget.setGeometry(QtCore.QRect(10, 290, 171, 91))
        self.formLayoutWidget.setObjectName("formLayoutWidget")
        self.ParameterLayout = QtWidgets.QFormLayout(self.formLayoutWidget)
        self.ParameterLayout.setContentsMargins(0, 0, 0, 0)
        self.ParameterLayout.setObjectName("ParameterLayout")
        self.label_6 = QtWidgets.QLabel(self.formLayoutWidget)
        self.label_6.setObjectName("label_6")
        self.ParameterLayout.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label_6)
        self.Label_Angle = QtWidgets.QLabel(self.formLayoutWidget)
        self.Label_Angle.setObjectName("Label_Angle")
        self.ParameterLayout.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.Label_Angle)
        self.label_8 = QtWidgets.QLabel(self.formLayoutWidget)
        self.label_8.setObjectName("label_8")
        self.ParameterLayout.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.label_8)
        self.Label_SpeedL = QtWidgets.QLabel(self.formLayoutWidget)
        self.Label_SpeedL.setObjectName("Label_SpeedL")
        self.ParameterLayout.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.Label_SpeedL)
        self.label_10 = QtWidgets.QLabel(self.formLayoutWidget)
        self.label_10.setObjectName("label_10")
        self.ParameterLayout.setWidget(2, QtWidgets.QFormLayout.LabelRole, self.label_10)
        self.Label_SpeedR = QtWidgets.QLabel(self.formLayoutWidget)
        self.Label_SpeedR.setObjectName("Label_SpeedR")
        self.ParameterLayout.setWidget(2, QtWidgets.QFormLayout.FieldRole, self.Label_SpeedR)
        self.label_7 = QtWidgets.QLabel(self.formLayoutWidget)
        self.label_7.setObjectName("label_7")
        self.ParameterLayout.setWidget(3, QtWidgets.QFormLayout.LabelRole, self.label_7)
        self.Label_Distance = QtWidgets.QLabel(self.formLayoutWidget)
        self.Label_Distance.setObjectName("Label_Distance")
        self.ParameterLayout.setWidget(3, QtWidgets.QFormLayout.FieldRole, self.Label_Distance)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "Form"))
        self.upButton.setText(_translate("Form", "前进"))
        self.leftButton.setText(_translate("Form", "左转"))
        self.rightButton.setText(_translate("Form", "右转"))
        self.downButton.setText(_translate("Form", "后退"))
        self.Connect.setText(_translate("Form", "Connect"))
        self.disConnet.setText(_translate("Form", "disConnect"))
        self.label.setText(_translate("Form", "BKP"))
        self.label_2.setText(_translate("Form", "BKD"))
        self.label_3.setText(_translate("Form", "VKP"))
        self.label_4.setText(_translate("Form", "VKI"))
        self.label_5.setText(_translate("Form", "VKD"))
        self.label_6.setText(_translate("Form", "Angle:"))
        self.Label_Angle.setText(_translate("Form", "TextLabel"))
        self.label_8.setText(_translate("Form", "SppedL:"))
        self.Label_SpeedL.setText(_translate("Form", "TextLabel"))
        self.label_10.setText(_translate("Form", "SppedR:"))
        self.Label_SpeedR.setText(_translate("Form", "TextLabel"))
        self.label_7.setText(_translate("Form", "Distance:"))
        self.Label_Distance.setText(_translate("Form", "TextLabel"))

