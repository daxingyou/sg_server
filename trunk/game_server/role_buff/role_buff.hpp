#ifndef __ROLE_BUFF_H__
#define __ROLE_BUFF_H__
#include "macros.hpp"
#include "protos_fwd.hpp"
#include <boost/asio/deadline_timer.hpp>

enum role_buff_type
{
	role_buff_type_addspeed = 1,		// 加速度
	role_buff_type_addattr = 2,			// 加属性
	role_buff_type_add_percent_speed = 3,// 加百分比速度
	role_buff_type_addexp = 4,			//万分比经验
	role_buff_type_change_plugin = 5,	// 道具变身
	role_buff_type_task_plugin = 6,		// 任务变身
};

class role_buff_t
{
public:
	role_buff_t(uint64_t owner);

	void set_tid(uint32_t val) { m_tid = val; }
	uint32_t get_tid() { return m_tid; }

	void del_timer();
public:
	void load_data(const proto::common::role_buff_single& buff_single);
	void peek_data(proto::common::role_buff_single* p_buff_single);
public:
	//BUFF生效，加时间参数是因为可能是下线再上线重新计算BUFF
	void do_effect(uint32_t remain_time, bool notify=true);
	//BUFF失效
	void lose_effect(bool notify);
private:
	uint64_t m_owner = 0;
	uint32_t m_tid = 0;
	boost::shared_ptr<boost::asio::deadline_timer> m_timer;
};
typedef boost::shared_ptr<role_buff_t> role_buff_ptr;

#endif // !__BOUNTY_H__

