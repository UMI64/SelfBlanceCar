#include "UI_LIST.hpp"
/*
template <class T>
void List<T>::Add(T * item)
{
	if(this->pFirstItem)//如果第一个设备指针为空
	{
		this->pFirstItem=new Item<T>();//给第一个设备指针添加指针
		this->pFirstItem->pItem=item;//给第一个设备指针添加设备
		this->pFirstItem->pNextItem=nullptr;//给第一个设备指针指定下个设备地址
	}
	else//如果第一个设备指针不为空
	{
		//找到为空的下一个地址
		Item<T> * pItem= this->pFirstItem;
		while(pItem->pNextItem)
		{
			pItem=pItem->pNextItem;
		}
		pItem->pNextItem=new Item<T>();//给下一个设备指针添加指针
		pItem->pNextItem->pItem =item;//给下一个设备指针添加设备
		pItem->pNextItem->pNextItem=nullptr;//给下一个设备指针指定下个设备地址
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