#define GET_BAUDRATE	'b'
#define READ_ENCODERS	'e'
#define MOTOR_SPEEDS	'm'
#define RESET_ENCODERS	'r'
#define UPDATE_PID	'u'
#define READ_PIDOUT	'F'
#define READ_PIDIN  	'i'
#define LEFT             0
#define RIGHT            1
#define FORWARDS 	true
#define BACKWARDS 	false

/*
GET_BAUDRATE 获取串口通讯的比特率
READ_ENCODERS 读取左右轮编码器的计数
MOTOR_SPEEDS 设置左右轮的期望速度 
RESET_ENCODERS 重置编码器的计数
UPDATE_PID 更新PID参数
READ_PIDOUT 读取PID计算的PWM值，为后续调整PID参数提供参考
READ_PIDIN 读取一个PID周期内编码器的计数，为后续调整PID参数提供参考

在每个PID周期内，比较左右轮期望速度和实际速度的差异，调整PWM的。

如果AUTO_STOP_INTERVAL毫秒内上位机没有期望速度，就会停止小车运动。
*/
