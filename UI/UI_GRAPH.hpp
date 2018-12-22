#ifndef _UIGRAPH_H
#define _UIGRAPH_H
#include "math.h"
#include "UI_LIST.hpp"
class REC
{
    public:
        float Height;//矩形的高度
        float Width;//矩形的宽度
        float PositionX;//矩形在父布局中的位置(左下角为标记点)
        float PositionY;//矩形在父布局中的位置(左下角为标记点)
        static bool IsIntersect(REC * r_1,REC * r_2)
        {
            float ymax=r_1->Height+r_2->Height/2;
            float xmax=r_1->Width+r_2->Width/2;
            float ymin=fabs((r_1->Height-r_2->Height)/2);
            float xmin=fabs((r_1->Width-r_2->Width)/2);
            float centerx= fabs(r_1->PositionX + r_1->Width/2 - r_2->PositionX - r_2->Width/2);
            float centery= fabs(r_1->PositionY + r_1->Height/2 - r_2->PositionY - r_2->Height/2);
            if(centerx<xmax&&centerx>xmin&&centery<ymax&&centery>ymin)
                return true;
            else 
                return false;
        }
        static bool IsContain(REC * r_1,REC * r_2)
        {
            float ymin=(r_1->Height-r_2->Height)/2;
            float xmin=(r_1->Width-r_2->Width)/2;
            float centerx= fabs(r_1->PositionX + r_1->Width / 2 - r_2->PositionX - r_2->Width / 2);
            float centery= fabs(r_1->PositionY + r_1->Height / 2 - r_2->PositionY - r_2->Height / 2);
            if(centerx<xmin&&centery<ymin)
                return true;
            else 
                return false;
        }
        static List<REC> * Subtract(REC * r_1,REC * r_2)
        {//矩形相减
			List<REC> * RecList = new List<REC>();
			//裁剪左边
			if ( (r_2->PositionX - r_1->PositionX) > 0.001f)
			{
				if (((r_2->PositionX - r_1->PositionX) - r_1->Width)<0.001f)
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
			if ((r_2->PositionY - r_1->PositionY) > 0.001f)
			{
				if (((r_2->PositionY - r_1->PositionY) - r_1->Height)<0.001f)
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
			if (((r_1->PositionX - r_2->PositionX)- (r_2->Width-r_1->Width))>0.001f)
			{
				if (((r_1->PositionX - r_2->PositionX)- r_2->Width)<0.001f)
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
			if (((r_1->PositionY - r_2->PositionY)- (r_2->Height - r_1->Height))>0.001f)
			{
				if (((r_2->PositionY - r_1->PositionY)- r_1->Height)<0.001f)
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