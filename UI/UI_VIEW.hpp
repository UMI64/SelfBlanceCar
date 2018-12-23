#ifndef __UIVIEW__H__
#define __UIVIEW__H__
#include "UI.hpp"
#include "UI_LIST.hpp"
#include "UI_GRAPH.hpp"
class View;
enum DRECType
{
	Request,
	Static,
	Painted,
};
class DREC : public REC
{
public:
	DREC(){}
	DREC(View &V);
	DREC(const REC &rec);
	DRECType GetDRECType()
	{
		if (FLAG_Draw && !V)
		{
			return Request;
		}
		else if (V)
		{
			return Painted;
		}
		return Static;
	}
	bool FLAG_LastDraw = false;
	bool FLAG_Draw = false;
	View * V = nullptr;
};
class View
{
	public:
		DisplayDEV * MonitorDEV;
		View * FatherView;
		List<View*> ChildViewList;
		bool FLAG_Repaint=false;
		int Height;
		int Width;
		int PositionX;
		int PositionY;
		View(){};
		virtual ~View()=default;
		void GetDREC(List<DREC> &DRECList);
		void AddChildView(View &V);
		void Requestpaint(DREC &rec);
		virtual void OnToch(float x,float y)=0;
		virtual void OnDraw()=0;
		virtual void Draw(REC &rec)=0;
	private:
		List<DREC> ViewDRECList;
		static List<DREC> EliminationOverlap(List<DREC> &Drec);
};
class TextView: public View
{

};
#endif