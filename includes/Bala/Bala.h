#ifndef _BALA_H
#define _BALA_H

#include "Arduino.h"
#include "Wire.h"
#include "MPU6050.h"
#include "TB6612FNG.h"
#include "Kalman.h"

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
	Kalman 			*roll_filter;
	Kalman			*pitch_filter;

	int16_t ax, ay, az, gx, gy, gz;
	uint32_t kal_timer;

	double tarAngle;

	double roll, pitch;
	double gyrox, gyroy, gyroz;

	int16_t speedL, speedR;

	uint8_t Velocity_Period;

	double Balance_Kp, Balance_Kd;
	double Velocity_Kp, Velocity_Ki, Velocity_Kd;
	double Turn_Kp, Turn_Ki, Turn_Kd;
	double Speed_Diff_K;

	uint8_t cardown_limen;

	int16_t Motor1, Motor2;

	void _constrain(int16_t &val, int16_t low, int16_t high);

	void getAttitude();
	void setMotor(int16_t M1, int16_t M2);

	int16_t balance();
	int16_t velocity(int16_t target = 0);
	int16_t turn();
	void PIDController();


public:
	Bala(MPU6050 &m, Kalman &kfr, Kalman &kfp, Tb6612fng &tb, TwoWire &w = Wire);

	void begin();
	void run();

	double getRoll() { return roll; };
	double getPitch() { return pitch; };
	double getGyroX() { return gyrox; };
	double getGyroY() { return gyroy; };
	int16_t getSpeedL() { return speedL; };
	int16_t getSpeedR() { return speedR; };
	double getParaK(uint8_t idx)
	{
		switch(idx)
		{
		case 0 : return Balance_Kp;
		case 1 : return Balance_Kd;
		case 2 : return Velocity_Kp;
		case 3 : return Velocity_Ki;
		case 4 : return Velocity_Kd;
		case 5 : return Turn_Kp;
		case 6 : return Turn_Ki;
		case 7 : return Turn_Kd;	
		case 8 : return Speed_Diff_K;	
		case 9 : return tarAngle;
		case 10 : return (double)Velocity_Period;
		case 11: return (double)cardown_limen;
		default: return 0; 
		}		
	}

	void setParaK(uint8_t idx, double val) 
	{
		switch(idx)
		{
		case 0 : Balance_Kp = val; break;
		case 1 : Balance_Kd = val; break;
		case 2 : Velocity_Kp = val; break;
		case 3 : Velocity_Ki = val; break;
		case 4 : Velocity_Kd = val; break;
		case 5 : Turn_Kp = val; break;
		case 6 : Turn_Ki = val; break;
		case 7 : Turn_Kd = val; break;
		case 8 : Speed_Diff_K = val; break;
		case 9 : tarAngle = val; break;
		case 10 : Velocity_Period = (uint8_t)val; break;
		case 11: cardown_limen = (uint8_t)val; break;
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