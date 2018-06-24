#pragma once
#include <vector>
#include <deque>
#include<windows.h>
#include <assert.h>
#include "Mutex.h"
using namespace std;
using namespace Base;


enum VAR_TYPE
{
	// MSGTYPE=[0-59999]  没有一个结构尺寸会大于这个值 ，小于该值表示结构
	VAR_TYPE_NONE = 59999 + 1,
	VAR_TYPE_CHAR,
	VAR_TYPE_UCHAR,
	VAR_TYPE_SHORT,
	VAR_TYPE_USHORT,
	VAR_TYPE_INT,
	VAR_TYPE_UINT,
	VAR_TYPE_FLOAT,
	VAR_TYPE_DOUBLE,
};

typedef struct CMessageStatus
{
	int m_nPortFrom;
	int m_nPacket;
	VAR_TYPE m_nVarType;
	int m_nError;
} *CMessageStatusPrt;

class IMessagePort
{
public:

	typedef struct CMessagePacket
	{
		enum { MAX_MSGPACKSIZE = 2048, };
		unsigned m_nPortFrom;
		unsigned m_nPacket;
		unsigned m_nVarType;
		char m_bufData[MAX_MSGPACKSIZE];

	} *CMessagePacketPtr;

	enum { INVALID_PORT = 59999+1,};

	enum
	{
		STATUS_FLAG_OK, 
		STATUS_FLAG_CLOSE,
		STATUS_FLAG_ERROR,
	};

public:
	virtual ~IMessagePort() {};

public:
	virtual int GetId() const = 0;
	virtual int GetMsgCount() const = 0;
	virtual int GetSize() const = 0;
	virtual int IsOpen() const = 0;

	virtual void Clear() = 0;
	virtual bool Close() = 0;
	virtual bool Open() = 0;
	virtual void SetRecycleLimitNum(int nLimitNum) = 0;

	virtual bool Recv(int Port, int nPacket, VAR_TYPE nVarType, char* pBuf, CMessageStatusPrt pStatus) = 0;
	virtual bool Send(int Port, int nPacket, VAR_TYPE nVarType, char* pBuf) = 0;
	virtual bool WaitMsg(int nMiliSec) = 0;

	static int SIZE_OF_TYPE(int type);
};

int IMessagePort::SIZE_OF_TYPE(int type)
{
	assert(0 < type && type <= VAR_TYPE_DOUBLE && type != VAR_TYPE_NONE);
	if (0 < type && type < VAR_TYPE_NONE)
		return type;

	if (type == VAR_TYPE_INT)
		return 4;

	int size_of[] = { 1, 1, 2, 2, 4, 4, 8 };
	int index = type - (VAR_TYPE_NONE + 1);

	if (0 < index && index < static_cast<int> (sizeof(size_of) / sizeof(size_of[0])))
		return size_of[index];

	// log
	return 0;
}

typedef class CMessagePort *CMessagePortPtr;

class CMessagePort : public IMessagePort
{
public:
	CMessagePort(int nPort);
	~CMessagePort();

public:
	enum PORT_STATE { STATE_OK, STATE_CLOSED, };
	typedef std::deque<CMessagePacketPtr> MSG_DEQUE;
	typedef std::vector<CMessagePortPtr> PORT_SET;

private:
	int m_debugPacket;
	int m_nSetMsgCount;  // 消息个数
	int m_id;  // PORTID
	int m_nState;
	int m_nRecycleLimitNum; //循环用


// #ifdef WIN32 
	HANDLE m_hHaveMsg; // 有事件时为有通知
	CMutex m_xCtrl; // 临界区用于m_setMsg互斥
//#else
//	
//#endif

	MSG_DEQUE m_dequeMsg;
	MSG_DEQUE m_dequeMsgRecycle;  // 回收用
	static PORT_SET s_setPort;  // 静态全局
private:
	CMessagePortPtr GetInterFace();
	bool Init();
	bool PushMsg(int nPort, int nPacket, unsigned nVarType, const void *pBuf);
	bool PopMsg(int* pPort, int* pPacket, VAR_TYPE* pVarType, char *pBuf);

public:
	virtual int GetId() const { return m_id; };
	virtual int GetMsgCount() const { return m_nSetMsgCount; };
	virtual int GetSize() const { return static_cast<int> (s_setPort.size()); };;
	virtual int IsOpen() const { return m_nState == STATE_OK; };

	virtual void Clear();
	virtual bool Close();
	virtual bool Open();
	virtual void SetRecycleLimitNum(int nLimitNum);
	virtual bool Recv(int nPort, int nPacket, VAR_TYPE nVarType, char* pBuf, CMessageStatusPrt pStatus);
	virtual bool Send(int nPort, int nPacket, VAR_TYPE nVarType, char* pBuf);
	virtual bool WaitMsg(int nMiliSec);

public:
	static bool InitPortSet(int nPortNum);
	static void ClearPortSet();
	static CMessagePortPtr GetInterFace(int nPort);

};
