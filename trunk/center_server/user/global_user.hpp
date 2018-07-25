#ifndef __CENTER_GLOBAL_USER_HPP__
#define __CENTER_GLOBAL_USER_HPP__

#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "network/tcp_socket.hpp"
#include "game/game_manager.hpp"
#include "gate/gate_manager.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "common.pb.h"
#include "server.pb.h"
#include "tblh/family_enum.hpp"
#include "tblh/country_enum.hpp"
#include "friend/friend_mgr.hpp"
#include "mail/mail_user_data.hpp"
#include "global_user_common.hpp"
#include "shop/shop_user_data.hpp"
#include "common_struct.hpp"

class global_user_t : public boost::enable_shared_from_this<global_user_t>
{
public:
	global_user_t() = delete;
    global_user_t(uint64_t uid);
    ~global_user_t();

	// ɾ��֮ǰ��һЩ������
	void clear_data();

    void ready_enter_scene();

	//////////////////////////////////////////////////////////////////////////
	// ״̬
	bool is_online() const { return m_online_state == common::ROLE_ONLINE_STATE_ONLINE; }
	bool is_offline() const { return m_online_state == common::ROLE_ONLINE_STATE_OFFLINE; }
	bool is_logout() const { return m_online_state == common::ROLE_ONLINE_STATE_LOGOUT; }

	// ���ߴ���
	void online(uint32_t online);
	// ���ߴ���
	void on_logout();
	// ���ߴ���
	void on_login();
	// ����֪ͨ����������
	void offline_notify_other();

	// ���ػ������ݣ�֪ͨ������½
	// role_type��ֵ˵�������˺�
	bool reg_user(uint32_t gate_id, bool is_new);
	// ����������ԭ������
	bool relogin(uint32_t gate_id);
	// �����½
	bool reg_cross(const proto::server::cross_server_data& data);
	// ���³���������ͬ����Ϣ
	void sync_role_date(const proto::server::global_user_data& data);

	// ״̬�仯֪ͨ����
	void update_online_state();

	// ���Ա仯֪ͨ
	void update_attr();

public:
    uint64_t get_uid() const { return m_uid; }

	const std::string& get_name() const { return m_common_info.m_name; }
	void set_name(const std::string& val);

	uint64_t get_troop_id() const { return m_common_info.m_troop_id; }
	void set_troop_id(uint64_t val) { m_common_info.m_troop_id = val; }

	common::ROLE_ONLINE_STATE get_online_state() const { return m_online_state; }

	uint32_t get_last_logout_time() const { return m_common_info.m_last_logout_time; }
	void set_last_logout_time(uint32_t val);

	uint32_t get_level() const { return m_common_info.m_level; }
	void set_level(uint32_t val);

	uint32_t get_fighting_value() const { return m_common_info.m_fighting_value; }
	void set_fighting_value(uint32_t val);

	///ͷ��ID�ͱ߿�ID(��16λͷ��ID ��16λ�߿�ID)
    uint32_t get_hero_plugin() const { return m_common_info.m_hero_plugin; }
	void set_hero_plugin(uint32_t val);

	//����ֵ
	uint32_t get_charm()const { return m_common_info.m_charm; }
	void set_charm(uint32_t val);
	//ģ��
	uint32_t get_plugin() const { return m_common_info.m_plugin; }
	void set_plugin(uint32_t val);

	uint64_t get_scene_id() const { return m_common_info.m_scene_id; }
	uint32_t get_map_tid() const { return m_common_info.m_map_tid; }
	void set_scene_id(uint64_t scene_id, uint32_t map_tid);
	void sync_scene_info(uint64_t scene_id, uint32_t map_tid);

	void one_day(uint32_t curr_time);

	//�Ƿ��ڻʳ� Ҫʵ��
	bool is_in_king_city();
	
	//����
	void change_name(const std::string& val);
	// �������
public:
	uint64_t get_family_id() const { return m_common_info.m_family_id; }

	uint32_t get_family_officer() const { return m_common_info.m_family_officer; }
	void set_family_officer(uint32_t val);

	uint32_t get_family_contribution() const { return m_common_info.m_family_contribution; }
	void set_family_contribution(uint32_t val);

	void join_family(uint64_t family_id, uint32_t officer, uint32_t country_id);
	void leave_family();

	// �������
public:
	uint32_t get_country_id() const { return m_common_info.m_country_id; }
	void set_country_id(uint32_t val);

	uint32_t get_country_officer() const { return m_common_info.m_country_officer; }
	void set_country_officer(uint32_t val);

	// ��������
public:
	// ����
	inline friend_mgr_t& get_friend_mgr() { return m_friend_mgr; }
	// �ʼ�
	inline mail_user_data_t& get_mail_data() { return m_mail_data; }
	// �̵�
	inline shop_user_data_t& get_shop_user_data() { return m_shop_goods_data; }

	// ����������
public:
    inline uint32_t get_gate_id() const { return m_gate_id; }
    inline void set_gate_id(uint32_t val) { m_gate_id = val; }
    const network::tcp_socket_ptr get_gate_socket() { return gate_manager_t::get_gate_socket_by_id(m_gate_id); }

	inline uint32_t get_game_id() const { return m_game_id; }
	inline void set_game_id(uint32_t val) { m_game_id = val; }
    const network::tcp_socket_ptr get_game_socket() { return game_manager_t::get_game_socket_by_id(m_game_id); }

    template<typename T_MSG>
    void send_msg_to_game(uint16_t cmd, const T_MSG& protobuf_msg)
    {
        const network::tcp_socket_ptr& p_game_socket = get_game_socket();
        if (NULL == p_game_socket)
        {
            log_error("NULL == p_game_socket");
            return;
        }
        env::server->send_msg_to_game(p_game_socket, cmd, m_uid, protobuf_msg);
    }

    template<typename T_MSG>
    void send_msg_to_client(uint16_t cmd, const T_MSG& protobuf_msg)
    {
        const network::tcp_socket_ptr& p_gate_socket = get_gate_socket();
        if (NULL == p_gate_socket)
        {
            log_error("NULL == p_gate_socket: send cmd[%d]", cmd);
            return;
        }
        env::server->send_msg_to_gate(p_gate_socket, cmd, m_uid, protobuf_msg);
    }

public:
	uint32_t get_channel_id() { return m_common_info.m_channel_id; }

	uint32_t get_create_time() { return m_common_info.m_create_time;  }
private:
    uint64_t m_uid = 0;
    uint32_t m_gate_id = 0;
	uint32_t m_game_id = 0;
    global_user_common_t m_common_info;

	// Ĭ�������ߣ��ȳ�����������½�Ż�����
	ROLE_ONLINE_STATE m_online_state = common::ROLE_ONLINE_STATE_LOGOUT;               //��ɫ�Ƿ�����(������ɾ������)

private:
	friend_mgr_t m_friend_mgr;		    // ����
	mail_user_data_t m_mail_data;		// �ʼ�
	shop_user_data_t m_shop_goods_data;	// �̵���޹���Ϣ
};

typedef boost::shared_ptr<global_user_t> global_user_ptr;

#endif
