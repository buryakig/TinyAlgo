#pragma once

template<class NodeType>
struct TLinkedListNode
{
	explicit TLinkedListNode()
		: Next{ nullptr }
	{}

	explicit TLinkedListNode(NodeType i, TLinkedListNode* in = nullptr)
		: Next{ in }
		, Info{ i }
	{}

	TLinkedListNode* Next;
	NodeType Info;
};

template<class NodeType>
struct TDoublyLinkedListNode
{
	explicit TDoublyLinkedListNode()
		: Next{ nullptr }
		, Prev{ nullptr }
	{}

	explicit TDoublyLinkedListNode(NodeType i, TDoublyLinkedListNode* n = nullptr, TDoublyLinkedListNode* p = nullptr)
		: Next{ n }
		, Prev {p}
		, Info{ i }
	{}

	void operator++ ()
	{
		this = Next;
	}

	TDoublyLinkedListNode* Next;
	TDoublyLinkedListNode* Prev;
	NodeType Info;
};

template<class NodeType>
struct TLinkedSkipListNode
{
	explicit TLinkedSkipListNode()
	{}
	

	TLinkedSkipListNode** Next;
	NodeType Info;
};