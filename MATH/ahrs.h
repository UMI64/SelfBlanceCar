/*-----------------------------------------------------------------------------------
										STM32F401+MPU6050+AHRS算法实现
	
	简介: 通过四元数矩阵运算把加速度计和陀螺仪的数据进行融合计算出欧拉角.
			 
	作者: BoX
	版本: V1.0
	平台: STM32F401RE Nucleo Board
	日期: 2014年7月6日
------------------------------------------------------------------------------------*/
#ifndef __AHRS__H__
#define __AHRS__H__

#include "stm32f10x.h"
#include "math.h"
/* include "arm_math.h" */

/*------------------------------------------
						宏定义数学运算
------------------------------------------*/
#define squa( Sq )      (((float)Sq)*((float)Sq))
#define toRad( Math_D )	((float)(Math_D)*0.0174532925f)
#define toDeg( Math_R )	((float)(Math_R)*57.2957795f)
	
#define MPU6050_SAMPLERATE_FEQ  	 ((uint16_t)500) // 500Hz
#define MPU6050_SAMPLERATE_PRI  	 ((float)0.002f) // 2.0ms
#define MPU6050_SAMPLERATE_HALFPRI ((float)0.001f) // 1.0ms
struct Vector
{
	float x;
	float y;
	float z;
};
/*------------------------------------------
							欧拉角结构体
------------------------------------------*/
struct AHRS_EulerAngleTypeDef
{
  float Pitch;
  float Roll;
  float Yaw;
};
/*------------------------------------------
							四元数结构体
------------------------------------------*/
struct AHRS_QuaternionTypeDef
{
  float q0;
  float q1;
  float q2;
  float q3;
};
/*------------------------------------------
							加速度结构体
------------------------------------------*/
struct ACCEL
{
	float x;
	float y;
	float z;
};
/*------------------------------------------
							角加速度结构体
------------------------------------------*/
struct GYRO
{
	float x;
	float y;
	float z;
};
/*------------------------------------------
							函数声明
------------------------------------------*/
class AHRS
{
	public:
	AHRS_EulerAngleTypeDef IMUupdateAGM(float * AGBuff);
	AHRS_QuaternionTypeDef IMUupdateAG(float * AGBuff);
	AHRS_QuaternionTypeDef IMUupdateAG(const ACCEL accel,const GYRO gyro);
	Vector QuadNumberRoll(Vector V,AHRS_QuaternionTypeDef QuadNumber);
	float AngleOf(Vector V1,Vector V2);
	float GET_ROLL(AHRS_QuaternionTypeDef QuadNumber);
	float GET_PATH(AHRS_QuaternionTypeDef QuadNumber);
	AHRS_EulerAngleTypeDef GET_EulerAngle(AHRS_QuaternionTypeDef QuadNumber);
	private:
	AHRS_QuaternionTypeDef  QuadNumber = {1, 0, 0, 0};
};
#endif

