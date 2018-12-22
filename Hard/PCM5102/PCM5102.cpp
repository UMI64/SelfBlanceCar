#include "PCM5102.h"
PCM5102::PCM5102()
{
	//Configure the GPIO Pins for I2S
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable GPIOB, GPIOC and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);
	/* I2S2 SD, CK and WS pins configuration */
	/*I2S2_SD-PB15;I2S2_ CK-PB13;I2S2_WS-PB12*/
	GPIO_InitStructure.GPIO_Pin = DIN | BCK | SCK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// Initialise and Configure the Mode for I2S
	I2S_InitTypeDef I2S_InitStructure;
	/* Enable I2S peripheral clocks*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	/*Reset SPI2*/
	SPI_I2S_DeInit(SPI2);
	I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
	I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;     
	I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16bextended;
	I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_48k;
	/*I2S clock steady state is low level */
	I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
	I2S_Init(SPI2, &I2S_InitStructure);
	SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_TXE, ENABLE);
	//Initialise the Nested Vectored Interrupt Controller
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	GPIO_ResetBits(GPIOB,XMT|DMP|FLT);
	GPIO_InitStructure.GPIO_Pin = XMT | DMP | FLT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void PCM5102::Init(BufferStruct * Buffer1,BufferStruct * Buffer2)
{
	Buffer_1=Buffer1;
	Buffer_1->Reading=false;
	Buffer_2=Buffer2;
	Buffer_2->Reading=false;
}
void PCM5102::Buffer_1_Set(void* setpBuffer,uint16_t Size,uint16_t Long)
{
	Buffer_1->pBuffer=setpBuffer;
	Buffer_1->Buffer_Size=Size;
	Buffer_1->Buffer_Long=Long;
}
void PCM5102::Buffer_2_Set(void* setpBuffer,uint16_t Size,uint16_t Long)
{
	Buffer_2->pBuffer=setpBuffer;
	Buffer_2->Buffer_Size=Size;
	Buffer_2->Buffer_Long=Long;
}
void PCM5102::Play()
{
	GPIO_SetBits(GPIOB,XMT);
	I2S_Cmd(SPI2, ENABLE);
	SPI_I2S_SendData(SPI2,0x0000);
}
void PCM5102::Stop()
{
	GPIO_ResetBits(GPIOB,XMT);
	I2S_Cmd(SPI2, DISABLE);
}
void PCM5102::End()
{
	GPIO_ResetBits(GPIOB,XMT);
	I2S_Cmd(SPI2, DISABLE);
	Buffer_1->Address_Count=0;
	Buffer_2->Address_Count=0;
}
BufferStruct Buffer_1;
BufferStruct Buffer_2;

extern "C" void SPI2_IRQHandler(void)
{
	if(Buffer_1.Reading)
	{
		SPI_I2S_SendData(SPI2,((uint16_t*)Buffer_1.pBuffer)[Buffer_1.Address_Count]);
		Buffer_1.Address_Count+=8;
		if(Buffer_1.Address_Count>=Buffer_1.Buffer_Long)
		{
			Buffer_1.Reading=false;
			Buffer_2.Reading=true;
			Buffer_1.Address_Count=0;
		}
	}
	else
	{
		SPI_I2S_SendData(SPI2,((uint16_t*)Buffer_2.pBuffer)[Buffer_2.Address_Count]);
		Buffer_2.Address_Count+=8;
		if(Buffer_2.Address_Count>=Buffer_2.Buffer_Long)
		{
			Buffer_1.Reading=true;
			Buffer_2.Reading=false;
			Buffer_2.Address_Count=0;
		}
	}
}