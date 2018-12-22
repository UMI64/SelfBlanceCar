#include "KeyBord.h"
extern "C" void TASK_Scan(void* parm) {

    (static_cast<KeyBord*>(parm))->Task_Scan();
}
KeyBord::KeyBord()
{
	Event=xEventGroupCreate();
	xTaskCreate((TaskFunction_t)TASK_Scan,"vLEDTASK", 200, this, 1, NULL);//…®√Ëº¸≈Ã
}
void KeyBord::Task_Scan()
{
	uint16_t Keys=0;
	uint16_t event=0;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	Key Key_1(GPIO_Pin_7,GPIOA);
	Key Key_2(GPIO_Pin_8,GPIOA);
	while(true)
	{
		Keys=~GPIO_ReadInputData(GPIOA);
		if(Key_1.Updata(Keys)==On_Click)
		{
			event|=KEY_1_On;
		}
		else if(Key_1.Click==Long_Click)
		{			
			event|=KEY_1_Long;
		}
		if(Key_2.Updata(Keys)==On_Click) 	
		{
			event|=KEY_2_On;
		}
		else if(Key_2.Click==Long_Click)
		{			
			event|=KEY_2_Long;
		}
		xEventGroupSetBits(Event,event);
	}
}