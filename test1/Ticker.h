#pragma once
#include "CommonType.h"
#include "DebugHelper.h"
#include "TinyList.h"
#include "noncopyable.h"
#include <vector>
using namespace std;

namespace Base
{
	class CTicker;
	struct STickInfo
	{
		CTicker* m_pTicker;
	};
	typedef STinyListNode<STickInfo> TickerNode_t;
	class CTickerMgrImpl;
	class __BASE_COMMON_API__ CTicker :
		public noncopyable
	{
	private:
		int64			m_nIntervalTime; ///< 定时器运行的间隔时间
		int64			m_nNextTickTime; ///< 下一次定时器运行时间
		uint64			m_nContext;
		uint16			m_nTickerID;
		CTickerMgrImpl*	m_pTickMgr;
		TickerNode_t*	m_pTickerNode;

		friend class CTickerMgrImpl;

	protected:
		virtual void OnTick( uint64 nContext ) = 0;

	public:
		CTicker();
		virtual ~CTicker();

		int64	GetIntervalTime() const { return this->m_nIntervalTime; }
		int64	GetNextTickTime() const { return this->m_nNextTickTime; }
		int64	GetRemainTime() const;
		bool	IsRegister() const { return this->m_pTickerNode != NULL; }
		void	SetTickerID( uint16 nID ) { this->m_nTickerID = nID; }
		uint64	GetContext() const { return this->m_nContext; }
	};

	class __BASE_COMMON_API__ CTickerMgr
	{
	private:
		CTickerMgrImpl*	m_pTickerMgrImpl;

	public:
		CTickerMgr();
		~CTickerMgr();

		void				Register( CTicker* pTicker, uint64 nIntervalTime, uint64 nContext );
		void				UnRegister( CTicker* pTicker );
		void				Update( uint32 nTime );
		uint64				GetCurTickTime() const;

		static void			SetTickerName( uint16 nTickerID, const char* szTickerName );
		static const char*	GetTickerName( uint16 nTickerID );
	};

	template<class T>
	class TTicker :
		public CTicker
	{
	public:
		typedef void (T::*funTicker)( uint64 nContext );

	private:
		T*        m_pObject;
		funTicker m_pfTicker;

	private:
		TTicker( TTicker<T>& lhs ) { }
		TTicker<T>& operator = ( TTicker<T>& lhs ) { }

	protected:
		virtual void OnTick( uint64 nContext )
		{
			DebugAst( this->m_pfTicker != NULL && this->m_pObject != NULL );
			(this->m_pObject->*this->m_pfTicker)( nContext );
		}

	public:
		TTicker( T* pObject, funTicker pfTicker ) : m_pObject( pObject ), m_pfTicker( pfTicker ) {}
		TTicker() : m_pObject(NULL), m_pfTicker(NULL) { }
		void SetContext( T* pObject, funTicker pfTicker, uint16 nID ) { this->m_pObject = pObject; this->m_pfTicker = pfTicker; this->SetTickerID( nID ); }
	};
}