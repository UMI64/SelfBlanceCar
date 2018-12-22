#ifndef __KEYBORD__H__
#define __KEYBORD__H__
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#define No_Click 	0x00
#define On_Click 	0x01
#define Long_Click 	0x02
class Key
{
	public:
		uint8_t Click=0;
	Key(uint16_t Key_Pin,GPIO_TypeDef* Key_GPIOx)
	{
		Key_ID=Key_Pin;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
		GPIO_InitTypeDef GPIO_InitData;
		GPIO_InitData.GPIO_Pin = Key_ID;
		GPIO_InitData.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitData.GPIO_Mode = GPIO_Mode_IPU;//иою╜
		GPIO_Init(Key_GPIOx, &GPIO_InitData);
	}
	uint16_t Updata(uint16_t Keys)
	{
		if(Keys&Key_ID)
		{
			if(count<600) count++;
			if(count>500) {Click=Long_Click;return Long_Click;}
			else if(count==10) {Click=On_Click;return On_Click;}
		}
		else {Click=No_Click;count=0;}
		return No_Click;
	}
	private:
	uint16_t Key_ID=0;
	uint16_t count=0;
};
class KeyBord
{
	public:
	KeyBord();
	void Task_Scan();
	EventGroupHandle_t Event=0;
};
#define KEY_1_On 0x0001;
#define KEY_1_Long 0x0002;
#define KEY_2_On 0x0004;
#define KEY_2_Long 0x0008;
#endif
