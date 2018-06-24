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
	// MSGTYPE=[0-59999]  û��һ���ṹ�ߴ��������ֵ ��С�ڸ�ֵ��ʾ�ṹ
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
	virtual int GetId() = 0;
	virtual int GetMsgCount() = 0;
	virtual int GetSize() = 0;
	virtual int IsOpen() = 0;

	virtual void Clear() = 0;
	virtual bool Close() = 0;
	virtual bool Open() = 0;
	virtual void SetRecycleLimitNum(int nLimitNum) = 0;

	virtual bool Recv(int Port, int nPacket, VAR_TYPE nVarType, char* pBuf, CMessageStatusPrt pStatus) = 0;
	virtual bool  Send(int Port, int nPacket, VAR_TYPE nVarType, char* pBuf) = 0;
	virtual bool  WaitMsg(int nMiliSec) = 0;

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
	int m_nSetMsgCount;  // ��Ϣ����
	int m_id;  // PORTID
	int m_nState;
	int m_nRecycleLimitNum; //ѭ����


// #ifdef WIN32 
	HANDLE m_hHaveMsg; // ���¼�ʱΪ��֪ͨ
	CMutex m_xCtrl; // �ٽ�������m_setMsg����
//#else
//	
//#endif

	static PORT_SET s_setPort;  // ��̬ȫ��

	MSG_DEQUE m_dequeMsg;
	MSG_DEQUE m_dequeMsgRecycle;  // ������

	static PORT_SET s_setPort; 

private:
	bool Init();
	bool PushMsg(int nPort, int nPacket, unsigned nVarType, const void *pBuf);
	bool PopMsg(int nPort, int nPacket, unsigned nVarType, const void *pBuf);

public:
	virtual int GetId() const;
	virtual int GetMsgCount() const;
	virtual int GetSize() const;
	virtual int IsOpen() const;

	virtual void Clear();
	virtual bool Close();
	virtual bool Open();
	virtual void SetRecycleLimitNum(int nLimitNum);

	virtual bool Recv(int Port, int nPacket, VAR_TYPE nVarType, char* pBuf, CMessageStatusPrt pStatus);
	virtual bool Send(int Port, int nPacket, VAR_TYPE nVarType, char* pBuf);
	virtual bool WaitMsg(int nMiliSec);


public:
	static bool InitPortSet(int nPortNum);
	static CMessagePortPtr GetIntreFase(int nPort);

};
