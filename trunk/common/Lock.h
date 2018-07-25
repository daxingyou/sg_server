#pragma once

#include "Noncopyable.h"

#ifdef _MSC_VER
class Lock
{

public:

	/**
	* \brief 构造函数，构造一个临界区对象
	*
	*/
	Lock() 
	{
		InitializeCriticalSection(&m_critical);
	}

	/**
	* \brief 析构函数，销毁一个临界区对象
	*
	*/
	~Lock()
	{
		DeleteCriticalSection(&m_critical);
	}

	/**
	* \brief 加锁一个临界区
	*
	*/
	inline void lock(  )
	{
		EnterCriticalSection(&m_critical);
	}

	/**
	* \brief 解锁一个临界区
	*
	*/
	inline void unlock(  )
	{
		LeaveCriticalSection(&m_critical);
	}

private:
	CRITICAL_SECTION    m_critical; // 系统临界区
	
};


/**
* \brief 定义类Thread
*
* 
*/
/**
* \brief 临界区，封装了系统临界区，避免了使用系统临界区时候需要手工初始化和销毁临界区对象的操作
*
*/
class Mutex : private Noncopyable
{

	friend class Cond;

public:

//	/**
//	* \brief 构造函数，构造一个临界区对象
//	*
//	*/
//	Mutex() 
//	{
//		InitializeCriticalSection(&m_critical);
//	}
//
//	/**
//	* \brief 析构函数，销毁一个临界区对象
//	*
//	*/
//	~Mutex()
//	{
//		DeleteCriticalSection(&m_critical);
//	}
//
//	/**
//	* \brief 加锁一个临界区
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
//	* \brief 解锁一个临界区
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
//	CRITICAL_SECTION    m_critical; // 系统临界区
	/**
	* \brief 构造函数，构造一个互斥体对象
	*
	*/
	Mutex() 
	{
		m_hMutex = CreateMutex(NULL,FALSE,NULL);
	}

	/**
	* \brief 析构函数，销毁一个互斥体对象
	*
	*/
	~Mutex()
	{
		CloseHandle(m_hMutex);
	}

	/**
	* \brief 加锁一个互斥体
	*
	*/
	inline void lock()
	{
		if( WaitForSingleObject(m_hMutex,10000) == WAIT_TIMEOUT )
		{
			char szName[MAX_PATH];
			GetModuleFileName(NULL,szName,sizeof(szName));
			::MessageBox(NULL,"发生死锁！", szName, MB_ICONERROR);
		}
	}

	/**
	* \brief 解锁一个互斥体
	*
	*/
	inline void unlock()
	{
		ReleaseMutex(m_hMutex);
	}

private:

	HANDLE m_hMutex;    /**< 系统互斥体 */

};


/**
* \brief 封装了系统条件变量，使用上要简单，省去了手工初始化和销毁系统条件变量的工作，这些工作都可以由构造函数和析构函数来自动完成
*
*/
class Cond : private Noncopyable
{

public:

	/**
	* \brief 构造函数，用于创建一个条件变量
	*
	*/
	Cond()
	{
		m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	}

	/**
	* \brief 析构函数，用于销毁一个条件变量
	*
	*/
	~Cond()
	{
		CloseHandle(m_hEvent);
	}

	/**
	* \brief 对所有等待这个条件变量的线程广播发送信号，使这些线程能够继续往下执行
	*
	*/
	void broadcast()
	{
		SetEvent(m_hEvent);
	}

	/**
	* \brief 对所有等待这个条件变量的线程发送信号，使这些线程能够继续往下执行
	*
	*/
	void signal()
	{
		SetEvent(m_hEvent);
	}

	/**
	* \brief 等待特定的条件变量满足
	*
	*
	* \param m_hMutex 需要等待的互斥体
	*/
	void wait(Mutex &mutex)
	{
		WaitForSingleObject(m_hEvent,INFINITE);
	}

private:

	HANDLE m_hEvent;    /**< 系统条件变量 */

};

/**
* \brief 封装了系统读写锁，使用上要简单，省去了手工初始化和销毁系统读写锁的工作，这些工作都可以由构造函数和析构函数来自动完成
*
*/
class RWLock : private Noncopyable
{

public:
	/**
	* \brief 构造函数，用于创建一个读写锁
	*
	*/
	RWLock()
	{
		m_hMutex = CreateMutex(NULL,FALSE,NULL);
	}

