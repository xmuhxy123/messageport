#pragma once

#if defined(WIN32)||defined(WIN64)
#include <windows.h>
#else
#include <pthread.h>
#endif

//#include "CommonType.h"
//#include "noncopyable.h"

namespace Base
{
	class  IMutex 
	{
	public:
		virtual ~IMutex() { }
		
		virtual void Lock() = 0;
		virtual bool TryLock() = 0;
		virtual void Unlock() = 0;
	};

	class  CMutex :
		public IMutex
	{
	private:
#if defined(WIN32)||defined(WIN64)
		CRITICAL_SECTION	m_mutex;
#else
		pthread_mutex_t		m_mutex;
#endif

	public:
		CMutex();
		~CMutex();

		virtual void Lock();
		virtual bool TryLock();
		virtual void Unlock();
	};

	class  CEmptyMutex :
		public IMutex
	{
	public:
		CEmptyMutex();
		~CEmptyMutex();

		virtual void Lock();
		virtual bool TryLock();
		virtual void Unlock();
	};

	class  CGuard
	{
	private:
		IMutex& m_lock;

	public:
		CGuard( IMutex& lock );
		~CGuard();
		void UnLock();
	};
}