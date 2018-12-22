#include "096oled.h"
const unsigned char F6x8[][6] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// sp
	0x00, 0x00, 0x00, 0x2f, 0x00, 0x00,// !
	0x00, 0x00, 0x07, 0x00, 0x07, 0x00,// "
	0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14,// #
	0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12,// $
	0x00, 0x62, 0x64, 0x08, 0x13, 0x23,// %
	0x00, 0x36, 0x49, 0x55, 0x22, 0x50,// &
	0x00, 0x00, 0x05, 0x03, 0x00, 0x00,// '
	0x00, 0x00, 0x1c, 0x22, 0x41, 0x00,// (
	0x00, 0x00, 0x41, 0x22, 0x1c, 0x00,// )
	0x00, 0x14, 0x08, 0x3E, 0x08, 0x14,// *
	0x00, 0x08, 0x08, 0x3E, 0x08, 0x08,// +
	0x00, 0x00, 0x00, 0xA0, 0x60, 0x00,// ,
	0x00, 0x08, 0x08, 0x08, 0x08, 0x08,// -
	0x00, 0x00, 0x60, 0x60, 0x00, 0x00,// .
	0x00, 0x20, 0x10, 0x08, 0x04, 0x02,// /
	0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
	0x00, 0x00, 0x42, 0x7F, 0x40, 0x00,// 1
	0x00, 0x42, 0x61, 0x51, 0x49, 0x46,// 2
	0x00, 0x21, 0x41, 0x45, 0x4B, 0x31,// 3
	0x00, 0x18, 0x14, 0x12, 0x7F, 0x10,// 4
	0x00, 0x27, 0x45, 0x45, 0x45, 0x39,// 5
	0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
	0x00, 0x01, 0x71, 0x09, 0x05, 0x03,// 7
	0x00, 0x36, 0x49, 0x49, 0x49, 0x36,// 8
	0x00, 0x06, 0x49, 0x49, 0x29, 0x1E,// 9
	0x00, 0x00, 0x36, 0x36, 0x00, 0x00,// :
	0x00, 0x00, 0x56, 0x36, 0x00, 0x00,// ;
	0x00, 0x08, 0x14, 0x22, 0x41, 0x00,// <
	0x00, 0x14, 0x14, 0x14, 0x14, 0x14,// =
	0x00, 0x00, 0x41, 0x22, 0x14, 0x08,// >
	0x00, 0x02, 0x01, 0x51, 0x09, 0x06,// ?
	0x00, 0x32, 0x49, 0x59, 0x51, 0x3E,// @
	0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C,// A
	0x00, 0x7F, 0x49, 0x49, 0x49, 0x36,// B
	0x00, 0x3E, 0x41, 0x41, 0x41, 0x22,// C
	0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C,// D
	0x00, 0x7F, 0x49, 0x49, 0x49, 0x41,// E
	0x00, 0x7F, 0x09, 0x09, 0x09, 0x01,// F
	0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A,// G
	0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F,// H
	0x00, 0x00, 0x41, 0x7F, 0x41, 0x00,// I
	0x00, 0x20, 0x40, 0x41, 0x3F, 0x01,// J
	0x00, 0x7F, 0x08, 0x14, 0x22, 0x41,// K
	0x00, 0x7F, 0x40, 0x40, 0x40, 0x40,// L
	0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F,// M
	0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F,// N
	0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E,// O
	0x00, 0x7F, 0x09, 0x09, 0x09, 0x06,// P
	0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
	0x00, 0x7F, 0x09, 0x19, 0x29, 0x46,// R
	0x00, 0x46, 0x49, 0x49, 0x49, 0x31,// S
	0x00, 0x01, 0x01, 0x7F, 0x01, 0x01,// T
	0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F,// U
	0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F,// V
	0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F,// W
	0x00, 0x63, 0x14, 0x08, 0x14, 0x63,// X
	0x00, 0x07, 0x08, 0x70, 0x08, 0x07,// Y
	0x00, 0x61, 0x51, 0x49, 0x45, 0x43,// Z
	0x00, 0x00, 0x7F, 0x41, 0x41, 0x00,// [
	0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55,// 55
	0x00, 0x00, 0x41, 0x41, 0x7F, 0x00,// ]
	0x00, 0x04, 0x02, 0x01, 0x02, 0x04,// ^
	0x00, 0x40, 0x40, 0x40, 0x40, 0x40,// _
	0x00, 0x00, 0x01, 0x02, 0x04, 0x00,// '
	0x00, 0x20, 0x54, 0x54, 0x54, 0x78,// a
	0x00, 0x7F, 0x48, 0x44, 0x44, 0x38,// b
	0x00, 0x38, 0x44, 0x44, 0x44, 0x20,// c
	0x00, 0x38, 0x44, 0x44, 0x48, 0x7F,// d
	0x00, 0x38, 0x54, 0x54, 0x54, 0x18,// e
	0x00, 0x08, 0x7E, 0x09, 0x01, 0x02,// f
	0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C,// g
	0x00, 0x7F, 0x08, 0x04, 0x04, 0x78,// h
	0x00, 0x00, 0x44, 0x7D, 0x40, 0x00,// i
	0x00, 0x40, 0x80, 0x84, 0x7D, 0x00,// j
	0x00, 0x7F, 0x10, 0x28, 0x44, 0x00,// k
	0x00, 0x00, 0x41, 0x7F, 0x40, 0x00,// l
	0x00, 0x7C, 0x04, 0x18, 0x04, 0x78,// m
	0x00, 0x7C, 0x08, 0x04, 0x04, 0x78,// n
	0x00, 0x38, 0x44, 0x44, 0x44, 0x38,// o
	0x00, 0xFC, 0x24, 0x24, 0x24, 0x18,// p
	0x00, 0x18, 0x24, 0x24, 0x18, 0xFC,// q
	0x00, 0x7C, 0x08, 0x04, 0x04, 0x08,// r
	0x00, 0x48, 0x54, 0x54, 0x54, 0x20,// s
	0x00, 0x04, 0x3F, 0x44, 0x40, 0x20,// t
	0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C,// u
	0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C,// v
	0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C,// w
	0x00, 0x44, 0x28, 0x10, 0x28, 0x44,// x
	0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C,// y
	0x00, 0x44, 0x64, 0x54, 0x4C, 0x44,// z
	0x14, 0x14, 0x14, 0x14, 0x14, 0x14,// horiz lines
};
const unsigned char StartUPcmd[] =
{
	0xAE,//display off
    0x20,//Set Memory Addressing Mode 
    0x10,//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    0xb0,//Set Page Start Address for Page Addressing Mode,0-7
    0xc8,//Set COM Output Scan Direction
    0x00,//---set low column address
    0x10,//---set high column address
    0x40,//--set start line address
    0x81,//--set contrast control register
    0xff,//���ȵ��� 0x00~0xff
    0xa1,//--set segment re-map 0 to 127
    0xa6,//--set normal display
    0xa8,//--set multiplex ratio(1 to 64)
    0x3F,//
    0xa4,//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    0xd3,//-set display offset
    0x00,//-not offset
    0xd5,//--set display clock divide ratio/oscillator frequency
    0xf0,//--set divide ratio
    0xd9,//--set pre-charge period
    0x22,//
    0xda,//--set com pins hardware configuration
    0x12,
    0xdb,//--set vcomh
    0x20,//0x20,0.77xVcc
    0x8d,//--set DC-DC enable
    0x14,//
    0xaf//--turn on oled panel
};
void OLED096::delay_ms(uint16_t ms)
{
	uint16_t i=0;  
   while(ms--)
   {
      i=12000;  //�Լ�����
      while(i--) ;    
   }
}
 /**
  * @brief  OLED_ON����OLED�������л���
  * @param  ��
    * @retval ��
  */
