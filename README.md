# ROSArduinoBridge.ino
ROS ROBOT

rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0 _baud:=115200

roslaunch ros_arduino_python arduino.launch

rosrun teleop_twist_keyboard teleop_twist_keyboard.py
