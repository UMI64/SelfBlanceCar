#include "UI.hpp"
DREC::DREC(View &V)
{
	this->Height = V.Height;
	this->Width = V.Width;
	this->PositionX = V.PositionX;
	this->PositionY = V.PositionY;
	this->FLAG_Draw = V.FLAG_Repaint;
	if (V.FLAG_Repaint) this->V = &V;
	V.FLAG_Repaint = false;
}
DREC::DREC(const REC &rec)
{
	this->Height = rec.Height;
	this->Width = rec.Width;
	this->PositionX = rec.PositionX;
	this->PositionY = rec.PositionY;
}
void View::GetDREC(List<DREC> &DRECList)
{
	for (int viewcount = 0; viewcount < ChildViewList.Count; ++viewcount)
	{
		View * V( ChildViewList.GetItembyid(viewcount));
		V->GetDREC(DRECList);
	}
	this->OnDraw();
	DREC drec(*this);
	ViewDRECList.Add(drec);
	auto ButtonDRECList(EliminationOverlap(ViewDRECList));
	ViewDRECList.Clear();

	for (int count = 0; count < DRECList.Count; ++count)
	{
		DREC TopDREC (DRECList.GetItembyid(count));
		List<DREC> ResultDRECList;
		for (int nocount = 0; nocount < ButtonDRECList.Count; ++nocount)
		{
			DREC ButtonDREC( ButtonDRECList.GetItembyid(nocount));
			if (TopDREC.GetDRECType() == Request)
			{
				if (REC::IsContain(ButtonDREC, TopDREC))
				{
					DRECList.Deletebyid(count);
					count -= 1;
					if (ButtonDREC.GetDRECType() == Static)
					{
						TopDREC.V = this;
						ResultDRECList.Add(TopDREC);

						List<REC> recs( REC::Subtract(ButtonDREC, TopDREC));
						for (int rcount = 0; rcount < recs.Count; ++rcount)
						{
							DREC drec(DREC(recs.GetItembyid(rcount)));
							drec.V = ButtonDREC.V;
							drec.FLAG_Draw = ButtonDREC.FLAG_Draw;
							drec.FLAG_LastDraw = ButtonDREC.FLAG_LastDraw;
							ResultDRECList.Add(drec);
						}
					}
					else
					{
						for (int i = nocount; i < ButtonDRECList.Count; ++i)
						{
							ResultDRECList.Add(DREC(ButtonDRECList.GetItembyid(i)));
						}
						break;
					}
				}
				else if (REC::IsOverlap(ButtonDREC, TopDREC))
				{
					List<REC> ResultTopREC (REC::Subtract(TopDREC, ButtonDREC));
					List<DREC>	ResultTopDREC;
					for (int rcount = 0; rcount < ResultTopREC.Count; ++rcount)
					{
						DREC drec(ResultTopREC.GetItembyid(rcount));
						drec.V = TopDREC.V;
						drec.FLAG_Draw = TopDREC.FLAG_Draw;
						drec.FLAG_LastDraw = TopDREC.FLAG_LastDraw;
						ResultTopDREC.Add(drec);
					}
					if (ButtonDREC.GetDRECType() == Static)
					{
						List<REC> AimdRecs;
						DREC drec (TopDREC);
						AimdRecs.Add(drec);
						for (int rcount = 0; rcount < ResultTopDREC.Count; ++rcount)
						{
							DREC drec( ResultTopDREC.GetItembyid(rcount));
							List<REC> NewAimdRecs;
							for (int acount = 0; acount < AimdRecs.Count; ++acount)
							{
								List<REC> reclist(REC::Subtract(AimdRecs.GetItembyid(acount), drec));
								NewAimdRecs.Add(reclist);
							}
							AimdRecs.Replace( NewAimdRecs);
						}
						for (int acount = 0; acount < AimdRecs.Count; ++acount)
						{
							DREC AimdRec(AimdRecs.GetItembyid(acount));
							AimdRec.V = this;
							AimdRec.FLAG_Draw = ButtonDREC.FLAG_Draw;
							AimdRec.FLAG_LastDraw = ButtonDREC.FLAG_LastDraw;
							ResultDRECList.Add(AimdRec);
						}
						List<REC> ResoutButtonDREC(REC::Subtract(ButtonDREC, TopDREC));
						for (int rcount = 0; rcount < ResoutButtonDREC.Count; ++rcount)
						{
							DREC drec(ResoutButtonDREC.GetItembyid(rcount));
							drec.V = ButtonDREC.V;
							drec.FLAG_Draw = ButtonDREC.FLAG_Draw;
							drec.FLAG_LastDraw = ButtonDREC.FLAG_LastDraw;
							ResultDRECList.Add(drec);
						}
					}
					else
					{
						ResultDRECList.Add(DREC(ButtonDREC));
					}
					TopDREC = ResultTopDREC.GetItembyid(0);
					DRECList.Replacebyid(count, ResultTopDREC);
				}
				else
				{
					ResultDRECList.Add(DREC(ButtonDREC));
				}
			}
			else//ÆÕÍ¨ÃüÖÐ
			{
				if (REC::IsContain(TopDREC, ButtonDREC))
				{
					int a = 0;
				}
				else if (REC::IsOverlap(TopDREC, ButtonDREC))
				{
					List<REC> recs(REC::Subtract(ButtonDREC, TopDREC));
					for (int rcount = 0; rcount < recs.Count; ++rcount)
					{
						DREC drec(recs.GetItembyid(rcount));
						drec.V = ButtonDREC.V;
						drec.FLAG_Draw = ButtonDREC.FLAG_Draw;
						drec.FLAG_LastDraw = ButtonDREC.FLAG_LastDraw;
						ResultDRECList.Add(drec);
					}
				}
				else
				{
					ResultDRECList.Add(DREC(ButtonDREC));
				}
			}
		}
		ButtonDRECList.Replace(ResultDRECList);
	}
	DRECList.Add(ButtonDRECList);
}
void View::Requestpaint(DREC &rec)
{
	ViewDRECList.Add(rec);
}
void View::AddChildView(View &V)
{
    ChildViewList.Add(&V);
	ChildViewList.GetLastItem()->FatherView=this;
}
List<DREC> View::EliminationOverlap(List<DREC> &Drec)
{
	List<DREC> ButtonDRECList;
	if (Drec.Count != 0)
	{
		ButtonDRECList.Add(Drec.GetItembyid(0));
		Drec.Deletebyid(0);
	}
	for (int count = 0; count < Drec.Count; ++count)
	{
		DREC TopDREC(Drec.GetItembyid(count));
		List<DREC> ResultDRECList;
		for (int nocount = 0; nocount < ButtonDRECList.Count; ++nocount)
		{
			DREC ButtonDREC(ButtonDRECList.GetItembyid(count));
			if (REC::IsContain(TopDREC, ButtonDREC))
			{
				ButtonDRECList.Deletebyid(nocount);
				nocount -= 1;
			}
			else
			{
				List<REC> recs(REC::Subtract(ButtonDREC, TopDREC));
				for (int rcount = 0; rcount < recs.Count; ++rcount)
				{
					DREC drec(recs.GetItembyid(rcount));
					drec.V = ButtonDREC.V;
					drec.FLAG_Draw = ButtonDREC.FLAG_Draw;
					drec.FLAG_LastDraw = ButtonDREC.FLAG_LastDraw;
					ResultDRECList.Add(drec);
				}
			}
		}
		ButtonDRECList.Replace(ResultDRECList);
		ButtonDRECList.Add(DREC(TopDREC));
	}
	return ButtonDRECList;
}