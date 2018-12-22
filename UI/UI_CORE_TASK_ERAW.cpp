#include "UI.hpp"
void UICORE::Task_Draw()
{
	//计算绘图区域
	View * V = this->ViewList->Getbyid(0);//获取第零个页面
	V->GetDREC(DrawRECList);//获得绘图链表
	for ( int mcount = 0; mcount < DevList->Count; mcount++)
	{
		DisplayDEV * nowdev=DevList->Getbyid(mcount);
		//绘制
		for ( int count = 0; count < DrawRECList->Count; count++)
		{
			auto * DREC = DrawRECList->Getbyid(count);
			if (!DREC->V) continue;
			DREC->V->MonitorDEV=nowdev;
			DREC->V->Draw(DREC);
		}
	}
	if (DrawRECList->haveDonotDeleteitem()) while (1);
	DrawRECList->Clear();
}