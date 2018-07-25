#pragma once
#include <unordered_map>
#include "log.hpp"
#include "Noncopyable.h"
#include "Lock.h"

#define MAX_NAMESIZE 32

/**
* \brief UniqueID模板
* 本模板实现了唯一ID生成器，并保证线程安全。
* 可以用各种长度的无符号整数作为ID。
*/
template <class T>
class UniqueID:private Noncopyable
{
private:
	Mutex mutex;
	//std::list<T,__pool_alloc<T> > ids;
	std::list<T> ids;
	T maxID;
	T minID;
	T curMaxID;
	void init(T min,T max)
	{
		minID=min;
		maxID=max;
		curMaxID=minID;
	}

public:
	/**
	* \brief 默认构造函数 
	* 开始ID为1，最大有效ID为(T)-2,无效ID为(T)-1
	*/
	UniqueID()
	{
		init(1,(T)-1);
	}

	/**
	* \brief 构造函数 
	* 用户自定义起始ID，最大有效ID为(T)-2,无效ID为(T)-1
	* \param startID 用户自定义的起始ID
	*/
	UniqueID(T startID)
	{
		init(startID,(T)-1);
	}

	/**
	* \brief 构造函数 
	* 用户自定义起始ID，及最大无效ID,最大有效ID为最大无效ID-1
	* \param startID 用户自定义的起始ID
	* \param endID 用户自定义的最大无效ID
	*/
	UniqueID(T startID,T endID)
	{
		init(startID,endID);
	}

	/**
	* \brief 析构函数 
	* 回收已分配的ID内存。
	*/
	~UniqueID()
	{
		mutex.lock();
		ids.clear();
		mutex.unlock();
	}

	/**
	* \brief 得到最大无效ID 
	* \return 返回最大无效ID
	*/
	T invalid()
	{
		return maxID;
	}

	/**
	* \brief 测试这个ID是否被分配出去
	* \return 被分配出去返回true,无效ID和未分配ID返回false
	*/
	bool hasAssigned(T testid)
	{
		mutex.lock();
		if (testid<maxID && testid>=minID)
		{
			typename std::list<T/*,__pool_alloc<T>*/ >::iterator iter = ids.begin();
			for(;iter != ids.end() ; iter ++)
			{
				if (*iter == testid)
				{
					mutex.unlock();
					return false;
				}
			}
			/*
			for(int i=0,n=ids.size() ;i<n;i++)
			{
			if (ids[i]==testid)
			{
			mutex.unlock();
			return false;
			}
			}
			// */
			mutex.unlock();
			return true;
		}
		mutex.unlock();
		return false;
	}

	/**
	* \brief 得到一个唯一ID 
	* \return 返回一个唯一ID，如果返回最大无效ID，比表示所有ID都已被用，无可用ID。
	*/
	T get()
	{
		T ret;
		mutex.lock();
		if (maxID>curMaxID)
		{
			ret=curMaxID;
			curMaxID++;
		}
		else
			ret=maxID;
		if (ret == maxID && !ids.empty())
		{
			ret=ids.back();
			ids.pop_back();
		}
		mutex.unlock();
		return ret;
	}

	/**
	* \brief 一次得到多个ID，这些ID都是相邻的,并且不回被放回去 
	* \param size 要分配的ID个数
	* \param count 实际分配ID的个数
	* \return 返回第一个ID，如果返回最大无效ID，比表示所有ID都已被用，无可用ID。
	*/
	T get(int size,int & count)
	{
		T ret;
		mutex.lock();
		if (maxID>curMaxID)
		{
			count=(maxID-curMaxID)>size?size:(maxID-curMaxID);
			ret=curMaxID;
			curMaxID+=count;
		}
		else
		{
			count=0;
			ret=maxID;
		}
		mutex.unlock();
		return ret;
	}

