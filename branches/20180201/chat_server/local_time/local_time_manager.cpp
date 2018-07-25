#include "local_time_manager.hpp"
#include <boost/locale/date_time.hpp>

uint64_t local_time_manager_t::get_loacl_seconds()
{
	boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
	boost::posix_time::time_duration time_from_epoch =
		boost::posix_time::second_clock::universal_time() - epoch;
	return time_from_epoch.total_seconds();
}

uint64_t local_time_manager_t::get_loacl_microseconds()
{
	boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
	boost::posix_time::time_duration time_from_epoch =
		boost::posix_time::microsec_clock::universal_time() - epoch;
	return time_from_epoch.total_microseconds();
}

boost::posix_time::ptime local_time_manager_t::get_loacl_day_time(uint32_t hour/* = 0*/, uint32_t minutes/* = 0*/)
{
	boost::gregorian::date local_day(boost::gregorian::day_clock::local_day());
	boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::ptime recover_time(local_day, boost::posix_time::hours(hour));

	if (now >= recover_time)
	{
		recover_time += boost::gregorian::days(1);
	}
	return recover_time;
}
