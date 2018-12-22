#ifndef __UILIST__H__
#define __UILIST__H__
//#include "stm32f10x.h"
/*一个元素*/
template <class T> class Item
{
	public:
		//创建一个由链表管理的对象
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
		T  * pTrueItem;//元素指针
		Item * pNextItem;//下一个元素的地址
		bool FLAG_DoNotDeleteItem=false;
};
/*链表管理类*/
template <class T> class List
{
	public:
		unsigned int Count=0;
		//加入一个元素的指针
		void Add(T *item)
		{
			if(this->pFirstItem==nullptr)//如果第一个元素的指针为空
			{
				this->pFirstItem=new Item<T>(item);//生成第一个元素对象
				this->pFirstItem->pNextItem=nullptr;//初始化第一个元素对象的指针
				this->pLastItem=this->pFirstItem;
			}
			else//如果第一个设备指针不为空
			{
				//找到为空的下一个地址
				Item<T> * pItem= this->pFirstItem;//临时元素指针
				while(pItem->pNextItem)//如果临时元素指针的指向为不为空
				{//临时元素指针指向下一个元素
					pItem=pItem->pNextItem;
				}
				pItem->pNextItem=new Item<T>(item);//生成元素对象并让上一个元素指向它
				pItem->pNextItem->pNextItem=nullptr;//初始化元素对象的指针
				this->pLastItem=pItem->pNextItem;
			}
			Count++;
		}
		void Add(List<T> * list)
		{
			if (list->Count)
			{
				if (!pFirstItem)//第一个元素是空
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
		//得到一个节点的元素(引用)
		T * Getbyid(unsigned int id)
		{
			if(!id)
			{
				return pFirstItem->pTrueItem;
			}
			Item<T> * pItem = this->pFirstItem;//临时元素指针
			while(id>0&&pItem&&pItem->pNextItem)//id大于0 并且临时元素指针不为空 并且临时元素指针的指向不为空
			{
				id--;
				pItem=pItem->pNextItem;
			}
			if(id)return nullptr;//如果id大于0表明没找到
			return pItem->pTrueItem;
		}
		//删除一个元素
		void Deletebyid(unsigned int id,bool IsDeleteItem)
		{
			if(id==0)
			{
				if(! pFirstItem) return;//第一个元素是空的就直接返回
				Item<T> * pItem= this->pFirstItem;//临时元素指针
				pFirstItem=pFirstItem->pNextItem;
				if(!pFirstItem)//现在的第一个就是最后一个
					this->pLastItem=nullptr;
				pItem->FLAG_DoNotDeleteItem=!IsDeleteItem;
				delete pItem;
			}
			else
			{
				Item<T> * pItem= this->pFirstItem;//临时元素指针
				while(id>1&&pItem&&pItem->pNextItem)//id大于1 并且临时元素指针不为空 并且临时元素指针的指向不为空
				{
					id--;
					pItem=pItem->pNextItem;
				}
				if(id>1)//如果id大于1表明没找到
					return;
				else
				{
					//找到的将被删除的前一个元素
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
			Item<T> * pItem= this->pFirstItem;//临时元素指针
			while(id>0&&pItem)//id大于0 并且临时元素指针不为空
			{
				id--;
				pItem=pItem->pNextItem;
			}
			if(pItem)//找到目标
			{
				delete pItem->pTrueItem;//删除原始目标
				pItem->pTrueItem=item;//加入新目标
			}
		}
		void Replacebyid(unsigned int id,List<T> *item)
		{
			if (item->Count)
			{
				Item<T> * pItem = this->pFirstItem;//临时元素指针
				if (!id)//替换第一个元素
				{
					auto pNext = pItem->pNextItem;//记录下一个目标地址
					delete pItem;//删除原始目标
					pFirstItem = item->pFirstItem;//设定本链表的第一个元素
					if (pNext)//如果不是最后一个元素
						item->pLastItem->pNextItem = pNext;//设定加入链表的最后一个元素
					else
						pLastItem = item->pLastItem;
					Count += item->Count - 1;
					item->FLAG_InternalDelete = true;
					delete item;
					return;
				}
				while (id > 1 && pItem)//id大于0 并且临时元素指针不为空
				{
					id--;
					pItem = pItem->pNextItem;
				}
				if (pItem)//找到目标
				{
					auto pNext = pItem->pNextItem->pNextItem;//记录下一个目标地址
					delete pItem->pNextItem;//删除原始目标
					pItem->pNextItem = item->pFirstItem;//链接头部
					if (!pNext)//如果尾部是空的
					{
						pLastItem = item->pLastItem;
					}
					else
					{
						item->pLastItem->pNextItem = pNext;
					}
					Count += item->Count - 1;
				}
				item->FLAG_InternalDelete = true;//内部删除
			}
			else
				Deletebyid(id,true);
			delete item;
		}
		void DoNotDeleteItem()
		{
			FLAG_DoNotDeleteItem=true;
		}
		void Clear()//删除所有元素
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
		Item<T> * pFirstItem;//第一个元素的地址
		Item<T> * pLastItem;//最后一个元素的地址
};
#endif