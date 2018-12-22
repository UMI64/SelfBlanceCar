#include"ADC.hpp"
ADCPORT::ADCPORT()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	GPIO_TypeDef *GPIOx=GPIOB;
	uint16_t Pin=GPIO_Pin_0;
	// Enable ADC and GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE);
	//ADC_TempSensorVrefintCmd(ENABLE);//使能温度传感器
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12MHZ 
	
	// Configure (ADC Channel) as analog input -------------------------*/
	GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC 工作模式:独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //AD 单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //AD 单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC 数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1; //顺序进行规则转换的 ADC 通道的数目 1
	// ADC1 configuration ------------------------------------------------------*/
	ADC_Init(ADCx, &ADC_InitStructure);
	// Enable ADC
	ADC_Cmd(ADCx, ENABLE);
	// Enable ADC reset calibration register */   
	ADC_ResetCalibration(ADCx);
	// Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADCx));
	// Start ADC calibration */
	ADC_StartCalibration(ADCx);
	// Check the end of ADC calibration */
	while(ADC_GetCalibrationStatus(ADCx));
}
u16 ADCPORT::Get_ADval(u8 ch)
{
	u8 times=0;
	u16 DataValue; 
	/* ADC1 regular channel14 configuration */ 
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_239Cycles5);
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
	/* Test if the ADC1 EOC flag is set or not */ 
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
	{
		if(times++>=50)
			break;
	}
	/*Returns the ADC1 Master data value of the last converted channel*/
	DataValue = ADC_GetConversionValue(ADCx);
	return DataValue; 
}
float ADCPORT::Get_avegVval(u8 ch,u8 n) 
{ 
	uint32_t ad_sum = 0; 
	for(u8 i=0;i<n;i++) 
	{ 
		ad_sum += Get_ADval(ch);
		delay_ms(2); 
	}
	float fad_=(float)ad_sum / n;
	float V = fad_*(3.3f/4096);
	return V;
}
void ADCPORT::delay_ms(uint16_t ms)
{
	uint16_t i=0;  
	while(ms--)
	{
	  i=12000;  //自己定义
	  while(i--) ;    
	}
}
