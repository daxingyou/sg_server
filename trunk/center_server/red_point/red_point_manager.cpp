#include "red_point_manager.hpp"

#include "protocpp/common.pb.h"
#include "user/global_user_manager.hpp"
#include "user/global_user.hpp"
#include <vector>
#include "goods/goods_manager.hpp"
void red_point_manager_t::red_point_notify(uint64_t user_id,proto::common::RED_POINT_TYPE red_point_type, uint32_t param )
{
	//TODO

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(user_id);

	// 如果人物不存在
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", user_id);
		return;
	}

	proto::server::eg_red_point_notify notify;

	notify.set_uid(user_id);
	notify.set_red_type(red_point_type);
	notify.add_param(param);
	p_user->send_msg_to_game(op_cmd::eg_red_point_notify, notify);

}

void red_point_manager_t::check_goods_point(uint64_t user_id)
{
	///TODO

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(user_id);

	// 如果人物不存在
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", user_id);
		return;
	}

	if ( 0 < goods_manager_t::get_profit(user_id))
	{
		red_point_notify(user_id, proto::common::RED_POINT_GOODS);
	}
}

