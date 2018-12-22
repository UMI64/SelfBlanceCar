#include "ahrs.h"
/*------------------------------------------
								IMU参数
------------------------------------------*/
#define Kp 							  2.0f
#define Ki 							  0.001f
//计算1/根号x
float InvSqrt(float x)
{
    float xhalf = 0.5f*x;
    int i = *(int*)&x;       // get bits for floating value
    i = 0x5f3759df - (i>>1); // gives initial guess y0
    x = *(float*)&i;         // convert bits back to float
    x = x*(1.5f-xhalf*x*x);  // Newton step, repeating increases accuracy
    return x;
}
//根据加速度，角加速度，地磁计算欧拉角
AHRS_EulerAngleTypeDef AHRS::IMUupdateAGM(float * AGBuff)
{
	AHRS_EulerAngleTypeDef EulerAngle={0};
	//static float AngZ_Temp = 0.0f;
	static float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;  
	float gx, gy, gz;
	float q0q0 = QuadNumber.q0 * QuadNumber.q0;
	float q0q1 = QuadNumber.q0 * QuadNumber.q1;
	float q0q2 = QuadNumber.q0 * QuadNumber.q2;
	float q0q3 = QuadNumber.q0 * QuadNumber.q3;
	float q1q1 = QuadNumber.q1 * QuadNumber.q1;
	float q1q2 = QuadNumber.q1 * QuadNumber.q2;
	float q1q3 = QuadNumber.q1 * QuadNumber.q3;
	float q2q2 = QuadNumber.q2 * QuadNumber.q2;
	float q2q3 = QuadNumber.q2 * QuadNumber.q3;
	float q3q3 = QuadNumber.q3 * QuadNumber.q3;
	// 测量正常化
	norm = InvSqrt(AGBuff[0]*AGBuff[0] + AGBuff[1]*AGBuff[1] + AGBuff[2]*AGBuff[2]);      
	AGBuff[0] = AGBuff[0] * norm;                   //单位化
	AGBuff[1] = AGBuff[1] * norm;
	AGBuff[2] = AGBuff[2] * norm;      
	// 估计方向的重力
	vx = 2*(q1q3 - q0q2);
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	// 错误的领域和方向传感器测量参考方向之间的交叉乘积的总和
	ex = (AGBuff[1]*vz - AGBuff[2]*vy);
	ey = (AGBuff[2]*vx - AGBuff[0]*vz);
	ez = (AGBuff[0]*vy - AGBuff[1]*vx);
	// 积分误差比例积分增益
	exInt = exInt + ex*Ki*MPU6050_SAMPLERATE_HALFPRI;
	eyInt = eyInt + ey*Ki*MPU6050_SAMPLERATE_HALFPRI;
	ezInt = ezInt + ez*Ki*MPU6050_SAMPLERATE_HALFPRI;
	// 调整后的陀螺仪测量
	gx = toRad(AGBuff[3]) + Kp*ex + exInt;
	gy = toRad(AGBuff[4]) + Kp*ey + eyInt;
	gz = toRad(AGBuff[5]) + Kp*ez + ezInt;
	// 整合四元数率和正常化
	QuadNumber.q0 = QuadNumber.q0 + (-QuadNumber.q1*gx - QuadNumber.q2*gy - QuadNumber.q3*gz)*MPU6050_SAMPLERATE_HALFPRI;
	QuadNumber.q1 = QuadNumber.q1 + (QuadNumber.q0*gx + QuadNumber.q2*gz - QuadNumber.q3*gy)*MPU6050_SAMPLERATE_HALFPRI;
	QuadNumber.q2 = QuadNumber.q2 + (QuadNumber.q0*gy - QuadNumber.q1*gz + QuadNumber.q3*gx)*MPU6050_SAMPLERATE_HALFPRI;
	QuadNumber.q3 = QuadNumber.q3 + (QuadNumber.q0*gz + QuadNumber.q1*gy - QuadNumber.q2*gx)*MPU6050_SAMPLERATE_HALFPRI;  
	// 正常化四元
	norm = InvSqrt(squa(QuadNumber.q0) + squa(QuadNumber.q1) + squa(QuadNumber.q2) + squa(QuadNumber.q3));
	QuadNumber.q0 = QuadNumber.q0 * norm;
	QuadNumber.q1 = QuadNumber.q1 * norm;
	QuadNumber.q2 = QuadNumber.q2 * norm;
	QuadNumber.q3 = QuadNumber.q3 * norm;
	EulerAngle.Pitch  = asin(- 2 * (QuadNumber.q1* QuadNumber.q3 - QuadNumber.q0 * QuadNumber.q2)); // pitch 
	EulerAngle.Roll = atan2(2 * (QuadNumber.q2 * QuadNumber.q3 + QuadNumber.q0 * QuadNumber.q1),1 - 2 * (squa(QuadNumber.q1) + squa(QuadNumber.q2))); // roll
	EulerAngle.Yaw = atan2(2*(QuadNumber.q1*QuadNumber.q2 + QuadNumber.q0*QuadNumber.q3),1 - 2 * (squa(QuadNumber.q2) + squa(QuadNumber.q3))); // yaw
	EulerAngle.Pitch=toDeg(EulerAngle.Pitch);
	EulerAngle.Roll=toDeg(EulerAngle.Roll);
	EulerAngle.Yaw=toDeg(EulerAngle.Yaw);
//	/* 互补滤波 */
//  #define CF_A 1f
//  #define CF_B 0f
//  AngZ_Temp = AngZ_Temp + AGBuff[5] * 0.0165f;
//  AngZ_Temp = CF_A * AngZ_Temp + CF_B * EulerAngle.Yaw;
//	
//  if(AngZ_Temp>180.0f)
//    EulerAngle.Yaw = -360.0f - AngZ_Temp;
//  else if(AngZ_Temp<-180.0f)
//    EulerAngle.Yaw = 360.0f + AngZ_Temp;
//  else
//    EulerAngle.Yaw = AngZ_Temp;
	return EulerAngle;
}
//根据加速度和角加速度计算四元数
AHRS_QuaternionTypeDef AHRS::IMUupdateAG(float * AGBuff)
{
	float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;  
	float gx, gy, gz;
	float q0q0 = QuadNumber.q0 * QuadNumber.q0;
	float q0q1 = QuadNumber.q0 * QuadNumber.q1;
	float q0q2 = QuadNumber.q0 * QuadNumber.q2;
	float q0q3 = QuadNumber.q0 * QuadNumber.q3;
	float q1q1 = QuadNumber.q1 * QuadNumber.q1;
	float q1q2 = QuadNumber.q1 * QuadNumber.q2;
	float q1q3 = QuadNumber.q1 * QuadNumber.q3;
	float q2q2 = QuadNumber.q2 * QuadNumber.q2;
	float q2q3 = QuadNumber.q2 * QuadNumber.q3;
	float q3q3 = QuadNumber.q3 * QuadNumber.q3;
	// 测量正常化
	norm = InvSqrt(AGBuff[0]*AGBuff[0] + AGBuff[1]*AGBuff[1] + AGBuff[2]*AGBuff[2]);      
	AGBuff[0] = AGBuff[0] * norm;                   //单位化
	AGBuff[1] = AGBuff[1] * norm;
	AGBuff[2] = AGBuff[2] * norm;      
	// 估计方向的重力
	vx = 2*(q1q3 - q0q2);
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	// 错误的领域和方向传感器测量参考方向之间的交叉乘积的总和
	ex = (AGBuff[1]*vz - AGBuff[2]*vy);
	ey = (AGBuff[2]*vx - AGBuff[0]*vz);
	ez = (AGBuff[0]*vy - AGBuff[1]*vx);
	// 积分误差比例积分增益
	exInt = exInt + ex*Ki*MPU6050_SAMPLERATE_HALFPRI;
	eyInt = eyInt + ey*Ki*MPU6050_SAMPLERATE_HALFPRI;
	ezInt = ezInt + ez*Ki*MPU6050_SAMPLERATE_HALFPRI;
	// 调整后的陀螺仪测量
	gx = toRad(AGBuff[3]) + Kp*ex + exInt;
	gy = toRad(AGBuff[4]) + Kp*ey + eyInt;
	gz = toRad(AGBuff[5]) + Kp*ez + ezInt;
	// 整合四元数率和正常化
	QuadNumber.q0 = QuadNumber.q0 + (-QuadNumber.q1*gx - QuadNumber.q2*gy - QuadNumber.q3*gz)*MPU6050_SAMPLERATE_HALFPRI;
	QuadNumber.q1 = QuadNumber.q1 + (QuadNumber.q0*gx + QuadNumber.q2*gz - QuadNumber.q3*gy)*MPU6050_SAMPLERATE_HALFPRI;
	QuadNumber.q2 = QuadNumber.q2 + (QuadNumber.q0*gy - QuadNumber.q1*gz + QuadNumber.q3*gx)*MPU6050_SAMPLERATE_HALFPRI;
	QuadNumber.q3 = QuadNumber.q3 + (QuadNumber.q0*gz + QuadNumber.q1*gy - QuadNumber.q2*gx)*MPU6050_SAMPLERATE_HALFPRI;  
	// 正常化四元
	norm = InvSqrt(squa(QuadNumber.q0) + squa(QuadNumber.q1) + squa(QuadNumber.q2) + squa(QuadNumber.q3));
	QuadNumber.q0 = QuadNumber.q0 * norm;
	QuadNumber.q1 = QuadNumber.q1 * norm;
	QuadNumber.q2 = QuadNumber.q2 * norm;
	QuadNumber.q3 = QuadNumber.q3 * norm;
	return QuadNumber;
}
//根据加速度和角加速度计算四元数
AHRS_QuaternionTypeDef AHRS::IMUupdateAG(ACCEL accel,GYRO gyro)
{
	float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;
	float gx, gy, gz;
	float q0q0 = QuadNumber.q0 * QuadNumber.q0;
	float q0q1 = QuadNumber.q0 * QuadNumber.q1;
	float q0q2 = QuadNumber.q0 * QuadNumber.q2;
	float q0q3 = QuadNumber.q0 * QuadNumber.q3;
	float q1q1 = QuadNumber.q1 * QuadNumber.q1;
	float q1q2 = QuadNumber.q1 * QuadNumber.q2;
	float q1q3 = QuadNumber.q1 * QuadNumber.q3;
	float q2q2 = QuadNumber.q2 * QuadNumber.q2;
	float q2q3 = QuadNumber.q2 * QuadNumber.q3;
	float q3q3 = QuadNumber.q3 * QuadNumber.q3;
	// 测量正常化
	norm = InvSqrt(accel.x*accel.x + accel.y*accel.y + accel.z*accel.z);      
	accel.x = accel.x * norm;                   //单位化
	accel.y = accel.y * norm;
	accel.z = accel.z * norm;      
	// 估计方向的重力
	vx = 2*(q1q3 - q0q2);
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	// 错误的领域和方向传感器测量参考方向之间的交叉乘积的总和
	ex = (accel.y*vz - accel.z*vy);
	ey = (accel.z*vx - accel.x*vz);
	ez = (accel.x*vy - accel.y*vx);
	// 积分误差比例积分增益
	exInt = exInt + ex*Ki*MPU6050_SAMPLERATE_HALFPRI;
	eyInt = eyInt + ey*Ki*MPU6050_SAMPLERATE_HALFPRI;
	ezInt = ezInt + ez*Ki*MPU6050_SAMPLERATE_HALFPRI;
	// 调整后的陀螺仪测量
	gx = toRad(gyro.x) + Kp*ex + exInt;
	gy = toRad(gyro.y) + Kp*ey + eyInt;
	gz = toRad(gyro.z) + Kp*ez + ezInt;
	// 整合四元数率和正常化
	QuadNumber.q0 = QuadNumber.q0 + (-QuadNumber.q1*gx - QuadNumber.q2*gy - QuadNumber.q3*gz)*MPU6050_SAMPLERATE_HALFPRI;
	QuadNumber.q1 = QuadNumber.q1 + (QuadNumber.q0*gx + QuadNumber.q2*gz - QuadNumber.q3*gy)*MPU6050_SAMPLERATE_HALFPRI;
	QuadNumber.q2 = QuadNumber.q2 + (QuadNumber.q0*gy - QuadNumber.q1*gz + QuadNumber.q3*gx)*MPU6050_SAMPLERATE_HALFPRI;
	QuadNumber.q3 = QuadNumber.q3 + (QuadNumber.q0*gz + QuadNumber.q1*gy - QuadNumber.q2*gx)*MPU6050_SAMPLERATE_HALFPRI;  
	// 正常化四元
	norm = InvSqrt(squa(QuadNumber.q0) + squa(QuadNumber.q1) + squa(QuadNumber.q2) + squa(QuadNumber.q3));
	QuadNumber.q0 = QuadNumber.q0 * norm;
	QuadNumber.q1 = QuadNumber.q1 * norm;
	QuadNumber.q2 = QuadNumber.q2 * norm;
	QuadNumber.q3 = QuadNumber.q3 * norm;
	return QuadNumber;
}
//向量根据四元数旋转
Vector AHRS::QuadNumberRoll(Vector V,AHRS_QuaternionTypeDef QuadNumber)
{
	Vector returnv;
	float trans[3][3];
	trans[0][0]=powf(QuadNumber.q0,2)+powf(QuadNumber.q1,2)-powf(QuadNumber.q2,2)-powf(QuadNumber.q3,2);
	trans[0][1]=2*(QuadNumber.q1*QuadNumber.q2-QuadNumber.q0*QuadNumber.q3);
	trans[0][2]=2*(QuadNumber.q1*QuadNumber.q3+QuadNumber.q0*QuadNumber.q2);
	trans[1][0]=2*(QuadNumber.q1*QuadNumber.q2+QuadNumber.q0*QuadNumber.q3);
	trans[1][1]=powf(QuadNumber.q0,2)-powf(QuadNumber.q1,2)+powf(QuadNumber.q2,2)-powf(QuadNumber.q3,2);
	trans[1][2]=2*(QuadNumber.q2*QuadNumber.q3-QuadNumber.q0*QuadNumber.q1);
	trans[2][0]=2*(QuadNumber.q1*QuadNumber.q3-QuadNumber.q0*QuadNumber.q2);
	trans[2][1]=2*(QuadNumber.q2*QuadNumber.q3+QuadNumber.q0*QuadNumber.q1);
	trans[2][2]=powf(QuadNumber.q0,2)-powf(QuadNumber.q1,2)-powf(QuadNumber.q2,2)+powf(QuadNumber.q3,2);
	returnv.x=trans[0][0]*V.x+trans[0][1]*V.y+trans[0][2]*V.z;
	returnv.y=trans[1][0]*V.x+trans[1][1]*V.y+trans[1][2]*V.z;
	returnv.z=trans[2][0]*V.x+trans[2][1]*V.y+trans[2][2]*V.z;
	return returnv;
}
//计算两个向量的夹角
float AHRS::AngleOf(Vector V1,Vector V2)
{
	float cos;
	cos=(V1.x*V2.x+V1.y*V2.y+V1.z*V2.z)/sqrt((V1.x*V1.x+V1.y*V1.y+V1.z*V1.z)*(V2.x*V2.x+V2.y*V2.y+V2.z*V2.z));
	return acos(cos);
}
//计算四元数的ROLL
float AHRS::GET_ROLL(AHRS_QuaternionTypeDef QuadNumber)
{
	float angle=atan2(2 * (QuadNumber.q2 * QuadNumber.q3 + QuadNumber.q0 * QuadNumber.q1),1 - 2 * (squa(QuadNumber.q1) + squa(QuadNumber.q2))); // roll;
	return toDeg(angle);
}
//计算四元数的PATH
float AHRS::GET_PATH(AHRS_QuaternionTypeDef QuadNumber)
{
	float angle=asin(- 2 * (QuadNumber.q1* QuadNumber.q3 - QuadNumber.q0 * QuadNumber.q2)); // pitch ;
	return toDeg(angle);
}
//用四元数计算欧拉角
AHRS_EulerAngleTypeDef AHRS::GET_EulerAngle(AHRS_QuaternionTypeDef QuadNumber)
{
	AHRS_EulerAngleTypeDef EulerAngle;
	EulerAngle.Pitch  = asin(- 2 * (QuadNumber.q1* QuadNumber.q3 - QuadNumber.q0 * QuadNumber.q2)); // pitch 
	EulerAngle.Roll = atan2(2 * (QuadNumber.q2 * QuadNumber.q3 + QuadNumber.q0 * QuadNumber.q1),1 - 2 * (squa(QuadNumber.q1) + squa(QuadNumber.q2))); // roll
	EulerAngle.Yaw = atan2(2*(QuadNumber.q1*QuadNumber.q2 + QuadNumber.q0*QuadNumber.q3),1 - 2 * (squa(QuadNumber.q2) + squa(QuadNumber.q3))); // yaw
	EulerAngle.Pitch=toDeg(EulerAngle.Pitch);
	EulerAngle.Roll=toDeg(EulerAngle.Roll);
	EulerAngle.Yaw=toDeg(EulerAngle.Yaw);
	return EulerAngle;
}
