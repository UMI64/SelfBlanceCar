#ifndef _UI_H
#define _UI_H
//#include "stm32f10x.h"
#include "stdbool.h"
#include "UI_DEV.hpp"
#include "UI_LIST.hpp"
#include "UI_GRAPH.hpp"
#include "UI_VIEW.hpp"
class UICORE
{
public:
	List<DisplayDEV> * DevList= new List<DisplayDEV>;//显示设备链表
	List<View> * ViewList= new List<View>;//视图链表
	List<DREC>  * DrawRECList= new List<DREC>;//绘制链表
	UICORE ()
	{
	}
	~UICORE()
	{
		delete DevList;
		delete ViewList;
		delete DrawRECList;
	}
	void Add_Device(DisplayDEV * dev);
	void Add_View(View * view);
	void Task_Draw();
};
#endif