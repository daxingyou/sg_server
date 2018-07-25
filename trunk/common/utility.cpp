#include "utility.hpp"

NS_COMMON_BEGIN

uint32_t time_util_t::m_delta_time = 0;

long time_util_t::my_timezone = 0L;

const std::string& common::time_util_t::get_local_tz()
{
	static std::string s;
	static bool init = false;

	if (!init)
	{
		std::ostringstream so;
		tzset();
		if (0L == my_timezone)
			my_timezone = timezone;
		so << tzname[0] << timezone / 3600;
		s = so.str();
		init = true;
	}

	return s;
}


void cTimer::reset(const uint32_t how_long, const uint32_t start)
{
	_long = how_long;
	_timer = start + _long;
}

cTimer::cTimer(const uint32_t how_long, const uint32_t start) : _long(how_long)
{
	_timer = start + _long;
}

void cTimer::next(const uint32_t start)
{
	_timer = start + _long;
}

bool cTimer::operator() (const uint32_t curr)
{
	if (_timer <= curr)
	{
		_timer = curr + _long;
		return true;
	}

	return false;
}

NS_COMMON_END
