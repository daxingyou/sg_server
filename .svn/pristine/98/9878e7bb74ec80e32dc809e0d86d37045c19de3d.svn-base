#ifndef __GAME_GM_CMD_H__
#define __GAME_GM_CMD_H__

#include "msg_handler.hpp"
#include "role/role.hpp"
#include <set>

class gm_cmd_t
{
public:
    static bool handle_gm_cmd(const network::msg_buf_ptr& msg_buf);
    static void parse_gm_cmd(std::string gm_cmd, std::vector<std::string>& gm_cmd_list);
    static void allot_gm_cmd(role_ptr p_role, std::vector<std::string>& gm_cmd_list);

	static void init_super_cmd_list();
	static std::set<std::string> super_cmd_list;
};

#endif
