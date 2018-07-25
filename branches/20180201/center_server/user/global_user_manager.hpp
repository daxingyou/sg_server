#ifndef __CENTER_GLOBAL_USER_MANAGER_HPP__
#define __CENTER_GLOBAL_USER_MANAGER_HPP__

#include "global_user.hpp"
#include "protos_fwd.hpp"
#include <map>

#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"

class global_user_manager_t
{
public:
    typedef std::map<uint64_t, global_user_ptr> global_user_map_type;

public:
    global_user_manager_t() {}
    ~global_user_manager_t() {}

    static void ready_enter_scene(uint64_t uid);

    static bool add_global_user(global_user_ptr p_user);

    static void kick_global_user(uint64_t user_uid, uint32_t kick_reason);
    static void kick_game_all_users(uint32_t game_id);
	static void kick_gate_all_users(uint32_t gate_id);
    static void kick_all_global_users();

    static global_user_ptr get_global_user_by_uid(uint64_t user_uid);
    static global_user_ptr get_global_user_by_name(const std::string& name);

    static const global_user_map_type& get_all_global_users() {return m_global_user_map;}

	// �������
	static void one_day(uint32_t curr_time);

	//////////////////////////////////////////////////////////////////////////
	// gate->center �������ע��
	static void reg_global_user(const network::tcp_socket_ptr& s, uint64_t uid);
	// gate->center ��Ҵ����µĽ�ɫ
	static void create_global_user(const network::tcp_socket_ptr& s, uint64_t uid, const std::string& nickname, uint32_t role_type);
	// db->center ������ɫ�ɹ���centerͬ������
	static void create_user_success(uint32_t gate_id, uint64_t uid, const std::string& nickname, uint32_t create_time, uint32_t role_type, uint32_t channel_id );
	// game|gate->center ע����ɫ
	static void unreg_global_user(uint64_t uid, uint32_t unreg_code, uint32_t kick_reason);
	// game->center ����ע��ɹ��ظ�
	static void scene_reg_user_success(const network::tcp_socket_ptr& s, uint64_t uid, const proto::server::global_user_data& data);
	// game->center ���ȷ��
	static uint32_t user_cross_scene_check(const global_user_ptr& p_user, const proto::server::cross_server_data& data);
	// gate->center �����ʼ
	static void user_cross_server(uint64_t uid, const proto::server::cross_server_data& data);

    // Ⱥ��������Ϣ������gate����Ϣ��һ����û����ѡ��
public:
	template<typename T_MSG>
	static void send_msg_to_country(uint32_t country_id, uint16_t cmd, const T_MSG& msg)
	{

		// û��ֱ��return
		if (m_global_user_map.empty())
		{
			return;
		}

		std::string cmd_data;
		network::wrap_msg(cmd_data, msg);

		proto::server::ea_broadcast_notify msg_to_gate;
		msg_to_gate.set_cmd(cmd);
		msg_to_gate.set_data(cmd_data);

		for (global_user_map_type::const_iterator citr = m_global_user_map.begin();
			citr != m_global_user_map.end(); ++citr)
		{
			if (citr->second != NULL && citr->second->get_country_id() == country_id)
			{
				msg_to_gate.add_role_list(citr->first);
			}
		}

		env::server->send_msg_to_all_gate(op_cmd::ea_broadcast_notify, 0, msg_to_gate);
	}

public:
	static uint32_t get_delta_level(global_user_ptr p_user, global_user_ptr p_target);
	static uint32_t get_all_user_num() { return m_global_user_map.size(); }
private:
    static global_user_map_type m_global_user_map;

    typedef std::map<std::string, uint64_t> nickname_uid_map_type;
    static nickname_uid_map_type m_nickname_uid_map;
};

#endif
