#include "UI.hpp"
void UICORE::Add_Device(DisplayDEV &dev)
{
	DevList.Add(&dev);
	dev.Open();
}
void UICORE::Add_View(View &view)
{
	ViewList.Add(&view);
}