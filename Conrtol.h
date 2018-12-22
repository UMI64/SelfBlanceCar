#ifndef __CONRTOL_H
#define __CONRTOL_H

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "PWM.h"
#include "mpu6050.h"
#include"ADC.hpp"
#include "PID.h"
class Conrtol
{
	public:
	Conrtol();
	void Task_Moto();
	void Task_PowerWatch();
	void Task_BB();
	void Task_GetAttitude();
	void Task_Count();//����
	/*״̬*/
	UBaseType_t Task_Moto_stacksize=0;
	float Lspeed=50;//�����ٶ�
	float Rspeed=50;//�����ٶ�
	ACCEL accel;//���ٶ�
	GYRO gyro;//�Ǽ��ٶ�
	AHRS_QuaternionTypeDef Quat;//��Ԫ����̬
	float Roll=0;//������̬
	float VB=0;//��ص�ѹ
	uint16_t Attitude_Count=0;//����
	uint16_t Attitude_1sCount=0;//����
	/*��־λ*/
	bool FLAG_UP=false;
	bool FLAG_LowPower=false;
	private:
	EventGroupHandle_t Event=0;
};
//#define MotosChange 	0x0001
#define MotosChange 	0x0002//������øı�
#define LightChange 	0x0004//�ƹ����øı�
#define PowerChange 	0x0008//��Դ���øı�
#define BB_1 	0x0010//BB��1
#define BB_2 	0x0020//BB��2
#define BB_3 	0x0040//BB��3
#define BB_4 	0x0080//BB��4

#endif