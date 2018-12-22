#include "PWM.h"
void PWMPORT::Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	F=72000000/(PSC*ARR);
	CYCLE_ms=(1.0f/F)*1000;
	/*GPIO配置*/
	if(GPIOx==GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if(GPIOx==GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if(GPIOx==GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if(GPIOx==GPIOD) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if(GPIOx==GPIOE) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if(GPIOx==GPIOF) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if(GPIOx==GPIOG) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin=PWM0_Pin | PWM1_Pin | PWM2_Pin | PWM3_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;//推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置时钟工作频率
	GPIO_Init(GPIOx, &GPIO_InitStructure);//向GPIO的寄存器赋值 
	/*重映射配置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	if (TIMx==TIM3)//TIM3重映射
	{
		if (GPIOx==GPIOB)
		{
			GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
		}
		else if (GPIOx==GPIOC)
			GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	}
	/*TIM配置*/
	if(TIMx==TIM1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	else if (TIMx==TIM2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	else if (TIMx==TIM3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitStructure.TIM_Period = ARR; //自动重装载寄存器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler =PSC; //TIMX预分频的值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; //时钟分割
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure); //根据以上功能对定时器进行初始化
	/*TIM1_PWM配置*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//选择定时器模式，TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//输出比较极性低
	TIM_OC1Init(TIMx, &TIM_OCInitStructure);//根据结构体信息进行初始化
	TIM_SetCompare1(TIMx,0);//
	TIM_OC2Init(TIMx, &TIM_OCInitStructure);//根据结构体信息进行初始化
	TIM_SetCompare2(TIMx,0);//
	TIM_OC3Init(TIMx, &TIM_OCInitStructure);//根据结构体信息进行初始化
	TIM_SetCompare3(TIMx,0);//
	TIM_OC4Init(TIMx, &TIM_OCInitStructure);//根据结构体信息进行初始化
	TIM_SetCompare4(TIMx,0);//
	/*配置结束*/
	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);  //使能TIMx在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);  //使能TIMx在CCR3上的预装载寄存器
	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);  //使能TIMx在CCR4上的预装载寄存器
	TIM_ARRPreloadConfig(TIMx, ENABLE);                //使能TIMx在ARR上的预装载寄存器
	ON();
}
void PWMPORT::OFF()
{
	TIM_Cmd(TIMx, DISABLE);//关闭定时器
	TIM_CtrlPWMOutputs(TIMx, DISABLE);
}
void PWMPORT::ON()
{
	TIM_Cmd(TIMx, ENABLE);//使能定时器
	TIM_CtrlPWMOutputs(TIMx, ENABLE);
}
void PWMPORT::SetPort_0(float TIME)
{
	TIM_SetCompare1(TIMx,ARR*(TIME/100.0f));
}
void PWMPORT::SetPort_1(float TIME)
{
	TIM_SetCompare2(TIMx,ARR*(TIME/100.0f));
}
void PWMPORT::SetPort_2(float TIME)
{
	TIM_SetCompare3(TIMx,ARR*(TIME/100.0f));
}
void PWMPORT::SetPort_3(float TIME)
{
	TIM_SetCompare4(TIMx,ARR*(TIME/100.0f));
}