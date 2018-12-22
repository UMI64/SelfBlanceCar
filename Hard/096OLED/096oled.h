#ifndef __096OLED_H
#define __096OLED_H
#include <stm32f10x.h>
#include <stdbool.h>
#include "I2C.h"

class OLED096
{
	public :
		I2CPORT I2CPort;
		void OLED_ON();
		void OLED_OFF();
		void OLED_SetPos(uint8_t x,uint8_t y);
		void OLED_Fill(uint8_t fill_Data);
		void OLED_Fill(uint8_t * fill_Data);
		void OLED_Set8Byte(uint8_t Data);
		void OLED_CLS();
		void OLED_ShowStr(unsigned char x, unsigned char y,const char ch[]);
		void OLED_Init();
		OLED096(I2CPORT* Port):I2CPort(*Port)
		{
			OLED_Init();
		}
	private :
		uint8_t OLED_ADDR=0x78;
		void delay_ms(uint16_t ms);
		 
};
#endif
