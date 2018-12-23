#ifndef __UIDEV__H__
#define __UIDEV__H__
//#include "stm32f10x.h"
class DisplayDEV
{
public:
 	unsigned char Width;
	unsigned char Height;
	virtual void Point(int x, int y, unsigned int Color) {};
	virtual void Open() {};
	virtual void Close() {};
	virtual ~DisplayDEV() = default;
};
#endif