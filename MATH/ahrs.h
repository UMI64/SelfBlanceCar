/*-----------------------------------------------------------------------------------
										STM32F401+MPU6050+AHRS�㷨ʵ��
	
	���: ͨ����Ԫ����������Ѽ��ٶȼƺ������ǵ����ݽ����ںϼ����ŷ����.
			 
	����: BoX
	�汾: V1.0
	ƽ̨: STM32F401RE Nucleo Board
	����: 2014��7��6��
------------------------------------------------------------------------------------*/
#ifndef __AHRS__H__
#define __AHRS__H__

#include "stm32f10x.h"
#include "math.h"
/* include "arm_math.h" */

/*------------------------------------------
						�궨����ѧ����
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
							ŷ���ǽṹ��
------------------------------------------*/
struct AHRS_EulerAngleTypeDef
{
  float Pitch;
  float Roll;
  float Yaw;
};
/*------------------------------------------
							��Ԫ���ṹ��
------------------------------------------*/
struct AHRS_QuaternionTypeDef
{
  float q0;
  float q1;
  float q2;
  float q3;
};
/*------------------------------------------
							���ٶȽṹ��
------------------------------------------*/
struct ACCEL
{
	float x;
	float y;
	float z;
};
/*------------------------------------------
							�Ǽ��ٶȽṹ��
------------------------------------------*/
struct GYRO
{
	float x;
	float y;
	float z;
};
/*------------------------------------------
							��������
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

