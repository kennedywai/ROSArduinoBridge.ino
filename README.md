# ROSArduinoBridge.ino
ROS ROBOT

rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0 _baud:=115200

**************************************************************************

roslaunch ros_arduino_python arduino.launch

rqt_plot /Lencoder /Lpidout 

以0.3m/s固定线速度运行。图表中/Lvel/data代表左轮期望转速（以编码器的计数器数目表示），/Lencoder/data代表左轮实际的转速，/Lpidout/data 代表电机PWM值。
rostopic pub -r 30  /cmd_vel geometry_msgs/Twist '{linear: {x: 0.3, y: 0, z: 0}, angular: {x: 0, y: 0, z: 0}}'

rosrun teleop_twist_keyboard teleop_twist_keyboard.py


