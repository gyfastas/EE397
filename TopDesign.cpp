#include <Wire.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <MPU6050.h>
#include <KalmanFilter.h>
#include <TB6612FNG.h>
#include <Bala.h>

#define EEPROM_SIZE 10
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
KalmanFilter kf;
Tb6612fng motorsDriver(STANDBY, MOTORL_IN1, MOTORL_IN2, PWML, MOTORR_IN1, MOTORR_IN2, PWMR);
Bala myBala(mpu, kf, motorsDriver, Wire);

uint8_t initEEPROM(Bala &b)
{
  Serial.println("Start EEPROM...");
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("Failed to initialise EEPROM");
    return 1;
  }
  if (byte(EEPROM.read(0)) == EEPROM_FLAG && byte(EEPROM.read(1)) == EEPROM_FLAG)
  {
    Serial.println("Read PID parameters from EEPROM ...");
    for (uint8_t i = 1; i < EEPROM_SIZE / 2; ++i)
    {
      b.setParaK(i - 1, ((float)((uint16_t)EEPROM.read(i << 1)) + (float)(((uint16_t)EEPROM.read((i << 1) + 1)) / 100.0)));  
    }      
  }
  else
  {
    Serial.println("First time to use EEPROM ... ");
    EEPROM.write(0, EEPROM_FLAG); EEPROM.write(1, EEPROM_FLAG);
    for (uint8_t i = 1; i < EEPROM_SIZE / 2; ++i)
    {
      EEPROM.write((i << 1), byte((uint16_t)b.getParaK(i - 1)));
      EEPROM.write((i << 1) + 1, byte((uint16_t)(b.getParaK(i - 1) * 100) % 100));    
    }  
    EEPROM.commit();
  }
  return 0;
}

void updateEEPROM(Bala &b)
{
  Serial.println("Update EEPROM ... ");
  for (uint8_t i = 1; i < EEPROM_SIZE / 2; ++i)
  {
    EEPROM.write((i << 1), byte((uint16_t)b.getParaK(i - 1)));
    EEPROM.write((i << 1) + 1, byte((uint16_t)(b.getParaK(i - 1) * 100) % 100));     
  }   
  EEPROM.commit();
}

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

  // Initialize bala ...
  myBala.begin();
  delay(500);

  // Update PID parameters ...
  if (initEEPROM(myBala))
  {
    Serial.println("Restart ...");
    ESP.restart();
  }

  // Connect to WiFi route and the remote TCP server
  WiFiConfig();
}

void onWiFi()
{
  static char info[50];
  String cmd = client.readStringUntil('\r');
  cmd.trim();
  Serial.print("\nGet command: ");
  Serial.print(cmd);
  if (cmd == String("#"))
  {
    sprintf(info, "Kp_B: %d:%.2d\r\nKd_B: %d:%.2d\r\nKp_V: %d:%.2d\r\nKd_V: %d:%.2d\r\n", 
      (uint16_t)myBala.getParaK(0), (uint16_t)(myBala.getParaK(0) * 100) % 100,
      (uint16_t)myBala.getParaK(1), (uint16_t)(myBala.getParaK(1) * 100) % 100,
      (uint16_t)myBala.getParaK(2), (uint16_t)(myBala.getParaK(2) * 100) % 100,
      (uint16_t)myBala.getParaK(3), (uint16_t)(myBala.getParaK(3) * 100) % 100);
    Serial.println(myBala.getParaK(0));
    Serial.println(myBala.getParaK(1));
    Serial.println(myBala.getParaK(2));
    Serial.println(myBala.getParaK(3));
    client.print(info);
  }
  else
  {
    myBala.setParaK((uint8_t)(cmd[0] - '0' - 1), cmd.substring(1).toFloat());
    client.print("OK");  
    client.println(cmd.substring(1).toFloat());
    updateEEPROM(myBala);    
  }
}

void loop() 
{
  // Serial display
  // static uint32_t disp_interval = millis() + 5;
  // if (millis() > disp_interval) 
  // {
  //   disp_interval = millis() + 20;
  //   Serial.print("V_L: ");
  //   Serial.println(myBala.getSpeedL());
  //   Serial.print("V_R: ");
  //   Serial.println(myBala.getSpeedR());
  //   Serial.print("Angle: ");
  //   Serial.println(myBala.getAngle());
  // }

  // Core control
  static uint32_t control_interval = millis() + 5;
  if (millis() > control_interval) 
  {
    control_interval = millis() + 5;
    myBala.run();
  }  

  // Get command from remote controller via WiFi
  if (client.available())
  {
    onWiFi();
  }
}
