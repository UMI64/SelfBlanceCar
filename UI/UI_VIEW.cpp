#include "UI.hpp"
DREC::DREC(View &V)
{
	this->Height = V.Height;
	this->Width = V.Width;
	this->PositionX = V.PositionX;
	this->PositionY = V.PositionY;
	this->FLAG_Draw = V.FLAG_Repaint; //��������ػ��Լ���
	if (V.FLAG_Repaint) this->V = &V;//���Լ������Լ�
	V.FLAG_Repaint = false;//�����ػ�����
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
    for(unsigned int viewcount=0;viewcount<ChildViewList->Count;viewcount++)//��߲������˲���
    {
        View * V=ChildViewList->Getbyid(viewcount);//ȡ��һ��view
        V->GetDREC(DRECList);//����ѻ����
    }
	this->OnDraw();
    //��װ�Լ�������������ѻ�����
    //���Լ�����������ϲ���һ��
	if (!ViewDRECList) ViewDRECList = new List<DREC>();
    DREC * ViewDREC= new DREC(*this);//�����Լ�View����һ������
    ViewDRECList->Add(ViewDREC);//�����View�ľ���
    List<DREC> * ButtonDRECList=EliminationOverlap(ViewDRECList);//��ȥ�ص���Ϊ���βü����²����(�ϲ���λ����²���κ����Ƭ���ϲ�ϲ�)
	delete ViewDRECList;//���ô�View�ľ�������
	ViewDRECList = nullptr;//���ô�View�ľ�������
	/*��ʼ�ü�*/
    for(int count=0;count<DRECList->Count;count++)//���ϲ�����ó�һ������ȥ����²����,�ռ�������Ƭ��Ϊ�µ��²����
    {
        DREC * TopDREC=DRECList->Getbyid(count);//���ϲ��������ȡ��һ������
        List<DREC> * ResultDRECList=new List<DREC>();//�²���ε���Ƭ�ռ�����
        for(int nocount=0;nocount<ButtonDRECList->Count;nocount++)//�ó��²���γ�������
        {
            DREC * ButtonDREC=ButtonDRECList->Getbyid(nocount);//ȡ��һ���²����
			//�ж���ʲô���λ��
			/*	������ƾ��� �ѻ���� ��̬����
				�ϲ�������ƾ���	�ϲ�������ƾ���	�ϲ�������ƾ���
				�²�������ƾ���	�²��ػ����		�²��ѻ����
				�ϼ���				�¼���				
			*/
			if (TopDREC->GetDRECType()== Request)//������ƾ��������·�����
			{
				if (REC::IsContain(ButtonDREC, TopDREC))//�·����ΰ����Ϸ�����
				{
					//ɾ���Ϸ�����(��ȫ����)
					DRECList->Deletebyid(count,false);
					count -= 1;
					if (ButtonDREC->GetDRECType()==Static)//�·������Ǿ�̬����
					{
						//�����ػ���� �����ڵ�View������
						TopDREC->V = this;
						ResultDRECList->Add(TopDREC);
						//�·�ʣ�����Ƭ
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
				else if(REC::IsOverlap(ButtonDREC, TopDREC))//�ص�
				{
					List<REC> * ResultTopREC = REC::Subtract(TopDREC, ButtonDREC);//�ռ������ϲ������Ƭ
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
					if (ButtonDREC->GetDRECType() == Static)//�·������Ǿ�̬����
					{
						//�����ػ���� �����ڵ�View������
						List<REC> * AimdRecs = new List<REC>();//���еľ���
						AimdRecs->Add(new DREC(*TopDREC));//��ʼΪ�ϲ����
						for (unsigned int rcount = 0; rcount < ResultTopDREC->Count; rcount++)
						{
							DREC * drec = ResultTopDREC->Getbyid(rcount);
							List<REC> * NewAimdRecs = new List<REC>();
							for (unsigned int acount = 0; acount < AimdRecs->Count; acount++)
							{
								NewAimdRecs->Add(REC::Subtract(AimdRecs->Getbyid(acount), drec));//�ϲ���μ�ȥ������Ƭ�����ص��Ĳ���
							}
							delete AimdRecs;
							AimdRecs = NewAimdRecs;
						}
						for (unsigned int acount = 0; acount < AimdRecs->Count; acount++)//��ӱ�Ҫ����Ϣ
						{
							DREC * AimdRec = new DREC(*(AimdRecs->Getbyid(acount)));
							AimdRec->V = this;
							AimdRec->FLAG_Draw = ButtonDREC->FLAG_Draw;
							AimdRec->FLAG_LastDraw = ButtonDREC->FLAG_LastDraw;
							ResultDRECList->Add(AimdRec);
						}
						delete AimdRecs;
						List<REC> * ResoutButtonDREC = REC::Subtract(ButtonDREC, TopDREC);//�²���ε���Ƭ
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
				else//������
				{
					ResultDRECList->Add(new DREC(*ButtonDREC));
				}
			}
			else //��ͨ����
			{
				if (REC::IsContain(TopDREC, ButtonDREC))//���û�����������ص����μ�ȥ�ѻ���β������������ص��ľ�������
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
    DRECList->Add(ButtonDRECList);//�ϲ����²����
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
List<DREC> * View::EliminationOverlap(List<DREC> * Drec)//��ȥ�ص�����ȡ���ص���������
{
	List<DREC> * ButtonDRECList = new List<DREC>();//�½�һ�����ص��ľ�������
	if (Drec->Count != 0)
	{
		ButtonDRECList->Add(Drec->Getbyid(0));//ȡ������εĵ�һ�����μ����������
		Drec->Deletebyid(0, false);//ɾ��ԭ�����������
	}
	for (unsigned int count = 0; count < Drec->Count; count++)
	{
		//�������������ȡ��һ������
		DREC * TopDREC = Drec->Getbyid(count);
		List<DREC> * ResultDRECList = new List<DREC>();//��������εĲü����
		for (int nocount = 0; nocount < ButtonDRECList->Count; nocount++)
		{
			DREC * ButtonDREC = ButtonDRECList->Getbyid(count);//ȡ��һ�����ص�����
			if (REC::IsContain((REC*)TopDREC, (REC*)ButtonDREC))//�����������ΰ����ص����θ�������ɾ�����ص�����
			{
				ButtonDRECList->Deletebyid(nocount, true);
				nocount -= 1;
			}
			else //���û�����������ص����μ�ȥ��������ټ�������������û���ص��ľ�������
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