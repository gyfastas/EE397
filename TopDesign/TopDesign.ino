#include <Wire.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <MPU6050.h>
#include <Kalman.h>
#include <TB6612FNG.h>
#include <Bala.h>
#include <Flash.h>

#define EEPROM_SIZE 12
#define EEPROM_FLAG 0x02

#define STANDBY 15
#define MOTORL_IN1 5
#define MOTORL_IN2 18
#define PWML 19
#define MOTORR_IN1 4
#define MOTORR_IN2 16
#define PWMR 17

#define NUMSSID 2
const String ssid[NUMSSID] = { "WiFiSSID1", "WiFiSSID2" };
const String pswd[NUMSSID] = { "WiFIPSWD1", "WiFIPSWD2" };

const char* host     = "REMOTETCP";
const int   hostPort = 80;

const char* TCPCert  = "USERNAME AND PASSWORD";

WiFiClient client;
MPU6050 mpu;
Kalman kfr, kfp;
Tb6612fng motorsDriver(STANDBY, MOTORL_IN1, MOTORL_IN2, PWML, MOTORR_IN1, MOTORR_IN2, PWMR);
Bala myBala(mpu, kfr, kfp, motorsDriver, Wire);
Flash myFlash(EEPROM_SIZE, EEPROM_FLAG);

void setup() 
{
  // Power on stabilizing ...
  delay(500);

  // Start I2C bus
  Wire.begin(/*SDA*/21, /*SCL*/22);
  Wire.setClock(400000UL);          // Set I2C frequency to 400kHz
  delay(500);

  // Start Serial for diplay debug message
  Serial.begin(115200);
  delay(500);

  // Create a task on RTOS for self-balance car control
  xTaskCreatePinnedToCore(
    balaControl,          /* Task function. */
    "balaControl",        /* String with name of task. */
    10000,                /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    2,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    1);                   /* Run on core 1. */

  // Create a task on RTOS for WiFi event
  xTaskCreatePinnedToCore(
    remoteControl,        /* Task function. */
    "remoteControl",      /* String with name of task. */
    10000,                /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    1,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    0);                   /* Run on core 0. */
}

void loop() 
{

}
