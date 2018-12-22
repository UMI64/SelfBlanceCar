#ifndef __ADC_H
#define __ADC_H
#include <stm32f10x.h>
#include <stdbool.h>
//PWM周期设定
//F=72.000.000/(psk*arr) HZ
//arr和psk介于0到65535
class ADCPORT
{
	public:
		ADCPORT ();
		u16 Get_ADval(u8 ch);
		float Get_avegVval(u8 ch,u8 n);
	private:
		ADC_TypeDef *ADCx=ADC1;
		void delay_ms(uint16_t ms);
};
#endif
