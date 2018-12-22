#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"
#include <stdbool.h>
class SPIPORT
{
	public :
		uint16_t MOSI;
		uint16_t SCK;
		uint16_t MISO;
		SPI_InitTypeDef SPI_InitStructure;
		GPIO_TypeDef * GPIOx;
		SPI_TypeDef * SPIx;
		SPIPORT(uint16_t Mosi,uint16_t Sck,uint16_t Miso,GPIO_TypeDef * Gpiox,SPI_TypeDef * Spix,SPI_InitTypeDef Spi_InitStructure)
		{
			MOSI=Mosi;SCK=Sck;MISO=Miso;GPIOx=Gpiox;SPIx=Spix;SPI_InitStructure=Spi_InitStructure;
			/*GPIO*/
			if(GPIOx==GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			else if(GPIOx==GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			else if(GPIOx==GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			else if(GPIOx==GPIOD) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			else if(GPIOx==GPIOE) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			else if(GPIOx==GPIOF) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			else if(GPIOx==GPIOG) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			
			GPIO_InitTypeDef GPIO_SPI;
			GPIO_SPI.GPIO_Pin=MOSI | SCK | MISO;
			GPIO_SPI.GPIO_Mode=GPIO_Mode_AF_PP;
			GPIO_SPI.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(GPIOx,&GPIO_SPI);
			
			GPIO_SetBits(GPIOx,MOSI | SCK | MISO);
			/*SPI*/
			if (SPIx==SPI1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
			else if( SPIx==SPI2) {RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1,ENABLE);}
			else if( SPIx==SPI3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
			
			SPI_Init(SPIx,&SPI_InitStructure);
			SPI_I2S_ITConfig(SPIx,SPI_I2S_IT_TXE,DISABLE);
			SPI_I2S_ITConfig(SPIx,SPI_I2S_IT_RXNE,DISABLE);
			SPI_Cmd(SPIx,ENABLE);
		}
		void Init (uint16_t Mosi,uint16_t Sck,uint16_t Miso,GPIO_TypeDef * Gpiox,SPI_TypeDef * Spix,SPI_InitTypeDef Spi_InitStructure);
		uint8_t ReadWriteByte(uint8_t TxData);
		uint8_t Write_Reg(GPIO_TypeDef * CSN_GPIOx,uint16_t CSN_pin,uint8_t reg,uint8_t value);
		uint8_t Read_Reg(GPIO_TypeDef * CSN_GPIOx,uint16_t CSN_pin,uint8_t reg);
		uint8_t Read_Buf(GPIO_TypeDef * CSN_GPIOx,uint16_t CSN_pin,uint8_t reg,uint8_t *pBuf,uint8_t len);
		uint8_t Write_Buf(GPIO_TypeDef * CSN_GPIOx,uint16_t CSN_pin,uint8_t reg, uint8_t *pBuf, uint8_t len);
};
#endif
