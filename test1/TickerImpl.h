#pragma once
#include "CommonType.h"
#include "DebugHelper.h"
#include "TinyList.h"
#include "noncopyable.h"
#include <vector>
using namespace std;

namespace Base
{

	class CTickerMgrImpl
	{
	private:
		enum
		{
			__Time_Size = 0x00010000,         ///< 刻度尺的长度 大部分情况下这个长度应该足够了，很少会有定时长度超过2^16（65536）ms的，即使存在也没事
			__Time_Mask = 0x0000ffff,          ///< 定时时间掩码
		};

	private:
		typedef CTinyList<TickerNode_t> TickerList_t;
		TickerList_t			m_listTicker[__Time_Size];         ///< 各个定时器挂在对应刻度的列表上， 可能两个触发时间相差2^16ms的定时器会挂在想通链表上，需要做处理
		int64					m_nCurTickTime;                 ///< 当前刻度时间    
		vector<TickerNode_t*>	m_vecTempTickerNode;

		static char*			s_szTickerNameArray[INVALID_16BIT];

	public:
		CTickerMgrImpl();
		~CTickerMgrImpl();

		void				InsertTicker( TickerNode_t* pTickerNode );
		void				Register( CTicker* pTicker, uint64 nIntervalTime, uint64 nContext );
		void				UnRegister( CTicker* pTicker );
		void				Update( uint32 nTime );
		uint64				GetCurTickTime() const { return this->m_nCurTickTime; }

		static void			SetTickerName( uint16 nTickerID, const char* szTickerName );
		static const char*	GetTickerName( uint16 nTickerID );
	};
}