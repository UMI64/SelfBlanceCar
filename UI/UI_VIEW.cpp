#include "UI.hpp"
DREC::DREC(View &V)
{
	this->Height = V.Height;
	this->Width = V.Width;
	this->PositionX = V.PositionX;
	this->PositionY = V.PositionY;
	this->FLAG_Draw = V.FLAG_Repaint; //如果请求重绘自己则
	if (V.FLAG_Repaint) this->V = &V;//由自己绘制自己
	V.FLAG_Repaint = false;//消除重绘请求
}
DREC::DREC(REC &rec)
{
	this->Height = rec.Height;
	this->Width = rec.Width;
	this->PositionX = rec.PositionX;
	this->PositionY = rec.PositionY;
}
void View::GetDREC(List<DREC> * DRECList)
{
    for(unsigned int viewcount=0;viewcount<ChildViewList->Count;viewcount++)//最高层跳过此步骤
    {
        View * V=ChildViewList->Getbyid(viewcount);//取出一个view
        V->GetDREC(DRECList);//获得已绘矩形
    }
	this->OnDraw();
    //组装自己的请求区域和已绘区域
    //把自己的请求区域合并到一起
	if (!ViewDRECList) ViewDRECList = new List<DREC>();
    DREC * ViewDREC= new DREC(*this);//根据自己View建立一个矩形
    ViewDRECList->Add(ViewDREC);//加入此View的矩形
    List<DREC> * ButtonDRECList=EliminationOverlap(ViewDRECList);//消去重叠作为本次裁剪的下层矩形(上层矩形击碎下层矩形后把碎片与上层合并)
	delete ViewDRECList;//重置此View的矩形链表
	ViewDRECList = nullptr;//重置此View的矩形链表
	/*开始裁剪*/
    for(int count=0;count<DRECList->Count;count++)//从上层矩形拿出一个矩形去打击下层矩形,收集到的碎片作为新的下层矩形
    {
        DREC * TopDREC=DRECList->Getbyid(count);//从上层矩形链表取出一个矩形
        List<DREC> * ResultDRECList=new List<DREC>();//下层矩形的碎片收集链表
        for(int nocount=0;nocount<ButtonDRECList->Count;nocount++)//拿出下层矩形出来挨打
        {
            DREC * ButtonDREC=ButtonDRECList->Getbyid(nocount);//取出一个下层矩形
			//判断是什么矩形混合
			/*	请求绘制矩形 已绘矩形 静态矩形
				上层请求绘制矩形	上层请求绘制矩形	上层请求绘制矩形
				下层请求绘制矩形	下层重绘矩形		下层已绘矩形
				上减下				下减上				
			*/
			if (TopDREC->GetDRECType()== Request)//请求绘制矩形命中下方矩形
			{
				if (REC::IsContain(ButtonDREC, TopDREC))//下方矩形包含上方矩形
				{
					//删除上方矩形(完全击碎)
					DRECList->Deletebyid(count,false);
					count -= 1;
					if (ButtonDREC->GetDRECType()==Static)//下方矩形是静态矩形
					{
						//生成重绘矩形 由所在的View绘制它
						TopDREC->V = this;
						ResultDRECList->Add(TopDREC);
						//下方剩余的碎片
						List<REC> * recs = REC::Subtract(ButtonDREC, TopDREC);
						for (unsigned int rcount = 0; rcount < recs->Count; rcount++)
						{
							DREC * drec = new DREC(*recs->Getbyid(rcount));
							drec->V = ButtonDREC->V;
							drec->FLAG_Draw = ButtonDREC->FLAG_Draw;
							drec->FLAG_LastDraw = ButtonDREC->FLAG_LastDraw;
							ResultDRECList->Add(drec);
						}
						delete recs;
					}
					else
					{
						for (int i= nocount; i< ButtonDRECList->Count;i++)
						{
							ResultDRECList->Add(new DREC(*ButtonDRECList->Getbyid(i)));
						}
						delete TopDREC;
						break;
					}
				}
				else if(REC::IsOverlap(ButtonDREC, TopDREC))//重叠
				{
					List<REC> * ResultTopREC = REC::Subtract(TopDREC, ButtonDREC);//收集到的上层矩形碎片
					List<DREC>*	ResultTopDREC = new List<DREC>();
					for (unsigned int rcount = 0; rcount < ResultTopREC->Count; rcount++)
					{
						REC * drec_1 = ResultTopREC->Getbyid(rcount);
						DREC * drec = new DREC(*drec_1);
						drec->V = TopDREC->V;
						drec->FLAG_Draw = TopDREC->FLAG_Draw;
						drec->FLAG_LastDraw = TopDREC->FLAG_LastDraw;
						ResultTopDREC->Add(drec);
					}
					delete ResultTopREC;
					if (ButtonDREC->GetDRECType() == Static)//下方矩形是静态矩形
					{
						//生成重绘矩形 由所在的View绘制它
						List<REC> * AimdRecs = new List<REC>();//命中的矩形
						AimdRecs->Add(new DREC(*TopDREC));//初始为上层矩形
						for (unsigned int rcount = 0; rcount < ResultTopDREC->Count; rcount++)
						{
							DREC * drec = ResultTopDREC->Getbyid(rcount);
							List<REC> * NewAimdRecs = new List<REC>();
							for (unsigned int acount = 0; acount < AimdRecs->Count; acount++)
							{
								NewAimdRecs->Add(REC::Subtract(AimdRecs->Getbyid(acount), drec));//上层矩形减去它的碎片等于重叠的部分
							}
							delete AimdRecs;
							AimdRecs = NewAimdRecs;
						}
						for (unsigned int acount = 0; acount < AimdRecs->Count; acount++)//添加必要的信息
						{
							DREC * AimdRec = new DREC(*(AimdRecs->Getbyid(acount)));
							AimdRec->V = this;
							AimdRec->FLAG_Draw = ButtonDREC->FLAG_Draw;
							AimdRec->FLAG_LastDraw = ButtonDREC->FLAG_LastDraw;
							ResultDRECList->Add(AimdRec);
						}
						delete AimdRecs;
						List<REC> * ResoutButtonDREC = REC::Subtract(ButtonDREC, TopDREC);//下层矩形的碎片
						for (unsigned int rcount = 0; rcount < ResoutButtonDREC->Count; rcount++)
						{
							DREC * drec = new DREC(*(ResoutButtonDREC->Getbyid(rcount)));
							drec->V = ButtonDREC->V;
							drec->FLAG_Draw = ButtonDREC->FLAG_Draw;
							drec->FLAG_LastDraw = ButtonDREC->FLAG_LastDraw;
							ResultDRECList->Add(drec);
						}
						delete ResoutButtonDREC;
					}
					else
					{
						ResultDRECList->Add(new DREC(*ButtonDREC));
					}
					TopDREC = ResultTopDREC->Getbyid(0);
					DRECList->Replacebyid(count, ResultTopDREC);
				}
				else//不包含
				{
					ResultDRECList->Add(new DREC(*ButtonDREC));
				}
			}
			else //普通命中
			{
				if (REC::IsContain(TopDREC, ButtonDREC))//如果没覆盖则用无重叠矩形减去已绘矩形并加入新无无重叠的矩形链表
				{
					int a=0;
				}
				else if(REC::IsOverlap(TopDREC, ButtonDREC))
				{
					List<REC> * recs = REC::Subtract(ButtonDREC, TopDREC);
					for (unsigned int rcount = 0; rcount < recs->Count; rcount++)
					{
						DREC * drec = new DREC(*recs->Getbyid(rcount));
						drec->V = ButtonDREC->V;
						drec->FLAG_Draw = ButtonDREC->FLAG_Draw;
						drec->FLAG_LastDraw = ButtonDREC->FLAG_LastDraw;
						ResultDRECList->Add(drec);
					}
					delete recs;
				}
				else
				{
					ResultDRECList->Add(new DREC(*ButtonDREC));
				}
			}
        }
        delete ButtonDRECList;
        ButtonDRECList=ResultDRECList;
    }
    DRECList->Add(ButtonDRECList);//合并上下层矩形
}
void View::Requestpaint(DREC* rec)
{
    if(!ViewDRECList)
        ViewDRECList=new List<DREC>();
     ViewDRECList->Add(rec);
}
void View::AddChildView(View * V)
{
    ChildViewList->Add(V);
    V->FatherView=this;
}
List<DREC> * View::EliminationOverlap(List<DREC> * Drec)//消去重叠区获取无重叠矩形链表
{
	List<DREC> * ButtonDRECList = new List<DREC>();//新建一个无重叠的矩形链表
	if (Drec->Count != 0)
	{
		ButtonDRECList->Add(Drec->Getbyid(0));//取请求矩形的第一个矩形加入这个链表
		Drec->Deletebyid(0, false);//删除原来的请求矩形
	}
	for (unsigned int count = 0; count < Drec->Count; count++)
	{
		//从请求矩形链表取出一个矩形
		DREC * TopDREC = Drec->Getbyid(count);
		List<DREC> * ResultDRECList = new List<DREC>();//与这个矩形的裁剪结果
		for (int nocount = 0; nocount < ButtonDRECList->Count; nocount++)
		{
			DREC * ButtonDREC = ButtonDRECList->Getbyid(count);//取出一个无重叠矩形
			if (REC::IsContain((REC*)TopDREC, (REC*)ButtonDREC))//如果后请求矩形把无重叠矩形覆盖了则删除无重叠矩形
			{
				ButtonDRECList->Deletebyid(nocount, true);
				nocount -= 1;
			}
			else //如果没覆盖则用无重叠矩形减去请求矩形再加上请求矩形组成没有重叠的矩形链表
			{
				List<REC> * recs = REC::Subtract((REC*)ButtonDREC, (REC*)TopDREC);
				for (unsigned int rcount = 0; rcount < recs->Count; rcount++)
				{
					DREC * drec = new DREC(*recs->Getbyid(rcount));
					drec->V = ButtonDREC->V;
					drec->FLAG_Draw = ButtonDREC->FLAG_Draw;
					drec->FLAG_LastDraw = ButtonDREC->FLAG_LastDraw;
					ResultDRECList->Add(drec);
				}
				delete recs;
			}
		}
		delete ButtonDRECList;
		ButtonDRECList = ResultDRECList;
		ButtonDRECList->Add(new DREC(*TopDREC));
	}
	return ButtonDRECList;
}