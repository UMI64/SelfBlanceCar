#ifndef __UILIST__H__
#define __UILIST__H__
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
		int Count=0;
		//����һ��Ԫ�ص�ָ��
		void Add(T *item);
		void Add(List<T> * list);
		//�õ�һ���ڵ��Ԫ��(����)
		T * Getbyid(unsigned int id);
		//ɾ��һ��Ԫ��
		void Deletebyid(unsigned int id, bool IsDeleteItem);
		void Replacebyid(unsigned int id, T *item);
		void Replacebyid(unsigned int id, List<T> *item);
		void DoNotDeleteItem()
		{
			FLAG_DoNotDeleteItem=true;
		}
		void Clear();//ɾ������Ԫ��
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
template <class T> class ListTest
{
public:
	T TrueItem;//Ԫ��ָ��
};
#endif