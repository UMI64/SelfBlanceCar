#include "Conrtol.h"
extern "C" void TASK_Moto(void* parm) {

    (static_cast<Conrtol*>(parm))->Task_Moto();
}
extern "C" void TASK_GetAttitude(void* parm) {

    (static_cast<Conrtol*>(parm))->Task_GetAttitude();
}
extern "C" void TASK_Count(void* parm) {

    (static_cast<Conrtol*>(parm))->Task_Count();
}
extern "C" void TASK_PowerWatch(void* parm) {

    (static_cast<Conrtol*>(parm))->Task_PowerWatch();
}
extern "C" void TASK_BB(void* parm) {

    (static_cast<Conrtol*>(parm))->Task_BB();
}
Conrtol::Conrtol()
{
	Event=xEventGroupCreate();
	xTaskCreate((TaskFunction_t)TASK_Moto,"vLEDTASK", 250, this, 1, NULL);//更新电机速度 电机速度标志置位时
	//xTaskCreate((TaskFunction_t)TASK_PowerWatch,"vLEDTASK", 200, this, 1, NULL);//电量监控 500ms
	xTaskCreate((TaskFunction_t)TASK_BB,"vLEDTASK", 250, this, 1, NULL);//BB响控制 BB标志位
	xTaskCreate((TaskFunction_t)TASK_GetAttitude,"vLEDTASK", 250, this, 2, NULL);//更新姿态 2ms
	xTaskCreate((TaskFunction_t)TASK_Count,"vLEDTASK", 250, this, 3, NULL);//统计1s内姿态解算次数 1s
}
/*B 1 2*/
void Conrtol::Task_GetAttitude()
{
	/*配置6050*/
	mpu6050 mpu6050_Dev=mpu6050();
	vTaskDelay(1000);
	mpu6050_Dev.MPU_6050_FixZero();
	/*设置PID*/
	PID Angle_PID=*(new PID());
	Angle_PID.Target_Value=0;
	Angle_PID.P=0.005f;
	Angle_PID.I=0;
	Angle_PID.D=0;
	Angle_PID.I_Limit_H=100;
	Angle_PID.I_Limit_L=-100;
	Angle_PID.Limit_H=10;
	Angle_PID.Limit_L=-10;
	
	PID Speed_PID=*(new PID());
	Speed_PID.Target_Value=0;
	Speed_PID.P=0.5;
	Speed_PID.I=0;
	Speed_PID.D=0;
	Speed_PID.I_Limit_H=100;
	Speed_PID.I_Limit_L=-100;
	Speed_PID.Limit_H=100;
	Speed_PID.Limit_L=-100;
	
	AHRS ahrs=*(new AHRS());
	portTickType xLastWakeTime = xTaskGetTickCount();
	while(true)
	{
		mpu6050_Dev.Get_ACCEL_GYRO(&accel,&gyro);
		Quat=ahrs.IMUupdateAG(accel,gyro);
		Roll=ahrs.GET_ROLL(Quat);
		if(FLAG_UP)
		{
			Angle_PID.Target_Value+=Speed_PID.PID_UpData(Lspeed);
		}
		else
		{
			Angle_PID.Target_Value=0;
		}
		Lspeed+=Angle_PID.PID_UpData(Roll);//角度PID控制姿态
		Rspeed=Lspeed;
		
		xEventGroupSetBits(Event,MotosChange);
		vTaskDelayUntil(&xLastWakeTime,2);
		Attitude_Count++;
	}
}
//测试
void Conrtol::Task_Count()
{
	while(true)
	{
		vTaskDelay(1000);
		Attitude_1sCount=Attitude_Count;
		Attitude_Count=0;
	}
}
/*B 0*/
void Conrtol::Task_PowerWatch()
{
	UBaseType_t stacksize=0;
	ADCPORT ADC=ADCPORT();
	while(true)
	{
		vTaskDelay(500);
		VB=3*ADC.Get_avegVval(8,3);
		if(VB<6.2f&&FLAG_LowPower==false)
		{
			FLAG_LowPower=true;
			xEventGroupSetBits(Event,BB_4|PowerChange);
		}
		else if (VB>7.5f&&FLAG_LowPower==true)
		{
			FLAG_LowPower=false;
			xEventGroupSetBits(Event,BB_1|PowerChange);
		}
stacksize=uxTaskGetStackHighWaterMark(NULL);		
	}
}
/*A 7*/
void Conrtol::Task_BB()
{
	UBaseType_t stacksize=0;
	uint16_t Interval_Time[4]={200,500,1000,5000};
	uint16_t Interval_Number[4]={10,5,5,3};
	uint8_t BB[4]={BB_1,BB_2,BB_3,BB_4};
	//BB响初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_TypeDef* BB_GPIOx=GPIOA;
	uint16_t BB_Pin=GPIO_Pin_7;
	GPIO_ResetBits(BB_GPIOx,BB_Pin);//关闭BB
	GPIO_InitTypeDef GPIO_InitData;
	GPIO_InitData.GPIO_Pin = BB_Pin;
	GPIO_InitData.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitData.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(BB_GPIOx, &GPIO_InitData);
	while(true)
	{
		uint16_t event=xEventGroupWaitBits(Event,0x00f0,pdTRUE,pdFALSE,5000);
		for(uint8_t i=0;i<4;i++)
		{
			if(event&BB[i])
			{
				uint16_t Number=Interval_Number[i];
				while(Number--)
				{
					GPIO_SetBits(BB_GPIOx,BB_Pin);//打开BB
					vTaskDelay(Interval_Time[i]);
					GPIO_ResetBits(BB_GPIOx,BB_Pin);//关闭BB
					vTaskDelay(Interval_Time[i]);
				}
			}	
		}
stacksize=uxTaskGetStackHighWaterMark(NULL);		
	}
}
/*A 0 1 2 3 4 5 6*/
void Conrtol::Task_Moto()
{
	//电机初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	uint16_t LF=GPIO_Pin_2;
	uint16_t LB=GPIO_Pin_3;
	
	uint16_t RF=GPIO_Pin_4;
	uint16_t RB=GPIO_Pin_5;
	
	uint16_t STBY=GPIO_Pin_6;
	
	GPIO_ResetBits(GPIOA,LF|LB|RF|RB|STBY);//关闭电机
	
	GPIO_InitTypeDef GPIO_InitData;
	GPIO_InitData.GPIO_Pin = LF|LB|RF|RB|STBY;
	GPIO_InitData.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitData.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitData);
	
	PWMPORT PWMPORT_1=PWMPORT();
	PWMPORT_1.TIMx=TIM2;
	PWMPORT_1.ARR=1000;
	PWMPORT_1.PSC=360;
	PWMPORT_1.GPIOx=GPIOA;
	PWMPORT_1.PWM0_Pin=GPIO_Pin_0;
	PWMPORT_1.PWM1_Pin=GPIO_Pin_1;
	PWMPORT_1.Init();
	
	PWMPORT_1.SetPort_0(0);//L
	PWMPORT_1.SetPort_1(0);//R
	
	while(true)
	{
		if(xEventGroupWaitBits(Event,MotosChange,pdTRUE,pdFALSE,5000)&MotosChange)
		{
			Lspeed=30;
			Rspeed=30;
			if(!FLAG_LowPower)
			{
				GPIO_SetBits(GPIOA,STBY);//开启电机
				if(Lspeed>0)
				{
					/*前*/
					GPIO_SetBits(GPIOA,LF);
					GPIO_ResetBits(GPIOA,LB);
					PWMPORT_1.SetPort_0(Lspeed);//L
				}
				else
				{
					/*后*/
					GPIO_SetBits(GPIOA,LB);
					GPIO_ResetBits(GPIOA,LF);
					PWMPORT_1.SetPort_0(-Lspeed);//L
					
				}
				if(Rspeed>0)
				{
					/*前*/
					GPIO_SetBits(GPIOA,RF);
					GPIO_ResetBits(GPIOA,RB);
					PWMPORT_1.SetPort_1(Rspeed);//R
				}
				else
				{
					/*后*/
					GPIO_SetBits(GPIOA,RB);
					GPIO_ResetBits(GPIOA,RF);
					PWMPORT_1.SetPort_1(-Rspeed);//R
				}
			}
			else
			{
				Lspeed=0;
				Rspeed=0;
				GPIO_ResetBits(GPIOA,LF|LB|RF|RB|STBY);//关闭电机
				PWMPORT_1.SetPort_0(0);
				PWMPORT_1.SetPort_1(0);
			}
		}
		Task_Moto_stacksize=uxTaskGetStackHighWaterMark(NULL);		
	}

}