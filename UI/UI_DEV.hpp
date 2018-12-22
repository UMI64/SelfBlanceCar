#ifndef __UIDEV__H__
#define __UIDEV__H__
//#include "stm32f10x.h"
class DisplayDEV
{
public:
 	unsigned char Width;
	unsigned char Height;
	virtual void Point(int x,int y,unsigned int Color)=0;
	virtual void Open()=0;
	virtual void Close()=0;
	virtual ~DisplayDEV() = default;
};
#endif