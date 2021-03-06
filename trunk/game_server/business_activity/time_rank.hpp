//运营活动-黄金十连系统

#ifndef _TIME_RANK_
#define _TIME_RANK_

#include "macros.hpp"
#include "protos_fwd.hpp"
#include "base_business_active.h"
#include <set>
#include <string>


class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class time_rank_t : public base_business_active_t
{

public:	
	time_rank_t(uint64_t role_id);

	void on_login();

public:
	//是否在活动中
	virtual bool is_active();

	//设置活动开始
	virtual void set_active_start( uint32_t start_time );

	//设置活动结束
	virtual void set_active_stop();

private:

	uint64_t m_role_uid;

	uint32_t m_start_time = 0;

};


#endif
