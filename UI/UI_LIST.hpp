#ifndef __UILIST__H__
#define __UILIST__H__
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
		int Count=0;
		//加入一个元素的指针
		void Add(T *item);
		void Add(List<T> * list);
		//得到一个节点的元素(引用)
		T * Getbyid(unsigned int id);
		//删除一个元素
		void Deletebyid(unsigned int id, bool IsDeleteItem);
		void Replacebyid(unsigned int id, T *item);
		void Replacebyid(unsigned int id, List<T> *item);
		void DoNotDeleteItem()
		{
			FLAG_DoNotDeleteItem=true;
		}
		void Clear();//删除所有元素
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
template <class T> class ListTest
{
public:
	T TrueItem;//元素指针
};
#endif