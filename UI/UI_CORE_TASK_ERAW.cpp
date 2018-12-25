#include "UI.hpp"
void UICORE::Task_Draw()
{
	View * V = this->ViewList.GetItembyid(0);
	V->GetDREC(DrawRECList);
	for ( int mcount = 0; mcount < DevList.Count; mcount++)
	{
		DisplayDEV * nowdev(DevList.GetItembyid(mcount));
		for ( int count = 0; count < DrawRECList.Count; count++)
		{
			auto DREC (DrawRECList.GetItembyid(count));
			if (!DREC.V) continue;
			DREC.V->MonitorDEV=nowdev;
			DREC.V->Draw(DREC);
		}
	}
	DrawRECList.Clear();
}