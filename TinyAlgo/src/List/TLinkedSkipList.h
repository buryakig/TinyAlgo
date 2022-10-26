#pragma once
#include "TLinkedListNode.h"

const int maxLevel = 4;

template<class NodeType>
class TLinkedSkipList
{
public:
	TLinkedSkipList();

	inline bool IsEmpty() const;
	inline void AddToList(const NodeType& el);
	inline NodeType DeleteFromList();
	inline NodeType* SearchInList(const NodeType& el) const;

	inline void ChoosePowers();
	inline int ChooseLevel();

private:
	typedef TLinkedSkipListNode<NodeType>* skipNodePtr;

	skipNodePtr Root[maxLevel];
	int Powers[maxLevel];
};

template <class NodeType>
inline TLinkedSkipList<NodeType>::TLinkedSkipList()
{
	for (int i = 0; i < maxLevel; ++i)
		Root[i] = nullptr;
}

template <class NodeType>
inline bool TLinkedSkipList<NodeType>::IsEmpty() const
{
	return Root[0] == nullptr;
}

template <class NodeType>
inline void TLinkedSkipList<NodeType>::AddToList(const NodeType& el)
{
	skipNodePtr curr[maxLevel], prev[maxLevel];
	int lvl;

	curr[maxLevel - 1] = Root[maxLevel - 1];
	prev[maxLevel - 1] = nullptr;

	for(lvl = maxLevel - 1; lvl >=0; --lvl)
	{
		while(curr[lvl] && curr[lvl]->Info < el)
		{
			prev[lvl] = curr[lvl];
			curr[lvl] = *(curr[lvl]->next + lvl);
		}

		if (curr[lvl] && curr[lvl]->Info == el) 	 // don’t include
			return;

		if(lvl > 0)
		{
			if (prev[lvl] == 0) 
			{ 
				curr[lvl - 1] = Root[lvl - 1];	
				prev[lvl - 1] = 0; 
			}
			else 
			{
				curr[lvl - 1] = *(prev[lvl]->next + lvl - 1);
				prev[lvl - 1] = prev[lvl];
			}
		}
	}

	lvl = ChooseLevel();

	skipNodePtr newNode = new TLinkedSkipListNode<NodeType>;
	newNode->next = new skipNodePtr[sizeof(skipNodePtr) * (lvl + 1)];
	newNode->Info = el;
	int i = 0;
	for (; i <= lvl; ++i) 
	{ 
		*(newNode->next + i) = curr[i]; 
		if (prev[i] == 0) 
			Root[i] = newNode; 
		else *(prev[i]->next + i) = newNode; 
	}
}

template <class NodeType>
inline NodeType TLinkedSkipList<NodeType>::DeleteFromList()
{
	return NodeType();
}

template <class NodeType>
inline NodeType* TLinkedSkipList<NodeType>::SearchInList(const NodeType& el) const
{
	if (IsEmpty()) return nullptr;

	skipNodePtr prev, curr;
	int lvl;	// stores the highest non-null
	for (lvl = maxLevel - 1; lvl >= 0 && !Root[lvl]; --lvl);
	prev = curr = Root[lvl];

	while (true)
	{
		if (el == curr->Info)
			return &curr->Info;
		else if (el < curr->Info)
		{
			if (lvl == 0)
				return nullptr;
			else if (curr == Root[lvl])
				curr = Root[--lvl];
			else
				curr = *(prev->next + --lvl);
		}
		else
		{
			prev = curr;
			if (*(curr->next + lvl) != nullptr)
				curr = *(curr->next + lvl);
			else
			{
				for (--lvl; lvl >= 0 && *(curr->next + lvl) == nullptr; --lvl);
				if (lvl >= 0)
					curr = *(curr->next + lvl);
				else
					return nullptr;
			}
		}
	}
}

template <class NodeType>
inline void TLinkedSkipList<NodeType>::ChoosePowers()
{
	Powers[maxLevel - 1] = (1 << maxLevel) - 1;
	for (int i = maxLevel - 2, j = 0; i >= 0; --i, ++j)
		Powers[i] = Powers[i + 1] - (2 << j);
}

template <class NodeType>
inline int TLinkedSkipList<NodeType>::ChooseLevel()
{
	int i, r = rand() % Powers[maxLevel - 1] + 1;
	for (i = 1; i < maxLevel; ++i)
		if (r < Powers[i])
			return i - 1;
	return i - 1;
}
