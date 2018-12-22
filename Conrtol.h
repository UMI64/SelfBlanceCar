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
	void Task_Count();//测试
	/*状态*/
	UBaseType_t Task_Moto_stacksize=0;
	float Lspeed=50;//左轮速度
	float Rspeed=50;//右轮速度
	ACCEL accel;//加速度
	GYRO gyro;//角加速度
	AHRS_QuaternionTypeDef Quat;//四元数姿态
	float Roll=0;//翻滚姿态
	float VB=0;//电池电压
	uint16_t Attitude_Count=0;//测试
	uint16_t Attitude_1sCount=0;//测试
	/*标志位*/
	bool FLAG_UP=false;
	bool FLAG_LowPower=false;
	private:
	EventGroupHandle_t Event=0;
};
//#define MotosChange 	0x0001
#define MotosChange 	0x0002//电机设置改变
#define LightChange 	0x0004//灯光设置改变
#define PowerChange 	0x0008//电源设置改变
#define BB_1 	0x0010//BB声1
#define BB_2 	0x0020//BB声2
#define BB_3 	0x0040//BB声3
#define BB_4 	0x0080//BB声4

#endif