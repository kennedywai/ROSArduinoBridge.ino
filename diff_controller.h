#include "commands.h"
#include "motor_driver.h"
#include "encoder_driver.h"

/*
PID的目的是通过改变电机PWM值，使电机实际的转速基本等于期望的转速。如果参数不合理，就会出现实际的转速和期望的转速相差很远。
也就是说，我们没办法精准控制小车。
调PID参数，就是把电机PWM值、期望的转速和实际的转速这三者的值用图表实时地描绘出来， 
根据PWM值和实际的转速的运动轨迹，不停地修改PID的参数，
让期望的转速和实际的转速能在很短时间内的达到一致。
调节顺序，先调P,再调I，最后调D，通常只需要P和I两个参数就可以了。

逐渐从0增大P值和I值，至小车能够快速响应且未开始抖动，达到理想值
*/
typedef struct {
  double TargetTicksPerFrame;     // target speed in ticks per frame
  long   Encoder;                 // encoder count
  long   PrevEnc;                 // last encoder count
  int    PrevInput;               // last input
  int    ITerm;                   // integrated term for PID
  long   output;                  // last motor setting
}SetPointInfo;

SetPointInfo leftPID, rightPID;

int Kp = 10;
int Kd = 12;
int Ki = 0; // PD controller
int Ko = 50;

unsigned char moving = 0; // To check the robot base if its in motion?

// Initializing PID Parameters
void resetPID(){
   leftPID.TargetTicksPerFrame = 0.0;
   leftPID.Encoder = readEncoder(LEFT);// present encoder value
   leftPID.PrevEnc = leftPID.Encoder;// previous encoder value
   leftPID.output = 0;
   leftPID.PrevInput = 0;
   leftPID.ITerm = 0;

   rightPID.TargetTicksPerFrame = 0.0;
   rightPID.Encoder = readEncoder(RIGHT);
   rightPID.PrevEnc = rightPID.Encoder;
   rightPID.output = 0;
   rightPID.PrevInput = 0;
   rightPID.ITerm = 0;
}

void doPID(SetPointInfo * p) {
  long Perror;
  long output;
  int input;
  input = p->Encoder - p->PrevEnc;
  Perror = p->TargetTicksPerFrame - input;
  output = (Kp * Perror - Kd * (input - p->PrevInput) + p->ITerm) / Ko; //PID equation
  p->PrevEnc = p->Encoder;
  output += p->output;
  if (output >= MAX_PWM)
    output = MAX_PWM;
  else if (output <= -MAX_PWM)
    output = -MAX_PWM;
  else  
    p->ITerm += Ki * Perror;
    
  p->output = output;
  p->PrevInput = input;
}

void updatePID() {
  leftPID.Encoder = readEncoder(LEFT);
  rightPID.Encoder = readEncoder(RIGHT);
  
  if (!moving){
    if (leftPID.PrevInput != 0 || rightPID.PrevInput != 0) 
    resetPID();
    return;
  }
  doPID(&rightPID);
  doPID(&leftPID);
  setMotorSpeeds(leftPID.output, rightPID.output);
}

// Reading PID input and output
// READ_PIDIN 读取一个PID周期内编码器的计数，为后续调整PID参数提供参考
long readPidIn(int i) {
  long pidin=0;
    if (i == LEFT){
    pidin = leftPID.PrevInput;
  }else {
    pidin = rightPID.PrevInput;
  }
  return pidin;
}
// READ_PIDOUT 读取PID计算的PWM值，为后续调整PID参数提供参考
long readPidOut(int i) {
  long pidout=0;
    if (i == LEFT){
    pidout = leftPID.output;
  }else {
    pidout = rightPID.output;
  }
  return pidout;
}