	/**
	* \brief 析构函数，用于销毁一个读写锁
	*
	*/
	~RWLock()
	{
		CloseHandle(m_hMutex);
	}

	/**
	* \brief 对读写锁进行读加锁操作
	*
	*/
	inline void rdlock()
	{
		WaitForSingleObject(m_hMutex,INFINITE);
	};

	/**
	* \brief 对读写锁进行写加锁操作
	*
	*/
	inline void wrlock()
	{
		WaitForSingleObject(m_hMutex,INFINITE);
	}

	/**
	* \brief 对读写锁进行解锁操作
	*
	*/
	inline void unlock()
	{
		ReleaseMutex(m_hMutex);
	}

private:

	HANDLE m_hMutex;    /**< 系统读写锁 */

};


/**
* \brief rdlock Wrapper
* 方便在复杂函数中读写锁的使用
*/
class RWLock_scope_rdlock : private Noncopyable
{

public:

	/**
	* \brief 构造函数
	* 对锁进行rdlock操作
	* \param m 锁的引用
	*/
	RWLock_scope_rdlock(RWLock &m) : rwlock(m)
	{
		rwlock.rdlock();
	}

	/**
	* \brief 析购函数
	* 对锁进行unlock操作
	*/
	~RWLock_scope_rdlock()
	{
		rwlock.unlock();
	}

private:

	/**
	* \brief 锁的引用
	*/
	RWLock &rwlock;

};

/**
* \brief wrlock Wrapper
* 方便在复杂函数中读写锁的使用
*/
class RWLock_scope_wrlock : private Noncopyable
{

public:

	/**
	* \brief 构造函数
	* 对锁进行wrlock操作
	* \param m 锁的引用
	*/
	RWLock_scope_wrlock(RWLock &m) : rwlock(m)
	{
		rwlock.wrlock();
	}

	/**
	* \brief 析购函数
	* 对锁进行unlock操作
	*/
	~RWLock_scope_wrlock()
	{
		rwlock.unlock();
	}

private:

	/**
	* \brief 锁的引用
	*/
	RWLock &rwlock;

};



/// @brief 模板读写锁
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
 * \brief 互斥体，封装了系统互斥体，避免了使用系统互斥体时候需要手工初始化和销毁互斥体对象的操作
 *
 */
class Mutex : private Noncopyable
{

	friend class Cond;

	public:

		/**
		 * \brief 构造函数，构造一个互斥体对象
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
		 * \brief 析构函数，销毁一个互斥体对象
		 *
		 */
		~Mutex()
		{
			//std::cout << __PRETTY_FUNCTION__ << std::endl;
			::pthread_mutex_destroy(&mutex);
		}

		/**
		 * \brief 加锁一个互斥体
		 *
		 */
		inline void lock()
		{
			::pthread_mutex_lock(&mutex);
		}

		/**
		 * \brief 解锁一个互斥体
		 *
		 */
		inline void unlock()
		{
			::pthread_mutex_unlock(&mutex);
		}

	public:

		pthread_mutex_t mutex;		/**< 系统互斥体 */

};

 /// @brief	封装了系统条件变量，使用上要简单
 // 			省去了手工初始化和销毁系统条件变量的工作
 // 			这些工作都可以由构造函数和析构函数来自动完成
class Cond : private Noncopyable
{

	public:

		 /// @brief	构造函数，用于创建一个条件变量
		Cond()
		{
			::pthread_cond_init(&cond, NULL);
		}

		 /// @brief	析构函数，用于销毁一个条件变量
		~Cond()
		{
			::pthread_cond_destroy(&cond);
		}

		 /// @brief	对所有等待这个条件变量的线程广播发送信号，使这些线程能够继续往下执行
		void broadcast()
		{
			::pthread_cond_broadcast(&cond);
		}

		 /// @brief	对所有等待这个条件变量的线程发送信号，但是只唤醒其中的一个线程
		void signal()
		{
			::pthread_cond_signal(&cond);
		}

		 /// @brief	等待特定的条件变量满足
		 /// @param 	mutex 需要等待的互斥体
		void wait(Mutex &mutex)
		{
			::pthread_cond_wait(&cond, &mutex.mutex);
		}

