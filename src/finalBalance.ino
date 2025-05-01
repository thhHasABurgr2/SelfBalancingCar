#include <Arduino.h>
#include "MeMegaPi.h"
#include <Wire.h>
#include <SoftwareSerial.h>
#define kp 45
#define ki 0
#define kd 0
#define DEBUG_INFO 0
#define PWM_MIN_OFFSET   0
#define dt .005
#define RELAX_ANGLE 0
#define PULSES_PER_REV 8

long lastTime = 0;
long tInit=0;
double  CompAngleY, CompAngleX, GyroXangle;
double  last_turn_setpoint_filter = 0.0;
double  last_speed_setpoint_filter = 0.0;
double  speed_Integral_average = 0.0;
double  angle_speed = 0.0;
double pulsesPerSec1=0.0;
double pulsesPerSec2=0.0;
long lasttime_angle = 0;
long lasttime_speed = 0;
long v1;
long v2;
long oldPos1=0;
long oldPos2=0;
long newPos1;
long newPos2;
volatile long last_Pulse_pos_encoder1 = 0;
volatile long last_Pulse_pos_encoder2 = 0;

boolean start_flag = false;
boolean move_flag=false;
const byte interruptPin1=18;
const byte interruptPin2=19;
const byte EN11=31;
const byte EN12=38;
MeMegaPiDCMotor motor1(PORT1B);
MeMegaPiDCMotor motor2(PORT2B);
MeGyro IMU=new MeGyro(PORT_8);
typedef struct{
  double P,I,D;
} gains;
typedef struct
{
  gains parameters;
  double Setpoint, Output, Integral,differential, last_error;
} PID;
gains angleGains, speedGains;
PID PID_angle, PID_speed, PID_turn;
typedef struct{
    int outerGear, innerGear;
}gearRatio;
gearRatio ratio={1,46};
double gearFraction(gearRatio ratio){
  return (double)ratio.outerGear/(double)ratio.innerGear;
}
void setGains(PID &controller,double newP, double newI,double newD){
  controller.parameters.P=newP;
  controller.parameters.I=newI;
  controller.parameters.D=newD;
}
gains getGains(PID controller){
  return controller.parameters;
}

void init_TIMER() {  
  cli();          
  TCCR1A = 0;   
  TCCR1B = 0;      
  OCR1A = 9999;    
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11);
  TIMSK1 |= (1 << OCIE1A);
  sei();       
}

void PID_angle_compute()   //PID
{
  
  double error = CompAngleX - PID_angle.Setpoint;

  PID_angle.Integral +=error*((millis()-lasttime_angle)/1000.0);
  PID_angle.Integral = constrain(PID_angle.Integral,-100,100); 
  PID_angle.differential = angle_speed;
  PID_angle.Output = PID_angle.parameters.P * error + PID_angle.parameters.I * PID_angle.Integral + PID_angle.parameters.D * PID_angle.differential;
  if(PID_angle.Output > 0)
  {
    PID_angle.Output = PID_angle.Output + PWM_MIN_OFFSET;
  }
  else
  {
    PID_angle.Output = PID_angle.Output - PWM_MIN_OFFSET;
  }

  double pwm_left = PID_angle.Output - PID_turn.Output;
  double pwm_right = -PID_angle.Output - PID_turn.Output;
  

/*#ifdef DEBUG_INFO
  Serial.print("Relay: ");
  Serial.print(PID_angle.Setpoint);
  Serial.print(" AngX: ");
  Serial.print(CompAngleX);
  Serial.print(" Output: ");
  Serial.print(PID_angle.Output);
  Serial.print("PID_angle.Integral: ");
  Serial.print(PID_angle.Integral);
  Serial.print(" dif: ");
  Serial.println(PID_angle.differential);
#endif*/
  

  pwm_left = constrain(pwm_left, -255, 255);
  pwm_right = constrain(pwm_right, -255, 255);
  motor1.run(pwm_left);
  motor2.run(pwm_right);
}


