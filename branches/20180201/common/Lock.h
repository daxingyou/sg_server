#pragma once

#include "Noncopyable.h"

#ifdef _MSC_VER
class Lock
{

public:

	/**
	* \brief ���캯��������һ���ٽ�������
	*
	*/
	Lock() 
	{
		InitializeCriticalSection(&m_critical);
	}

	/**
	* \brief ��������������һ���ٽ�������
	*
	*/
	~Lock()
	{
		DeleteCriticalSection(&m_critical);
	}

	/**
	* \brief ����һ���ٽ���
	*
	*/
	inline void lock(  )
	{
		EnterCriticalSection(&m_critical);
	}

	/**
	* \brief ����һ���ٽ���
	*
	*/
	inline void unlock(  )
	{
		LeaveCriticalSection(&m_critical);
	}

private:
	CRITICAL_SECTION    m_critical; // ϵͳ�ٽ���
	
};


/**
* \brief ������Thread
*
* 
*/
/**
* \brief �ٽ�������װ��ϵͳ�ٽ�����������ʹ��ϵͳ�ٽ���ʱ����Ҫ�ֹ���ʼ���������ٽ�������Ĳ���
*
*/
class Mutex : private Noncopyable
{

	friend class Cond;

public:

//	/**
//	* \brief ���캯��������һ���ٽ�������
//	*
//	*/
//	Mutex() 
//	{
//		InitializeCriticalSection(&m_critical);
//	}
//
//	/**
//	* \brief ��������������һ���ٽ�������
//	*
//	*/
//	~Mutex()
//	{
//		DeleteCriticalSection(&m_critical);
//	}
//
//	/**
//	* \brief ����һ���ٽ���
//	*
//	*/
//	inline void lock()
//	{
////		Dawn::logger->debug("Locking - %0.8X - %s(%u)", (uint32_t)this, file,line );
//		EnterCriticalSection(&m_critical);
////		Dawn::logger->debug("Locked - %0.8X - %s(%u)", (uint32_t)this, file,line );
//	}
//
//	/**
//	* \brief ����һ���ٽ���
//	*
//	*/
//	inline void unlock()
//	{
////		Dawn::logger->debug("UnLock - %0.8X - %s(%u)", (uint32_t)this, file,line );
//		LeaveCriticalSection(&m_critical);
//	}
//
//private:
//
//	CRITICAL_SECTION    m_critical; // ϵͳ�ٽ���
	/**
	* \brief ���캯��������һ�����������
	*
	*/
	Mutex() 
	{
		m_hMutex = CreateMutex(NULL,FALSE,NULL);
	}

	/**
	* \brief ��������������һ�����������
	*
	*/
	~Mutex()
	{
		CloseHandle(m_hMutex);
	}

	/**
	* \brief ����һ��������
	*
	*/
	inline void lock()
	{
		if( WaitForSingleObject(m_hMutex,10000) == WAIT_TIMEOUT )
		{
			char szName[MAX_PATH];
			GetModuleFileName(NULL,szName,sizeof(szName));
			::MessageBox(NULL,"����������", szName, MB_ICONERROR);
		}
	}

	/**
	* \brief ����һ��������
	*
	*/
	inline void unlock()
	{
		ReleaseMutex(m_hMutex);
	}

private:

	HANDLE m_hMutex;    /**< ϵͳ������ */

};


/**
* \brief ��װ��ϵͳ����������ʹ����Ҫ�򵥣�ʡȥ���ֹ���ʼ��������ϵͳ���������Ĺ�������Щ�����������ɹ��캯���������������Զ����
*
*/
class Cond : private Noncopyable
{

public:

	/**
	* \brief ���캯�������ڴ���һ����������
	*
	*/
	Cond()
	{
		m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	}

	/**
	* \brief ������������������һ����������
	*
	*/
	~Cond()
	{
		CloseHandle(m_hEvent);
	}

	/**
	* \brief �����еȴ���������������̹߳㲥�����źţ�ʹ��Щ�߳��ܹ���������ִ��
	*
	*/
	void broadcast()
	{
		SetEvent(m_hEvent);
	}

	/**
	* \brief �����еȴ���������������̷߳����źţ�ʹ��Щ�߳��ܹ���������ִ��
	*
	*/
	void signal()
	{
		SetEvent(m_hEvent);
	}

	/**
	* \brief �ȴ��ض���������������
	*
	*
	* \param m_hMutex ��Ҫ�ȴ��Ļ�����
	*/
	void wait(Mutex &mutex)
	{
		WaitForSingleObject(m_hEvent,INFINITE);
	}

private:

	HANDLE m_hEvent;    /**< ϵͳ�������� */

};

