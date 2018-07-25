#ifndef __GATE_USER_MSG_CHECKER_HPP__
#define __GATE_USER_MSG_CHECKER_HPP__

#include <set>
#include "macros.hpp"
#include "user.hpp"

class user_msg_checker_t
{
	typedef std::set<uint16_t> msg_id_set;

public:
	static bool init();
	static void add_masked_msg(uint16_t cmd);
	static void remove_masked_msg(uint16_t cmd);
	static bool check_masked_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr msg_buf, bool notify = true);
	static void load_config();

private:
	static msg_id_set m_masked_msg_gm; // gm已屏蔽的消息列表
	static msg_id_set m_masked_msg_config; // 配置表已屏蔽的消息列表
};


#endif //__GATE_USER_MSG_CHECKER_HPP__