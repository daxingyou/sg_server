#pragma once 

#include <assert.h>
#include <cstddef>
#include "Noncopyable.h"

template <typename T>
class SingletonFactory
{
  public:
    static T* instance()
    {
      return new T();
    }
};

template <typename T,typename MANA = SingletonFactory<T> > 
class Singleton
{
  private:
    /**
     * \brief 拷贝构造函数，没有实现，禁用掉了
     *
     */
    Singleton(const Singleton&);

    /**
     * \brief 赋值操作符号，没有实现，禁用掉了
     *
     */
    const Singleton & operator= (const Singleton &);
  protected:

    static T* ms_Singleton;
    Singleton( void )
    {
    }

    ~Singleton( void )
    {
    }

  public:
  
    static void delInstance(void)
    {//可以在子类的destoryMe中被调用
      if (ms_Singleton)
      {
        delete ms_Singleton;
        ms_Singleton = 0;
      }
    }

    static T* instance( void )
    {
      if (!ms_Singleton)
      {
        ms_Singleton = MANA::instance();
      }
      
      return ms_Singleton;
    }

    static T& getInstance(void)
    {
      return *instance();
    }

};

template <typename T,typename MANA>
T* Singleton<T,MANA>::ms_Singleton = 0;


template <typename T>
class SingletonBase : public Noncopyable
{
public:
	SingletonBase() {}
	virtual ~SingletonBase() {}
	static T& getInstance()
	{
		assert(instance);
		return *instance;
	}
	static T* getInstancePtr()
	{
		//assert(instance);
		return instance;
	}
	static void newInstance()
	{
		SAFE_DELETE_P(instance);
		instance = new T();
	}
	static void delInstance()
	{
		SAFE_DELETE_P(instance);
	}
	static bool checkInstance()
	{
		return NULL != instance;
	}
protected:
	static T* instance;
private:
	SingletonBase(const SingletonBase&);
	SingletonBase & operator= (const SingletonBase &);
};
template <typename T> T* SingletonBase<T>::instance = new T();


template <typename T>
struct singleton_default
{
private:
	singleton_default();

public:
	typedef T object_type;

	static object_type & instance()
	{
		return obj;
	}

	static object_type obj;
};
template <typename T>
typename singleton_default<T>::object_type singleton_default<T>::obj;
