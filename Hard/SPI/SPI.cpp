#include "SPI.h"
void SPIPORT::Init (uint16_t Mosi,uint16_t Sck,uint16_t Miso,GPIO_TypeDef * Gpiox,SPI_TypeDef * Spix,SPI_InitTypeDef Spi_InitStructure)
{
	MOSI=Mosi;SCK=Sck;GPIOx=Gpiox;SPIx=Spix;SPI_InitStructure=Spi_InitStructure;
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
	
	GPIO_SPI.GPIO_Pin=MISO;
	GPIO_SPI.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_SPI.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&GPIO_SPI);
	
	
	GPIO_SetBits(GPIOx,MOSI | SCK | MISO);
	/*SPI*/
	if (SPIx==SPI1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	else if( SPIx==SPI2) {RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB2Periph_AFIO, ENABLE);GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1,ENABLE);}
	else if( SPIx==SPI3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	SPI_Init(SPIx,&SPI_InitStructure);
	SPI_I2S_ITConfig(SPIx,SPI_I2S_IT_TXE,DISABLE);
	SPI_I2S_ITConfig(SPIx,SPI_I2S_IT_RXNE,DISABLE);
	SPI_Cmd(SPIx,ENABLE);
}
uint8_t SPIPORT::ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
	retry++;
	if(retry>200)return 0;
	}
	SPI_I2S_SendData(SPIx, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
	retry++;
	if(retry>200)return 0;
	} 						    
	return SPI_I2S_ReceiveData(SPIx); //����ͨ��SPIx������յ�����					    
}
//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
uint8_t SPIPORT::Write_Reg(GPIO_TypeDef * CSN_GPIOx,uint16_t CSN_pin,uint8_t reg,uint8_t value)
{
	uint8_t status;	
   	GPIO_ResetBits (CSN_GPIOx,CSN_pin);//ʹ��SPI����
  	status =ReadWriteByte(reg);		//���ͼĴ����� 
  	ReadWriteByte(value);      		//д��Ĵ�����ֵ
  	GPIO_SetBits (CSN_GPIOx,CSN_pin);	//��ֹSPI����	   
  	return status;       							//����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
uint8_t SPIPORT::Read_Reg(GPIO_TypeDef * CSN_GPIOx,uint16_t CSN_pin,uint8_t reg)
{
	uint8_t reg_val;	    
 	GPIO_ResetBits (CSN_GPIOx,CSN_pin);//ʹ��SPI����		
  	ReadWriteByte(reg);					//���ͼĴ�����
  	reg_val=ReadWriteByte(0XFF);	//��ȡ�Ĵ�������
  	GPIO_SetBits (CSN_GPIOx,CSN_pin);//��ֹSPI����		    
  	return reg_val;           				//����״ֵ̬
}
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
uint8_t SPIPORT::Read_Buf(GPIO_TypeDef * CSN_GPIOx,uint16_t CSN_pin,uint8_t reg,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,u8_ctr;	       
	GPIO_ResetBits (CSN_GPIOx,CSN_pin);//ʹ��SPI����
	status=ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬  	   
	for(u8_ctr=0;u8_ctr<len;u8_ctr++)  pBuf[u8_ctr]=ReadWriteByte(0XFF);//��������
	GPIO_SetBits (CSN_GPIOx,CSN_pin);//�ر�SPI����
	return status;        //���ض�����״ֵ̬
}
uint8_t SPIPORT::Write_Buf(GPIO_TypeDef * CSN_GPIOx,uint16_t CSN_pin,uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status,u8_ctr;
	GPIO_ResetBits (CSN_GPIOx,CSN_pin);//ʹ��SPI����
	status = ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	for(u8_ctr=0; u8_ctr<len; u8_ctr++)ReadWriteByte(*pBuf++); //д������	 
	GPIO_SetBits (CSN_GPIOx,CSN_pin);//�ر�SPI����
	return status;          //���ض�����״ֵ̬
}
