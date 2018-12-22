#ifndef __PWM_H
#define __PWM_H
#include <stm32f10x.h>
#include <stdbool.h>
#include <math.h>
//PWM周期设定
//F=72.000.000/(psk*arr) HZ
//arr和psk介于0到65535
class PWMPORT
{
	public:
		uint32_t F;
		float CYCLE_ms;
		uint16_t PWM0_Pin;
		uint16_t PWM1_Pin;
		uint16_t PWM2_Pin;
		uint16_t PWM3_Pin;
		uint16_t ARR;
		uint16_t PSC;
		TIM_TypeDef* TIMx;
		GPIO_TypeDef * GPIOx;
		void Init ();
		void ON();
		void OFF();
		void SetPort_0(float TIME);
		void SetPort_1(float TIME);
		void SetPort_2(float TIME);
		void SetPort_3(float TIME);
		~PWMPORT()
		{
			while(true);
		}
};
#endif
