#ifndef _BALA_H
#define _BALA_H

#include "Arduino.h"
#include "Wire.h"
#include "MPU6050.h"
#include "TB6612FNG.h"
#include "KalmanFilter.h"

#define ENCODER_L 2
#define DIRECTION_L 0
#define ENCODER_R 34
#define DIRECTION_R 35

class Bala
{
private:
	TwoWire 		*wire;
	MPU6050 		*mpu;
	Tb6612fng 		*motors;
	KalmanFilter 	*filter;

	int16_t ax, ay, az, gx, gy, gz;
	int16_t angle, gyro;

	int16_t speedL, speedR;

	float Balance_Kp, Balance_Kd;
	float Velocity_Kp, Velocity_Kd;

	int16_t Motor1, Motor2;

	void _constrain(int16_t &val, int16_t low, int16_t high);

	void getAttitude();
	void setMotor(int16_t M1, int16_t M2);

	int16_t balance();
	int16_t velocity(int16_t target = 0);
	void PIDController();


public:
	Bala(MPU6050 &m, KalmanFilter &kf, Tb6612fng &tb, TwoWire &w = Wire);

	void begin();
	void run();

	float getAngle() { return angle; };
	int16_t getSpeedL() { return speedL; };
	int16_t getSpeedR() { return speedR; };
	float getParaK(uint8_t idx)
	{
		switch(idx)
		{
		case 0: return Balance_Kp;
		case 1: return Balance_Kd;
		case 2: return Velocity_Kp;
		case 3: return Velocity_Kd;
		default: return 0; 
		}		
	}

	void setParaK(uint8_t idx, float val) 
	{
		switch(idx)
		{
		case 0: Balance_Kp = val; break;
		case 1: Balance_Kd = val; break;
		case 2: Velocity_Kp = val; break;
		case 3: Velocity_Kd = val; break;
		default: break;
		}
	}

	// control
	// void stop();
	// void move(int16_t speed, uint16_t duration = 0);
	// void turn(int16_t speed, uint16_t duration = 0);
	// void rotate(int16_t speed, uint16_t duration = 0);
};

#endif // _BALA_H