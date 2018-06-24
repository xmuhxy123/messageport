#include "LogMgr.h"
#include "DebugHelper.h"
namespace Base
{
	template<class T>
	STinyListNode<T>::STinyListNode()
	{
		this->m_pNext = NULL;
		this->m_pPre  = NULL;
		this->m_Val = T();
	}

	template<class T>
	STinyListNode<T>::~STinyListNode()
	{
		this->Remove();
	}

	template<class T>
	void STinyListNode<T>::Remove()
	{
		if( this->m_pNext != NULL && this->m_pPre != NULL )
		{
			this->m_pPre->m_pNext = this->m_pNext;
			this->m_pNext->m_pPre = this->m_pPre;
		}
		this->m_pPre = NULL;
		this->m_pNext = NULL;
	}

	template<class T>
	bool STinyListNode<T>::IsHang() const
	{
		return this->m_pNext != NULL && this->m_pPre != NULL;
	}

	template<class NodeType>
	CTinyList<NodeType>::CTinyList()
	{
		this->m_Head.m_pNext = &this->m_Tail;
		this->m_Tail.m_pPre  = &this->m_Head;

		this->m_Head.m_pPre  = NULL;
		this->m_Tail.m_pNext = NULL;
	}

	template<class NodeType>
	CTinyList<NodeType>::~CTinyList()
	{
		// nothing
	}

	template<class NodeType>
	void CTinyList<NodeType>::PushBack( NodeType *pNode )
	{
		DebugAst( pNode != NULL );

		if( pNode->IsHang() )
		{
			PrintMsg ( "the node had been link in the list." );
			return;
		}

		pNode->m_pNext = &this->m_Tail;
		pNode->m_pPre  = this->m_Tail.m_pPre;
		this->m_Tail.m_pPre->m_pNext = pNode;
		this->m_Tail.m_pPre = pNode;
	}

	template<class NodeType>
	NodeType* CTinyList<NodeType>::GetBack()
	{
		if( this->IsEmpty() )
		{
			return NULL;
		}

		return this->m_Tail.m_pPre;
	}

	template<class NodeType>
	void CTinyList<NodeType>::PushFront( NodeType *pNode )
	{
		DebugAst( pNode != NULL );

		if( pNode->IsHang() )
		{
			PrintMsg ( "the node had been link in the list." );
			return;
		}

		pNode->m_pPre  = &this->m_Head;
		pNode->m_pNext = this->m_Head.m_pNext;
		this->m_Head.m_pNext->m_pPre = pNode;
		this->m_Head.m_pNext = pNode;
	}

	template<class NodeType>
	NodeType* CTinyList<NodeType>::GetFront()
	{
		if( this->IsEmpty() )
		{
			return NULL;
		}

		return this->m_Head.m_pNext;
	}

	template<class NodeType>
	bool CTinyList<NodeType>::IsEmpty() const
	{
		return this->m_Head.m_pNext == &this->m_Tail;
	}
}