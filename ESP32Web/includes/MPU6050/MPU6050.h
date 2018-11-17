#ifndef _MPU6050_H_
#define _MPU6050_H_

#include <Arduino.h>

#define MPU6050_ADDR         0x68 // MPU-6050 device address
#define MPU6050_SMPLRT_DIV   0x19 // MPU-6050 register address
#define MPU6050_CONFIG       0x1a
#define MPU6050_GYRO_CONFIG  0x1b
#define MPU6050_ACCEL_CONFIG 0x1c
#define MPU6050_WHO_AM_I     0x75
#define MPU6050_PWR_MGMT_1   0x6b
#define MPU6050_ACCEL_XOUT_H 0x3b

class MPU6050
{
private:
	int16_t buffer[7];
	void readRegister();

public:
	MPU6050() { for (uint8_t i = 0; i < 7; ++i) buffer[i] = 0; }
	void begin();
	void getMotion6(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz);
};

#endif // _MPU6050_H_