	/**
	* \brief 将ID放回ID池，以便下次使用。 
	* 
	* 放回的ID必须是由get函数得到的。并且不能保证放回的ID,没有被其他线程使用。
	* 所以用户要自己保证还在使用的ID不会被放回去。以免出现ID重复现象。
	* \param id 由get得到的ID.
	*/
	void put(T id)
	{
		mutex.lock();
		if (id<maxID && id>=minID)
		{
			bool hasID=false;
			typename std::list<T/*,__pool_alloc<T> */>::iterator iter = ids.begin();
			for(;iter != ids.end() ; iter ++)
			{
				if (*iter == id)
				{
					hasID=true;
					break;
				}
			}
			/*
			for(int i=0,n=ids.size() ;i<n;i++)
			{
			if (ids[i]==id)
			{
			hasID=true;
			break;
			}
			}
			// */
			if (!hasID) ids.push_front(id);
			//if (!hasID) ids.insert(ids.begin(),id);
			//if (!hasID) ids.push_back(id);
		}
		mutex.unlock();
	}
};


/**
* \brief entry管理器定义文件
*/

/**
* \brief 回调函数类模板
*/
template <typename T,typename RTValue = bool>
struct EntryCallback
{
	virtual RTValue exec(T *e)=0;
	virtual ~EntryCallback(){};
};

class Entry : private Noncopyable
{
public:
	/**
	* \brief entry的数据ID，不同类型的Entry可能会重复,此时不能实现从ID查找entry
	*/
	uint32_t m_id;
	/**
	* \brief entry的临时id,建议在实现EntryManager时，保证分配唯一
	*/
	uint32_t m_tempid;
	/**
	* \brief entry的名字，不同类型的Entry可能会重复,此时不能实现从名字查找entry
	*/
	char m_name[MAX_NAMESIZE];

	bool m_use_gb2312_name;

	Entry() : m_id( 0 ), m_tempid( 0 ), m_use_gb2312_name( false )
	{
		bzero(m_name,sizeof(m_name));
	};

	Entry( const uint32_t id ) : m_id( id ), m_tempid( 0 ), m_use_gb2312_name( false )
	{
		bzero(m_name,sizeof(m_name));
	};

	Entry( const uint32_t id, const char* name ) : m_id( id ), m_tempid( 0 ), m_use_gb2312_name( false )
	{
		strncpy( this->m_name, name, sizeof( this->m_name ) );
	};

	

	virtual ~Entry(){};

	///< 类名称
	virtual const char* getClassName() const = 0;
	///< 日志相关
	void debug( const char* pattern, ... );
	void error( const char* pattern, ... );
	void info( const char* pattern, ... );
	void fatal( const char* pattern, ... );
	void warn( const char* pattern, ... );

	std::string getGB2312Name();
	uint32_t get_id() const { return m_id; }
	std::string get_name() const { return std::string(m_name); };
};

/**
* \brief key值等值比较,目前支持 (uint32_t,char *)，两种类型
*/
template <class keyT>
struct my_key_equal : public std::binary_function<keyT,keyT,bool>
{
	inline bool operator()(const keyT s1,const keyT s2) const
	{
		return false;
	}
};

/**
* \brief 模板偏特化
* 对字符串进行比较
*/
template<>
inline bool my_key_equal<const char *>::operator()(const char * s1,const char * s2) const
{
	return strcmp(s1,s2) == 0;
}

/**
* \brief 模板偏特化
* 对整数进行比较
*/
template<>
inline bool my_key_equal<uint32_t>::operator()(const uint32_t s1,const uint32_t s2) const
{
	return s1  == s2;
}

/**
* \brief 有限桶Hash管理模板,非线程安全
*
* 目前支持两种key类型(uint32_t,char *),value类型不作限制,但此类型要可copy的。
* \param keyT key类型(uint32_t,char *)
* \param valueT value类型
*/
template <class keyT,class valueT>
class LimitHash:private Noncopyable
{
protected:

	/**
	* \brief hash_map容器
	*/
	typedef std::unordered_map<keyT, valueT> hashmap;
	typedef typename hashmap::iterator iter;
	typedef typename hashmap::const_iterator const_iter;
	hashmap ets;


	/**
	* \brief 插入数据，如果原来存在相同key值的数据，原来数据将会被替换
	* \param key key值
	* \param value 要插入的数据
	* \return 成功返回true，否则返回false
	*/
	inline bool insert(const keyT &key,valueT &value)
	{
		ets[key]=value;
		return true;
	}

