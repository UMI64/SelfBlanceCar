#include "PWM.h"
void PWMPORT::Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	F=72000000/(PSC*ARR);
	CYCLE_ms=(1.0f/F)*1000;
	/*GPIO����*/
	if(GPIOx==GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if(GPIOx==GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if(GPIOx==GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if(GPIOx==GPIOD) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if(GPIOx==GPIOE) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if(GPIOx==GPIOF) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if(GPIOx==GPIOG) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin=PWM0_Pin | PWM1_Pin | PWM2_Pin | PWM3_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;//�������   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ʱ�ӹ���Ƶ��
	GPIO_Init(GPIOx, &GPIO_InitStructure);//��GPIO�ļĴ�����ֵ 
	/*��ӳ������*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	if (TIMx==TIM3)//TIM3��ӳ��
	{
		if (GPIOx==GPIOB)
		{
			GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
		}
		else if (GPIOx==GPIOC)
			GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	}
	/*TIM����*/
	if(TIMx==TIM1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	else if (TIMx==TIM2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	else if (TIMx==TIM3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitStructure.TIM_Period = ARR; //�Զ���װ�ؼĴ�����ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler =PSC; //TIMXԤ��Ƶ��ֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; //ʱ�ӷָ�
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure); //�������Ϲ��ܶԶ�ʱ�����г�ʼ��
	/*TIM1_PWM����*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//ѡ��ʱ��ģʽ��TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//����Ƚϼ��Ե�
	TIM_OC1Init(TIMx, &TIM_OCInitStructure);//���ݽṹ����Ϣ���г�ʼ��
	TIM_SetCompare1(TIMx,0);//
	TIM_OC2Init(TIMx, &TIM_OCInitStructure);//���ݽṹ����Ϣ���г�ʼ��
	TIM_SetCompare2(TIMx,0);//
	TIM_OC3Init(TIMx, &TIM_OCInitStructure);//���ݽṹ����Ϣ���г�ʼ��
	TIM_SetCompare3(TIMx,0);//
	TIM_OC4Init(TIMx, &TIM_OCInitStructure);//���ݽṹ����Ϣ���г�ʼ��
	TIM_SetCompare4(TIMx,0);//
	/*���ý���*/
	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR4�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIMx, ENABLE);                //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	ON();
}
void PWMPORT::OFF()
{
	TIM_Cmd(TIMx, DISABLE);//�رն�ʱ��
	TIM_CtrlPWMOutputs(TIMx, DISABLE);
}
void PWMPORT::ON()
{
	TIM_Cmd(TIMx, ENABLE);//ʹ�ܶ�ʱ��
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