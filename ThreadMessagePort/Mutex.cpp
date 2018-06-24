#include "stdafx.h"
#include "Mutex.h"

namespace Base
{
	CMutex::CMutex()
	{
#if defined(WIN32)||defined(WIN64)
		::InitializeCriticalSection( &this->m_mutex );
#else
		pthread_mutexattr_t attr;
		pthread_mutexattr_init( &attr );
		pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
		pthread_mutex_init( &this->m_mutex, &attr );
#endif
	}

	CMutex::~CMutex()
	{
#if defined(WIN32)||defined(WIN64)
		::DeleteCriticalSection( &this->m_mutex );
#else
		pthread_mutex_destroy( &this->m_mutex );
#endif
	}

	void CMutex::Lock()
	{
#if defined(WIN32)||defined(WIN64)
		::EnterCriticalSection( &this->m_mutex );
#else
		pthread_mutex_lock( &this->m_mutex );
#endif
	}

	bool CMutex::TryLock()
	{
#if defined(WIN32)||defined(WIN64)
		return !!TryEnterCriticalSection( &this->m_mutex );
#else
		return pthread_mutex_trylock( &this->m_mutex ) == 0;
#endif
	}

	void CMutex::Unlock()
	{
#if defined(WIN32)||defined(WIN64)
		::LeaveCriticalSection( &this->m_mutex );
#else
		pthread_mutex_unlock( &this->m_mutex );
#endif
	}

	CEmptyMutex::CEmptyMutex()
	{

	}

	CEmptyMutex::~CEmptyMutex()
	{

	}

	void CEmptyMutex::Lock()
	{

	}

	bool CEmptyMutex::TryLock()
	{
		return true;
	}

	void CEmptyMutex::Unlock()
	{

	}

	CGuard::CGuard( IMutex& lock ) : m_lock( lock )
	{
		this->m_lock.Lock();
	}

	CGuard::~CGuard()
	{
		this->m_lock.Unlock();
	}

	void CGuard::UnLock() { this->m_lock.Unlock(); }
}