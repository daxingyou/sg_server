#ifndef __GLOBAL_SCENE_H__
#define __GLOBAL_SCENE_H__

#include "scene_common.hpp"
#include "server.pb.h"

#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr.hpp>
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "network/tcp_socket.hpp"
#include "game/game_manager.hpp"
#include "gate/gate_manager.hpp"
#include "user/global_user.hpp"

class global_scene_t;
typedef boost::shared_ptr<global_scene_t> global_scene_ptr;

class global_dungeon_t;
typedef boost::shared_ptr<global_dungeon_t> global_dungeon_ptr;

class family_dungeon_t;
typedef boost::shared_ptr<family_dungeon_t> family_dungeon_ptr;

class global_scene_t : public scene_base, public boost::enable_shared_from_this<global_scene_t>
{
public:
	global_scene_t(uint64_t scene_id, uint32_t map_tid);
	virtual ~global_scene_t() {};

	virtual bool init();

public:
	uint64_t get_scene_id() const { return m_scene_id; }
	uint32_t get_map_tid() const { return m_map_tid; }
	proto::server::em_scene_type get_scene_type() const { return m_scene_type; }

	const network::tcp_socket_ptr get_game_socket() { return game_manager_t::get_game_socket_by_id(m_game_id); }

	virtual void set_scene_state(uint32_t set);

	// ����������ж�
	uint32_t check_scene_troop();

	virtual uint32_t check_troop_join();

	// ���볡��
	virtual void on_role_enter(const global_user_ptr& p_user);

	// ����Ƿ��ܽ�����
	virtual uint32_t scene_enter_check(const global_user_ptr& p_user);

public:
	// �Ƿ��Ǹ���
	virtual bool is_dungeon_scene() const { return false; }
	// ��ȡ��������ָ��
	virtual global_dungeon_ptr get_global_dungeon_ptr() { return global_dungeon_ptr(); }

public:
	// �Ƿ��Ǽ���ս����
	virtual bool is_family_dungeon() const { return false; }
	// ��ȡ����ս��������ָ��
	virtual family_dungeon_ptr get_family_dungeon_ptr() { return family_dungeon_ptr(); }

public:
	// �������������
	void add_user(uint64_t uid);
	// ɾ�����
	void del_user(uint64_t uid);
	// ͳ�Ƴ������������
	uint32_t get_user_count() const;
	// ��������������
	void set_max_user(uint32_t val) { m_scene_max_user = val; }
	// �������������
	uint32_t get_max_user() const { return m_scene_max_user; }
	// �����Ƿ��Ƿ�����ͼ
	void set_is_diversion(bool is_diversion) { m_is_diversion = is_diversion; }
	// �����Ƿ��Ƿ�����ͼ
	bool is_diversion() const { return m_is_diversion; }

public:
	template<typename T_MSG>
	void send_msg_to_game(uint16_t cmd, const T_MSG& protobuf_msg)
	{
		if (NULL == env::server)
		{
			log_error("env::server null error!");
			return;
		}

		const network::tcp_socket_ptr& p_socket = get_game_socket();
		if (NULL == p_socket)
		{
			log_error("NULL == p_socket");
			return;
		}
		env::server->send_msg_to_game(p_socket, cmd, 0, protobuf_msg);
	}

protected:
	uint64_t m_scene_id = 0;
	uint32_t m_map_tid = 0;
	proto::server::em_scene_type m_scene_type = proto::server::em_scene_type_none;
	uint32_t m_scene_param = 0;

	std::set<uint64_t> m_scene_users;
	uint32_t m_scene_max_user = 0;
	bool m_is_diversion = false;
};

#endif
