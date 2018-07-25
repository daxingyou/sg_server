#include "rank_manager.hpp"
#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"

rank_manager_t::rank_manager_t()
{
}

rank_manager_t::~rank_manager_t()
{
}

void rank_manager_t::update_rank(uint32_t type, uint64_t uid, uint32_t key, const std::vector<std::string>& custom_data)
{
	proto::server::ge_update_rank_notify ntf;
	ntf.set_rank_type(type);
	ntf.set_owner(uid);
	ntf.set_key(key);
	for (auto str : custom_data)
	{
		ntf.add_custom_data(str);
	}

	env::server->send_msg_to_center(op_cmd::ge_update_rank_notify, uid, ntf);
}