void PID_speed_compute(double currMotor1, double currMotor2)
{
  double speed_now = (currMotor1 + currMotor2)/2;

  last_speed_setpoint_filter  = last_speed_setpoint_filter  * 0.8;
  last_speed_setpoint_filter  += PID_speed.Setpoint * 0.2;
 
  if((move_flag == true) && (abs(speed_now) < 8) && (PID_speed.Setpoint == 0))
  {
    move_flag = false;
    last_speed_setpoint_filter = 0;
    PID_speed.Integral = speed_Integral_average;
  }

  double error = speed_now - last_speed_setpoint_filter;
  PID_speed.Integral += error*((millis()-lasttime_speed)/1000.0);
  

  if(move_flag == true) 
  { 
    PID_speed.Integral = constrain(PID_speed.Integral , -2000, 2000);
    PID_speed.Output = PID_speed.parameters.P * error + PID_speed.parameters.I * PID_speed.Integral;
    PID_speed.Output = constrain(PID_speed.Output , -8.0, 8.0);
  }
  else
  {  
    PID_speed.Integral = constrain(PID_speed.Integral , -2000, 2000);
    PID_speed.Output = PID_speed.parameters.P * speed_now + PID_speed.parameters.I * PID_speed.Integral;
    PID_speed.Output = constrain(PID_speed.Output , -10.0, 10.0);
    speed_Integral_average = 0.8 * speed_Integral_average + 0.2 * PID_speed.Integral;
  }
  
/*#ifdef DEBUG_INFO
  Serial.print(speed_now);
  Serial.print(","); 
  Serial.print(PID_speed.Setpoint);
  Serial.print(",");      
  Serial.print(",");
  Serial.print(last_speed_setpoint_filter);
  Serial.print(",");
  Serial.print(PID_speed.Integral);
  Serial.print(",");
  Serial.println(PID_speed.Output);
#endif*/
  PID_angle.Setpoint =  RELAX_ANGLE + PID_speed.Output;
}

void balanced_model(){

  
  
}

int16_t agx_start_count;
void reset(void)
{
  if((start_flag == false) && (abs(IMU.getAngleX()) < 5))
  {
    agx_start_count++;
  }
  if((start_flag == true) && (abs(IMU.getAngleX()) > 53))
  {
    agx_start_count = 0;
    motor1.stop();
    motor2.stop();
    PID_speed.Integral = 0;
    PID_angle.Setpoint = RELAX_ANGLE;
    PID_speed.Setpoint = 0;
    PID_turn.Setpoint = 0;
    /*encoders[0].setPulsePos(0);
    encoders[1].setPulsePos(0);*/
    PID_speed.Integral = 0;
    start_flag = false;
    last_speed_setpoint_filter = 0.0;
    last_turn_setpoint_filter = 0.0;
/*#ifdef DEBUG_INFO
    Serial.println("> 32");
#endif*/
  }
  else if(agx_start_count > 20)
  {
    agx_start_count = 0;
    PID_speed.Integral = 0;
    motor1.stop();
    motor2.stop();
    PID_angle.Setpoint = RELAX_ANGLE;
    /*encoders[0].setPulsePos(0);
    encoders[1].setPulsePos(0);*/
    lasttime_speed = millis();
    lasttime_angle = millis();
    start_flag = true;
/*#ifdef DEBUG_INFO
    Serial.println("< 5");
#endif*/
  }
}

void encoderSetup(){
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  pinMode(EN11, INPUT);
  pinMode(EN12, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), encoderISR1,RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), encoderISR2,RISING);
}
void pidSetup(){
  setGains(PID_angle,kp,ki,kd);
  setGains(PID_speed,0.06,0.005,0);
}
void setup() {
  encoderSetup();
  pidSetup();
  IMU.begin();
  Serial.begin(9600);
}

void loop() {
  

  if(millis()-tInit>=5){
  IMU.update();
  noInterrupts();
  newPos1=last_Pulse_pos_encoder1;
  newPos2=last_Pulse_pos_encoder2;
  interrupts();
  CompAngleX = -IMU.getAngleX();
  angle_speed=-IMU.getGyroX();
  int timeDuration=millis()-tInit;
  pulsesPerSec1=(newPos1-oldPos1)/timeDuration;
  pulsesPerSec2=(newPos2-oldPos2)/timeDuration;
  v1=(pulsesPerSec1/ PULSES_PER_REV)*60/gearFraction(ratio);
  v2=(pulsesPerSec2/ PULSES_PER_REV)*60/gearFraction(ratio);
  oldPos1=newPos1;
  oldPos2=newPos2;
  tInit=millis();
  Serial.println(CompAngleX);
  Serial.flush();

  }
    
  balanced_model();
  




  

}

void encoderISR1(){
  if (digitalRead(EN11)==digitalRead(EN12))last_Pulse_pos_encoder1++;
  else last_Pulse_pos_encoder1--;

}
void encoderISR2(){
  if(digitalRead(EN12)==digitalRead(EN11)) last_Pulse_pos_encoder2++;
  else last_Pulse_pos_encoder2--;
}

