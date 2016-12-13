Note:

Speed control

The speed of the motors can be adjusted by connecting PWM outputs from your robot's microcontroller to the ENA and ENB input pins on the motor driver board. The ENA pin controls Motor A and the ENB pin controls Motor B. When these pins are HIGH, power is output to the motor. By using PWM, you are turning power on and off very quickly to adjust the speed of the motor. The longer the PWM duty cycle is, the faster the motor will turn. We recommend always using a PWM duty cycle of 90% or less.

Direction control

The direction that the motors turn is controlled using the IN1, IN2, IN3 and IN4 input pins on the motor driver board. Connect these pins to digital outputs on your robots microcontroller. To make Motor A go forward, set IN1=HIGH and IN2=LOW. To make Motor A go backward set IN1=LOW and IN2=HIGH. The same method is used to control Motor B: set IN3=HIGH and IN4=LOW to o forward and set IN3=LOW and IN4=HIGH to go backwards. Note that "forward" and "backwards" refer to the direction of the motors themselves. If your robot does not move in the expected direction, reverse the motor polarity by swapping the green screw terminals for Motor A + and - and/or Motor B + and -.

Stopping

To remove power from the motors, simply set ENA=LOW for Motor A and ENB=LOW for Motor B. This will result in the motors stopping slowly and naturally from friction. To perform a quick braking operation, set ENA=LOW, IN1=LOW and IN2=LOW for Motor A and ENB=LOW, IN3=LOW and IN4=LOW for Motor B. The motors will come to an instant stop. Here are some handy tables to show the various modes of operation.

***************************************************************************************************************************

# ROSArduinoBridge.ino
ROS ROBOT

rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0 _baud:=115200

***************************************************************************************************************************

roslaunch ros_arduino_python arduino.launch

rqt_plot /Lencoder /Lpidout 

以0.3m/s固定线速度运行。图表中/Lvel/data代表左轮期望转速（以编码器的计数器数目表示），/Lencoder/data代表左轮实际的转速，/Lpidout/data 代表电机PWM值。
rostopic pub -r 30  /cmd_vel geometry_msgs/Twist '{linear: {x: 0.3, y: 0, z: 0}, angular: {x: 0, y: 0, z: 0}}'

rosrun teleop_twist_keyboard teleop_twist_keyboard.py


