#ifndef __UILIST__H__
#define __UILIST__H__
//#include "stm32f10x.h"
/*һ��Ԫ��*/
template <class T> class Item
{
	public:
		//����һ�����������Ķ���
		Item(T *item)
		{
			pTrueItem=item;
		}
		~Item()
		{
			if(!FLAG_DoNotDeleteItem)
				delete pTrueItem;
			else
			{
				int a=1;
			}
		}
		T  * pTrueItem;//Ԫ��ָ��
		Item * pNextItem;//��һ��Ԫ�صĵ�ַ
		bool FLAG_DoNotDeleteItem=false;
};
/*���������*/
template <class T> class List
{
	public:
		unsigned int Count=0;
		//����һ��Ԫ�ص�ָ��
		void Add(T *item)
		{
			if(this->pFirstItem==nullptr)//�����һ��Ԫ�ص�ָ��Ϊ��
			{
				this->pFirstItem=new Item<T>(item);//���ɵ�һ��Ԫ�ض���
				this->pFirstItem->pNextItem=nullptr;//��ʼ����һ��Ԫ�ض����ָ��
				this->pLastItem=this->pFirstItem;
			}
			else//�����һ���豸ָ�벻Ϊ��
			{
				//�ҵ�Ϊ�յ���һ����ַ
				Item<T> * pItem= this->pFirstItem;//��ʱԪ��ָ��
				while(pItem->pNextItem)//�����ʱԪ��ָ���ָ��Ϊ��Ϊ��
				{//��ʱԪ��ָ��ָ����һ��Ԫ��
					pItem=pItem->pNextItem;
				}
				pItem->pNextItem=new Item<T>(item);//����Ԫ�ض�������һ��Ԫ��ָ����
				pItem->pNextItem->pNextItem=nullptr;//��ʼ��Ԫ�ض����ָ��
				this->pLastItem=pItem->pNextItem;
			}
			Count++;
		}
		void Add(List<T> * list)
		{
			if (list->Count)
			{
				if (!pFirstItem)//��һ��Ԫ���ǿ�
				{
					pFirstItem = list->pFirstItem;
					pLastItem = list->pLastItem;
					Count = list->Count;
				}
				else
				{
					pLastItem->pNextItem = list->pFirstItem;
					pLastItem = list->pLastItem;
					Count += list->Count;
				}
				list->FLAG_InternalDelete = true;
			}
			delete list;
		}
		//�õ�һ���ڵ��Ԫ��(����)
		T * Getbyid(unsigned int id)
		{
			if(!id)
			{
				return pFirstItem->pTrueItem;
			}
			Item<T> * pItem = this->pFirstItem;//��ʱԪ��ָ��
			while(id>0&&pItem&&pItem->pNextItem)//id����0 ������ʱԪ��ָ�벻Ϊ�� ������ʱԪ��ָ���ָ��Ϊ��
			{
				id--;
				pItem=pItem->pNextItem;
			}
			if(id)return nullptr;//���id����0����û�ҵ�
			return pItem->pTrueItem;
		}
		//ɾ��һ��Ԫ��
		void Deletebyid(unsigned int id,bool IsDeleteItem)
		{
			if(id==0)
			{
				if(! pFirstItem) return;//��һ��Ԫ���ǿյľ�ֱ�ӷ���
				Item<T> * pItem= this->pFirstItem;//��ʱԪ��ָ��
				pFirstItem=pFirstItem->pNextItem;
				if(!pFirstItem)//���ڵĵ�һ���������һ��
					this->pLastItem=nullptr;
				pItem->FLAG_DoNotDeleteItem=!IsDeleteItem;
				delete pItem;
			}
			else
			{
				Item<T> * pItem= this->pFirstItem;//��ʱԪ��ָ��
				while(id>1&&pItem&&pItem->pNextItem)//id����1 ������ʱԪ��ָ�벻Ϊ�� ������ʱԪ��ָ���ָ��Ϊ��
				{
					id--;
					pItem=pItem->pNextItem;
				}
				if(id>1)//���id����1����û�ҵ�
					return;
				else
				{
					//�ҵ��Ľ���ɾ����ǰһ��Ԫ��
					if(pItem->pNextItem)
					{
						auto pDeleteItem=pItem->pNextItem;
						pItem->pNextItem=pDeleteItem->pNextItem;
						if(!pItem->pNextItem)
							this->pLastItem=pItem;
						pDeleteItem->FLAG_DoNotDeleteItem=!IsDeleteItem;	
						delete pDeleteItem;
					}
				}
			}
			Count--;
		}
		void Replacebyid(unsigned int id,T *item)
		{
			Item<T> * pItem= this->pFirstItem;//��ʱԪ��ָ��
			while(id>0&&pItem)//id����0 ������ʱԪ��ָ�벻Ϊ��
			{
				id--;
				pItem=pItem->pNextItem;
			}
			if(pItem)//�ҵ�Ŀ��
			{
				delete pItem->pTrueItem;//ɾ��ԭʼĿ��
				pItem->pTrueItem=item;//������Ŀ��
			}
		}
		void Replacebyid(unsigned int id,List<T> *item)
		{
			if (item->Count)
			{
				Item<T> * pItem = this->pFirstItem;//��ʱԪ��ָ��
				if (!id)//�滻��һ��Ԫ��
				{
					auto pNext = pItem->pNextItem;//��¼��һ��Ŀ���ַ
					delete pItem;//ɾ��ԭʼĿ��
					pFirstItem = item->pFirstItem;//�趨������ĵ�һ��Ԫ��
					if (pNext)//����������һ��Ԫ��
						item->pLastItem->pNextItem = pNext;//�趨������������һ��Ԫ��
					else
						pLastItem = item->pLastItem;
					Count += item->Count - 1;
					item->FLAG_InternalDelete = true;
					delete item;
					return;
				}
				while (id > 1 && pItem)//id����0 ������ʱԪ��ָ�벻Ϊ��
				{
					id--;
					pItem = pItem->pNextItem;
				}
				if (pItem)//�ҵ�Ŀ��
				{
					auto pNext = pItem->pNextItem->pNextItem;//��¼��һ��Ŀ���ַ
					delete pItem->pNextItem;//ɾ��ԭʼĿ��
					pItem->pNextItem = item->pFirstItem;//����ͷ��
					if (!pNext)//���β���ǿյ�
					{
						pLastItem = item->pLastItem;
					}
					else
					{
						item->pLastItem->pNextItem = pNext;
					}
					Count += item->Count - 1;
				}
				item->FLAG_InternalDelete = true;//�ڲ�ɾ��
			}
			else
				Deletebyid(id,true);
			delete item;
		}
		void DoNotDeleteItem()
		{
			FLAG_DoNotDeleteItem=true;
		}
		void Clear()//ɾ������Ԫ��
		{
			Item<T> * pItem=pFirstItem;
			while(pItem)
			{
				Item<T> * pNextItem=pItem->pNextItem;
				pItem->FLAG_DoNotDeleteItem=FLAG_DoNotDeleteItem;
				delete pItem;
				pItem=pNextItem;
			}
			pFirstItem = nullptr;
			Count=0;
		}
		bool haveDonotDeleteitem()
		{
			Item<T> *pitem = pFirstItem;
			for (int i = 0; i < Count; i++)
			{
				if (pitem->FLAG_DoNotDeleteItem) return true;
			}
			return false;
		}
		~List()
		{
			if(!FLAG_InternalDelete)
				Clear();
		}
	private:
		bool FLAG_DoNotDeleteItem=false;
		bool FLAG_InternalDelete=false;
		Item<T> * pFirstItem;//��һ��Ԫ�صĵ�ַ
		Item<T> * pLastItem;//���һ��Ԫ�صĵ�ַ
};
#endif