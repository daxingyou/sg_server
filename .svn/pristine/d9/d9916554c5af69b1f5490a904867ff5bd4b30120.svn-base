#include "achieve_common.hpp"
#include "user/global_user_manager.hpp"
#include "user/global_user.hpp"

void achieve_common_t::notify_progress_state(uint64_t uid, proto::common::Achieve_Event_Type type, uint32_t param1 /*= 1*/, uint32_t param2 /*= 1*/)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	// 如果人物不存在
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return;
	}

	proto::server::eg_notify_progress_state_notify notify;
	notify.set_type(type);
	notify.set_param1(param1);
	notify.set_param2(param2);
	p_user->send_msg_to_game(op_cmd::eg_notify_progress_state_notify, notify);
}
