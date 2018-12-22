#ifndef __DISPLAY__H__
#define __DISPLAY__H__
#include "stm32f10x.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "096oled.h"
#include "ahrs.h"
#include "UI.hpp"
class Display
{
	public:
	Display();
	void Task_Main();
	/*显示的数据*/
	float *Roll;
	float *VB;
	float *Speed;
	uint16_t *Attitude_1sCount;
	private:
	OLED096* Display_DEV;
	UICORE * Ui;
};
#endif