/**
* \brief ��װ��ϵͳ��д����ʹ����Ҫ�򵥣�ʡȥ���ֹ���ʼ��������ϵͳ��д���Ĺ�������Щ�����������ɹ��캯���������������Զ����
*
*/
class RWLock : private Noncopyable
{

public:
	/**
	* \brief ���캯�������ڴ���һ����д��
	*
	*/
	RWLock()
	{
		m_hMutex = CreateMutex(NULL,FALSE,NULL);
	}

	/**
	* \brief ������������������һ����д��
	*
	*/
	~RWLock()
	{
		CloseHandle(m_hMutex);
	}

	/**
	* \brief �Զ�д�����ж���������
	*
	*/
	inline void rdlock()
	{
		WaitForSingleObject(m_hMutex,INFINITE);
	};

	/**
	* \brief �Զ�д������д��������
	*
	*/
	inline void wrlock()
	{
		WaitForSingleObject(m_hMutex,INFINITE);
	}

	/**
	* \brief �Զ�д�����н�������
	*
	*/
	inline void unlock()
	{
		ReleaseMutex(m_hMutex);
	}

private:

	HANDLE m_hMutex;    /**< ϵͳ��д�� */

};


/**
* \brief rdlock Wrapper
* �����ڸ��Ӻ����ж�д����ʹ��
*/
class RWLock_scope_rdlock : private Noncopyable
{

public:

	/**
	* \brief ���캯��
	* ��������rdlock����
	* \param m ��������
	*/
	RWLock_scope_rdlock(RWLock &m) : rwlock(m)
	{
		rwlock.rdlock();
	}

	/**
	* \brief ��������
	* ��������unlock����
	*/
	~RWLock_scope_rdlock()
	{
		rwlock.unlock();
	}

private:

	/**
	* \brief ��������
	*/
	RWLock &rwlock;

};

/**
* \brief wrlock Wrapper
* �����ڸ��Ӻ����ж�д����ʹ��
*/
class RWLock_scope_wrlock : private Noncopyable
{

public:

	/**
	* \brief ���캯��
	* ��������wrlock����
	* \param m ��������
	*/
	RWLock_scope_wrlock(RWLock &m) : rwlock(m)
	{
		rwlock.wrlock();
	}

	/**
	* \brief ��������
	* ��������unlock����
	*/
	~RWLock_scope_wrlock()
	{
		rwlock.unlock();
	}

private:

	/**
	* \brief ��������
	*/
	RWLock &rwlock;

};



/// @brief ģ���д��
template < bool lock=true >
class RWLocker
{
public:
	void rdlock(){ rwlock.rdlock(); }
	void wrlock(){ rwlock.wrlock(); }
	void unlock(){ rwlock.unlock(); }

private:
	RWLock rwlock;

};

template<>
class RWLocker<false>
{
public:
	void rdlock(){}
	void wrlock(){}
	void unlock(){}	
};

#else
	
#include <pthread.h>
#include <iostream>
#include <sys/time.h>

/**
 * \brief �����壬��װ��ϵͳ�����壬������ʹ��ϵͳ������ʱ����Ҫ�ֹ���ʼ�������ٻ��������Ĳ���
 *
 */
class Mutex : private Noncopyable
{

	friend class Cond;

	public:

		/**
		 * \brief ���캯��������һ�����������
		 *
		 */
		Mutex(int kind = PTHREAD_MUTEX_FAST_NP) 
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			pthread_mutexattr_t attr;
			::pthread_mutexattr_init(&attr);
			::pthread_mutexattr_settype(&attr, kind);
			::pthread_mutex_init(&mutex, &attr);
		}

		/**
		 * \brief ��������������һ�����������
		 *
		 */
		~Mutex()
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_mutex_destroy(&mutex);
		}

		/**
		 * \brief ����һ��������
		 *
		 */
		inline void lock()
		{
			::pthread_mutex_lock(&mutex);
		}

		/**
		 * \brief ����һ��������
		 *
		 */
		inline void unlock()
		{
			::pthread_mutex_unlock(&mutex);
		}

	public:

		pthread_mutex_t mutex;		/**< ϵͳ������ */

};

 /// @brief	��װ��ϵͳ����������ʹ����Ҫ��
 // 			ʡȥ���ֹ���ʼ��������ϵͳ���������Ĺ���
 // 			��Щ�����������ɹ��캯���������������Զ����
class Cond : private Noncopyable
{

	public:

		 /// @brief	���캯�������ڴ���һ����������
		Cond()
		{
			::pthread_cond_init(&cond, NULL);
		}

		 /// @brief	������������������һ����������
		~Cond()
		{
			::pthread_cond_destroy(&cond);
		}

		 /// @brief	�����еȴ���������������̹߳㲥�����źţ�ʹ��Щ�߳��ܹ���������ִ��
		void broadcast()
		{
			::pthread_cond_broadcast(&cond);
		}

		 /// @brief	�����еȴ���������������̷߳����źţ�����ֻ�������е�һ���߳�
		void signal()
		{
			::pthread_cond_signal(&cond);
		}

