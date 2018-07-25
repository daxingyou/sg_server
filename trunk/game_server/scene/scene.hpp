#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "scene_common.hpp"

#include <vector>
#include <map>
#include <set>

#include "object/object_position.hpp"
#include "monster/monster.hpp"
#include "map.hpp"
#include "tblh/Monster.tbls.h"
#include "config/config_manager.hpp"
#include "main/game_server_fwd.hpp"
#include "object/object_manager.hpp"
#include "role/role_manager.hpp"
#include "collect/collect_manager.hpp"

//////////////////////////////////////////////////////////////////////////
class scene_t;
typedef boost::shared_ptr<scene_t> scene_ptr;

class king_war_scene_t;
typedef boost::shared_ptr<king_war_scene_t> king_war_scene_ptr;

class dungeon_scene_t;
typedef boost::shared_ptr<dungeon_scene_t> dungeon_scene_ptr;

class family_war_scene_t;
typedef boost::shared_ptr<family_war_scene_t> family_war_scene_ptr;

// 
struct scene_select_t
{
	virtual ~scene_select_t() {};
	virtual bool exec(const role_ptr& p_role) const = 0;
};

// ������init,Ҫʹ�þ�start ��Ҫ�˾�unload
class scene_t : public scene_base, public boost::enable_shared_from_this<scene_t>
{
public:
	scene_t(uint64_t scene_id, uint32_t map_id, proto::server::em_scene_type scene_type, uint32_t type_param);
	virtual ~scene_t();

	// ��ʼ����������
	virtual bool init();
	// ����ˣ������Լ���գ����ɾ����ͼ���ȵ��ã�,����ű���������֮��ͱ���ɾ���ó�����
	virtual void clear();
	// ���ȫ�����������������
	void clear_all_monster();
	// ��������
	virtual bool start();
	// ж�س���
	bool unload();
	// �������е�ͼ����(��̬npc�пͻ����Լ�����)
	virtual void load_map_object();
	// �������е�ͼ���вɼ���
	virtual void load_map_collect();
	// ��ʼ����������
	bool init_object(monster_ptr p_monster, const monster_common::monster_define_t& define);
	// ��������
	bool revive_object(monster_ptr p_monster, float x, float y, float dir);
	// ��ʼ���ɼ���
	bool init_collect_point(collect_point_ptr p_collect_point, bool is_new = true);
	// �ٻ�һ������
	template <typename Monster_T>
	monster_ptr summon_monster(uint32_t id, float x, float y, float dir = 0, uint64_t object_id = 0);

	collect_point_ptr summon_collect_point(uint32_t tid, float x, float y);

	uint64_t get_scene_id() const { return m_scene_id; }
	uint32_t get_map_tid() const { return m_map_tid; }
	uint32_t get_map_group_id();
	proto::server::em_map_type get_map_type() const { return m_map_type; }
	uint32_t get_city_id();

	// �����Ƿ��Ƿ�������
	void set_diversion(bool is_diversion, uint32_t num);
	uint32_t is_diversion() const { return m_is_diversion; }
	uint32_t get_scene_max_num() const { return m_scene_max_num; }

	// �õ���������
	proto::server::em_scene_type get_scene_type() const { return m_scene_type; }
	// �õ��������Ӳ���
	uint32_t get_scene_type_param() const { return m_type_param; }

	inline map_ptr get_map_ptr() { return m_map_wptr.lock(); }

	// ��ȡ����ָ��
	scene_ptr get_scene_ptr() { return shared_from_this(); }
	// �Ƿ�������
	virtual bool is_king_war_city() { return false; }
	// ��ȡ����ս����ָ��
	virtual king_war_scene_ptr get_king_war_scene_ptr() { return king_war_scene_ptr(); }
	// �Ƿ��Ǹ���
	virtual bool is_dungeon_scene() { return false; }
	// ��ȡ��������ָ��
	virtual dungeon_scene_ptr get_dungeon_scene_ptr() { return dungeon_scene_ptr(); }
	// ��ø���id
	virtual uint32_t get_dungeon_id() { return 0; }
	// �Ƿ��Ǽ���ս����
	virtual bool is_family_war_scene() { return false; }
	// ��ȡ��������ָ��
	virtual family_war_scene_ptr get_family_war_scene_ptr() { return family_war_scene_ptr(); }

	/*�������*/
public:
	// �õ�������ҽ����ĳ�������
	virtual sPos get_scene_born_position(const role_ptr& p_role);
	// ����Ƿ��ܽ�����
	virtual uint32_t scene_enter_check(const role_ptr& p_role) { return 0; }
	// ���볡��
	bool on_enter_s(object_base_ptr p_object, float x, float y, float dir = 0.f);
	// �뿪����
	void on_leave_s(object_id_type object_id);
	void on_leave_s(object_base_ptr p_object);
	// �ƶ�ͬ��
	bool on_move_s(const object_base_ptr& p_object, float to_x, float to_y, float direction, uint64_t time_stamp, uint32_t high);
	// �õ����︽���������
	bool get_nearby_objects(float x, float y, std::vector<object_id_type>& objects_nearby, bool role_only = false, float distance = 0, bool check_block = false);
	// ֪ͨ�ƶ���Ϣ
	void notify_nearbies_move_s(object_base_ptr p_object, const nine_vector& nines);

	// ����9������
	bool object_into_vision(object_base_ptr p_object, const nine_vector& nines);
	// ������Ұ����
	bool role_into_vision(role_ptr p_role, const nine_vector& nines);
	// �����뿪9��
	bool object_out_of_vision(object_base_ptr p_object, const nine_vector& nines);
	// ������Ұ����
	bool role_out_of_vision(role_ptr p_role, const nine_vector& nines);

