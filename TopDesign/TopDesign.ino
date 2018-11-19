#include <Wire.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <MPU6050.h>
#include <Kalman.h>
#include <TB6612FNG.h>
#include <Bala.h>
#include <Flash.h>
#include <Ultrasonic.h>
#include <WebServer.h>
#define EEPROM_SIZE 13
#define EEPROM_FLAG 0x02

#define STANDBY 15
#define MOTORL_IN1 5
#define MOTORL_IN2 18
#define PWML 19
#define MOTORR_IN1 4
#define MOTORR_IN2 16
#define PWMR 17

#define TRIG 25
#define ECHO 39
#define SONIC_DIST_CM 500

#define NUMSSID 2
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

WebServer server(80);

const char* www_username = "admin";
const char* www_password = "LWYGN";
// allows you to set the realm of authentication Default:"Login Required"
const char* www_realm = "Custom Auth Realm";
// the Content of the HTML response in case of Unautherized Access Default:empty
String authFailResponse = "Authentication Failed";

MPU6050 mpu;
Kalman kfr, kfp;
Tb6612fng motorsDriver(STANDBY, MOTORL_IN1, MOTORL_IN2, PWML, MOTORR_IN1, MOTORR_IN2, PWMR);
Bala myBala(mpu, kfr, kfp, motorsDriver, Wire);
Flash myFlash(EEPROM_SIZE, EEPROM_FLAG);
Ultrasonic mySonic(TRIG, ECHO, SONIC_DIST_CM);

// Global variables
uint16_t dist_cm = 0;

void setup() 
{
  // Power on stabilizing ...
  delay(500);

  // Start I2C bus
  Wire.begin(/*SDA*/21, /*SCL*/22);
  Wire.setClock(400000UL);          // Set I2C frequency to 400kHz
  delay(500);
  WiFi.softAP(ssid, password);
  
  server.on("/", handleRoot);
  server.on("/update", handleUpdate);
  server.on("/Control",HTTP_POST,handlePost);
  server.onNotFound(handleNotFound);
  
  server.begin();

  Serial.println(String("Open http://") + WiFi.softAPIP() + " in your browser to see it working.");
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

  // Create a task on RTOS for ultrasonic distance detection
  xTaskCreatePinnedToCore(
    distDetection,       /* Task function. */
    "distDetection",     /* String with name of task. */
    1000,                  /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    3,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    0);                   /* Run on core 0. */ 
	
  // Create a task on RTOS for WiFi event
  xTaskCreatePinnedToCore(
    server.handleClient(),        /* Task function. */
    "handleClient",      /* String with name of task. */
    10000,                /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    1,                    /* Priority of the task. */
    NULL,                 /* Task handle. */
    0);                   /* Run on core 0. */
  
}

void loop()
{
	
}
