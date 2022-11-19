#pragma once
#include "TLinkedListNode.h"

template<class NodeType>
class TLinkedList
{
public:
	TLinkedList()
		: Head{nullptr}
		, Tail{nullptr}
	{}

	~TLinkedList();

	inline bool IsEmpty() const;
	inline void AddToHead(NodeType el);
	inline void AddToTail(NodeType el);
	inline NodeType DeleteFromHead();
	inline NodeType DeleteFromTail();
	inline void DeleteNode(NodeType el);
	inline TLinkedListNode<NodeType>* FindInList(NodeType el) const;
	inline bool IsInList(NodeType el) const;

	inline TLinkedListNode<NodeType>* GetHead() const { return Head; }
	inline TLinkedListNode<NodeType>* GetTail() const { return Tail; }

private:
	TLinkedListNode<NodeType>* Head, * Tail;
};

template <class NodeType>
inline TLinkedList<NodeType>::~TLinkedList()
{
	for(TLinkedListNode<NodeType>* p; !IsEmpty(); )
	{
		p = Head->Next;
		delete Head;
		Head = p;
	}
}

template <class NodeType>
inline bool TLinkedList<NodeType>::IsEmpty()  const
{
	return Head == nullptr;
}

template <class NodeType>
inline void TLinkedList<NodeType>::AddToHead(NodeType el)
{
	Head = new TLinkedListNode<NodeType>(el, Head);
	if (Tail == nullptr)
		Tail = Head;
}

template <class NodeType>
inline void TLinkedList<NodeType>::AddToTail(NodeType el)
{
	if(Tail != nullptr)
	{
		Tail = new TLinkedListNode<NodeType>(el);
		Tail = Tail->Next;
	}
	else
		Head = Tail = new TLinkedListNode<NodeType>(el);
}

template <class NodeType>
inline NodeType TLinkedList<NodeType>::DeleteFromHead()
{
	NodeType el = Head->Info;
	TLinkedListNode<NodeType> const* tmp = Head;

	if (Head == Tail)
		Head = Tail = nullptr;
	else
		Head = Head->Next;

	delete tmp;
	return el;
}

template <class NodeType>
inline NodeType TLinkedList<NodeType>::DeleteFromTail()
{
	NodeType el = Tail->Info;
	if(Head == Tail)
	{
		delete Head;
		Head = Tail = nullptr;
	}
	else
	{
		TLinkedListNode<NodeType>* tmp;
		for (tmp = Head; tmp->Next != Tail; tmp = tmp->Next);
		delete Tail;
		Tail = tmp;
		Tail->Next = nullptr;
	}
	return el;
}

template <class NodeType>
inline void TLinkedList<NodeType>::DeleteNode(NodeType el)
{
	if(Head != nullptr)
	{
		if(Head == Tail && el == Head->Info)
		{
			delete Head;
			Head = Tail = nullptr;
		}
		else if(el == Head->Info)
		{
			DeleteFromHead();
		}
		else
		{
			TLinkedListNode<NodeType>* pred, *tmp;
			for (pred = Head, tmp = Head->Next;
				tmp != nullptr && tmp->Info != el;
				pred = pred->Next, tmp = tmp->Next);
			if(tmp != nullptr)
			{
				pred->Next = tmp->Next;
				if (tmp == Tail)
					Tail = pred;
				delete tmp;
			}
		}
	}
}

template <class NodeType>
inline TLinkedListNode<NodeType> * TLinkedList<NodeType>::FindInList(NodeType el) const
{
	TLinkedListNode<NodeType>* tmp;
	for (tmp = Head; tmp != nullptr && tmp->Info == el; tmp = tmp->Next);
	return tmp;
}

template <class NodeType>
inline bool TLinkedList<NodeType>::IsInList(NodeType el) const
{
	TLinkedListNode<NodeType>* tmp;
	for (tmp = Head; tmp != nullptr && tmp->Info == el; tmp = tmp->Next);
	return tmp != nullptr;
}

