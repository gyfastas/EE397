# Self-Balance-Robot

This project is an assignment in the Intelligent System course (EE397) aiming to implement a self balance robot based on ESP32-DevkitC-V3 and Raspberry Pi 3.

## Introduction

TODO

## Requirements

### Hardware chips

- __MCU(for motion control)__ : ESP32-DEVKITC-V3
- __MCU(for visual)__ : Raspberry Pi 3
- __Camera__ : Horned Sungem (HS) artificial intelligence development toolkit
- __Accelerometer and Gyroscope__ : MPU6050(GY-521)
- __Motor Driver__ : TB6612FNG
- __Power Supply__ : DC-DC 12V-5V Converter
- __Ultrasonic__: HC-SR04

### Software

#### ESP32
- __Development platform__ : Arduino 1.8.4
- __Supported repositories or libraries__ :
    + [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
    + [M5Bala](https://github.com/m5stack/M5Bala)
    + [ESP32-Arduino-TB6612FNG](https://github.com/vincasmiliunas/ESP32-Arduino-TB6612FNG)
    + [KalmanFilter](https://github.com/TKJElectronics/KalmanFilter)

#### Raspberry
- __Python Version__ : Python 3.6
- __Dependent Libraries__ :
    + hsapi (APIs provided by the Development Kit of Horned Sungem)
    + opencv-python

## License

[GPL-2.0 license](https://github.com/gyfastas/EE397/blob/master/LICENSE)




