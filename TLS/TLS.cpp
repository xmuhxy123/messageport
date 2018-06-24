// TLS.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include <stdio.h>
//#include <windows.h>
//#include <process.h>
//
//// 利用TLS记录线程的运行时间
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
//	// 初始化开始时间
//	InitStartTime();
//
//	// 模拟长时间工作
//	i = 10000 * 10000;
//	while (i--) {}
//
//	// 打印出本线程运行的时间
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
//	// 通过在进程位数组中申请一个索引，初始化线程运行时间记录系统
//	g_tlsUsedTime = ::TlsAlloc();
//
//	// 令十个线程同时运行，并等待它们各自的输出结果
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
//	// 通过释放线程局部存储索引，释放时间记录系统占用的资源
//	::TlsFree(g_tlsUsedTime);
//	return 0;
//}
//
//// 初始化线程的开始时间
//void InitStartTime()
//{
//	// 获得当前时间，将线程的创建时间与线程对象相关联
//	DWORD dwStart = ::GetTickCount();
//	::TlsSetValue(g_tlsUsedTime, (LPVOID)dwStart);
//}
//
//// 取得一个线程已经运行的时间
//DWORD GetUsedTime()
//{
//	// 获得当前时间，返回当前时间和线程创建时间的差值
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
	    //TlsSetValue(tlsIndex, PVOID(begin));  // 利用TlsSetValue 设置 值
	
	     printf("\nbegin thread: %d, clock_t: %d", GetCurrentThreadId(), begin);
	
	     Sleep(3000);
	
	     clock_t end = clock();
		 clock_t diff = end - begin;//reinterpret_cast<clock_t>(TlsGetValue(tlsIndex));
	    double sec = 1.0 * diff / CLOCKS_PER_SEC; // 利用TlsGetValue取得值
	
	     printf("\nend thread: %d, begin: %d clock: %d, live time %f", GetCurrentThreadId(), begin, end, sec);
	
	     return 0;
	}

int main(int argc, char *argv[])
{	  	     // 分配Tls索引
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