# Self-Balance-Robot

This project is an assignment in the Intelligent System course (EE397) aiming to implement a self balance robot based on ESP32-DevkitC-V3 and Raspberry Pi 3.

This sub repository contains the source codes of ESP32 in our project.

## Requirements

### Hardware chips

- __MCU(for motion control)__ : ESP32-DEVKITC-V3
- __Accelerometer and Gyroscope__ : MPU6050(GY-521)
- __Motor Driver__ : TB6612FNG
- __Power Supply__ : DC-DC 12V-5V Converter
- __Ultrasonic__: HC-SR04

### Software

- __Development platform__ : Arduino 1.8.4
- __Supported repositories and libraries__ :
    + [M5Bala](https://github.com/m5stack/M5Bala)
    + [ESP32-Arduino-TB6612FNG](https://github.com/vincasmiliunas/ESP32-Arduino-TB6612FNG)
    + [KalmanFilter](https://github.com/TKJElectronics/KalmanFilter)

## Pin Map

| Peripheral | Pin of Peripheral  | Pin of ESP32 |
| ---------- | ------------------ | ------------ |
| TB6612FNG  | STANDBY            | IO15         |
| TB6612FNG  | AIN1               | IO5          |
| TB6612FNG  | AIN2               | IO18         |
| TB6612FNG  | PWMA               | IO19         |
| TB6612FNG  | BIN1               | IO4          |
| TB6612FNG  | BIN2               | IO16         |
| TB6612FNG  | PWMB               | IO17         |
| MPU6050    | SDA                | IO21         |
| MPU6050    | SCL                | IO22         |
| Encoder    | MOTOR_L_A          | IO2          |
| Encoder    | MOTOR_L_B          | IO0          |
| Encoder    | MOTOR_R_A          | IO34         |
| Encoder    | MOTOR_R_A          | IO35         |
| HC-SR04    | TRIG               | IO25         |
| HC-SR04    | ECHO               | IO39(VN)     |

![image](https://github.com/gyfastas/EE397/raw/master/MCU%20(ESP32)/images/ESP32-DEVKIT-V2-Board-Pinout-36-GPIOs.jpg)