		 /// @brief   可设置超时时间的等待
		 /// @param 	mutex 需要等待的互斥体
		 //			timeout 超时时间(单位：毫秒)
		 /// @return	true 等待事件发生
		 //			false 等待超时
		bool timedwait(Mutex &mutex, unsigned int timeout)
		{
			struct timeval now; // 精确到微秒
			::gettimeofday( &now, NULL );
			
			struct timespec tsp; // 精确到纳秒
			tsp.tv_sec = now.tv_sec + timeout / 1000;
			tsp.tv_nsec = now.tv_usec * 1000 + ( timeout % 1000 ) * 1000 * 1000;

			if ( 0 == ::pthread_cond_timedwait(&cond, &mutex.mutex, &tsp) )
				return true;
			else
				return false;
		}

	private:

		 /// @brief	系统条件变量
		pthread_cond_t cond;

};


 /// @brief	封装了系统读写锁，使用上要简单
 // 			省去了手工初始化和销毁系统读写锁的工作
 // 			这些工作都可以由构造函数和析构函数来自动完成
class RWLock : private Noncopyable
{

	public:

		 /// @brief	构造函数，用于创建一个读写锁
		RWLock() { ::pthread_rwlock_init(&rwlock, NULL); }

		 /// @brief	析构函数，用于销毁一个读写锁
		~RWLock() { ::pthread_rwlock_destroy(&rwlock); }

		 /// @brief	对读写锁进行读加锁操作
		void rdlock() { ::pthread_rwlock_rdlock(&rwlock); } 

		 /// @brief	对读写锁进行写加锁操作
		void wrlock() { ::pthread_rwlock_wrlock(&rwlock); }

		 /// @brief	对读写锁进行解锁操作
		void unlock() { ::pthread_rwlock_unlock(&rwlock); } 

	private:

		 /// @brief	系统读写锁
		pthread_rwlock_t rwlock;

};

 /// @brief	定义一个锁类,实现lock和unlock函数
template <bool lock=true>
class RWLocker
{
	private:
		 /// @brief	读写锁实例
		RWLock rwlock;
	public:
		 /// @brief	锁
		void rdlock() { rwlock.rdlock(); }
		 /// @brief	锁
		void wrlock() { rwlock.wrlock(); }
		 /// @brief	解锁
		void unlock() { rwlock.unlock(); }
};

 /// @brief	特化锁类,实现锁的空函数,也就是不加锁
template <>
class RWLocker<false>
{
	public:
		 /// @brief	锁
		void rdlock() { }
		 /// @brief	锁
		void wrlock() { }
		 /// @brief	解锁
		void unlock() { }
};

 /// @brief	rdlock Wrapper
 // 方便在复杂函数中读写锁的使用
class RWLock_scope_rdlock : private Noncopyable
{

	public:

		 /// @brief	构造函数
		 // 		对锁进行rdlock操作
		 /// @param m 锁的引用
		RWLock_scope_rdlock(RWLock &m) : rwlock(m) { rwlock.rdlock(); }

		 /// @brief	析购函数
		 // 		对锁进行unlock操作
		~RWLock_scope_rdlock() { rwlock.unlock(); }

	private:

		 /// @brief	锁的引用
		RWLock &rwlock;

};

 /// @brief	wrlock Wrapper
 // 方便在复杂函数中读写锁的使用
class RWLock_scope_wrlock : private Noncopyable
{

	public:

		 /// @brief	构造函数
		 // 		对锁进行wrlock操作
		 /// @param m 锁的引用
		RWLock_scope_wrlock(RWLock &m) : rwlock(m) { rwlock.wrlock(); }

		 /// @brief	析购函数
		 // 		对锁进行unlock操作
		~RWLock_scope_wrlock() { rwlock.unlock(); }

	private:

		 /// @brief	锁的引用
		RWLock &rwlock;

};

#endif
/**
 * \brief Wrapper
 * 方便在复杂函数中锁的使用
 */
class Mutex_scope_lock : private Noncopyable
{

	public:

		/**
		 * \brief 构造函数
		 * 对锁进行lock操作
		 * \param m 锁的引用
		 */
		Mutex_scope_lock(Mutex &m) : mlock(m)
		{
			mlock.lock();
		}

		/**
		 * \brief 析购函数
		 * 对锁进行unlock操作
		 */
		~Mutex_scope_lock()
		{
			mlock.unlock();
		}

	private:

		/**
		 * \brief 锁的引用
		 */
		Mutex &mlock;

};
