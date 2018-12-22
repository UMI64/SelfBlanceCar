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
	DREC(REC &rec);
	DRECType GetDRECType()
	{
		if (FLAG_Draw && !V)//请求绘图却没有指定View
		{
			return Request;
		}
		else if (V)//指定了视图
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
		View * FatherView;//父布局指针
		List<View> * ChildViewList=new List<View>();//子布局指针链表
		bool FLAG_Repaint=false;//重绘标记
		int Height;//view的高度
		int Width;//view的宽度
		int PositionX;//view在父布局中的位置(左下角为标记点)
		int PositionY;//view在父布局中的位置(左下角为标记点)
		View(){}
		virtual ~View()
		{
			delete ChildViewList;
		}
		void GetDREC(List<DREC> * DRECList);//获取此View的绘图链表
		void AddChildView(View * V);//加入一个子View
		void Requestpaint(DREC* rec);//请求绘制某个区域
		virtual void OnToch(float x,float y)=0;//点击事件
		virtual void OnDraw()=0;//绘制事件
		virtual void Draw(REC * rec)=0;//绘制过程
	private:
		List<DREC> * ViewDRECList=nullptr;
		static List<DREC> * EliminationOverlap(List<DREC> * Drec);
};
class TextView: public View
{

};
#endif