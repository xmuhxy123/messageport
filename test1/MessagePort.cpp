#include "stdafx.h"
#include "MessagePort.h"

#ifdef WIN32 
#define LOCKOBJ CGuard xLock(m_xCtrl)
#define UNLOCKOBJ xLock.UnLock()
#endif
CMessagePort::CMessagePort(int nPort)
	: m_id(nPort)
	, m_nRecycleLimitNum(1)
{
	m_debugPacket = 0;
	m_nSetMsgCount = 0;
	m_nState = STATE_CLOSED;
#ifdef WIN32 
	 m_hHaveMsg = NULL; 
#endif
}


CMessagePort::~CMessagePort()
{
	Clear();
}

void CMessagePort::Clear()
{
	LOCKOBJ;

	for ()
	{
	}



}


bool CMessagePort::PushMsg(int nPort, int nPacket, unsigned nVarType, const void *pBuf)
{
	// ...
	CMessagePacketPtr pMsg = NULL;

	if (m_dequeMsgRecycle.size() > 0)
	{
		pMsg = *m_dequeMsgRecycle.begin();
		m_dequeMsgRecycle.pop_front();
	}

	if (!pMsg)
		new CMessagePacket();

	// pMsg消息赋值等处理 略...

	m_dequeMsg.push_back(pMsg);
	// ...
	return true;
}


bool CMessagePort::PopMsg(int nPort, int nPacket, unsigned nVarType, const void *pBuf)
{
	// ...
	MSG_DEQUE::iterator iter = m_dequeMsg.begin();
	CMessagePacketPtr pMsg = *iter;
	if (m_dequeMsgRecycle.size() > m_nRecycleLimitNum)
	{
		m_dequeMsgRecycle.push_back(pMsg);
	}
	else
	{
		delete pMsg;
	}

	m_dequeMsg.erase(iter);

	//....
	return true;
}