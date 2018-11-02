#include <Wire.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <MPU6050.h>
#include <Kalman.h>
#include <TB6612FNG.h>
#include <Bala.h>
#include <Flash.h>

#define EEPROM_SIZE 8
#define EEPROM_FLAG 0x02

#define STANDBY 15
#define MOTORL_IN1 5
#define MOTORL_IN2 18
#define PWML 19
#define MOTORR_IN1 4
#define MOTORR_IN2 16
#define PWMR 17

const char* ssid     = "WiFiSSID";
const char* password = "WiFIPSWD";

const char* host     = "REMOTETCP";
const int   hostPort = 80;

const char* TCPCert  = "USERNAME AND PASSWORD";

WiFiClient client;
MPU6050 mpu;
Kalman kfr, kfp;
Tb6612fng motorsDriver(STANDBY, MOTORL_IN1, MOTORL_IN2, PWML, MOTORR_IN1, MOTORR_IN2, PWMR);
Bala myBala(mpu, kfr, kfp, motorsDriver, Wire);
Flash myFlash(EEPROM_SIZE, EEPROM_FLAG);

void WiFiConfig()
{
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) 
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println();
	Serial.println("WiFi connected");

	Serial.print("Connecting to remote TCP server ");
	Serial.println(host);

	if (!client.connect(host, hostPort)) 
	{
		Serial.println("connection failed");
		return;
	}

	// Send username and password to login
	client.print(TCPCert);
}

void remoteControl(void *parameter)
{
  // Connect to WiFi route and the remote TCP server
  WiFiConfig();
  while(1)
  {
    // Get command from remote controller via WiFi
    if (client.available())
    {
      String cmd = client.readStringUntil('\r');
      cmd.trim();
      Serial.print("\nGet command: ");
      Serial.println(cmd);
      if (cmd == String("#"))
      {
        String info = "Angle: ";     info += myBala.getPitch();
        info += "\r\nKp_B: ";        info += myBala.getParaK(0);
        info += "\r\nKd_B: ";        info += myBala.getParaK(1);
        info += "\r\nKp_V: ";        info += myBala.getParaK(2);
        info += "\r\nKi_V: ";        info += myBala.getParaK(3);
        info += "\r\nKd_V: ";        info += myBala.getParaK(4);
        info += "\r\nTargetAngle: "; info += myBala.getParaK(5);
        info += "\r\nVecPeriod: ";   info += myBala.getParaK(6);
        info += "\r\nCarDown: ";     info += myBala.getParaK(7);
        client.println(info.c_str());
        for (uint8_t i = 0; i < EEPROM_SIZE; ++i)
          Serial.println(myBala.getParaK(i));
      }
      else
      {
        myBala.setParaK((uint8_t)(cmd[0] - '0' - 1), cmd.substring(1).toFloat());
        myFlash.updateEEPROM(myBala);    
      }
    } 
    vTaskDelay(10);
  }
  vTaskDelete(NULL);  
}

void balaControl(void *parameter)
{
  // Initialize bala ...
  myBala.begin();
  delay(500);

  // Update PID parameters ...
  if (myFlash.initEEPROM(myBala))
  {
    Serial.println("Restart ...");
    ESP.restart();
  }

  while(1)
  {
    static uint32_t control_interval = millis() + 5;
    if (millis() > control_interval) 
    {
      control_interval = millis() + 5;
      myBala.run();
    }  
    vTaskDelay(10); 
  }
  vTaskDelete(NULL);  
}

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