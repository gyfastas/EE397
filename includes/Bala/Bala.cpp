#include "Bala.h"

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

	this->tarAngle = 0;

	this->Velocity_Period = 5;

	this->Balance_Kp = 12.0;
	this->Balance_Kd = 10.0;
	this->Velocity_Kp = 0.8;
	this->Velocity_Ki = 0.004;
	this->Velocity_Kd = 0;
	this->Turn_Kp = 0;
	this->Turn_Ki = 0;
	this->Turn_Kd = 0;
	this->Speed_Diff_K = 0;
	
	this->cardown_limen = 30;
	this->motor_dead_zone = 20;
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
	this->gyrox = gx / 131.0;
	this->gyroy = gy / 131.0;
	this->gyroz = gz / 131.0;

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
	int16_t balance = this->Balance_Kp * (this->roll - this->tarAngle) + this->Balance_Kd * this->gyroy;
	return balance;
}

int16_t Bala::velocity(int16_t target)
{
	static double Encoder, Encoder_last;
	static int16_t Encoder_Int, Encoder_Diff;
	int16_t Velocity;
	Encoder = 0.8 * Encoder + 0.2 * ((this->speedL + this->speedR) - target);	 // apply first-order low pass filter 
	Encoder_Int += Encoder;													     // integrate
	this->_constrain(Encoder_Int, -3000, +3000);
	Encoder_Diff = Encoder - Encoder_last;                                       // differential
	Encoder_last = Encoder;
	Velocity = this->Velocity_Kp * Encoder + this->Velocity_Ki * Encoder_Int + this->Velocity_Kd * Encoder_Diff;
	return Velocity;
}

int16_t Bala::turn()
{
	// static int16_t Turn_Target, Turn_Convert = 3, Turn_Int;
	// int16_t Turn;
	// if (1 == Flag_Left)             Turn_Target += Turn_Convert;
	// else if (1 == Flag_Right)       Turn_Target -= Turn_Convert;
	// else Turn_Target = 0;
	// this->_constrain(Turn_Target, -80, 80);
	// Turn_Int += Turn_Target;													// integrate
	// this->_constrain(Turn_Int, -3000, +3000);
	// Turn = this->Turn_Kp * -Turn_Target + this->Turn_Ki * Turn_Int + this->Turn_Kd * this->gyroz;
	// return Turn;
	return 0;
}

void Bala::PIDController()
{
	static int16_t Balance_Pwm, Velocity_Pwm, Turn_Pwm;
	static int16_t speed_diff, speed_diff_adjust;

	// balance loop
	Balance_Pwm = this->balance();

	// velocity loop
	Velocity_Pwm = this->velocity();
	
	// speed diff
	speed_diff = (this->speedL - this->speedR);
	speed_diff_adjust = (this->Speed_Diff_K * speed_diff);

	// turn loop
	Turn_Pwm = this->turn();

	// blend loops
	this->Motor1 = Balance_Pwm - Velocity_Pwm + Turn_Pwm - speed_diff_adjust;
	this->Motor2 = Balance_Pwm - Velocity_Pwm - Turn_Pwm;
}

void Bala::begin()
{
	// Power on stabilizing ...
	delay(500);

	// Initialize adc
	adcAttachPin(BATTERY_VOLTAGE_TEST);
	adcStart(BATTERY_VOLTAGE_TEST);
	analogReadResolution(10); 		// 10-bits resolution, maximum raw value is 1023
	analogSetAttenuation(ADC_6db);  // at 6dB attenuation, the maximum voltage is 2.2V
	pinMode(BATTERY_VOLTAGE_TEST, INPUT);

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
	static uint16_t Voltage_Sum;

	// Attitude sample
	this->getAttitude();

	// Battery voltage sample
  	Voltage_Count++;                                   // counter for average
  	Voltage_Sum += analogRead(BATTERY_VOLTAGE_TEST);   // sample battery voltage and integrate
 	if(Voltage_Count == 200)                           // calculate average
 	{
 		// voltage = sample / 1024 * VRef * 11, where VRef is 2.2V at 6dB attenuaton
  		this->battery_voltage = Voltage_Sum * 0.00011816;
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