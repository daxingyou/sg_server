#include "family_dungeon.hpp"
#include "user_troop/user_troop_mgr.hpp"

family_dungeon_t::family_dungeon_t(uint64_t scene_id, uint32_t map_tid)
	: global_scene_t(scene_id, map_tid)
{
	
}

void family_dungeon_t::on_role_enter(const global_user_ptr& p_user)
{
	if (NULL == p_user)
	{
		log_error("p_user NULL error");
		return;
	}

	if (p_user->get_troop_id() != 0)
	{
		proto::client::ec_troop_quit_reply reply;
		uint32_t reply_code = global_troop_manager::quit_troop(p_user);
		reply.set_reply_code(reply_code);
		p_user->send_msg_to_client(op_cmd::ec_troop_quit_reply, reply);
	}
}
