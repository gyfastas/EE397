#include "Bala.h"

// #define RESTRICT_PITCH

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

	this->Balance_Kp = 8.0;
	this->Balance_Kd = 0.02;
	this->Velocity_Kp = 3.0;
	this->Velocity_Ki = 0.015;
	this->Velocity_Kd = 0;
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

	// Cal delta time
	double dt = (double)(micros() - this->kal_timer) / 1000000; 

	// Cal the angles using Kalman filter
	this->roll = this->roll_filter->getAngle(newroll, this->gyrox, dt);
	this->pitch = this->pitch_filter->getAngle(newpitch, this->gyroy, dt);

	this->kal_timer = micros();
}

void Bala::setMotor(int16_t M1, int16_t M2)
{
	// Map PWM signals from [-255, +255] to [-1,+1]
	this->motors->drive((float)(M1 / 255.0), (float)(M2 / 255.0)); 
}

int16_t Bala::balance()
{
	int16_t balance = this->Balance_Kp * this->pitch + this->Balance_Kd * this->gyroy;
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

void Bala::PIDController()
{
	static int16_t Balance_Pwm, Velocity_Pwm, Turn_Pwm;

	// balance loop
	Balance_Pwm = balance();

	// velocity loop
	Velocity_Pwm = velocity();

	// blend loops
	this->Motor1 = Balance_Pwm - Velocity_Pwm; 
	this->Motor2 = Balance_Pwm - Velocity_Pwm;

	this->_constrain(this->Motor1, -250, +250); 
	this->_constrain(this->Motor1, -250, +250); 
}

void Bala::begin()
{
	// Power on stabilizing ...
	delay(500);

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
	static int Velocity_Count, Turn_Count;

	// Attitude sample
	this->getAttitude();

	// Car down
	if (abs(this->pitch) > 40) 
	{
		this->setMotor(0, 0);
		return;
	}

	// Encoder sample
	if (++Velocity_Count >= 1)
	{
		this->speedL = +Velocity_L;  Velocity_L = 0;
		this->speedR = +Velocity_R;  Velocity_R = 0;
		Velocity_Count = 0;
	}

	// Compute PID
	this->PIDController();

	// Motor out
	this->setMotor(+this->Motor1, -this->Motor2);
}