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
#define BATTERY_VOLTAGE_TEST 36

#define ENCODER_PULSE_PER_ROTATION 390
#define WHEEL_DIAMETER_CM 7

class Bala
{
private:
	TwoWire 		*wire;
	MPU6050 		*mpu;
	Tb6612fng 		*motors;
	Kalman 			*roll_filter;
	Kalman			*pitch_filter;

	uint8_t status;

	/*------------------------- Motion status -------------------------*/
	double battery_voltage;

	// MPU6050 data and KalmanFilter Timer
	int16_t ax, ay, az, gx, gy, gz;
	uint32_t kal_timer;

	// Euler angles and gyros
	double roll, pitch;
	double gyrox, gyroy, gyroz;

	// Wheels speed
	int16_t rotationL, rotationR;
	double speedL, speedR;
	int16_t Motor1, Motor2;

	// Distance
	uint8_t measure_distance;
	double distance;

	/*------------------------- Control Parameters -------------------------*/
	double target_angle;
	int16_t movement_step;
	double target_turn_base;     // because our robot's weight is unbalance, the right part is heavier than the left part, we need a offset in turn loop for adjustment
	double turn_step_base;
	uint8_t motor_dead_zone;

	int16_t movement;
	double turn_step;
	
	uint8_t Velocity_Period;
	uint8_t cardown_limen;

	uint8_t termination_flag;

	// PID parameters
	double Balance_Kp, Balance_Kd;
	double Velocity_Kp, Velocity_Ki;
	double Turn_Kp, Turn_Kd;

	void _constrain(int16_t &val, int16_t low, int16_t high);
	void _constrain(double &val, double low, double high);

	void getAttitude();
	void setMotor(int16_t M1, int16_t M2);

	int16_t balance();
	int16_t velocity(int16_t target = 0);
	int16_t turn();
	void PIDController();


public:
	Bala(MPU6050 &m, Kalman &kfr, Kalman &kfp, Tb6612fng &tb, TwoWire &w = Wire);

	void begin();
	uint8_t isbegin() { return status; }
	void run();

	double getRoll() { return roll; };
	double getPitch() { return pitch; };
	double getGyroX() { return gyrox; };
	double getGyroY() { return gyroy; };
	double getGyroZ() { return gyroz; };
	double getSpeedL() { return speedL; };
	double getSpeedR() { return speedR; };
	double getBatteryVoltage() { return battery_voltage; };
	double getDistance() { return distance; };
	double getParaK(uint8_t idx)
	{
		switch(idx)
		{
		case 0 : return Balance_Kp;
		case 1 : return Balance_Kd;
		case 2 : return Velocity_Kp;
		case 3 : return Velocity_Ki;
		case 4 : return Turn_Kp;
		case 5 : return Turn_Kd;		
		case 6 : return (double)motor_dead_zone;
		case 7 : return target_angle;
		case 8 : return (double)movement_step;
		case 9 : return target_turn_base;
		case 10 : return turn_step_base;
		default: return 0; 
		}		
	}

	void setParaK(uint8_t idx, double val) 
	{
		if (!this->isbegin()) return;
		switch(idx)
		{
		case 0 : Balance_Kp = val; break;
		case 1 : Balance_Kd = val; break;
		case 2 : Velocity_Kp = val; break;
		case 3 : Velocity_Ki = val; break;
		case 4 : Turn_Kp = val; break;
		case 5 : Turn_Kd = val; break;
		case 6 : motor_dead_zone = (uint8_t)val; break;
		case 7 : target_angle = val; break;
		case 8 : movement_step = (int16_t)val; break;
		case 9 : target_turn_base = val; break;
		case 10 : turn_step_base = val; break;
		default: break;
		}
	}

	// control
	void stop();
	void move(uint8_t direction, int16_t speed = 0, uint16_t duration = 0);
	void turn(uint8_t direction, int16_t speed = 0, uint16_t duration = 0);
	// void rotate(int16_t speed, uint16_t duration = 0);
	void dist(uint8_t sw);
};

#endif // _BALA_H