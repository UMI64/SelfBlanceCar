#include "Display.h"
extern "C" void TASK_UI_Draw(void* parm) {

	while(true)
	{
		vTaskDelay(50);
		(static_cast<UICORE*>(parm))->Task_Draw();
	}
}
class OLED:public DisplayDEV
{
	public:
		OLED()//构造函数初始化OLED
		{
			Width=128;
			Height=64;
			Buffer = new uint8_t *[Width];
			for (int i = 0; i < Width; i++)
			{
				Buffer[i] = new uint8_t [Height/8];
			}
			for(int y=0;y<Height/8;y++)
				for(int x=0;x<Width;x++)
				{
					Buffer[x][y]=0;
				}
			uint16_t SDA=GPIO_Pin_9;
			uint16_t SCL=GPIO_Pin_8;
			I2CPORT *I2C_DEV=new I2CPORT(SCL,SDA,GPIOB);
			Display_DEV=new OLED096(I2C_DEV);
		}
		void Point(int x,int y,unsigned int Color)//画点函数
		{
			if(x>=Width|| x<0 || y>=Height|| y<0) return;
			unsigned char c=1<<(y%8);
			if(Color) Buffer[x][y/8]|=c;
			else
			{
				c=~c;
				Buffer[x][y/8]&=c;
			}
			Display_DEV->OLED_SetPos(x,y/8);
			Display_DEV->OLED_Set8Byte(Buffer[x][y/8]);
		}
		void Fillrectangle(int x, int y, int h, int w,unsigned int Color)//画填充矩形
		{
			unsigned char c=0xff;
			char yS=y/8;
			char yE=(y+h)/8;
			for(int nowY=yS;nowY<=yE;nowY++)
			{
				if(nowY>=0 && nowY<Height/8)
				{
					c=0xff;
					if(nowY==yS) 
						c=0xff<<(y%8);
					if(nowY==yE)
						c&=c>>(8-((y+h)%8));
					for(int nowX=x;nowX<w+x;nowX++)
					{
						if(nowX<Width && nowX>=0)
						{
							if(Color)							
								Buffer[nowX][nowY]|=c;
							else
							{
								c=~c;
								Buffer[nowX][nowY]&=c;
							}
						}
					}
				}
			}
			for(int nowY=yS;nowY<=yE;nowY++)
			{
				if(nowY>=0 && nowY<Height/8)
				{
					if(x<0) x=0;
					Display_DEV->OLED_SetPos(x,nowY);
					for(int nowX=x;nowX<w+x;nowX++)
					{
						if(nowX<Width && nowX>=0)
							Display_DEV->OLED_Set8Byte(Buffer[nowX][nowY]);
					}
				}
			}
		}
		void Open()//打开设备
		{
			Display_DEV->OLED_ON();
			Display_DEV->OLED_Fill((unsigned char )0x00);
		}
		void Close()//关闭设备
		{
			Display_DEV->OLED_OFF();
		}
		~OLED()//析构函数关闭设备
		{
			this->Close();
			delete Display_DEV;
			delete I2C_DEV;
			for (int i = 0; i < Width; i++)
			{
				delete[] Buffer[i];
			}
			delete[]Buffer;
		}
	private:
		uint16_t SDA;
		uint16_t SCL;
		I2CPORT *I2C_DEV;//IIC口
		OLED096* Display_DEV;//OLED设备
		unsigned char ** Buffer;//设备图像缓存
};
class RecView : public View
{
	public :
		unsigned int Color = 0x1111;
		RecView(unsigned int Color)
		{
			this->Color = Color;
			this->FLAG_Repaint = true;
		}
		void OnToch(float x, float y)
		{

		}
		void OnDraw()
		{
			
			this->FLAG_Repaint = true;
			if(Color== 0x1111)
				this->PositionX+=0.2;
			else
				this->PositionY+=0.1;
			
		}
		void Draw(REC * rec)
		{
			//MonitorDEV->Point(1,1,0xff);
			((class OLED *)MonitorDEV)->Fillrectangle(rec->PositionX,rec->PositionY,rec->Height,rec->Width,Color);
			//((class OLED *)MonitorDEV)->Fillrectangle(rec->PositionX, rec->PositionY+ rec->Height, rec->PositionX+ rec->Width, rec->PositionY);
			//setfillcolor(Color);
			//fillrectangle(rec->PositionX, rec->PositionY+ rec->Height, rec->PositionX+ rec->Width, rec->PositionY);
		}
};
/*A 8 9*/
Display::Display()
{
	OLED * Monitor_1 = new OLED();
	Ui = new UICORE();
	Ui->Add_Device(Monitor_1);
	RecView * RecFather = new RecView(0x1111);
	RecFather->Height = 9;
	RecFather->Width = 9;
	RecFather->PositionX = 100;
	RecFather->PositionY = 0;
	Ui->Add_View(RecFather);

	RecView * RecGreen = new RecView(0xa5a5);
	RecGreen->Height = 9;
	RecGreen->Width = 9;
	RecGreen->PositionX = 100;
	RecGreen->PositionY = 40;
	RecFather->AddChildView(RecGreen);
	
	xTaskCreate((TaskFunction_t)TASK_UI_Draw,"vLEDTASK", 250, Ui, 1, NULL);
}
void Display::Task_Main()
{
	while(true)
	{
		//Ui->Task_Draw();
		/*
		//初始化界面
		Display_DEV->OLED_ShowStr(0,0,"R:");
		Display_DEV->OLED_ShowStr(60,0,"Count:");
		Display_DEV->OLED_ShowStr(60,1,"VB:");
		Display_DEV->OLED_ShowStr(0,1,"S:");
		while(true)
		{
			char ch[5]={0};
			sprintf(ch ,"%2.1f",*Roll);
			Display_DEV->OLED_ShowStr(15,0,ch);
			sprintf(ch ,"%u",*Attitude_1sCount);
			Display_DEV->OLED_ShowStr(95,0,ch);
			sprintf(ch ,"%2.2f",*VB);
			Display_DEV->OLED_ShowStr(80,1,ch);
			sprintf(ch ,"%2.2f",*Speed);
			Display_DEV->OLED_ShowStr(15,1,ch);
			vTaskDelay(50);
		}
		vTaskSuspend(NULL);
		*/
	}
}