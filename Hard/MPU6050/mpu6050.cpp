#include "mpu6050.h"
mpu6050::mpu6050()
{
	uint16_t SDA=GPIO_Pin_1;
	uint16_t SCL=GPIO_Pin_2;
	pI2CPort=new I2CPORT(SCL,SDA,GPIOB);
	
	uint8_t MAXerrorTime=5,IDbuffer;
	uint16_t i=0,j=100;  
	while(j--)
	{
	  i=12000;  //自己定义
	  while(i--) ;    
	}
	do
	{
		MPU6050_I2C_BufferRead(0xD0,&IDbuffer,MPU6050_RA_WHO_AM_I,1);
		if (MPU6050_I2C_ByteWrite(0xD0,0x00,MPU6050_RA_PWR_MGMT_1)==false) continue;      // reg107, 唤醒，8M内部时钟源
		if (MPU6050_I2C_ByteWrite(0xD0,0x07,MPU6050_RA_SMPLRT_DIV)==false) continue;         //采用频率 1000
		if (MPU6050_I2C_ByteWrite(0xD0,0x06,MPU6050_RA_CONFIG)==false) continue;                 
		if (MPU6050_I2C_ByteWrite(0xD0,0x01,MPU6050_RA_ACCEL_CONFIG)==false) continue;     //加速度量程 2g
		if (MPU6050_I2C_ByteWrite(0xD0,0x18,MPU6050_RA_GYRO_CONFIG)==false) continue;          //角速度量程 2000度/s
		for(uint16_t i=0;i<1000;i++);//延时
		mpu6050_Status=MPU6050_OK;
		return;
	}
	while (MAXerrorTime--);//当最大错误数累计到0时返回错误
	mpu6050_Status=MPU6050_CLOUSE;
}
bool mpu6050::MPU6050_I2C_ByteWrite(u8 slaveAddr, u8 pBuffer, u8 writeAddr)
{
	uint8_t MAXerrortime=5;
	/**/
	do
	{
		pI2CPort->Signal_GenerateSTART();
		pI2CPort->Signal_Send7bitAddress(slaveAddr,0);//w
		if (pI2CPort->Signal_Check ()==false) {mpu6050_ERROR_FLAG=wDEVaddr_NOask;pI2CPort->Signal_GenerateSTOP ();continue;}
		pI2CPort->Signal_Write8bit (writeAddr);
		if (pI2CPort->Signal_Check ()==false) {mpu6050_ERROR_FLAG=wREGaddr_NOask;pI2CPort->Signal_GenerateSTOP ();continue;}
		pI2CPort->Signal_Write8bit (pBuffer);
		if (pI2CPort->Signal_Check ()==false) {mpu6050_ERROR_FLAG=wDATA_NOask;pI2CPort->Signal_GenerateSTOP ();continue;}
		pI2CPort->Signal_GenerateSTOP ();
		/**/
		return true;
	}
	while (MAXerrortime--);
	/**/
	return false;
}
bool mpu6050::MPU6050_I2C_BufferRead(u8 slaveAddr, uint8_t* pBuffer, u8 readAddr,uint8_t readtimes)
{
	uint8_t n=0,MAXerrortime=5;
	/**/
	do
	{
		pI2CPort->Signal_GenerateSTART();
		pI2CPort->Signal_Send7bitAddress (slaveAddr,0);//w
		if (pI2CPort->Signal_Check ()==false) {mpu6050_ERROR_FLAG=wDEVaddr_NOask;pI2CPort->Signal_GenerateSTOP ();continue;}
		pI2CPort->Signal_Write8bit (readAddr);
		if (pI2CPort->Signal_Check ()==false) {mpu6050_ERROR_FLAG=wREGaddr_NOask;pI2CPort->Signal_GenerateSTOP ();continue;}
		pI2CPort->Signal_GenerateSTOP ();
		pI2CPort->Signal_GenerateSTART ();
		pI2CPort->Signal_Send7bitAddress (slaveAddr,1);//r
		if (pI2CPort->Signal_Check ()==false) {mpu6050_ERROR_FLAG=wDEVaddr_NOask;pI2CPort->Signal_GenerateSTOP ();continue;}
		while (readtimes)
		{	
			pBuffer[n++]=pI2CPort->Signal_Read8bit();
			if (--readtimes==0) {pI2CPort->Signal_Nack ();break;};
			pI2CPort->Signal_Ack();
		}
		pI2CPort->Signal_GenerateSTOP ();
		/**/
		return true;
	}
	while (MAXerrortime--);
	/**/
	return false;
}
bool mpu6050::Get_TEMP (float* TEMPbuff)
{
	uint8_t tmpBuffer[2];
	short TEMP=0;
    if (MPU6050_I2C_BufferRead(0xD0,tmpBuffer,MPU6050_RA_TEMP_OUT_H,2)==false) return false;;     //读取温度值
    TEMP = (tmpBuffer[0] << 8) | tmpBuffer[1];
	*TEMPbuff=36.53f+TEMP/340.0f;
	return true;
}
bool mpu6050::Get_ACCEL (float* ACCEL)
{
	uint8_t  tmpBuffer[6],i;
	if (MPU6050_I2C_BufferRead(0xD0,tmpBuffer,MPU6050_RA_ACCEL_XOUT_H,6)==false) return false;
	for (i=0;i<3;i++)
	{
		ACCEL[i]=((float)((short)((tmpBuffer[i*2]<<8) | (tmpBuffer[(i*2)+1]))))/16384;
	}
	return true;
}
bool mpu6050::Get_GYRO (float* GYRO)
{
	uint8_t tmpBuffer[6],i;  
	if (MPU6050_I2C_BufferRead(0xD0,tmpBuffer,MPU6050_RA_GYRO_XOUT_H,6)==false) return false;
	for (i=0;i<3;i++)
	{
		GYRO[i]=(((float)((short)((tmpBuffer[i*2]<<8) | (tmpBuffer[(i*2)+1]))))/16.4f)-fix_gyro_zero[i];
	}
	if (GYRO[2]<0.3f && GYRO[2]>-0.3f) GYRO[2]=0.0f;
	return true;
}
bool mpu6050::Get_ACCEL_GYRO (float * ACCEL_GYRO)
{
	float A[3],G[3];
	if ((Get_ACCEL (A)&&Get_GYRO (G))==false) return false;
	for (uint8_t i=0;i<3;i++)
	{
		ACCEL_GYRO[i]=A[i];
	}
		for (uint8_t i=0;i<3;i++)
	{
		ACCEL_GYRO[i+3]=G[i];
	}
	return true;
}
bool mpu6050::Get_ACCEL_GYRO (ACCEL* accel_BUFF,GYRO* gyro_BUFF)
{
	float A[3],G[3];
	if ((Get_ACCEL (A)&&Get_GYRO (G))==false) return false;
	accel_BUFF->x=A[0];
	accel_BUFF->y=A[1];
	accel_BUFF->z=A[2];
	
	gyro_BUFF->x=G[0];
	gyro_BUFF->y=G[1];
	gyro_BUFF->z=G[2];
	return true;
}
/*角加速度归零值*/
void mpu6050::MPU_6050_FixZero()
{
	uint8_t tmpBuffer[6];
	while(1)
	{
		for (uint16_t j=0;j<100;++j)//采样角加速度100次
		{
			MPU6050_I2C_BufferRead(0xD0,tmpBuffer,MPU6050_RA_GYRO_XOUT_H,6);
			for (uint8_t i=0;i<3;i++)
			{
				fix_gyro_zero[i] +=((float)((short)((tmpBuffer[i*2]<<8) | (tmpBuffer[(i*2)+1]))))/16.4f;
			}
		}
		for (uint8_t j=0;j<3;++j)
		{
			fix_gyro_zero[j] /= 100;
		}
		if (fabs(fix_gyro_zero[0])<15.0f && fabs(fix_gyro_zero[1])<5.0f && fabs(fix_gyro_zero[2])<1.8f) break;//防止角加速度归零值没有被干扰
		for (uint8_t j=0;j<3;++j)
		{
			fix_gyro_zero[j] = 0;
		}
	}
}
