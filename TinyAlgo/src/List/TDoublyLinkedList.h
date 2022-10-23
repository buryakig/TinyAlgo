#pragma once
#include "TLinkedListNode.h"

template<class NodeType>
class TDoublyLinkedList
{
public:
	TDoublyLinkedList()
		: Head{nullptr}
		, Tail{nullptr}
	{}

	inline void AddToTail(const NodeType& el);
	inline NodeType DeleteFromTail();

private:
	TDoublyLinkedListNode<NodeType>* Head, *Tail;
};

template <class NodeType>
void TDoublyLinkedList<NodeType>::AddToTail(const NodeType& el)
{
	if(Tail)
	{
		Tail = new TDoublyLinkedList<NodeType>(el, nullptr, Tail);
		Tail->Prev->Next = Tail;
	}
}

template <class NodeType>
NodeType TDoublyLinkedList<NodeType>::DeleteFromTail()
{
	NodeType el = Tail->Info;
	if(Head == Tail)
	{
		delete Tail;
		Head = Tail = nullptr;
	}
	else
	{
		Tail = Tail->Prev;
		delete Tail->Next;
		Tail->Next = nullptr;
	}

	return el;
}
