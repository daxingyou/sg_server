#pragma once

#include "Entry.h"
/**
* \brief Entry管理器接口,用户应该根据不同使用情况继承它
*/

template<typename e1,typename e2=EntryNone<1>,typename e3=EntryNone<2> >
class EntryManager:protected e1,protected e2,protected e3
{
protected:

	//unsigned long count;

	/**
	* \brief 添加Entry,对于重复索引的Entry添加失败
	* \param e 被添加的 Entry指针
	* \return 成功返回true，否则返回false 
	*/
	inline bool addEntry(Entry * e)
	{

		if(NULL == e)
			return false;
		if (e1::push(e))
		{
			if (e2::push(e))
			{ 

				if (e3::push(e))
					return true;
				else
				{
					e2::remove(e);
					e1::remove(e);
				}
			}
			else
				e1::remove(e);
		}
		return false;
	}

	/**
	* \brief 删除Entry
	* \param e 被删除的Entry指针
	*/
	inline void removeEntry(Entry * e)
	{
		e1::remove(e);
		e2::remove(e);
		e3::remove(e);
	}


	EntryManager() { }
	/**
	* \brief 虚析构函数
	*/
	~EntryManager() { };

	/**
	* \brief 统计管理器中Entry的个数
	* \return 返回Entry个数
	*/
	inline int size() const
	{
		return e1::size();
	}

	/**
	* \brief 判断容器是否为空
	*/
	inline bool empty() const
	{
		return e1::empty();
	}

	/**
	* \brief 清除所有Entry
	*/
	inline void clear()
	{
		e1::clear();
		e2::clear();
		e3::clear();
	}

	/**
	* \brief 对每个Entry进行处理
	* 当处理某个Entry返回false时立即打断处理返回
	* \param eee 处理接口
	* \return 如果全部执行完毕返回true,否则返回false
	*/
	template <class YourEntry>
	inline bool execEveryEntry(execEntry<YourEntry> &eee)
	{
		typedef typename e1::iter my_iter;
		for(my_iter it=e1::ets.begin();it!=e1::ets.end();it++)
		{
			if (!eee.exec((YourEntry *)it->second))
				return false;
		}
		return true;
	}

	/**
	* \brief 删除满足条件的Entry
	* \param pred 测试条件接口
	*/
	template <class YourEntry>
	inline void removeEntry_if (removeEntry_Pred<YourEntry> &pred)
	{
		typedef typename e1::iter my_iter;
		my_iter it=e1::ets.begin();
		while(it!=e1::ets.end())
		{
			if (pred.isIt((YourEntry *)it->second))
			{
				pred.removed.push_back((YourEntry *)it->second);
			}
			it++;
		}

		for(uint32_t i=0;i<pred.removed.size();i++)
		{
			removeEntry(pred.removed[i]);
		}
	}

	void deleteAll()
	{
		typename e1::iter it = e1::ets.begin();
		while( it != e1::ets.end() )
		{
			SAFE_DELETE_P( it->second );
			++it;
		}
		clear();
	}

	void show()
	{
		e2::show();
	}
};
