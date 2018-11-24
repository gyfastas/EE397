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

#include "Bala.h"
#include <driver/adc.h>

#define RESTRICT_PITCH

volatile int16_t Velocity_L, Velocity_R = 0;   // encoder data of wheels

/*------------------------------ ISR -----------------------------------*/
void IRAM_ATTR READ_ENCODER_L() 
{
	if (digitalRead(ENCODER_L) == LOW) 
	{     // if falling edge ...
		if (digitalRead(DIRECTION_L) == LOW)      Velocity_L--;
		else      Velocity_L++;
	}
	else 
	{     // if rising edge ...
		if (digitalRead(DIRECTION_L) == LOW)      Velocity_L++;
		else     Velocity_L--;
	}
}

void IRAM_ATTR READ_ENCODER_R() 
{
	if (digitalRead(ENCODER_R) == LOW) 
	{     // if falling edge ...
		if (digitalRead(DIRECTION_R) == LOW)      Velocity_R++;
		else      Velocity_R--;
	}
	else 
	{     // if rising edge ...
		if (digitalRead(DIRECTION_R) == LOW)      Velocity_R--;
		else     Velocity_R++;
	}
}
/*----------------------------------------------------------------------*/

Bala::Bala(MPU6050 &m, Kalman &kfr, Kalman &kfp, Tb6612fng &tb, TwoWire &w)
{
	this->wire = &w;
	this->motors = &tb;
	this->mpu = &m;
	this->roll_filter = &kfr;
	this->pitch_filter = &kfp;

	this->kal_timer = micros();

	this->target_angle = 0;
	this->movement_step = 100;
	this->target_turn_base = 3;
	this->turn_step_base = 1.0;

	this->Velocity_Period = 5;
	this->cardown_limen = 35;
	this->motor_dead_zone = 20;

	this->Balance_Kp = 11.0;
	this->Balance_Kd = 0.9;
	this->Velocity_Kp = 1.2;
	this->Velocity_Ki = 0.005;
	this->Turn_Kp = 2.0;
	this->Turn_Kd = 0.7;
	this->Speed_Diff_K = 0;

	this->movement = 0;
	this->turn_step = 0;
}

void Bala::_constrain(int16_t &val, int16_t low, int16_t high)
{
	if (val < low) val = low;
	else if (val > high) val = high;
}

void Bala::getAttitude()
{
	this->mpu->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	// Cal roll and pitch
#ifdef RESTRICT_PITCH
	double newroll  = atan2(ay, az) * 57.296;
	double newpitch = atan(-ax / sqrt(ay * ay + az * az)) * 57.296;
#else
	double newroll  = atan(ay / sqrt(ax * ax + az * az)) * 57.296;
	double newpitch = atan2(-ax, az) * 57.296;
#endif
	this->gyrox = gx / 131.0 + 3.74;
	this->gyroy = gy / 131.0 - 1.90;
	this->gyroz = gz / 131.0 + 0.40;

	// Cal delta time
	double dt = (double)(micros() - this->kal_timer) / 1000000; 

	// Cal the angles using Kalman filter
	this->roll = this->roll_filter->getAngle(newroll, this->gyrox, dt);
	this->pitch = this->pitch_filter->getAngle(newpitch, this->gyroy, dt);

	this->kal_timer = micros();
}

void Bala::setMotor(int16_t M1, int16_t M2)
{
	// Constrain the range
	this->_constrain(M1, -250, +250); 
	this->_constrain(M2, -250, +250); 

	// Dead Zone
	if (abs(M1) < this->motor_dead_zone) M1 = 0;
	if (abs(M2) < this->motor_dead_zone) M2 = 0;

	// Map PWM signals from [-255, +255] to [-1,+1]
	this->motors->drive((float)(M1 / 255.0), (float)(M2 / 255.0)); 
}

int16_t Bala::balance()
{
	int16_t balance = this->Balance_Kp * (this->roll - this->target_angle) + this->Balance_Kd * this->gyrox;
	return balance;
}

int16_t Bala::velocity(int16_t target)
{
	static double Encoder;
	static int16_t Encoder_Int;
	int16_t Velocity;
	Encoder = 0.8 * Encoder + 0.2 * ((this->speedL + this->speedR) - target);	 // apply first-order low pass filter 
	Encoder_Int += Encoder;													     // integrate
	Encoder_Int -= this->movement;                                               // motion cotrol : move forword and backward
	this->_constrain(Encoder_Int, -10000, +10000);
	Velocity = this->Velocity_Kp * Encoder + this->Velocity_Ki * Encoder_Int;
	return Velocity;
}

