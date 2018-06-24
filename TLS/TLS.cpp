// TLS.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
//#include <stdio.h>
//#include <windows.h>
//#include <process.h>
//
//// ����TLS��¼�̵߳�����ʱ��
//
//DWORD g_tlsUsedTime;
//void InitStartTime();
//DWORD GetUsedTime();
//
//
//UINT __stdcall ThreadFunc(LPVOID)
//{
//	int i;
//
//	// ��ʼ����ʼʱ��
//	InitStartTime();
//
//	// ģ�ⳤʱ�乤��
//	i = 10000 * 10000;
//	while (i--) {}
//
//	// ��ӡ�����߳����е�ʱ��
//	printf(" This thread is coming to end. Thread ID: %-5d, Used Time: %d \n",
//		::GetCurrentThreadId(), GetUsedTime());
//	return 0;
//}
//
//int main(int argc, char* argv[])
//{
//	UINT uId;
//	int i;
//	HANDLE h[10];
//
//	// ͨ���ڽ���λ����������һ����������ʼ���߳�����ʱ���¼ϵͳ
//	g_tlsUsedTime = ::TlsAlloc();
//
//	// ��ʮ���߳�ͬʱ���У����ȴ����Ǹ��Ե�������
//	for (i = 0; i < 10; i++)
//	{
//		h[i] = (HANDLE)::_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &uId);
//	}
//	for (i = 0; i < 10; i++)
//	{
//		::WaitForSingleObject(h[i], INFINITE);
//		::CloseHandle(h[i]);
//	}
//
//	// ͨ���ͷ��ֲ߳̾��洢�������ͷ�ʱ���¼ϵͳռ�õ���Դ
//	::TlsFree(g_tlsUsedTime);
//	return 0;
//}
//
//// ��ʼ���̵߳Ŀ�ʼʱ��
//void InitStartTime()
//{
//	// ��õ�ǰʱ�䣬���̵߳Ĵ���ʱ�����̶߳��������
//	DWORD dwStart = ::GetTickCount();
//	::TlsSetValue(g_tlsUsedTime, (LPVOID)dwStart);
//}
//
//// ȡ��һ���߳��Ѿ����е�ʱ��
//DWORD GetUsedTime()
//{
//	// ��õ�ǰʱ�䣬���ص�ǰʱ����̴߳���ʱ��Ĳ�ֵ
//	DWORD dwElapsed = ::GetTickCount();
//	dwElapsed = dwElapsed - (DWORD)::TlsGetValue(g_tlsUsedTime);
//	return dwElapsed;
//}



 #include <windows.h>
 #include <process.h>

 #include <ctime>
 #include <cstdlib>
 #include <vector>
 #include <iostream>

using namespace std;

unsigned int __stdcall threadProc(void *arg)
{
	    //DWORD tlsIndex = reinterpret_cast<DWORD>(arg);
	    clock_t begin = clock();
	    //TlsSetValue(tlsIndex, PVOID(begin));  // ����TlsSetValue ���� ֵ
	
	     printf("\nbegin thread: %d, clock_t: %d", GetCurrentThreadId(), begin);
	
	     Sleep(3000);
	
	     clock_t end = clock();
		 clock_t diff = end - begin;//reinterpret_cast<clock_t>(TlsGetValue(tlsIndex));
	    double sec = 1.0 * diff / CLOCKS_PER_SEC; // ����TlsGetValueȡ��ֵ
	
	     printf("\nend thread: %d, begin: %d clock: %d, live time %f", GetCurrentThreadId(), begin, end, sec);
	
	     return 0;
	}

int main(int argc, char *argv[])
{	  	     // ����Tls����
	//DWORD tlsIndex = TlsAlloc();

	vector<HANDLE> threads;
	for (int i = 0; i < 50; ++i) 
	{
		HANDLE h = (HANDLE)_beginthreadex(NULL, 0, threadProc, (void*)0, 0, NULL);
		threads.push_back(h);

	}

	for (size_t i = 0; i < threads.size(); ++i) 
	{
		WaitForSingleObject(threads[i], INFINITE);
		CloseHandle(threads[i]);

	}
	return 0;
}