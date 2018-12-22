#include "I2C.h"

void I2CPORT::Init ()
{	
	if(GPIOx==GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if(GPIOx==GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if(GPIOx==GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if(GPIOx==GPIOD) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if(GPIOx==GPIOE) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if(GPIOx==GPIOF) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if(GPIOx==GPIOG) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitData;
	GPIO_InitData.GPIO_Pin = scl | sda;
	GPIO_InitData.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitData.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOx, &GPIO_InitData);
	
	GPIO_SetBits(GPIOx,scl);//ÊÍ·Å×ÜÏß
	GPIO_SetBits(GPIOx,sda);//
}
void I2CPORT::SDA_ToInput()
{
	GPIO_InitTypeDef GPIOstr;
	GPIOstr.GPIO_Pin = sda;
	GPIOstr.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOstr.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOx, &GPIOstr);
}
void I2CPORT::SDA_ToOutput()
{
	GPIO_InitTypeDef GPIOstr;
	GPIOstr.GPIO_Pin = sda;
	GPIOstr.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOstr.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOx, &GPIOstr);
}
inline void I2CPORT::Wait()
{
	uint16_t time=waittime;
	while (time--);
}
void I2CPORT::Signal_GenerateSTART()
{
	GPIO_SetBits(GPIOx,scl);//1
	GPIO_SetBits(GPIOx,sda);//1
	Wait();
	GPIO_ResetBits(GPIOx,sda);//0
}
void I2CPORT::Signal_GenerateSTOP()
{
	GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::sda);//0
	Wait();
	GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::scl);//1
	Wait();
	GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::sda);//1
}
void I2CPORT::Signal_Ack ()
{
	GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::scl);//0
	Wait();
	GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::sda);//0
	Wait();
	GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::scl);//1
	Wait();
	GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::scl);//0
	GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::sda);//1
	Wait();
}
void I2CPORT::Signal_Nack()
{
	GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::scl);//0
	Wait();
	GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::sda);//1
	Wait();
	GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::scl);//1
	Wait();
	GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::scl);//0
}
bool I2CPORT::Signal_Check ()
{
	uint16_t c=0;
	GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::scl);//0
	//Wait();
	//GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::sda);//1
	SDA_ToInput();
	GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::sda);//1
	GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::scl);//1
	while ((I2CPORT::GPIOx->IDR & I2CPORT::sda) != (uint32_t)Bit_RESET)
	{
		c++;
		if (c==3000)
		{
			SDA_ToOutput();
			return false;
		}
	}
	GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::scl);//0
	SDA_ToOutput();
	return true;
}
void I2CPORT::Signal_Send7bitAddress(uint8_t slaveAddr,uint8_t rw)
{
	uint8_t i,data=slaveAddr | rw;
	for (i=0;i<8;i++)
		{
			GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::scl);//0
			Wait();
			if (data&0x80)
			{
				GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::sda);//1
			}
			else
			{
				GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::sda);//0
			}
			Wait();
			GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::scl);//1
			Wait();
			Wait();
			GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::scl);//0
			data<<=1;
		}
}
void I2CPORT::Signal_Write8bit(uint8_t data)
{
	uint8_t i;
	for (i=0;i<8;i++)
	{
		GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::scl);//0
		Wait();
		if (data&0x80)
		{
			GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::sda);//1
		}
		else
		{
			GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::sda);//0
		}
		Wait();
		GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::scl);//1
		Wait();
		Wait();
		GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::scl);//0
		data<<=1;
	}
}
uint8_t I2CPORT::Signal_Read8bit()
{
	uint8_t data=0,i=0;
	SDA_ToInput();
	while (i<8)
	{
		GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::scl);//0
		Wait();
		GPIO_SetBits(I2CPORT::GPIOx,I2CPORT::scl);//1
		Wait();
		data=data | ((I2CPORT::GPIOx->IDR & I2CPORT::sda) != (uint32_t)Bit_RESET);
		GPIO_ResetBits(I2CPORT::GPIOx,I2CPORT::scl);//0
		if (i==7) 
		{
			SDA_ToOutput();	
			return data;
		}
		i++;
		data<<=1;
	}
	SDA_ToOutput();
	return 0;
}
uint8_t I2CPORT::Device_WriteN8bit(u8 Times , u8 slaveAddr , u8 data)
{
		Signal_GenerateSTART();
		Signal_Send7bitAddress (slaveAddr,0);//w
		if (Signal_Check ()==false) {Signal_GenerateSTOP ();return 0;}
		for (uint8_t time=0;time<Times;time++)
		{
			Signal_Write8bit (data);
			if (Signal_Check ()==false) {Signal_GenerateSTOP ();return 2;}
		}
		Signal_GenerateSTOP ();
		return 3;
}
uint8_t I2CPORT::Device_WriteN8bit(u8 Times , u8 slaveAddr , u8 writeAddr , u8 * data)
{
		Signal_GenerateSTART();
		Signal_Send7bitAddress (slaveAddr,0);//w
		if (Signal_Check ()==false) {Signal_GenerateSTOP ();return 0;}
		Signal_Write8bit (writeAddr);
		if (Signal_Check ()==false) {Signal_GenerateSTOP ();return 1;}
		for (uint8_t time=0;time<Times;time++)
		{
			Signal_Write8bit (*data);
			if (Signal_Check ()==false) {Signal_GenerateSTOP ();return 2;}
		}
		Signal_GenerateSTOP ();
		return 3;
}
uint8_t I2CPORT::Device_WriteN8bit(u8 Times , u8 slaveAddr , u8 writeAddr , u8 data)
{
		Signal_GenerateSTART();
		Signal_Send7bitAddress (slaveAddr,0);//w
		if (Signal_Check ()==false) {Signal_GenerateSTOP ();return 0;}
		Signal_Write8bit (writeAddr);
		if (Signal_Check ()==false) {Signal_GenerateSTOP ();return 1;}
		for (uint8_t time=0;time<Times;time++)
		{
			Signal_Write8bit (data);
			if (Signal_Check ()==false) {Signal_GenerateSTOP ();return 2;}
		}
		Signal_GenerateSTOP ();
		return 3;
}
uint8_t I2CPORT::Device_WriteN8bit(u8 Times , u8 slaveAddr , u8 FirstWriteAddr, u8 SecondWriteAddr,u8 * data)
{
		Signal_GenerateSTART();
		Signal_Send7bitAddress (slaveAddr,0);//w
		if (Signal_Check ()==false) {Signal_GenerateSTOP ();return 0;}
		Signal_Write8bit (FirstWriteAddr);
		if (Signal_Check ()==false) {Signal_GenerateSTOP ();return 1;}
		Signal_Write8bit (SecondWriteAddr);
		if (Signal_Check ()==false) {Signal_GenerateSTOP ();return 1;}
		for (uint8_t time=0;time<Times;time++)
		{
			Signal_Write8bit (*data);
			if (Signal_Check ()==false) 
			{
				Signal_GenerateSTOP ();return 2;
			}
			data++;
		}
		Signal_GenerateSTOP ();
		return 3;
}
uint8_t I2CPORT::Device_ReadN8bit(u8 Times , u8 slaveAddr , u8 FirstReadAddr, u8 SecondReadAddr,u8 * data)
{
	if(Device_WriteN8bit(0,slaveAddr,FirstReadAddr,SecondReadAddr,data)!=3) return 4;
	Signal_GenerateSTART();
	Signal_Send7bitAddress (slaveAddr,1);//r
	for(uint8_t time=0;time<Times;time++)
	{
		Signal_Ack();
		*data=Signal_Read8bit();
		data+=1;
	}
	Signal_Nack();
	Signal_GenerateSTOP ();
	return 3;
}
