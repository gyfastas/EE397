#include "MPU6050.h"
#include <Wire.h>

void writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
	Wire.beginTransmission(devAddr);
	Wire.write(regAddr);
	Wire.write(data);
	Wire.endTransmission(true);
}

uint8_t readByte(uint8_t devAddr, uint8_t regAddr) 
{
  Wire.beginTransmission(devAddr);
  Wire.write(regAddr);
  Wire.endTransmission(true);
  Wire.requestFrom((uint16_t)devAddr, (uint8_t)1); 
  uint8_t data = Wire.read();
  return data;
}

void MPU6050::readRegister()
{
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_ACCEL_XOUT_H);
  Wire.endTransmission(true);
  Wire.requestFrom((uint16_t)MPU6050_ADDR, (uint8_t)14);
  
  for (uint8_t i = 0; i < 7; ++i)
  	this->buffer[i] = Wire.read() << 8 | Wire.read();
}

void MPU6050::begin()
{
  writeByte(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x80);   // reset
  delay(300);
  writeByte(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x00);   // disable sleep mode
  delay(300);
  while (readByte(MPU6050_ADDR, MPU6050_WHO_AM_I) != 0x68) 
  {
    Serial.println("WHO_AM_I Error! ");
    writeByte(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x80); // reset
    delay(300);
    writeByte(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x00);   // disable sleep mode
    delay(300);
  }

  writeByte(MPU6050_ADDR, MPU6050_SMPLRT_DIV, 0x13);   // sample rate: 8kHz/(7+1) = 1kHz
  writeByte(MPU6050_ADDR, MPU6050_CONFIG, 0x04);       // disable DLPF, gyro output rate = 8kHz
  writeByte(MPU6050_ADDR, MPU6050_GYRO_CONFIG, 0x00);  // gyro range: ±200dps
  writeByte(MPU6050_ADDR, MPU6050_ACCEL_CONFIG, 0x00); // accel range: ±2g
}

void MPU6050::getMotion6(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz)
{
  readRegister();
  *ax = buffer[0];
  *ay = buffer[1];
  *az = buffer[2];
  *gx = buffer[4];
  *gy = buffer[5];
  *gz = buffer[6];
}