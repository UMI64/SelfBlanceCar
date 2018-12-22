#ifndef __I2C_H
#define __I2C_H
#include "stm32f10x.h"
#include <stdbool.h>

class I2CPORT
{
	public:
		uint16_t scl;
		uint16_t sda;
		GPIO_TypeDef * GPIOx;
		uint16_t waittime;
		void Init();
		void Signal_GenerateSTART();
		void Signal_GenerateSTOP();
		void Signal_Ack ();
		void Signal_Nack();
		bool Signal_Check ();
		void Signal_Send7bitAddress(uint8_t slaveAddr,uint8_t rw);
		void Signal_Write8bit(uint8_t data);
		uint8_t Signal_Read8bit();
		uint8_t Device_WriteN8bit(u8 Times , u8 slaveAddr , u8 data);
		uint8_t Device_WriteN8bit(u8 Times , u8 slaveAddr , u8 writeAddr , u8 * data);
		uint8_t Device_WriteN8bit(u8 Times , u8 slaveAddr , u8 writeAddr , u8 data);
		uint8_t Device_WriteN8bit(u8 Times , u8 slaveAddr , u8 FirstWriteAddr, u8 SecondWriteAddr,u8 * data);
		uint8_t Device_ReadN8bit(u8 Times , u8 slaveAddr , u8 FirstReadAddr, u8 SecondReadAddr,u8 * data);
		I2CPORT(uint16_t SCL,uint16_t SDA,GPIO_TypeDef * Gpiox)
		{
			scl=SCL;
			sda=SDA;
			GPIOx=Gpiox;
			waittime=1;
			Init();
		};
	private:
		void SDA_ToInput();
		void SDA_ToOutput();
		void Wait();
};
#endif

