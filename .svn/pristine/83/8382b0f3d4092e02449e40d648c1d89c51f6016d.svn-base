#ifndef _BASE_BUSINESS_ACTIVE_T
#define _BASE_BUSINESS_ACTIVE_T

#include "protos_fwd.hpp"

class base_business_active_t
{
public:
	//是否在活动中
	virtual bool is_active() { return false; };

	//是否领取了所有奖励
	virtual bool is_get_all_prize() { return true; }

	//是否是结束时间了
	virtual bool is_end_show_time() { return true;  }

	//获取活动开始时间, 如果是0表示永远不结束
	virtual uint32_t get_active_start_time() { return 0; }

	//获取活动结束时间,放在模块做，方便扩展0表示永不结束
	virtual uint32_t get_active_end_time() { return 0; }

	//设置活动开始
	virtual void set_active_start( uint32_t time ) {}
	
	//设置活动结束
	virtual void set_active_stop() {}

	//获取红点
	virtual uint32_t get_red_num(){ return 0; }

};

enum BA_OPEN_TYPE
{
	TYPE_LEVEL = 1,
}; 

enum BA_DEL_TYPE
{
	TYPE_NONE = 0,	//永不移除
	TYPE_ZERO = 1,	//0点移除
};

#endif
