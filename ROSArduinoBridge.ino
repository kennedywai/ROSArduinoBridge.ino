// Main Program
#define BAUDRATE 115200
#define MAX_PWM 255 //8-bit PWM signal analog output
#define PID_RATE 30 //Hz

#include "Arduino.h"
#include "commands.h"
#include "motor_driver.h"
#include "encoder_driver.h"
#include "diff_controller.h"
/*
The main loop in motor_controller.ino converts encoder tick counts into the current rpm of both motors and controls 
the speed of the motors using PID control. The rosserial library conveniently allows the program to communicate with 
the rest of the ros nodes in the pc via usb. The program subscribes to the cmd_vel topic which sets the desired speed of 
the motors, and publishes the actual speeds on the rpm topic. I used two interrupt pins in the mega, one for each encoder. 
The PID control and rpm publishing are all done in the same loop cycle, at a desired rate of 10Hz (every 100 ms). 
I modifed the Adafruit motorshield v2 library such that setSpeed for DC motors used the full 4096 PWM resolution, 
i.e. in Adafruit_MotorShield.cpp
*/
const int PID_INTERVAL = 1000 / PID_RATE;
unsigned long nextPID = PID_INTERVAL;

#define AUTO_STOP_INTERVAL 2000
long lastMotorCommand = AUTO_STOP_INTERVAL;

/* Variable initialization */
int arg = 0;
int index = 0;
char chr;
char cmd;
char argv1[16];
char argv2[16];
long arg1;
long arg2;

void resetCommand() {
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));// fastest way to initializing an array
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg = 0;
  index = 0;
}

int runCommand() {
  int i = 0;
  char *p = argv1;
  char *str;
  int pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);

  switch (cmd) {
    case GET_BAUDRATE:
      Serial.println(BAUDRATE);
      break;

    case READ_PIDIN:
      Serial.print( readPidIn(LEFT));
      Serial.print(" ");
      Serial.println( readPidIn(RIGHT));
      break;      
      
    case READ_PIDOUT:
      Serial.print( readPidOut(LEFT));
      Serial.print(" ");
      Serial.println( readPidOut(RIGHT));
      break;
      
    case READ_ENCODERS:
      Serial.print(readEncoder(LEFT));
      Serial.print(" ");
      Serial.println(readEncoder(RIGHT));
      break;
      
    case RESET_ENCODERS:
      resetEncoders();
      resetPID();
      Serial.println("OK");
      break;
    case MOTOR_SPEEDS:
      lastMotorCommand = millis();
      if (arg1 == 0 && arg2 == 0) {
        setMotorSpeeds(0, 0);
        moving = 0;
      }
      else moving = 1;
      leftPID.TargetTicksPerFrame = arg1;
      rightPID.TargetTicksPerFrame = arg2;
      Serial.println("OK");
      break;
    case UPDATE_PID:
      while ((str = strtok_r(p, ":", &p)) != '\0') {
        pid_args[i] = atoi(str);
        i++;
      }
      Kp = pid_args[0];
      Kd = pid_args[1];
      Ki = pid_args[2];
      Ko = pid_args[3];
      Serial.println("OK");
      break;

      default:
      Serial.println("Invalid Command");
      break;
  }
}
 
unsigned long time = 0, old_time = 0;

void setup() {
  Serial.begin(BAUDRATE);
  initEncoders();
  initMotorController();
  resetPID(); // "initializing" PID
}

void loop() {
  while (Serial.available() > 0) {
    chr = Serial.read();
    if (chr == 13) {
      if (arg == 1) argv1[index] = NULL;
      else if (arg == 2) argv2[index] = NULL;
      runCommand();
      resetCommand();
    }
    else if (chr == ' ') {
      if (arg == 0) arg = 1;
      else if (arg == 1)  {
        argv1[index] = NULL;
        arg = 2;
        index = 0;
      }
      continue;
    }
    else {
      if (arg == 0) {
        cmd = chr;
      }
      else if (arg == 1) {
        argv1[index] = chr;
        index++;
      }
      else if (arg == 2) {
        argv2[index] = chr;
        index++;
      }
    }
  }
  if (millis() > nextPID) {
    updatePID();
    nextPID += PID_INTERVAL;
  }
  
  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {
    ;
    setMotorSpeeds(0, 0);
    moving = 0;
  }
}

