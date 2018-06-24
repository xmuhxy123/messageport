#include "stdafx.h"
#include "MessagePort.h"

#ifdef WIN32 
#define LOCKOBJ CGuard xLock(m_xCtrl)
#define UNLOCKOBJ xLock.UnLock()
#endif


CMessagePort::PORT_SET CMessagePort::s_setPort;

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

bool CMessagePort::Close()
{
	LOCKOBJ;
	m_nState = STATE_CLOSED;
	return true;
}

void CMessagePort::SetRecycleLimitNum(int nLimitNum)
{ 
	if (nLimitNum <= 0) 
		m_nRecycleLimitNum = 1; 
	else 
		m_nRecycleLimitNum = nLimitNum; 
}

void CMessagePort::Clear()
{
	LOCKOBJ;

	for (MSG_DEQUE::iterator iter = m_dequeMsg.begin(); iter != m_dequeMsg.end(); ++iter)
	{
		delete *iter;
	}

	m_dequeMsg.clear();
	m_nSetMsgCount = 0;

	while (!m_dequeMsgRecycle.empty())
	{
		MSG_DEQUE::iterator iter = m_dequeMsgRecycle.begin();
		m_dequeMsgRecycle.pop_front();
		delete *iter;
	}
#ifdef WIN32 
	if (NULL != m_hHaveMsg)
	{
		::CloseHandle(m_hHaveMsg);
		m_hHaveMsg = NULL;
	}
#endif
}


