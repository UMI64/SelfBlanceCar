#include "pid.h"
float PID::PID_UpData (float Measured_Value)
{
	float EP=Target_Value-Measured_Value;//���
	/*����*/
	EI+=EP;
	if (EI>I_Limit_H) EI=I_Limit_H;
	if (EI<I_Limit_L) EI=I_Limit_L;
	/*΢��*/
	float ED=Last_Measured_Value-Measured_Value;
	PID_OUT=P*EP+EI*I+ED*D;//���PID
	
	Last_Measured_Value=Measured_Value;
	if (PID_OUT>Limit_H)//�޷�
	{
		PID_OUT=Limit_H;
		return PID_OUT;;
	}
	if (PID_OUT<Limit_L)//�޷�
	{
		PID_OUT=Limit_L;
		return PID_OUT;
	}
	return PID_OUT;
}
