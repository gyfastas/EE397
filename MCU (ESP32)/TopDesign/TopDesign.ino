/* Copyright (C) 2018 Charles. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <Wire.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <MPU6050.h>
#include <Kalman.h>
#include <TB6612FNG.h>
#include <Bala.h>
#include <Flash.h>
#include <Ultrasonic.h>
#include <ESPAsyncWebServer.h>
#include <HardwareSerial.h>

#define EEPROM_SIZE 11
#define EEPROM_FLAG 0x01

#define STANDBY 15
#define MOTORL_IN1 5
#define MOTORL_IN2 18
#define PWML 19
#define MOTORR_IN1 4
#define MOTORR_IN2 16
#define PWMR 17

#define TRIG1 25
#define ECHO1 39
#define TRIG2 33
#define ECHO2 26
#define SONIC_MAXDIST_CM 120

#define HWRX 12
#define HWTX 14

const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

AsyncWebServer server(80);

const char* www_username = "admin";
const char* www_password = "LWYGN";

MPU6050 mpu;
Kalman kfr, kfp;
Tb6612fng motorsDriver(STANDBY, MOTORL_IN1, MOTORL_IN2, PWML, MOTORR_IN1, MOTORR_IN2, PWMR);
Bala myBala(mpu, kfr, kfp, motorsDriver, Wire);
Flash myFlash(EEPROM_SIZE, EEPROM_FLAG);
Ultrasonic mySonic1(TRIG1, ECHO1, SONIC_MAXDIST_CM); // forward
Ultrasonic mySonic2(TRIG2, ECHO2, SONIC_MAXDIST_CM); // left side

HardwareSerial MySerial(1);

// Global variables
// distance detection
uint16_t distance_forward_cm = 0;
uint16_t distance_left_cm = 0;

// avoidance
uint8_t avoidance_en = 0;
uint32_t backward_time = 500;
double bypass_degree = 85;
uint16_t safe_distance_cm = 30;

// maze solver
uint8_t MSmode = 0;
uint8_t maze_solver_en = 0;
uint8_t maze_opt_switch = 0;
uint16_t forward_distance_threshold_cm = 30;
uint16_t left_distance_threshold_cm = 30;
double target_yaw_left = 85;
double target_yaw_right = -90;
uint16_t buff_dist = 10;
String path = "";  // buffer to store all actions: L, R, F, B

// communication with raspberry
uint8_t raspberry_en = 0;
String command = "N";

// move certain distance event
uint8_t move_dist_en = 0;
double target_dist = 0;

// rotate certain yaw angle event
uint8_t rotate_yaw_en = 0;
double target_yaw = 0;

void setup() 
{
  // Power on stabilizing ...
  delay(500);

  // Start I2C bus
  Wire.begin(/*SDA*/21, /*SCL*/22);
  Wire.setClock(400000UL);          // Set I2C frequency to 400kHz
  delay(500);
  
  // Create a task on RTOS for self-balance car control
  xTaskCreatePinnedToCore(
    balaControl,          /* Task function. */
    "balaControl",        /* String with name of task. */
    10000,                /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    4,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    1);                   /* Run on core 1. */

  // Create a task on RTOS for ultrasonic distance detection
  xTaskCreatePinnedToCore(
    distForwardDetection, /* Task function. */
    "distForwardDetection",/* String with name of task. */
    1000,                 /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    1,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    0);                   /* Run on core 0. */ 

  // Create a task on RTOS for ultrasonic distance detection
  xTaskCreatePinnedToCore(
    distLeftDetection,    /* Task function. */
    "distLeftDetection",  /* String with name of task. */
    1000,                 /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    1,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    0);                   /* Run on core 0. */ 
  
  // Create a task on RTOS for WiFi event
  xTaskCreatePinnedToCore(
    WiFiControl,          /* Task function. */
    "WiFiControl",        /* String with name of task. */
    10000,                /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    2,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    0);                   /* Run on core 0. */

  // Create a task on RTOS for communication with raspberry
  xTaskCreatePinnedToCore(
    raspberryControl,     /* Task function. */
    "raspberryControl",   /* String with name of task. */
    10000,                /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    3,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    0);                   /* Run on core 0. */ 

  // Create a task on RTOS for moving-certain-distance event
  xTaskCreatePinnedToCore(
    moveCertainDist,      /* Task function. */
    "moveCertainDist",    /* String with name of task. */
    1000,                 /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    1,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    0);                   /* Run on core 0. */

  // Create a task on RTOS for rotate-certain-yaw-angle event
  xTaskCreatePinnedToCore(
    rotateCertainYaw,     /* Task function. */
    "rotateCertainYaw",   /* String with name of task. */
    1000,                 /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    1,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    0);                   /* Run on core 0. */

  // Create a task on RTOS for avoidance control
  xTaskCreatePinnedToCore(
    avoidanceControl,     /* Task function. */
    "avoidanceControl",   /* String with name of task. */
    1000,                 /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    1,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    0);                   /* Run on core 0. */

  // Create a task on RTOS for solving maze
  xTaskCreatePinnedToCore(
    mazeSolver,           /* Task function. */
    "mazeSolver",         /* String with name of task. */
    10000,                 /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    1,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    0);                   /* Run on core 0. */
  
}

void loop()
{
  
}