	/**
	* \brief 根据key值查找并得到数据
	* \param key 要寻找的key值
	* \param value 返回结果将放入此处,未找到将不会改变此值
	* \return 查找到返回true，未找到返回false
	*/
	inline bool find(const keyT &key,valueT &value) const
	{
		const_iter it = ets.find(key);
		if (it != ets.end())
		{
			value = it->second;
			return true;
		}
		else
			return false;
	}

	/**
	* \brief 查找并得到一个数据
	* \param value 返回结果将放入此处,未找到将不会改变此值
	* \return 查找到返回true，未找到返回false
	*/
	inline bool findOne(valueT &value) const
	{
		if (!ets.empty())
		{
			value=ets.begin()->second;
			return true;
		}
		return false;
	}

	/**
	* \brief 构造函数
	*
	*/
	LimitHash()
	{
	}

	/**
	* \brief 析构函数,清除所有数据
	*/
	~LimitHash()
	{
		clear();
	}

	/**
	* \brief 移除数据
	* \param key 要移除的key值
	*/
	inline void remove(const keyT &key)
	{
		ets.erase(key);
	}

	/**
	* \brief 清除所有数据
	*/
	inline void clear()
	{
		ets.clear();
	}

	/**
	* \brief 统计数据个数
	*/
	inline uint32_t size() const
	{
		return ets.size();
	}

	/**
	* \brief 判断容器是否为空
	*/
	inline bool empty() const
	{
		return ets.empty();
	}
};

/**
* \brief 有限桶MultiHash管理模板,非线程安全
*
* 目前支持两种key类型(uint32_t,char *),value类型不作限制,但此类型要可copy的。
* \param keyT key类型(uint32_t,char *)
* \param valueT value类型
*/
template <class keyT,class valueT>
class MultiHash:private Noncopyable
{
protected:

	/**
	* \brief hash_multimap容器
	*/
	typedef std::unordered_multimap<keyT, valueT> hashmap;
	typedef typename hashmap::iterator iter;
	typedef typename hashmap::const_iterator const_iter;
	hashmap ets;

	/**
	* \brief 插入数据，如果原来存在相同key值的数据，原来数据将会被替换
	* \param key key值
	* \param value 要插入的数据
	* \return 成功返回true，否则返回false
	*/
	inline bool insert(const keyT &key,valueT &value)
	{
		//if(ets.find(key) == ets.end())
		ets.insert(std::pair<keyT,valueT>(key,value));
		return true;
	}

	/**
	* \brief 构造函数
	*
	*/
	MultiHash()
	{
	}

	/**
	* \brief 析构函数,清除所有数据
	*/
	~MultiHash()
	{
		clear();
	}

	/**
	* \brief 清除所有数据
	*/
	inline void clear()
	{
		ets.clear();
	}

	/**
	* \brief 统计数据个数
	*/
	inline uint32_t size() const
	{
		return ets.size();
	}

	/**
	* \brief 判断容器是否为空
	*/
	inline bool empty() const
	{
		return ets.empty();
	}
};

/**
* \brief Entry以临时ID为key值的指针容器，需要继承使用
*/
class EntryTempID:public LimitHash<uint32_t,Entry *>
{
protected:

	EntryTempID() {}
	virtual ~EntryTempID() {}

	/**
	* \brief 将Entry加入容器中,tempid重复添加失败
	* \param e 要加入的Entry
	* \return 成功返回true,否则返回false
	*/
	inline bool push(Entry * e)
	{
		if (e!=NULL && getUniqeID(e->m_tempid))
		{
			Entry *temp;
			if (!find(e->m_tempid,temp))
			{
				if (insert(e->m_tempid,e))
					return true;
			}
			putUniqeID(e->m_tempid);
		}
		return false;
	}

	/**
	* \brief 移除Entry
	* \param e 要移除的Entry
	*/
	inline void remove(Entry * e)
	{
		if (e!=NULL)
		{
			putUniqeID(e->m_tempid);
			LimitHash<uint32_t,Entry *>::remove(e->m_tempid);
		}
	}

