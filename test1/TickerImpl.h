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
			__Time_Size = 0x00010000,         ///< �̶ȳߵĳ��� �󲿷�������������Ӧ���㹻�ˣ����ٻ��ж�ʱ���ȳ���2^16��65536��ms�ģ���ʹ����Ҳû��
			__Time_Mask = 0x0000ffff,          ///< ��ʱʱ������
		};

	private:
		typedef CTinyList<TickerNode_t> TickerList_t;
		TickerList_t			m_listTicker[__Time_Size];         ///< ������ʱ�����ڶ�Ӧ�̶ȵ��б��ϣ� ������������ʱ�����2^16ms�Ķ�ʱ���������ͨ�����ϣ���Ҫ������
		int64					m_nCurTickTime;                 ///< ��ǰ�̶�ʱ��    
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