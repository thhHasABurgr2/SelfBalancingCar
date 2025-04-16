#include <Arduino.h>
#include "MeMegaPi.h"
#include <Wire.h>
#include <SoftwareSerial.h>
#include <math.h>
#define PULSES_PER_REV 8

MeGyro IMU = MeGyro(PORT_8);
MeMegaPiDCMotor motor1(PORT1B);   
uint8_t motorSpeed=100;
MeMegaPiDCMotor motor2(PORT2B);   

// state stuff

float maxAngle = 15.0;
float theta = 0.0;
float thetaRate=0.0;
float v1;
float v2;
float pulsesPerSec1=0.0;
float pulsesPerSec2=0.0;
long oldPos1=0;
long oldPos2=0;
long newPos1;
long newPos2;
volatile long last_Pulse_pos_encoder1 = 0;
volatile long last_Pulse_pos_encoder2 = 0;

unsigned long lastTime = 0;
uint8_t T=5; //5 milliseconds
//loop stuff
char reader = 0;
bool ready=false;
unsigned long timeReady=0;
unsigned long lastTime2=0;
bool setReadyTime=true;
// interrupt/encoder stuff

const byte interruptPin1=18;
const byte interruptPin2=19;
const byte EN11=31;
const byte EN12=38;

typedef struct{
    int outerGear, innerGear;
}gearRatio;
gearRatio ratio={1,46};
double gearFraction(gearRatio ratio){
  return (double)ratio.outerGear/(double)ratio.innerGear;
}
void encoderSetup(){
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  pinMode(EN11, INPUT);
  pinMode(EN12, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), encoderISR1,RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), encoderISR2,RISING);
}

void setup() {
  encoderSetup();
  IMU.begin();
  Serial.begin(9600);
}


void loop() {
  // Check for serial input
  if (Serial.available() > 0) {
    reader = Serial.read();
    if (reader == 'R') {  
      ready = true;  
      
    } 
    else if (reader == 'S') {  
      ready = false;
      setReadyTime=true;
    }
  }
  IMU.fast_update();

  if (ready) {
      unsigned long currentTime = millis();
      motor1.run(-motorSpeed);
      motor2.run(motorSpeed);
      
    if (fabs(theta) < maxAngle) {
      if(setReadyTime){
          timeReady=currentTime;
          setReadyTime=false;
    }
      if(float(millis()-lastTime)>=float(T)){
          // <-- NEW
      theta = IMU.getAngleX();
      thetaRate=IMU.getGyroX();
      float timeDuration = (currentTime - lastTime) / 1000.0; // in seconds
      noInterrupts();
      newPos1=last_Pulse_pos_encoder1;
      newPos2=last_Pulse_pos_encoder2;
      interrupts();
      pulsesPerSec1=(newPos1-oldPos1)/timeDuration;
      pulsesPerSec2=(newPos2-oldPos2)/timeDuration;
      v1=(pulsesPerSec1/ PULSES_PER_REV)*60*gearFraction(ratio);
      v2=(pulsesPerSec2/ PULSES_PER_REV)*60*gearFraction(ratio);
      
      oldPos1=newPos1;
      oldPos2=newPos2;


        Serial.print(float(currentTime-timeReady)/1000.0);
        Serial.print(",");
        Serial.print(theta);
        Serial.print(",");
        Serial.print(thetaRate);
        Serial.print(",");
        Serial.print(v1);
        Serial.print(",");
        Serial.println(v2);
        lastTime = currentTime;
      }
    }else{
      Serial.println("F");
      ready=false;
      setReadyTime=true;
      motor1.stop();
      motor2.stop();
      
    }
  }else{
    if(millis()-lastTime2>50){
      Serial.print("X:");
      theta=IMU.getAngleX();
      thetaRate=IMU.getGyroX();
      Serial.print(theta );
      Serial.print(" gyroX:");
      Serial.println(thetaRate);
      lastTime2=millis();
    }
  }
}
void encoderISR1(){
  if (digitalRead(EN11)==digitalRead(EN12))last_Pulse_pos_encoder1++;
  else last_Pulse_pos_encoder1--;

}
void encoderISR2(){
  if(digitalRead(EN12)==digitalRead(EN11)) last_Pulse_pos_encoder2++;
  else last_Pulse_pos_encoder2--;
}