void OLED096::OLED_ON()
{
    I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,0X8D);//���õ�ɱ�
    I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,0X14);//������ɱ�
    I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,0XAF);//OLED����
}
void OLED096::OLED_OFF()
{
	I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,0xAE);//display off
    I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,0xAE);//display off
}
 /**
  * @brief  OLED_SetPos�����ù��
  * @param  x,���xλ��
    *                   y�����yλ��
  * @retval ��
  */
void OLED096::OLED_SetPos(uint8_t x,uint8_t y) //������ʼ������
{
    I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,0xb0+y);
    I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,((x&0xf0)>>4)|0x10);
    //I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,(x&0x0f)|0x01);//����͵͵��һ
	I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,x&0x0f);
}
void OLED096::OLED_Set8Byte(uint8_t Data)
{
	I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x40,Data);
}
void OLED096::OLED_Fill(uint8_t * fill_Data)//ȫ�����
{
	unsigned char m,n;
    for(m=0;m<8;m++)
    {
        I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,0xb0+m);       //page0-page1
        I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,(u8)0x00);     //low column start address
        I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,0x10);     //high column start address
        for(n=0;n<128;n++)
		{
			I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x40,*fill_Data);
			fill_Data++;
		}
    }
}
 /**
  * @brief  OLED_Fill�����������Ļ
  * @param  fill_Data:Ҫ��������
    * @retval ��
  */
void OLED096::OLED_Fill(uint8_t fill_Data)//ȫ�����
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,0xb0+m);       //page0-page1
        I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,(u8)0x00);     //low column start address
        I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,0x10);     //high column start address
        for(n=0;n<128;n++)
		{
			I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x40,fill_Data);
		}
    }
}

void OLED096::OLED_CLS()//����
{
    OLED_Fill((uint8_t)0x00);
}

/**
  * @brief  OLED_ShowStr����ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
  * @param  x,y : ��ʼ������(x:0~127, y:0~7);
    *                   ch[] :- Ҫ��ʾ���ַ���; 
    *                   TextSize : �ַ���С(1:6*8 ; 2:8*16)
    * @retval ��
  */
void OLED096::OLED_ShowStr(unsigned char x, unsigned char y,const char * ch)
{
    unsigned char c = 0,i = 0,j = 0;
	while(ch[j] != '\0')
	{
		c = ch[j] - 32;
		if(x > 126)
		{
			x = 0;
			y++;
		}
		OLED_SetPos(x,y);
		for(i=0;i<6;i++)
			I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x40,F6x8[c][i]);
		x += 6;
		j++;
	}
}
void OLED096::OLED_Init()
{
    delay_ms(100); //�������ʱ����Ҫ
	uint8_t Lenth=sizeof(StartUPcmd)/sizeof(StartUPcmd[0]);
	for (uint8_t time=0;time<Lenth;time++)
	{
		I2CPort.Device_WriteN8bit(1,OLED_ADDR,0x00,StartUPcmd[time]);
	}	
	OLED_CLS();
}