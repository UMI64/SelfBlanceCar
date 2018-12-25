#ifndef _UIGRAPH_H
#define _UIGRAPH_H
#include "math.h"
#include "UI_LIST.hpp"
class REC
{
    public:
        int Height;
		int Width;
		int PositionX;
		int PositionY;
		virtual ~REC() = default;
		static bool IsContain(REC &r_1, REC &r_2)
		{
			if (r_2.PositionX < r_1.PositionX || r_2.PositionX>r_1.PositionX + r_1.Width) return false;
			if (r_2.PositionY < r_1.PositionY || r_2.PositionY>r_1.PositionY + r_1.Height) return false;
			if (r_2.PositionX + r_2.Width < r_1.PositionX || r_2.PositionX + r_2.Width>r_1.PositionX + r_1.Width) return false;
			if (r_2.PositionY + r_2.Height < r_1.PositionY || r_2.PositionY + r_2.Height>r_1.PositionY + r_1.Height) return false;
			return true;
		}
		static bool IsOverlap(REC &r_1, REC &r_2)
		{
			bool xOverlap = fabs(r_1.PositionX + r_1.Width / 2.0f - r_2.PositionX - r_2.Width / 2.0f) < (r_1.Width / 2.0f + r_2.Width / 2.0f);
			bool yOverlap = fabs(r_1.PositionY + r_1.Height / 2.0f - r_2.PositionY - r_2.Height / 2.0f) < (r_1.Height / 2.0f + r_2.Height / 2.0f);
			if (xOverlap &&yOverlap) return true;
			else return false;
		}
        static List<REC>Subtract(REC r_1,REC& r_2)
        {
			List<REC> RecList;
			if (r_2.PositionX > r_1.PositionX)
			{
				if ((r_2.PositionX - r_1.PositionX) < r_1.Width)
				{
					REC Rec =REC(r_1);
					Rec.Width = r_2.PositionX - r_1.PositionX;
					RecList.Add(Rec);
					r_1.Width -= Rec.Width;
					r_1.PositionX += Rec.Width;
				}
				else
				{
					REC Rec = REC(r_1);
					RecList.Add(Rec);
					return RecList;
				}
			}
			if (r_2.PositionY > r_1.PositionY)
			{
				if ((r_2.PositionY - r_1.PositionY) < r_1.Height)
				{
					REC Rec = REC(r_1);
					Rec.Height = r_2.PositionY - r_1.PositionY;
					RecList.Add(Rec);
					r_1.Height -= Rec.Height;
					r_1.PositionY += Rec.Height;
				}
				else
				{
					REC Rec = REC(r_1);
					RecList.Add(Rec);
					return RecList;
				}
			}
			if ((r_1.PositionX - r_2.PositionX)> (r_2.Width-r_1.Width))
			{
				if ((r_1.PositionX - r_2.PositionX)< r_2.Width)
				{
					REC Rec = REC(r_1);
					Rec.Width = r_1.PositionX + r_1.Width - r_2.PositionX - r_2.Width;
					Rec.PositionX = r_2.PositionX + r_2.Width;
					RecList.Add(Rec);
					r_1.Width -= Rec.Width;
				}
				else
				{
					REC Rec = REC(r_1);
					RecList.Add(Rec);
					return RecList;
				}
			}			
			if ((r_1.PositionY - r_2.PositionY)> (r_2.Height - r_1.Height))
			{
				if ((r_2.PositionY - r_1.PositionY)< r_1.Height)
				{
					REC Rec = REC(r_1);
					Rec.Height = r_1.PositionY + r_1.Height - r_2.PositionY - r_2.Height;
					Rec.PositionY = r_2.PositionY+ r_2.Height;
					RecList.Add(Rec);
					r_1.Height -= Rec.Height;
				}
				else
				{
					REC Rec = REC(r_1);
					RecList.Add(Rec);
					return RecList;
				}
			}
			return RecList;
        }
};
#endif