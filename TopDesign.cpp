#include <Wire.h>
#include <MPU6050.h>
#include <TB6612FNG.h>
#include <KalmanFilter.h>

#define STANDBY 15
#define MOTORL_IN1 5
#define MOTORL_IN2 18
#define PWML 19
#define MOTORR_IN1 4
#define MOTORR_IN2 16
#define PWMR 17
#define ENCODER_L 2
#define DIRECTION_L 0
#define ENCODER_R 34
#define DIRECTION_R 35

int16_t ax, ay, az, gx, gy, gz;
float Balance_Kp=15,Balance_Kd=0.4,Velocity_Kp=2,Velocity_Ki=0.01;
int Angle;
int Motor1, Motor2;

volatile long Velocity_L, Velocity_R = 0;   // encoder data of wheels
int Velocity_Left, Velocity_Right = 0;      // rotation speed of wheels

MPU6050 mpu;
Tb6612fng motors(STANDBY, MOTORL_IN1, MOTORL_IN2, PWML, MOTORR_IN1, MOTORR_IN2, PWMR);
KalmanFilter KalFilter;

/*
Parameters of Kalman Filter
*/
float K1 = 0.05;  // weight of accelator
float Q_angle = 0.001, Q_gyro = 0.005;
float R_angle = 0.5 , C_0 = 1;
float dt = 0.005; // sample period


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


/*------------------------- helper funcs -------------------------------*/
void constrain_PWM()
{
  int Amplitude = 250; 
  if (Motor1 < -Amplitude) Motor1 = -Amplitude;
  if (Motor1 > Amplitude)  Motor1 = Amplitude;
  if (Motor2 < -Amplitude) Motor2 = -Amplitude;
  if (Motor2 > Amplitude)  Motor2 = Amplitude;
}
/*----------------------------------------------------------------------*/


/*---------------------------- PID funcs -------------------------------*/
int balance(float Angle, float Gyro)
{
  float Bias;
  int balance;
  Bias = Angle - 0;
  balance = Balance_Kp * Bias + Gyro * Balance_Kd;
  return balance;
}

int velocity(int encoder_left, int encoder_right)
{
  static float Velocity, Encoder_Least, Encoder;
  static float Encoder_Integral, Target_Velocity;

  Encoder_Least = (encoder_left + encoder_right) - 0;               //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零）
  Encoder *= 0.7;                                                   //===一阶低通滤波器
  Encoder += Encoder_Least * 0.3;                                   //===一阶低通滤波器
  Encoder_Integral += Encoder;                                      //===积分出位移 积分时间：40ms
  if (Encoder_Integral > 21000)    Encoder_Integral = 21000;        //===积分限幅
  if (Encoder_Integral < -21000) Encoder_Integral = -21000;         //===积分限幅
  Velocity = Encoder * Velocity_Kp + Encoder_Integral * Velocity_Ki;//===速度控制
  return Velocity;
}

void PIDController()
{
  static int Balance_Pwm, Velocity_Pwm, Turn_Pwm;

  // balance loop
  Balance_Pwm = balance(KalFilter.angle, KalFilter.Gyro_x);

  // velocity loop
  Velocity_Pwm = velocity(Velocity_Left, Velocity_Right);

  // blend loops
  Motor1 = Balance_Pwm - Velocity_Pwm; 
  Motor2 = Balance_Pwm - Velocity_Pwm;

  constrain_PWM(); 
}
/*----------------------------------------------------------------------*/


void getAttitude()
{
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  KalFilter.Angletest(ax, ay, az, gx, gy, gz, dt, Q_angle, Q_gyro, R_angle, C_0, K1);
  Angle = KalFilter.angle;
}

void setMotor(int M1, int M2)
{
  // Map PWM signals from [-255, +255] to [-1,+1]
  motors.drive((float)(M1 / 255.0), (float)(M2 / 255.0)); 
}

void coreControl()
{
  static int Velocity_Count, Turn_Count;

  // Attitude sample
  getAttitude();

  // Car down
  if (abs(KalFilter.angle) > 45) 
  {
    setMotor(0, 0);
    return;
  }

  // Encoder sample
  if (++Velocity_Count >= 8)
  {
    Velocity_Left = -Velocity_L;   Velocity_L = 0;
    Velocity_Right = Velocity_R;   Velocity_R = 0;
    Velocity_Count = 0;
  }

  // Compute PID
  PIDController();

  // Motor out
  setMotor(Motor1, -Motor2);
}


void setup() 
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
  motors.begin();

  // Start I2C bus
  Wire.begin(/*SDA*/21, /*SCL*/22);
  Wire.setClock(400000UL);          // Set I2C frequency to 400kHz
  delay(500);

  // Start Serial for diplay debug message
  Serial.begin(115200);
  delay(500);

  // Initialize mpu ...
  mpu.begin();
  delay(500);
}


void loop() 
{
  // Serial display
  static uint32_t disp_interval = millis() + 5;
  if (millis() > disp_interval) {
    disp_interval = millis() + 20;
    Serial.print("V_L: ");
    Serial.println(Velocity_Left);
    Serial.print("V_R: ");
    Serial.println(Velocity_Right);
    Serial.print("Angle: ");
    Serial.println(Angle);
  }

  // Core control
  static uint32_t control_interval = millis() + 5;
  if (millis() > control_interval) {
    control_interval = millis() + 5;
    coreControl();
  }  
}