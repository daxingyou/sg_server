#ifndef __CTIME_HPP__
#define __CTIME_HPP__

#include <time.h>
#include <sys/time.h>
#include <sstream>

#define DAY_SECS (24 * 60 * 60)

class sRTime
{

private:

	/**
	* \brief 真实时间换算为毫秒
	*
	*/
	uint64_t _msecs;

	/**
	* \brief 得到当前真实时间
	*
	* \return 真实时间，单位毫秒
	*/
	uint64_t _now();

	/**
	* \brief 得到当前真实时间延迟后的时间
	* \param delay 延迟，可以为负数，单位毫秒
	*/
	void nowByDelay(int32_t delay);

public:

	/**
	* \brief 构造函数
	*
	* \param delay 相对于现在时间的延时，单位毫秒
	*/
	sRTime(uint32_t delay = 0);

	/**
	* \brief 拷贝构造函数
	*
	* \param rt 拷贝的引用
	*/
	sRTime(const sRTime &rt);

	/**
	* \brief 获取当前时间
	*
	*/
	void now();

	/**
	* \brief 返回秒数
	*
	* \return 秒数
	*/
	uint64_t sec() const;

	/**
	* \brief 返回毫秒数
	*
	* \return 毫秒数
	*/
	uint64_t msec() const;

	/**
	* \brief 返回总共的毫秒数
	*
	* \return 总共的毫秒数
	*/
	uint64_t msecs() const;

	/**
	* \brief 返回总共的毫秒数
	*
	* \return 总共的毫秒数
	*/
	void setmsecs(uint64_t data);

	/**
	* \brief 加延迟偏移量
	*
	* \param delay 延迟，可以为负数，单位毫秒
	*/
	void addDelay(int32_t delay);

	/**
	* \brief 重载=运算符号
	*
	* \param rt 拷贝的引用
	* \return 自身引用
	*/
	sRTime & operator= (const sRTime &rt);

	/**
	* \brief 重构+操作符
	*
	*/
	const sRTime & operator+ (const sRTime &rt);

	/**
	* \brief 重构-操作符
	*
	*/
	const sRTime & operator- (const sRTime &rt);

	/**
	* \brief 重构>操作符，比较zRTime结构大小
	*
	*/
	bool operator > (const sRTime &rt) const;

	/**
	* \brief 重构>=操作符，比较zRTime结构大小
	*
	*/
	bool operator >= (const sRTime &rt) const;

	/**
	* \brief 重构<操作符，比较zRTime结构大小
	*
	*/
	bool operator < (const sRTime &rt) const;

	/**
	* \brief 重构<=操作符，比较zRTime结构大小
	*
	*/
	bool operator <= (const sRTime &rt) const;

	/**
	* \brief 重构==操作符，比较zRTime结构是否相等
	*
	*/
	bool operator == (const sRTime &rt) const;

	/**
	* \brief 计时器消逝的时间，单位毫秒
	* \param rt 当前时间
	* \return 计时器消逝的时间，单位毫秒
	*/
	uint64_t elapse(const sRTime &rt) const;

};

class cTimer
{
private:
	uint64_t m_uTickTerm;
	uint64_t m_uTickOld;

public:
	bool m_bOper;

public:
	cTimer()
	{
		clear();
	}

	bool is_settimer() { return m_bOper; }

	void set_term_time(uint64_t uTerm) { m_uTickTerm = uTerm; }
	uint64_t get_term_time() { return m_uTickTerm; }

	uint64_t get_tick_oldtime() { return m_uTickOld; }

	void clear()
	{
		m_uTickTerm = 0;
		m_bOper = false;
		m_uTickOld = 0;
	}

	void begin_timer(uint64_t uTerm, uint64_t uNow)
	{
		m_bOper = true;
		m_uTickTerm = uTerm;
		m_uTickOld = uNow;
	}

	bool counting_timer(uint64_t uNow)
	{
		if (!m_bOper)
			return false;

		uint64_t uNew = uNow;

		if (uNew < m_uTickOld + m_uTickTerm)
			return false;

		m_uTickOld = uNew;

		return true;
	}
	uint64_t get_leave_time(uint64_t uNow)//剩余时间;
	{
		if (!counting_timer(uNow))
		{
			return m_uTickTerm + m_uTickOld - uNow;
		}
		return 0;
	}
};

#endif