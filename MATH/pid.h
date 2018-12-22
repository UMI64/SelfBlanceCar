#ifndef __PID__H__
#define __PID__H__

#include "stm32f10x.h"
#include "math.h"
class PID
{
	public:
	float PID_UpData (float Measured_Value);;//测量值
	float Last_Measured_Value;//内部上次的测量值
	float Target_Value;//目标值
	float EI;//内部累积的错误值
	float P;//比列
	float I;//积分
	float D;//微分
	float Limit_H;//最高输出限制
	float Limit_L;//最低输出限制
	float I_Limit_H;
	float I_Limit_L;
	float PID_OUT;
};
#endif
