#ifndef __GAME_HOLIDAY_MANAGER_H__
#define __GAME_HOLIDAY_MANAGER_H__

#include "main/game_server_fwd.hpp"
#include "item/drop_manager.hpp"

//节日表id和时间表id相同，服务器启动读取节日表注册时间回调
class holiday_manager_t
{

public:
	static bool init();
	
	static void on_holiday(uint32_t id, bool is_in_time);

	static void drop(role_ptr p_role, std::map<uint32_t, drop_info> &drop_all_result);
private:
	static std::map<uint32_t,uint32_t> holiday_drop;
};

#endif // !__GAME_CULTIVATE_MANAGER_H__