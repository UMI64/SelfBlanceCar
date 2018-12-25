#include "UI_LIST.hpp"
template <class T>
void List<T>::Add(const T &item)
{
	if(this->pFirstItemNode ==nullptr)
	{
		this->pFirstItemNode =new ItemNode<T>(item);
		this->pLastItemNode =this->pFirstItemNode;
	}
	else
	{
		this->pLastItemNode->CreateNextItemNode(item);
		this->pLastItemNode=this->pLastItemNode->pNextItemNode;
	}
	Count++;
}
template <class T>
void List<T>::Add(const List<T> &const_list)
{
	List<T> &list (const_cast<List<T>&>(const_list));
	if (list.Count)
	{
		unsigned int count = 0;
		if (pFirstItemNode == nullptr)//如果没有第一个节点
		{
			T temp (list.GetItembyid(0));
			pFirstItemNode = new ItemNode<T>(temp);
			count = 1;
		}
		ItemNode<T> * pTemp= pFirstItemNode;
		for (; count < list.Count; ++count)
		{
			pTemp->CreateNextItemNode(list.GetItembyid(count));
			pTemp = pTemp->pNextItemNode;
		}
		pLastItemNode = pTemp;
		Count += list.Count;
	}
}
template <class T>
T& List<T>::GetItembyid(unsigned int id)
{
	if (!id)
	{
		return pFirstItemNode->Item;
	}
	ItemNode<T> * pItemNode = pFirstItemNode;
	while (id > 0 && pItemNode&&pItemNode->pNextItemNode)
	{
		--id;
		pItemNode = pItemNode->pNextItemNode;
	}
	return pItemNode->Item;
}
template <class T>
T& List<T>::GetLastItem()
{
	return pLastItemNode->Item;
}
template <class T>
void  List<T>::Deletebyid(unsigned int id)
{
	if (id >= Count) while (1);
	ItemNode<T> * pTemp = pFirstItemNode;
	if (id == 0)pFirstItemNode= pFirstItemNode->pNextItemNode;
	if (id == Count - 1)pLastItemNode= pLastItemNode->pPreItemNode;
	while (id)
	{
		pTemp = pTemp->pNextItemNode;
		--id;
	}
	delete pTemp;
	--Count;
}
template <class T>
void  List<T>::Replacebyid(unsigned int id, const T &item)
{
	ItemNode<T> * pItemNode = this->pFirstItemNode;
	while (id > 0)
	{
		id--;
		pItemNode = pItemNode->pNextItem;
	}
	pItemNode->Item = item;
}
template <class T>
void  List<T>::Replacebyid(unsigned int id,const List<T> &const_list)
{
	List<T> &list = const_cast<List<T>&>(const_list);
	if (list.Count)
	{
		unsigned int const_id = id;
		ItemNode<T> * pTempNode = pFirstItemNode;
		while (id>0)
		{
			pTempNode = pTempNode->pNextItemNode;
			id--;
		}
		pTempNode->Item = list.pFirstItemNode->Item;
		for (unsigned int count = 1; count < list.Count; ++count)
		{
			pTempNode->CreateNextItemNode(list.GetItembyid(count));
			pTempNode = pTempNode->pNextItemNode;
		}
	}
	else
		Deletebyid(id);
	Count += list.Count;
}
template <class T>
void  List<T>::Replace(const List<T> &const_list)
{
	List<T> &list(const_cast<List<T>&>(const_list));
	Clear();
	pFirstItemNode = const_list.pFirstItemNode;
	pLastItemNode = const_list.pLastItemNode;
	Count = const_list.Count;
	list.FLAG_DoNotDeleteNode = true;
}
template <class T>
void List<T>::Clear()
{
	ItemNode<T> * pItemNode = pFirstItemNode;
	while (pItemNode)
	{
		ItemNode<T> * pNextItemNode = pItemNode->pNextItemNode;
		delete pItemNode;
		pItemNode = pNextItemNode;
	}
	pFirstItemNode = nullptr;
	pLastItemNode = nullptr;
	Count = 0;
}