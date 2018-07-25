#ifndef _FUNC_TIME_H_
#define _FUNC_TIME_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#define BOOST_DATE_TIME_SOURC

#include "macros.hpp"
#include "Lock.h"

#define OPEN_FUNC_TIME 1

#define TIMES_HASH 1024
#define FUNC_NAME 128

struct func_timer
{
	struct sTimes
	{
		sTimes() :_log_timer(600), _times(0), _total_time(0)
		{
			bzero(_dis, sizeof(_dis));
		}
		int32_t _log_timer;
		char _dis[FUNC_NAME];
		uint32_t _times;
		uint64_t _total_time;
		Mutex _mutex;
	};

private:
	static sTimes _times[TIMES_HASH];
	int _which;
	boost::posix_time::ptime _time_1;
	boost::posix_time::ptime _time_2;

public:
	func_timer(const int32_t which, const char* dis);
	~func_timer();
};

#endif
