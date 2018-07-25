#ifndef __GAME_LOCAL_TIME_MANAGER_H__
#define __GAME_LOCAL_TIME_MANAGER_H__
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/locale/date_time.hpp>

class local_time_manager_t
{
public:
	static uint64_t get_loacl_seconds();

	static uint64_t get_loacl_microseconds();

	static boost::posix_time::ptime get_loacl_day_time(uint32_t hour = 0, uint32_t minutes = 0);

};

#endif // !__GAME_LOCAL_TIME_MANAGER_H__