#ifndef __SINE__H__
#define __SINE__H__

#include "stm32f10x.h"
#include "math.h"
#define SINARRAYSIZE 1024
#define SINARRAYSCALE 32767

extern short int sinDataI16[];
void InitSinArray(void);
float fastSin(float x);

#endif
