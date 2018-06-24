// test1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include <deque>
#include <vector>
using namespace std;

//#	define	IF_NOT(Exp) if( !(Exp) ? ( (std::cout<<"★IF_NOT(%s)★ in %s, %d", #Exp, __FILE__, __LINE__),1 ) : 0 )
//#	define	IF_OK(Exp)  if( (Exp) ? 1 : ( (std::cout<<"★IF_OK(%s)★ in %s, %d", #Exp, __FILE__, __LINE__),0 ) )
//
//void GetAllId(int arrarId[4])
//{
//	for (int i  = 0; i < 3; i++)
//		arrarId[i] = i;
//}
//
//int total = 10;
//int Reduce() { return --total;}

//class MyClass
//{
//public:
//	MyClass();
//	~MyClass();
//
//	MyClass GetPort(int n) { return s_vetPort[n]; }
//
//private:
//	static std::vector<MyClass> s_vetPort;
//
//};
//
//MyClass::MyClass()
//{
//}
//
//MyClass::~MyClass()
//{
//}


struct SDbCommand
{
	std::vector<char>	m_vecCommandBuf;
	unsigned			m_nCommandSize;
	//uint32				m_nHandlerID;
	//uint64				m_nContext;
	//SDbResultHolder*	m_pDbResult;

	SDbCommand()
	{
		//this->m_pDbResult = new SDbResultHolder();
		//this->m_nHandlerID = 0;
		this->m_nCommandSize = 0;
	}

	~SDbCommand()
	{
		//SAFE_DELETE(this->m_pDbResult);
	}

	void Write(const void* pBuf, unsigned nSize)
	{
		unsigned nOldPos = this->m_vecCommandBuf.size();
		this->m_nCommandSize += nSize;
		if (this->m_vecCommandBuf.size() < this->m_nCommandSize)
			this->m_vecCommandBuf.resize(this->m_nCommandSize);
		memcpy(&this->m_vecCommandBuf[nOldPos], pBuf, nSize);
	}
};
#pragma pack(push,1)
struct TestMsg
{
	int nData1;
	bool bData2;
	float fData3;
};
#pragma pack(pop)
void OnDbCommand(void* pCommand, int nSize)
{
	TestMsg* pMsgHead = (TestMsg*)pCommand;
}

int _tmain(int argc, _TCHAR* argv[])
{

	int n = 1; 
	if (n != 1 && n != 2)
		return 0;



	char ucData;
	bool bIs = false;
	ucData = bIs;
	ucData = true;
	if (ucData)
		cout << "ucData is : %d" <<ucData << endl;
	else 
		cout << ucData << endl;



	SDbCommand sData;
	//string s1("hello ");
	//string s2("world!");
	//sData.Write(s1.c_str(), s1.size());
	//sData.Write(s2.c_str(), s2.size());
	
	TestMsg msg;
	msg.nData1 = 1;
	msg.bData2 = true;
	msg.fData3 = 2.0;
	sData.Write(&sData, sizeof(msg));
	TestMsg* pMsgHead = (TestMsg*)&sData.m_vecCommandBuf[0];
	OnDbCommand(&sData.m_vecCommandBuf[0], sData.m_nCommandSize);
		
	system("PAUSE");

	//int n = Reduce();

	//int l = 0;
	//int k = !(l) ? (std::cout << "★IF_NOT(%s)★ in %s, %d", 3) : 0;
	//if (!(n) ? (std::cout << "★IF_NOT(%s)★ in %s, %d", 1) : 0)
	//	n = 0;


	//IF_NOT(0)
	//	n = 0;

	//IF_OK(0)
	//	n = 1;
	//int arrayId[3] = { 1 };
	//GetAllId(arrayId);



	//默认构造函数 创建一个空的deque
	//deque<int> c;

	////拷贝构造
	//deque<int> c1(c);

	////赋值拷贝
	//deque<int> c2 = c1;

	////指定元素个数创建
	//deque<int> c3(5, 6);
	//for (auto i : c3) {
	//	cout << i << ",";
	//}
	//cout << "deque(个数, 元素)" << endl;

	////指定区间创建
	//deque<int> c4(c3.begin() + 2, c3.begin() + 3);
	//for (auto i : c4) {
	//	cout << i << ",";
	//}
	//cout << "deque(区间, 区间)" << endl;

	////指定初始化列表创建
	//deque<int> c5({ 2,3,4,5 });
	//for (auto i : c5) {
	//	cout << i << ",";
	//}
	//cout << "deque({})" << endl;

	////指定初始化列表创建
	//deque<int> c6 = { 2,3,4,5 };
	//for (auto i : c6) {
	//	cout << i << ",";
	//}
	//cout << "deque = {}" << endl;
	//cout << endl;

	return 0;
}

