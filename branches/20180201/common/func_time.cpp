#include "func_time.hpp"
#include "log.hpp"

func_timer::sTimes func_timer::_times[TIMES_HASH];

func_timer::func_timer(const int32_t which, const char* dis)
{
	if (OPEN_FUNC_TIME)
	{
		_which = which;
		_times[_which]._mutex.lock();
		if (!_times[_which]._times)
		{
			strncpy(_times[_which]._dis, dis, sizeof(_times[_which]._dis));
		}

		_time_1 = boost::posix_time::microsec_clock::universal_time();
	}
}

func_timer::~func_timer()
{
	if (OPEN_FUNC_TIME)
	{
		_time_2 = boost::posix_time::microsec_clock::universal_time();
		boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse = _time_2 - _time_1;
		_times[_which]._times++;
		_times[_which]._total_time += time_elapse.ticks();
		//printf("\033[31m [%s]:%lu \033[0m\n", _times[_which]._dis, time_elapse.ticks());
		if (time_elapse.ticks() > _times[_which]._log_timer)
		{
			log_warn("FUNCTION[%d] TIMES:%u, ALLTIME:%lu MICROSEC, DESC:%s", _which, _times[_which]._times, _times[_which]._total_time, _times[_which]._dis);
			_times[_which]._times = 0;
			_times[_which]._total_time = 0;
		}
		_times[_which]._mutex.unlock();
	}
}