int16_t Bala::turn()
{
	static double Turn_Target;
	int16_t Turn;
	if (abs(this->turn_step - 0) < 0.000001)			// if not in turn
	{
		if (this->movement == 0) Turn_Target = 0;									// if stay, do not adjust
		Turn_Target = ((this->movement > 0) ? -1 : +1) * this->target_turn_base;    // if go straight, adjust
	}
	else
		Turn_Target += this->turn_step;
	this->_constrain(Turn_Target, -80, 80);
	Turn = -this->Turn_Kp * Turn_Target - this->Turn_Kd * this->gyroz;
	return Turn;
}

void Bala::PIDController()
{
	static int16_t Balance_Pwm, Velocity_Pwm, Turn_Pwm;

	// balance loop
	Balance_Pwm = this->balance();

	// velocity loop
	Velocity_Pwm = this->velocity();

	// turn loop
	Turn_Pwm = this->turn();

	// blend loops
	this->Motor1 = Balance_Pwm - Velocity_Pwm + Turn_Pwm;
	this->Motor2 = Balance_Pwm - Velocity_Pwm - Turn_Pwm;
}

void Bala::begin()
{
	// Power on stabilizing ...
	delay(500);

	// Initialize adc
	adcAttachPin(BATTERY_VOLTAGE_TEST);
	adcStart(BATTERY_VOLTAGE_TEST);
	analogReadResolution(10);       // 10-bits resolution, maximum raw value is 1023
	analogSetAttenuation(ADC_6db);  // at 6dB attenuation, the maximum voltage is 2.2V

	// Initialize encoder ...
	pinMode(ENCODER_L, INPUT);
	pinMode(DIRECTION_L, INPUT); 
	pinMode(ENCODER_R, INPUT); 
	pinMode(DIRECTION_R, INPUT);
	attachInterrupt(ENCODER_L, READ_ENCODER_L, CHANGE);
	attachInterrupt(ENCODER_R, READ_ENCODER_R, CHANGE); 

	// Initialize motor driver TB6612FNG ...
	this->motors->begin();

	// Initialize MPU6050 ...
	this->mpu->begin();
	delay(500);	
}

void Bala::run()
{
	static uint8_t Velocity_Count, Turn_Count, Voltage_Count;
	static uint32_t Voltage_Sum;

	// Attitude sample
	this->getAttitude();

	// Battery voltage sample
	Voltage_Count++;                                   // counter for average
	Voltage_Sum += analogRead(BATTERY_VOLTAGE_TEST);   // sample battery voltage and integrate
	if(Voltage_Count == 200)                           // calculate average
	{
		Voltage_Sum /= 200; 
		// voltage = sample / 1024 * VRef * 11, where VRef is 2.2V(ideal) at 6dB attenuation, in real, we set VRef = 1.75V
		this->battery_voltage = Voltage_Sum * 0.0187988;
		Voltage_Sum = 0;
		Voltage_Count = 0;
	}

	// Encoder sample
	if (++Velocity_Count >= this->Velocity_Period)
	{
		this->speedL = +Velocity_L;  Velocity_L = 0;
		this->speedR = +Velocity_R;  Velocity_R = 0;
		Velocity_Count = 0;
	}

	// Tip over or low battery voltage
	if (abs(this->roll) > this->cardown_limen /*|| battery_voltage < 10.0*/) 
	{
		this->setMotor(0, 0);
		return;
	}
	
	// Compute PID
	this->PIDController();

	// Motor out
	this->setMotor(+this->Motor1, -this->Motor2);
}

void Bala::stop()
{
	this->movement = 0;
	this->turn_step = 0;
}
 
void Bala::move(uint8_t direction, int16_t speed, uint16_t duration)
{
	if (direction == 1) this->movement = +this->movement_step;          // forward
	else if (direction == 2) this->movement = -this->movement_step;     // backward
	else this->movement = 0;
}

void Bala::turn(uint8_t direction, int16_t speed, uint16_t duration)
{
	if (direction == 1) this->turn_step = -this->turn_step_base;          // left
	else if (direction == 2) this->turn_step = +this->turn_step_base;     // right
	else this->turn_step = 0;	
}