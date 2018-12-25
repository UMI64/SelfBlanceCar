#ifndef __UILIST__H__
#define __UILIST__H__
template <class T> class ItemNode
{
	public:
		ItemNode(const T &item)
		{
			Item=item;
		}
		~ItemNode()
		{
			if (pPreItemNode) pPreItemNode->pNextItemNode = pNextItemNode;
			if (pNextItemNode) pNextItemNode->pPreItemNode = pPreItemNode;
		}
		void CreateNextItemNode(const T &item)
		{
			ItemNode *ptemp = pNextItemNode;
			pNextItemNode = new ItemNode(item);
			pNextItemNode->pNextItemNode = ptemp;
			if (ptemp)
			{
				ptemp->pPreItemNode = pNextItemNode;
			}
			pNextItemNode->pPreItemNode = this;
		}
		T Item;
		ItemNode * pPreItemNode=nullptr;
		ItemNode * pNextItemNode=nullptr;
};
template <class T> class List
{
	public:
		int Count=0;
		void Add(const T &item);
		void Add(const List<T> &const_list);
		T& GetItembyid(unsigned int id);
		T& GetLastItem();
		void Deletebyid(unsigned int id);
		void Replacebyid(unsigned int id, const T &item);
		void Replacebyid(unsigned int id, const List<T> &const_list);
		void Replace(const List<T> &const_list);
		void Clear();
		List()
		{

		}
		List(const List<T> &list)
		{
			Replace(list);
			//this->Add(list);
		}
		~List()
		{
			if(FLAG_DoNotDeleteNode==false)
				Clear();
		}
		void operator =(const List<T> &list)
		{
			this->Clear();
			this->Add(list);
		}
	private:
		bool FLAG_DoNotDeleteNode=false;
		ItemNode<T> * pFirstItemNode;
		ItemNode<T> * pLastItemNode;
};
#endif