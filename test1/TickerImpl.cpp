#include "stdafx.h"
#include "Ticker.h"
#include "CommonTime.h"
#include "DebugHelper.h"
#include "LogMgr.h"
#include "ProfilerMgr.h"
#include "TickerImpl.h"

#include <vector>
#include <algorithm>
using namespace std;

namespace Base
{
#define TICK_DEBUG_INFO_SIZE 1024*10

	CTickerMgrImpl::CTickerMgrImpl()
	{
		this->m_nCurTickTime = GetLocalTime();
	}

	CTickerMgrImpl::~CTickerMgrImpl()
	{
		for( uint32 i = 0; i < __Time_Size; ++i )
		{
			TickerList_t& listTicker = this->m_listTicker[i];
			if( !listTicker.IsEmpty() )
			{
				PrintMsg( "Ticker Is Not UnRegister" );
			}
		}
	}

	void CTickerMgrImpl::Register( CTicker* pTicker, uint64 nIntervalTime, uint64 nContext )
	{
		DebugAst( pTicker != NULL );
		if( nIntervalTime== 0 )
		{
			PrintErr( "nIntervalTime is 0 error adjust 1" );
			nIntervalTime = 1;
		}
		DebugAst( !pTicker->IsRegister() );
		pTicker->m_nIntervalTime = nIntervalTime;
		pTicker->m_nNextTickTime = this->m_nCurTickTime + nIntervalTime;
		pTicker->m_nContext = nContext;
		pTicker->m_pTickMgr = this;
		TickerNode_t* pTickerNode = new TickerNode_t();
		pTickerNode->m_Val.m_pTicker = pTicker;
		pTicker->m_pTickerNode = pTickerNode;
		this->InsertTicker( pTickerNode );
	}

	void CTickerMgrImpl::UnRegister( CTicker* pTicker )
	{
		DebugAst( pTicker != NULL );
		if( !pTicker->IsRegister() )
		{
			return;
		}
		if( pTicker->m_pTickerNode->IsHang() )
		{
			pTicker->m_pTickerNode->Remove();
			pTicker->m_pTickerNode->m_Val.m_pTicker = NULL;
			SAFE_DELETE( pTicker->m_pTickerNode );
		}
		else
		{
			pTicker->m_pTickerNode->m_Val.m_pTicker = NULL;
			pTicker->m_pTickerNode = NULL;
		}
	}

	void CTickerMgrImpl::InsertTicker( TickerNode_t* pTickerNode )
	{
		CTicker* pTicker = pTickerNode->m_Val.m_pTicker;
		uint32 nPos = (uint32)(pTicker->m_nNextTickTime&__Time_Mask);
		TickerList_t& listTicker = this->m_listTicker[nPos];
		listTicker.PushBack( pTickerNode );
	}

	void CTickerMgrImpl::Update( uint32 nTime )
	{
		///< 每一次更新都将刻度时间慢慢推进到与当前时间一样
		int64 nEndTime = (int64)nTime + this->m_nCurTickTime;
		for( ; this->m_nCurTickTime < nEndTime; ++this->m_nCurTickTime )
		{
			uint32 nPos = (uint32)(this->m_nCurTickTime&__Time_Mask);
			TickerList_t& listTicker = this->m_listTicker[nPos];
			this->m_vecTempTickerNode.clear();
			while( !listTicker.IsEmpty() )
			{
				TickerNode_t* pTickerNode = listTicker.GetFront();
				pTickerNode->Remove();
				CTicker* pTicker = pTickerNode->m_Val.m_pTicker;
				if( pTicker->m_nNextTickTime <= this->m_nCurTickTime )
				{
					try
					{
#ifdef __PROFILER_OPEN
						const char* szTickerName = CTickerMgr::GetTickerName( pTicker->m_nTickerID );
						CProfilerMgr::Inst()->ProfilerBegin( szTickerName );
#endif
						pTicker->OnTick( pTicker->m_nContext );
#ifdef __PROFILER_OPEN
						CProfilerMgr::Inst()->ProfilerEnd( szTickerName );
#endif
					}
					catch( ... )
					{
						uint32 nTickerID = 0;
						if( pTickerNode->m_Val.m_pTicker != NULL )
						{
							nTickerID = pTickerNode->m_Val.m_pTicker->m_nTickerID;
							pTickerNode->m_Val.m_pTicker->m_pTickerNode = NULL;
						}
						PrintErr( " *** CTickerMgr::Update Crash()! TickerID[%d] *** ", nTickerID );

						SAFE_DELETE( pTickerNode );
						continue;
					}
					if( pTickerNode->m_Val.m_pTicker == NULL )
					{
						SAFE_DELETE( pTickerNode );
						continue;
					}

					pTicker->m_nNextTickTime += pTicker->m_nIntervalTime;
					this->m_vecTempTickerNode.push_back( pTickerNode );
				}
				else
				{
					this->m_vecTempTickerNode.push_back( pTickerNode );
				}
			}
			uint32 nCount = this->m_vecTempTickerNode.size();
			for( uint32 i = 0; i < nCount; ++i )
			{
				TickerNode_t* pTickerNode = this->m_vecTempTickerNode[i];
				///< 有可能后执行的定时器删除了前执行的定时器
				if( pTickerNode->m_Val.m_pTicker == NULL )
				{
					SAFE_DELETE( pTickerNode );
					continue;
				}
				this->InsertTicker( this->m_vecTempTickerNode[i] );
			}
		}
	}
	char* CTickerMgrImpl::s_szTickerNameArray[] = { NULL };

	void CTickerMgrImpl::SetTickerName( uint16 nTickerID, const char* szTickerName )
	{
		s_szTickerNameArray[nTickerID] = const_cast<char*>(szTickerName);
	}

	const char* CTickerMgrImpl::GetTickerName( uint16 nTickerID )
	{
		const char* szTickerName = s_szTickerNameArray[nTickerID];
		if( NULL == szTickerName )
			return "Unknown Ticker";
		return szTickerName;
	}
}