# EE397
Our EE397 project

## Environment

- Arduino 1.8.4

## Requirements

#### Hardware chips

- ESP32-DEV-KIT
- MPU6050
- TB6612FNG
- DC-DC 12V-5V Converter
- HC-SR04

### Pin Map

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

![image](https://github.com/gyfastas/EE397/raw/Charles/images/ESP32-DEVKIT-V2-Board-Pinout-36-GPIOs.jpg)