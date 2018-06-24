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

	CTicker::CTicker()
	{
		this->m_nIntervalTime = 0;
		this->m_nNextTickTime = 0;
		this->m_nTickerID = 0;
		this->m_pTickMgr = NULL;
		this->m_pTickerNode = NULL;
	}

	CTicker::~CTicker()
	{
		if( this->IsRegister() )
		{
			if( this->m_pTickMgr != NULL )
				this->m_pTickMgr->UnRegister( this );
		}
	}

	int64 CTicker::GetRemainTime() const
	{
		int64 nRemainTime = this->m_nNextTickTime - GetLocalTime();
		return nRemainTime < 0 ? 0 : nRemainTime;
	}

	CTickerMgr::CTickerMgr()
	{
		this->m_pTickerMgrImpl = new CTickerMgrImpl();
	}

	CTickerMgr::~CTickerMgr()
	{
		SAFE_DELETE( this->m_pTickerMgrImpl );
	}

	void CTickerMgr::Register( CTicker* pTicker, uint64 nIntervalTime, uint64 nContext )
	{
		this->m_pTickerMgrImpl->Register( pTicker, nIntervalTime, nContext );
	}

	void CTickerMgr::UnRegister( CTicker* pTicker )
	{
		this->m_pTickerMgrImpl->UnRegister( pTicker );
	}

	void CTickerMgr::Update( uint32 nTime )
	{
		this->m_pTickerMgrImpl->Update( nTime );
	}

	uint64 CTickerMgr::GetCurTickTime() const
	{
		return this->m_pTickerMgrImpl->GetCurTickTime();
	}
	
	void CTickerMgr::SetTickerName( uint16 nTickerID, const char* szTickerName )
	{
		CTickerMgrImpl::SetTickerName( nTickerID, szTickerName );
	}

	const char* CTickerMgr::GetTickerName( uint16 nTickerID )
	{
		return CTickerMgrImpl::GetTickerName( nTickerID );
	}
}