#include "motor_driver.h"
#include "commands.h";

volatile long left_enc_pos = 0L;
volatile long right_enc_pos = 0L;
int left_rotate = 0;
int right_rotate = 0;

//Left Motor
int enA = 11;//analog
int in1 = 7;//digital
int in2 = 8;//digital
int encoderL_A = 2;//interrupt pin for right motor a
int encoderL_B = 4;//not using an interrupt pin for left motor B
//Right Motor
int enB = 13;//analog
int in3 = 9;//digital
int in4 = 10;//digital
int encoderR_A = 3;//interrupt pin for left motor a
int encoderR_B = 5;//not using an interrupt pin for right motor B

// variables to store the number of encoder pulses
// for each motor
volatile unsigned long left_enc_pos = 0L;
volatile unsigned long right_enc_pos = 0L;
int left_rotate = 0;
int right_rotate = 0;

void initEncoders(){
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(19, INPUT);
  pinMode(18, INPUT);
  attachInterrupt(0, encoderLeftISR, CHANGE);  
  attachInterrupt(1, encoderLeftISR,  CHANGE);  
  attachInterrupt(4, encoderRightISR, CHANGE); 
  attachInterrupt(5, encoderRightISR, CHANGE);
}

void encoderLeftISR(){
    if(direction(LEFT) == BACKWARDS){
        left_enc_pos--;
    }else{
        left_enc_pos++;
    }
}

void encoderRightISR(){
  if(direction(RIGHT) == BACKWARDS){
      right_enc_pos--;
    }else{
      right_enc_pos++;
    }
}

long readEncoder(int i) {
  long encVal = 0L;
  if (i == LEFT)  {
    noInterrupts();
    encVal = left_enc_pos;
    interrupts();
  }
  else {
    noInterrupts();
    encVal = right_enc_pos;
    interrupts();
  }
  return encVal;
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT){
      noInterrupts();
      left_enc_pos=0L;
      interrupts();
      return;
    } else { 
      noInterrupts();
      right_enc_pos=0L;
      interrupts();
      return;
    }
  }

void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