	// ��ȡ9��������Ϣ
	bool send_nine_to_me(uint32_t nine_key, proto::client::gc_role_into_vision_notify* notify);

	// ��ȡ����ID
	uint32_t get_area_id(float x, float y);

	// �õ������
	uint32_t get_revive_point();

	// �����Ƿ��ܹ����
	virtual uint32_t check_scene_troop();

	/*��ʱ��*/
public:
	// �������ʱ������
	virtual bool reach_max_time(uint32_t curr) const { return false; }
	// ���ﳡ��ʱ������
	virtual bool reach_end_time(uint32_t curr) const { return false; }

	// 1�붨ʱ��
	virtual void update(uint32_t curr_time) {};

	/*�������*/
public:
	virtual void on_role_enter(role_ptr p_role);
	virtual void on_role_leave(role_ptr p_role);

	uint32_t get_role_size() const { return m_roles.size(); }

	// �麯������뿪�����е�һ������Ļص�
	virtual void role_leave_area(role_ptr p_role, uint32_t area_id) {};
	// �麯����ҽ��볡���е�һ������Ļص�
	virtual void role_enter_area(role_ptr p_role, uint32_t area_id) {};

	// ����ĳ���
	virtual void on_revive_role(role_ptr p_role);
	virtual void revive_all_role() {};

	// �����ɱ�˱���
	virtual void on_role_kill_other(role_ptr p_role, object_base_ptr p_enemy) {};

	// �Լ������˻�ɱ
	virtual void on_other_kill_me(role_ptr p_role, object_base_ptr p_enemy);

	// ��ʾ�������
	virtual void show_revive_ui(role_ptr p_role);

	// ��½��������
	virtual void on_login_die(role_ptr p_role);

	// �Ƿ�������ڸ�����ս��
	virtual bool is_have_fight();

	// ��������ս��ʧ��
	virtual void set_all_combat_failed();

	// ǿ����������뿪����(Ĭ���ǿպ���������Щ���������ڵĳ��������������)
	virtual void forced_all_leave() {};

	/*����BOSS���*/
public:
	void add_field_boss(uint64_t object_id);
	void del_field_boss(uint64_t object_id);

	bool scene_have_boss() const { return !m_field_boss.empty(); }

	/* ��Ϣ��� */
public:
	// ����ת���������ѡ�����������ûص�
	template<typename T_MSG>
	void send_msg_to_scene(uint16_t cmd, const T_MSG& msg, const scene_select_t* SELECT = NULL)
	{
		// û��ֱ��return
		if (m_roles.empty())
		{
			return;
		}

		proto::server::ga_broadcast_notify msg_to_gate;

		for (std::set<uint64_t>::const_iterator citr = m_roles.begin();
			citr != m_roles.end(); ++citr)
		{
			if (NULL != SELECT)
			{
				role_ptr p_role = role_manager_t::find_role(*citr);
				if (NULL == p_role || !SELECT->exec(p_role))
				{
					continue;
				}
			}

			msg_to_gate.add_role_list(*citr);
		}

		// ��gateת��
		if (msg_to_gate.role_list_size() > 0)
		{
			msg_to_gate.set_cmd(cmd);

			std::string cmd_data;
			network::wrap_msg(cmd_data, msg);
			msg_to_gate.set_data(cmd_data);

			env::server->send_msg_to_all_gate(op_cmd::ga_broadcast_notify, 0, msg_to_gate);
		}
	}

	// 9��ת��
	void send_msg_to_nine(uint32_t nine_key, uint16_t cmd, const std::string& cmd_data);
	
	// ��Ա����
protected:
	uint64_t m_scene_id = 0;
	uint32_t m_map_tid = 0;
	proto::server::em_scene_type m_scene_type = proto::server::em_scene_type_none;
	uint32_t m_type_param = 0;
	bool m_is_diversion = false;	// �Ƿ����������ķ�������
	uint32_t m_scene_max_num = 0;	// �����������������
	proto::server::em_map_type m_map_type = proto::server::map_type_none;
	map_wptr m_map_wptr;	// map����ָ��

protected:
	// ���Ӷ���
	typedef std::set<object_id_type> scene_grid;
	// ��Ұ��Χ�ڵĸ���
	typedef std::vector<scene_grid*> scene_vision;

	std::set<uint64_t> m_roles;
	uint32_t m_grid_num = 0;	// ��ȫ�����¼������
	scene_grid** m_scene_objects = NULL;//new ά��������ÿ�����Ӧ������

	// һЩ������Ϣ
protected:
	std::set<uint64_t> m_field_boss;	// �ó����е�����id
};

template <typename Monster_T>
monster_ptr scene_t::summon_monster(uint32_t id, float x, float y, float dir/* = 0*/, uint64_t object_id/* = 0*/)
{
	Monster* monster = GET_CONF(Monster, id);
	if (NULL == monster) {
		log_error("scene::load_map_object not found monster [%u]", id);
		return NULL;
	}

	monster_common::monster_define_t define;
	define.id = id;
	define.pos_x = x;
	define.pos_y = y;
	define.dir = dir;

	monster_ptr p_monster(new Monster_T());
	if (NULL == p_monster) {
		log_error("monster null error");
		return NULL;
	}

	if (!p_monster->init_monster(define, monster, object_id)) {
		log_error("monster[%u] init error", define.id);
		return NULL;
	}

	if (!init_object(p_monster, define)) {
		log_error("monster[%u] scene init error", define.id);
		return NULL;
	}

	SCENE_LOG("map[%u] summon monster[%u] pos[%f, %f]", m_map_tid, id, x, y);
	return p_monster;
}

#endif // !__GAME_SCENE_H__