	/**
	* \brief 通过临时ID得到Entry
	* \param tempid 要得到Entry的临时ID
	* \return 返回Entry指针,未找到返回NULL
	*/
	inline Entry * getEntryByTempID(const uint64_t tempid) const
	{
		Entry *ret=NULL;
		LimitHash<uint32_t,Entry *>::find(tempid,ret);
		return ret;
	}

	/**
	* \brief 得到一个临时ID
	* \param tempid 存放要得到的临时ID
	* \return 得到返回true,否则返回false
	*/
	virtual bool getUniqeID(uint32_t &tempid) =0;
	/**
	* \brief 放回一个临时ID
	* \param tempid 要放回的临时ID
	*/
	virtual void putUniqeID(const uint32_t &tempid) =0;
};

/**
* \brief Entry以ID为key值的指针容器，需要继承使用
*/
class EntryID:public LimitHash<uint32_t,Entry *>
{
protected:
	/**
	* \brief 将Entry加入容器中
	* \param e 要加入的Entry
	* \return 成功返回true,否则返回false
	*/
	inline bool push(Entry * &e)
	{
		if (NULL == e)
		{
			return false;
		}

		Entry *temp;
		if (!find(e->m_id,temp))
			return insert(e->m_id,e);
		else
			return false;
	}

	/**
	* \brief 移除Entry
	* \param e 要移除的Entry
	*/
	inline void remove(Entry * e)
	{
		if (e!=NULL)
		{
			LimitHash<uint32_t,Entry *>::remove(e->m_id);
		}
	}

	/**
	* \brief 通过ID得到Entry
	* \param id 要得到Entry的ID
	* \return 返回Entry指针,未找到返回NULL
	*/
	inline Entry * getEntryByID(const uint32_t id) const
	{
		Entry *ret=NULL;
		LimitHash<uint32_t,Entry *>::find(id,ret);
		return ret;
	}
};


#ifdef _MSC_VER

/**
* \brief Entry以名字为key值的指针容器，需要继承使用
*/
class EntryName:public LimitHash<std::string,Entry *>
{
protected:
	/**
	* \brief 将Entry加入容器中,如果容器中有相同key值的添加失败
	* \param e 要加入的Entry
	* \return 成功返回true,否则返回false
	*/
	inline bool push(Entry * &e)
	{
		if (NULL == e)
		{
			return false;
		}

		Entry *temp;
		if (!find(std::string(e->m_name),temp))
			return insert(std::string(e->m_name),e);
		else
			return false;
	}

	/**
	* \brief 移除Entry
	* \param e 要移除的Entry
	*/
	inline void remove(Entry * e)
	{
		if (e!=NULL)
		{
			LimitHash<std::string,Entry *>::remove(std::string(e->m_name));
		}
	}

	/**
	* \brief 通过名字得到Entry
	* \param name 要得到Entry的名字
	* \return 返回Entry指针,未找到返回NULL
	*/
	inline Entry * getEntryByName( const char * name) const
	{
		Entry *ret=NULL;
		LimitHash<std::string,Entry *>::find(std::string(name),ret);
		return ret;
	}

	/**
	* \brief 通过名字得到Entry
	* \param name 要得到Entry的名字
	* \return 返回Entry指针,未找到返回NULL
	*/
	inline Entry * getEntryByName(const std::string  &name) const
	{
		return getEntryByName(name.c_str());
	}

	void show()
	{
		for( const_iter it = LimitHash<std::string,Entry *>::ets.begin(); it != LimitHash<std::string,Entry *>::ets.end(); ++it )
		{
			log_info( "LimitHash::show %s %p", it->first.c_str(), it->second );
		}
	}
};

#else

/**
* \brief Entry以名字为key值的指针容器，需要继承使用
*/
//class EntryName:public LimitHash<std::string,Entry *>
class EntryName:public LimitHash<const char*,Entry *>
{
protected:
	/**
	* \brief 将Entry加入容器中,如果容器中有相同key值的添加失败
	* \param e 要加入的Entry
	* \return 成功返回true,否则返回false
	*/
	inline bool push(Entry * &e)
	{
		if (NULL == e)
		{
			return false;
		}

		Entry *temp;
		if (!find(e->m_name,temp))
			return insert(e->m_name,e);
		else
			return false;
	}

