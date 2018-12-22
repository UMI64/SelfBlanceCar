#ifndef _UIGRAPH_H
#define _UIGRAPH_H
#include "math.h"
#include "UI_LIST.hpp"
class REC
{
    public:
        int Height;//矩形的高度
		int Width;//矩形的宽度
		int PositionX;//矩形在父布局中的位置(左下角为标记点)
		int PositionY;//矩形在父布局中的位置(左下角为标记点)
		virtual ~REC() = default;
        static bool IsContain(REC * r_1,REC * r_2)
        {
			int x, y;
			for (char r2count = 0; r2count < 4; r2count++)
			{
				switch (r2count)
				{
					case 0:x = r_2->PositionX; y = r_2->PositionY; break;
					case 1:x += r_2->Width; break;
					case 2: y += r_2->Height; break;
					case 3:x -= r_2->Width;
				}
				if (x < r_1->PositionX || x>r_1->PositionX+ r_1->Width) return false;
				if(y < r_1->PositionY || y>r_1->PositionY + r_1->Height) return false;
			}
			return true;
        }
		static bool IsOverlap(REC * r_1, REC * r_2)
		{
			bool xOverlap = fabs(r_1->PositionX + r_1->Width / 2 - r_2->PositionX - r_2->Width / 2) < (r_1->Width / 2 + r_2->Width / 2);
			bool yOverlap = fabs(r_1->PositionY + r_1->Height / 2 - r_2->PositionY - r_2->Height / 2) < (r_1->Height / 2 + r_2->Height / 2);
			if (xOverlap &&yOverlap) return true;
			else return false;
		}
        static List<REC> * Subtract(REC * r_1,REC * r_2)
        {//矩形相减
			List<REC> * RecList = new List<REC>();
			//裁剪左边
			if (r_2->PositionX > r_1->PositionX)
			{
				if ((r_2->PositionX - r_1->PositionX) < r_1->Width)
				{//裁掉左边并加入链表
					REC * Rec = new REC(*r_1);
					Rec->Width = r_2->PositionX - r_1->PositionX;
					RecList->Add(Rec);
					r_1->Width -= Rec->Width;
					r_1->PositionX += Rec->Width;
				}
				else//被减矩形完全在左边
				{
					REC * Rec = new REC(*r_1);
					RecList->Add(Rec);
					return RecList;
				}
			}
			//裁剪下边
			if (r_2->PositionY > r_1->PositionY)
			{
				if ((r_2->PositionY - r_1->PositionY) < r_1->Height)
				{//裁掉下边并加入链表
					REC * Rec = new REC(*r_1);
					Rec->Height = r_2->PositionY - r_1->PositionY;
					RecList->Add(Rec);
					r_1->Height -= Rec->Height;
					r_1->PositionY += Rec->Height;
				}
				else//被减矩形完全在下边
				{
					REC * Rec = new REC(*r_1);
					RecList->Add(Rec);
					return RecList;
				}
			}
			//裁剪右边
			if ((r_1->PositionX - r_2->PositionX)> (r_2->Width-r_1->Width))
			{
				if ((r_1->PositionX - r_2->PositionX)< r_2->Width)
				{
					REC * Rec = new REC(*r_1);
					Rec->Width = r_1->PositionX + r_1->Width - r_2->PositionX - r_2->Width;
					Rec->PositionX = r_2->PositionX + r_2->Width;
					RecList->Add(Rec);
					r_1->Width -= Rec->Width;
				}
				else//被减矩形完全在左边
				{
					REC * Rec = new REC(*r_1);
					RecList->Add(Rec);
					return RecList;
				}
			}			
			//裁剪上边
			if ((r_1->PositionY - r_2->PositionY)> (r_2->Height - r_1->Height))
			{
				if ((r_2->PositionY - r_1->PositionY)< r_1->Height)
				{
					REC * Rec = new REC(*r_1);
					Rec->Height = r_1->PositionY + r_1->Height - r_2->PositionY - r_2->Height;
					Rec->PositionY = r_2->PositionY+ r_2->Height;
					RecList->Add(Rec);
					r_1->Height -= Rec->Height;
				}
				else//被减矩形完全在上边
				{
					REC * Rec = new REC(*r_1);
					RecList->Add(Rec);
					return RecList;
				}
			}
			return RecList;
        }
};
#endif