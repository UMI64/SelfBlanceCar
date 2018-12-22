#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Conrtol.h"
#include "Display.h"
#include "PCM5102.h"
#include "math.h"
void SetSine(short* Buff)
{
	for(uint16_t count=0;count<2000;count++)
	{
		Buff[count]=30000*sinf(6.2831855f*count/2000.0f);
	}
}
extern "C" int main()
{
	static Conrtol* Conrtol_0=new Conrtol();
	static Display* Display_0=new Display();
	Display_0->Roll=&(Conrtol_0->Roll);
	Display_0->VB=&(Conrtol_0->VB);
	Display_0->Speed=&(Conrtol_0->Lspeed);
	Display_0->Attitude_1sCount=&(Conrtol_0->Attitude_1sCount);
	
	short *Buff =new short[2000];
	SetSine(Buff);
	static PCM5102* Pcm5102_dev=new PCM5102();
	Pcm5102_dev->Init(&Buffer_1,&Buffer_2);
	Pcm5102_dev->Buffer_1_Set(Buff,16,2000);
	Pcm5102_dev->Buffer_2_Set(Buff,16,2000);
	Pcm5102_dev->Play();
	vTaskStartScheduler();
}
void * operator new(size_t size)
{
    return pvPortMalloc(size);
}
void operator delete (void * pointer)
{
    vPortFree(pointer);
}
