#pragma once
#include "CommonType.h"

namespace Base
{
	template<class T>
	struct STinyListNode
	{
		STinyListNode* m_pPre;
		STinyListNode* m_pNext;
		T              m_Val;

		STinyListNode();
		~STinyListNode();

		void Remove();
		bool IsHang() const;
	};

	template<class NodeType>
	class CTinyList
	{
	private:
		NodeType m_Head;
		NodeType m_Tail;

	public:
		CTinyList();
		~CTinyList();

		void      PushBack( NodeType* pNode );
		NodeType* GetBack();

		void      PushFront( NodeType* pNode );
		NodeType* GetFront();

		bool      IsEmpty() const;
	};

}

#include "TinyList.inl"