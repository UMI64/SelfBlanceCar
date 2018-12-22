#include "pid.h"
float PID::PID_UpData (float Measured_Value)
{
	float EP=Target_Value-Measured_Value;//误差
	/*积分*/
	EI+=EP;
	if (EI>I_Limit_H) EI=I_Limit_H;
	if (EI<I_Limit_L) EI=I_Limit_L;
	/*微分*/
	float ED=Last_Measured_Value-Measured_Value;
	PID_OUT=P*EP+EI*I+ED*D;//输出PID
	
	Last_Measured_Value=Measured_Value;
	if (PID_OUT>Limit_H)//限幅
	{
		PID_OUT=Limit_H;
		return PID_OUT;;
	}
	if (PID_OUT<Limit_L)//限幅
	{
		PID_OUT=Limit_L;
		return PID_OUT;
	}
	return PID_OUT;
}
