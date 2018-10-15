#include "Bala.h"

volatile int16_t Velocity_L, Velocity_R = 0;   // encoder data of wheels

/*------------------------------ ISR -----------------------------------*/
void READ_ENCODER_L() 
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

void READ_ENCODER_R() 
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

Bala::Bala(MPU6050 &m, KalmanFilter &kf, Tb6612fng &tb, TwoWire &w)
{
	this->wire = &w;
	this->motors = &tb;
	this->mpu = &m;
	this->filter = &kf;

	this->Balance_Kp = 32.0;
	this->Balance_Kd = 2.0;
	this->Velocity_Kp = 10.0;
  this->Velocity_Kd = 0;
}

void Bala::_constrain(int16_t &val, int16_t low, int16_t high)
{
	if (val < low) val = low;
	else if (val > high) val = high;
}

void Bala::getAttitude()
{
	/* Parameters of Kalman Filter */
	static const float K1 = 0.05;  						// weight of accelator
	static const float Q_angle = 0.001, Q_gyro = 0.005;
	static const float R_angle = 0.5 , C_0 = 1;
	static const float dt = 0.005; 						// sample period

	this->mpu->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
	this->filter->Angletest(ax, ay, az, gx, gy, gz, dt, Q_angle, Q_gyro, R_angle, C_0, K1);
	this->angle = this->filter->angle;
	this->gyro = this->filter->Gyro_x;
}

void Bala::setMotor(int16_t M1, int16_t M2)
{
	// Map PWM signals from [-255, +255] to [-1,+1]
	this->motors->drive((float)(M1 / 255.0), (float)(M2 / 255.0)); 
}

int16_t Bala::balance()
{
	int16_t balance = this->Balance_Kp * this->angle + this->Balance_Kd * this->gyro;
	return balance;
}

int16_t Bala::velocity(int16_t target)
{
	static float Encoder, Encoder_last;
  static int16_t Encoder_Diff;
	int16_t Velocity;
	Encoder = 0.7 * Encoder + 0.3 * ((this->speedL + this->speedR) - target);	 // apply first-order low pass filter 
  Encoder_Diff = Encoder - Encoder_last;                                     // differential
  Encoder_last = Encoder;
	Velocity = this->Velocity_Kp * Encoder + this->Velocity_Kd * Encoder_Diff;
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
  if (abs(this->angle) > 60) 
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