#include "UI_LIST.hpp"
/*
template <class T>
void List<T>::Add(T * item)
{
	if(this->pFirstItem)//�����һ���豸ָ��Ϊ��
	{
		this->pFirstItem=new Item<T>();//����һ���豸ָ�����ָ��
		this->pFirstItem->pItem=item;//����һ���豸ָ������豸
		this->pFirstItem->pNextItem=nullptr;//����һ���豸ָ��ָ���¸��豸��ַ
	}
	else//�����һ���豸ָ�벻Ϊ��
	{
		//�ҵ�Ϊ�յ���һ����ַ
		Item<T> * pItem= this->pFirstItem;
		while(pItem->pNextItem)
		{
			pItem=pItem->pNextItem;
		}
		pItem->pNextItem=new Item<T>();//����һ���豸ָ�����ָ��
		pItem->pNextItem->pItem =item;//����һ���豸ָ������豸
		pItem->pNextItem->pNextItem=nullptr;//����һ���豸ָ��ָ���¸��豸��ַ
	}
}
template <class T>
T * List<T>::Getbyid(uint8_t id)
{
	Item<T> * pItem= this->pFirstItem;
	while(id>0&&pItem->pNextItem)
	{
		id--;
		pItem=pItem->pNextItem;
	}
	if(id)return nullptr;
	return pItem->pItem;
}
*/