	/**
	* \brief 移除Entry
	* \param e 要移除的Entry
	*/
	inline void remove(Entry * e)
	{
		if (e!=NULL)
		{
			LimitHash<const char*,Entry *>::remove(e->m_name);
		}
	}

	/**
	* \brief 通过名字得到Entry
	* \param name 要得到Entry的名字
	* \return 返回Entry指针,未找到返回NULL
	*/
	inline Entry * getEntryByName( const char * name) const
	{
		Entry *ret=NULL;
		LimitHash<const char*,Entry *>::find(name,ret);
		return ret;
	}

	/**
	* \brief 通过名字得到Entry
	* \param name 要得到Entry的名字
	* \return 返回Entry指针,未找到返回NULL
	*/
	inline Entry * getEntryByName(const std::string& name) const
	{
		return getEntryByName(name.c_str());
	}

	void show()
	{
		for( const_iter it = LimitHash<const char*,Entry *>::ets.begin(); it != LimitHash<const char*,Entry *>::ets.end(); ++it )
		{
			log_info( "LimitHash::show %s %p", it->first, it->second );
		}
	}
};

#endif

/**
* \brief Entry以名字为key值的指针容器，需要继承使用
*/
class MultiEntryName:public MultiHash</*const char **/std::string,Entry *>
{
protected:
	/**
	* \brief 将Entry加入容器中,如果容器中有相同key值的添加失败
	* \param e 要加入的Entry
	* \return 成功返回true,否则返回false
	*/
	inline bool push(Entry * &e)
	{
		if (NULL == e)
		{
			return false;
		}

		return insert(std::string(e->m_name),e);
	}

	/**
	* \brief 将Entry从容器中移除
	* \param e 需要移除的Entry
	*/
	inline void remove(Entry * &e)
	{
		if (NULL == e)
		{
			return;
		}

        /*std::pair<iter, iter> its = ets.equal_range(std::string(e->m_name));
        for (iter it = its.first; it != its.second; it++)
        {
            if (it->second == e)
            {
                ets.erase(it);
                return;
            }
        }*/

        iter it = ets.find(std::string(e->m_name));
        if (it != ets.end())
        {
            if (it->second == e)
            {
                ets.erase(it);
                return;
            }
        }
	}

	/**
	* \brief 根据key值查找并得到数据
	* \param name 要寻找的name值
	* \param e 返回结果将放入此处,未找到将不会改变此值
	* \param r 如果有多项匹配，是否随机选择
	* \return 查找到返回true，未找到返回false
	*/
	inline bool find(const char * &name,Entry * &e,const bool r=false) const
	{
		int rd = ets.count(std::string(name));
		if (rd > 0)
		{
			int mrd = 0,j = 0;
			if (r)
				mrd = rand() % rd;
			std::pair<const_iter,const_iter> its = ets.equal_range(std::string(name));
			for(const_iter it = its.first; it != its.second && j < rd; it++,j++)
			{
				if (mrd == j)
				{
					e = it->second;
					return true;
				}
			}
		}
		return false;
	}

};

template<int i>
class EntryNone
{
protected:
	inline bool push(Entry * &e) { return true; }
	inline void remove(Entry * &e) { }
	inline void clear(){}
};

/**
* \brief Entry处理接口,由<code>EntryManager::execEveryEntry</code>使用
*/
template <class YourEntry>
struct execEntry
{
	virtual bool exec(YourEntry *entry) =0;
	virtual ~execEntry(){}
};

/**
* \brief Entry删除条件接口,由<code>EntryManager::removeEntry_if</code>使用
*/
template <class YourEntry>
struct removeEntry_Pred
{
	/**
	* \brief 被删除的entry存储在这里
	*/
	std::vector<YourEntry *> removed;
	/**
	* \brief 测试是否要删除的entry,需要实现
	* \param 要被测试的entry
	*/
	virtual bool isIt(YourEntry *entry) =0;
	/**
	* \brief 析构函数
	*/
	virtual ~removeEntry_Pred(){}
};
