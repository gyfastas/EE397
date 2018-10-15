#include <Wire.h>
#include <MPU6050.h>
#include <KalmanFilter.h>
#include <TB6612FNG.h>
#include <Bala.h>

#define STANDBY 15
#define MOTORL_IN1 5
#define MOTORL_IN2 18
#define PWML 19
#define MOTORR_IN1 4
#define MOTORR_IN2 16
#define PWMR 17

MPU6050 mpu;
KalmanFilter kf;
Tb6612fng motorsDriver(STANDBY, MOTORL_IN1, MOTORL_IN2, PWML, MOTORR_IN1, MOTORR_IN2, PWMR);
Bala myBala(mpu, kf, motorsDriver, Wire);

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
}

void loop() 
{
  // Serial display
  // static uint32_t disp_interval = millis() + 5;
  // if (millis() > disp_interval) {
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
  if (millis() > control_interval) {
    control_interval = millis() + 5;
    myBala.run();
  }  
}