		 /// @brief	�ȴ��ض���������������
		 /// @param 	mutex ��Ҫ�ȴ��Ļ�����
		void wait(Mutex &mutex)
		{
			::pthread_cond_wait(&cond, &mutex.mutex);
		}

		 /// @brief   �����ó�ʱʱ��ĵȴ�
		 /// @param 	mutex ��Ҫ�ȴ��Ļ�����
		 //			timeout ��ʱʱ��(��λ������)
		 /// @return	true �ȴ��¼�����
		 //			false �ȴ���ʱ
		bool timedwait(Mutex &mutex, unsigned int timeout)
		{
			struct timeval now; // ��ȷ��΢��
			::gettimeofday( &now, NULL );
			
			struct timespec tsp; // ��ȷ������
			tsp.tv_sec = now.tv_sec + timeout / 1000;
			tsp.tv_nsec = now.tv_usec * 1000 + ( timeout % 1000 ) * 1000 * 1000;

			if ( 0 == ::pthread_cond_timedwait(&cond, &mutex.mutex, &tsp) )
				return true;
			else
				return false;
		}

	private:

		 /// @brief	ϵͳ��������
		pthread_cond_t cond;

};


 /// @brief	��װ��ϵͳ��д����ʹ����Ҫ��
 // 			ʡȥ���ֹ���ʼ��������ϵͳ��д���Ĺ���
 // 			��Щ�����������ɹ��캯���������������Զ����
class RWLock : private Noncopyable
{

	public:

		 /// @brief	���캯�������ڴ���һ����д��
		RWLock() { ::pthread_rwlock_init(&rwlock, NULL); }

		 /// @brief	������������������һ����д��
		~RWLock() { ::pthread_rwlock_destroy(&rwlock); }

		 /// @brief	�Զ�д�����ж���������
		void rdlock() { ::pthread_rwlock_rdlock(&rwlock); } 

		 /// @brief	�Զ�д������д��������
		void wrlock() { ::pthread_rwlock_wrlock(&rwlock); }

		 /// @brief	�Զ�д�����н�������
		void unlock() { ::pthread_rwlock_unlock(&rwlock); } 

	private:

		 /// @brief	ϵͳ��д��
		pthread_rwlock_t rwlock;

};

 /// @brief	����һ������,ʵ��lock��unlock����
template <bool lock=true>
class RWLocker
{
	private:
		 /// @brief	��д��ʵ��
		RWLock rwlock;
	public:
		 /// @brief	��
		void rdlock() { rwlock.rdlock(); }
		 /// @brief	��
		void wrlock() { rwlock.wrlock(); }
		 /// @brief	����
		void unlock() { rwlock.unlock(); }
};

 /// @brief	�ػ�����,ʵ�����Ŀպ���,Ҳ���ǲ�����
template <>
class RWLocker<false>
{
	public:
		 /// @brief	��
		void rdlock() { }
		 /// @brief	��
		void wrlock() { }
		 /// @brief	����
		void unlock() { }
};

 /// @brief	rdlock Wrapper
 // �����ڸ��Ӻ����ж�д����ʹ��
class RWLock_scope_rdlock : private Noncopyable
{

	public:

		 /// @brief	���캯��
		 // 		��������rdlock����
		 /// @param m ��������
		RWLock_scope_rdlock(RWLock &m) : rwlock(m) { rwlock.rdlock(); }

		 /// @brief	��������
		 // 		��������unlock����
		~RWLock_scope_rdlock() { rwlock.unlock(); }

	private:

		 /// @brief	��������
		RWLock &rwlock;

};

 /// @brief	wrlock Wrapper
 // �����ڸ��Ӻ����ж�д����ʹ��
class RWLock_scope_wrlock : private Noncopyable
{

	public:

		 /// @brief	���캯��
		 // 		��������wrlock����
		 /// @param m ��������
		RWLock_scope_wrlock(RWLock &m) : rwlock(m) { rwlock.wrlock(); }

		 /// @brief	��������
		 // 		��������unlock����
		~RWLock_scope_wrlock() { rwlock.unlock(); }

	private:

		 /// @brief	��������
		RWLock &rwlock;

};

#endif
/**
 * \brief Wrapper
 * �����ڸ��Ӻ���������ʹ��
 */
class Mutex_scope_lock : private Noncopyable
{

	public:

		/**
		 * \brief ���캯��
		 * ��������lock����
		 * \param m ��������
		 */
		Mutex_scope_lock(Mutex &m) : mlock(m)
		{
			mlock.lock();
		}

		/**
		 * \brief ��������
		 * ��������unlock����
		 */
		~Mutex_scope_lock()
		{
			mlock.unlock();
		}

	private:

		/**
		 * \brief ��������
		 */
		Mutex &mlock;

};
