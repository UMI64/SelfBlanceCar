#ifndef _UI_H
#define _UI_H
#include "stdbool.h"
#include "UI_LIST.hpp"
#include "UI_LIST.cpp"
#include "UI_DEV.hpp"
#include "UI_GRAPH.hpp"
#include "UI_VIEW.hpp"
class UICORE
{
public:
	List<DisplayDEV*> DevList;
	List<View*> ViewList;
	List<DREC>  DrawRECList;
	~UICORE() = default;
	void Add_Device(DisplayDEV &dev);
	void Add_View(View &view);
	void Task_Draw();
};
#endif