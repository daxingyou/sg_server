#include "sTime.hpp"


/**
* \brief 得到当前真实时间
*
* \return 真实时间，单位毫秒
*/
uint64_t sRTime::_now()
{
	uint64_t retval = 0LL;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	retval = tv.tv_sec;
	retval *= 1000;
	retval += tv.tv_usec / 1000;
	return retval;
}

void sRTime::nowByDelay(int32_t delay)
{
	_msecs = _now();
	addDelay(delay);
}

sRTime::sRTime(uint32_t delay /*= 0*/)
{
	nowByDelay(delay);
}

sRTime::sRTime(const sRTime &rt)
{
	_msecs = rt._msecs;
}

void sRTime::now()
{
	_msecs = _now();
}

uint64_t sRTime::sec() const
{
	return _msecs / 1000L;
}

uint64_t sRTime::msec() const
{
	return _msecs % 1000L;
}

uint64_t sRTime::msecs() const
{
	return _msecs;
}

void sRTime::setmsecs(uint64_t data)
{
	_msecs = data;
}

void sRTime::addDelay(int32_t delay)
{
	_msecs += delay;
}

sRTime & sRTime::operator= (const sRTime &rt)
{
	_msecs = rt._msecs;
	return *this;
}

const sRTime & sRTime::operator+ (const sRTime &rt)
{
	_msecs += rt._msecs;
	return *this;
}

const sRTime & sRTime::operator- (const sRTime &rt)
{
	_msecs -= rt._msecs;
	return *this;
}

bool sRTime::operator > (const sRTime &rt) const
{
	return _msecs > rt._msecs;
}

bool sRTime::operator >= (const sRTime &rt) const
{
	return _msecs >= rt._msecs;
}

bool sRTime::operator < (const sRTime &rt) const
{
	return _msecs < rt._msecs;
}

bool sRTime::operator <= (const sRTime &rt) const
{
	return _msecs <= rt._msecs;
}

bool sRTime::operator == (const sRTime &rt) const
{
	return _msecs == rt._msecs;
}

uint64_t sRTime::elapse(const sRTime &rt) const
{
	if (rt._msecs > _msecs)
		return (rt._msecs - _msecs);
	else
		return 0LL;
}

