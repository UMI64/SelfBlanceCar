#include "sine.h"
short int sinDataI16[SINARRAYSIZE];

void InitSinArray(void)
{
    for (int i = 0; i < SINARRAYSIZE; i++)
    {
        float x = i * 6.2831852 / SINARRAYSIZE;
        sinDataI16[i] = (short int)round(sinf(x) * SINARRAYSCALE);
    }
}

float fastSin(float x)
{
    if (x >= 0)
    {
        int ix = ((int)(x / 360 * (float)SINARRAYSIZE)) % SINARRAYSIZE;
        return sinDataI16[ix] / (float)SINARRAYSCALE;
    }
    else
    {
        int ix = ((int)(-x / 360 * (float)SINARRAYSIZE)) % SINARRAYSIZE;
        return -sinDataI16[ix] / (float)SINARRAYSCALE;
    }
}