bool CMessagePort::Init()
{
	LOCKOBJ;
#ifdef WIN32 
	if ((m_hHaveMsg = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL)
	{
		// log
		return false;
	}
#endif
	return true;
}

bool CMessagePort::InitPortSet(int nPortNum)
{
	for (int i = 0 ; i  < nPortNum; ++i)
	{
		CMessagePortPtr pPort = new CMessagePort(i);
		if (!pPort)
			return false;

		if (!pPort->Init())
			return false;

		s_setPort.push_back(pPort);
	}

	return true;
}

void CMessagePort::ClearPortSet()
{
	for (PORT_SET::iterator it = s_setPort.begin(); it != s_setPort.end(); ++it)
	{
		delete (*it);
	}

	s_setPort.clear();
}

CMessagePortPtr CMessagePort::GetInterFace()
{
	return this;
}

CMessagePortPtr CMessagePort::GetInterFace(int nPort)
{
	if (0 <= nPort && nPort < static_cast<int>(s_setPort.size()))
		return s_setPort[nPort]->GetInterFace();

	return NULL;
}

bool CMessagePort::Open()
{
	LOCKOBJ;
	m_nState = STATE_OK;
	for (MSG_DEQUE::iterator iter = m_dequeMsg.begin(); iter != m_dequeMsg.end(); ++iter)
	{
		delete *iter;
	}

	m_dequeMsg.clear();
	m_nSetMsgCount = 0;

#ifdef WIN32 
	::ResetEvent(m_hHaveMsg);
#endif
	return true;
}

bool CMessagePort::PopMsg(int* pPort, int* pPacket, VAR_TYPE* pVarType, char *pBuf)
{
	assert(pPort);
	assert(pPacket);
	assert(pVarType);
	assert(pBuf);

	if (STATE_OK != m_nState)
		return false;

	if (m_nSetMsgCount <= 0)
		return false;

	MSG_DEQUE::iterator iter = m_dequeMsg.begin();
	CMessagePacketPtr pMsg = *iter;

	*pPort = pMsg->m_nPortFrom;
	*pPacket = pMsg->m_nPacket;
	*pVarType = (VAR_TYPE)pMsg->m_nVarType;

	int nSize = SIZE_OF_TYPE(*pVarType);
	if (nSize > 0)
		memcpy(pBuf, pMsg->m_bufData, nSize);

	if ((int)m_dequeMsgRecycle.size() > m_nRecycleLimitNum)
		m_dequeMsgRecycle.push_back(pMsg);
	else
		delete pMsg;

	m_dequeMsg.erase(iter);
	--m_nSetMsgCount;

	return true;
}

bool CMessagePort::PushMsg(int nPort, int nPacket, unsigned nVarType, const void *pBuf)
{
	assert(0 <= nPort && nPort < static_cast<int>(s_setPort.size()));
	assert(pBuf);

	LOCKOBJ;

	if (STATE_OK != m_nState)
		return false;

	if ((0 > nPort && nPort >= static_cast<int>(s_setPort.size())))
	{
		// log
		return false;
	}

	int nSize = SIZE_OF_TYPE(nVarType);
	if (nSize <= 0 || nSize > CMessagePacket::MAX_MSGPACKSIZE)
	{
		// LOG
		return false;
	}

	CMessagePacketPtr pMsg = NULL;

	if (m_dequeMsgRecycle.size() > 0)
	{
		pMsg = *m_dequeMsgRecycle.begin();
		m_dequeMsgRecycle.pop_front();
	}

	if (!pMsg)
		new CMessagePacket();

	if (!pMsg)
		return false;

	pMsg->m_nPortFrom = nPort;
	pMsg->m_nPacket = nPacket;
	pMsg->m_nVarType = nVarType;
	memcpy(pMsg->m_bufData, pBuf, nSize);
	m_dequeMsg.push_back(pMsg);
	++m_nSetMsgCount;

#ifdef WIN32
	::SetEvent(m_hHaveMsg);
//#else

#endif

	//if (m_nSetMsgCount >= 5000)
	//{
	//	//log
	//}
	return true;
}

bool CMessagePort::Recv(int nPort, int nPacket, VAR_TYPE nVarType, char* pBuf, CMessageStatusPrt pStatus)
{
	assert(pBuf);
	assert(NULL != pStatus);

	LOCKOBJ;
	if (STATE_CLOSED == m_nState)
	{
		pStatus->m_nError = STATUS_FLAG_CLOSE;
		return false;
	}

	int nRecPort = nPort;
	int nRecPacket = nPacket;
	VAR_TYPE nRcvVarType = VAR_TYPE_NONE;

	if (PopMsg(&nRecPort, &nRecPacket, &nVarType, pBuf))
	{
		if (SIZE_OF_TYPE(nRcvVarType) > SIZE_OF_TYPE(nVarType))
		{
			pStatus->m_nError = STATUS_FLAG_ERROR;
		}

		pStatus->m_nPortFrom = nRecPort;
		pStatus->m_nPacket = nRecPacket;
		pStatus->m_nVarType = nRcvVarType;
		pStatus->m_nError = STATUS_FLAG_OK;
		return true;
	}

	pStatus->m_nError = STATUS_FLAG_ERROR;
	return false;
}

bool CMessagePort::Send(int nPort, int nPacket, VAR_TYPE nVarType, char* pBuf)
{
	assert(0 <= nPort && nPort < static_cast<int>(s_setPort.size()));
	assert(pBuf);

	LOCKOBJ;

	if (!IsOpen())
		return false;

	if (nPort == INVALID_PORT)
		return false;

	if ((0 > nPort && nPort >= static_cast<int>(s_setPort.size())))
	{
		// log
		return false;
	}

	UNLOCKOBJ;
	return s_setPort[nPort]->PushMsg(m_id, nPacket, nVarType, pBuf);
}

bool CMessagePort::WaitMsg(int nMiliSec)
{
	LOCKOBJ;
	if (STATE_CLOSED == m_nState)
		return false;

	if (m_nSetMsgCount > 0)
		return true;

	DWORD ret = ::WaitForSingleObject(m_hHaveMsg, nMiliSec);
	switch (ret)
	{
	case WAIT_OBJECT_0:
		return true;
		break;
	case WAIT_ABANDONED:
		Close();
		break;
	case WAIT_TIMEOUT:
	default:
		break;
	}

	return false;
}
