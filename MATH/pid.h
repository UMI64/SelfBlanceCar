#ifndef __PID__H__
#define __PID__H__

#include "stm32f10x.h"
#include "math.h"
class PID
{
	public:
	float PID_UpData (float Measured_Value);;//����ֵ
	float Last_Measured_Value;//�ڲ��ϴεĲ���ֵ
	float Target_Value;//Ŀ��ֵ
	float EI;//�ڲ��ۻ��Ĵ���ֵ
	float P;//����
	float I;//����
	float D;//΢��
	float Limit_H;//����������
	float Limit_L;//����������
	float I_Limit_H;
	float I_Limit_L;
	float PID_OUT;
};
#endif
