#ifndef __GAME_KING_WAR_MAN_H__
#define __GAME_KING_WAR_MAN_H__

#include <map>

#include "king_war_common.hpp"
#include <boost/shared_ptr.hpp>
#include "server.pb.h"
#include "client.pb.h"
#include "role/role.hpp"
#include "scene/scene_manager.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

typedef boost::shared_ptr<country_scene_info_t> country_scene_info_ptr;
typedef std::map<uint32_t, country_scene_info_ptr> country_scene_map;

class king_war_manager_t
{
public:
	static bool close_clear();

public:
	// ���������������
	static bool add_country_scene(uint32_t country_id, uint64_t scene_id);

	// ֪ͨ��������ս״̬
	static void update_king_war_state(uint32_t country_id, const proto::server::eg_king_war_state_notify& notify);

	// ֪ͨ����ս������ʱ�䵽��
	static void king_war_ending(uint32_t country_id);

	// ���㽱��
	static void king_war_settlement(uint32_t country_id);

	// �õ��Լ����ҳ���������ս����
	static void get_king_war_family_num(uint32_t country_id, proto::client::gc_king_war_scene_num_reply& notify);

	// �����������ս����
	static uint32_t ask_enter_king_war_scene(role_ptr p_role);

	// ��������������
	static uint32_t ask_defence_long(role_ptr p_role, uint64_t object_id);

	// ���󹥻�����
	static uint32_t ask_attack_long(role_ptr p_role, uint64_t object_id);

	// ������������
	static void attack_long_end(role_ptr p_role, uint64_t object_id);

	// ��������˺�(�ж�)
	static void ask_hurt_long(role_ptr p_role, uint64_t object_id, proto::client::gc_king_war_attack_end_reply& reply);

	// ����ʹ������
	static void user_artifacts(role_ptr p_role, uint32_t index);

	// ��������ս���
	static void ask_king_war_result(role_ptr p_role, proto::client::gc_king_war_result_reply& reply);

public:
	// ����ս��������
	static void add_long_gas(uint32_t country_id, uint64_t family_id, uint32_t val);

	// ��һ�ɱ����
	static void role_kill_other(role_ptr p_role, role_ptr p_enemy);

	// �Լ����˻�ɱ
	static void other_kill_self(role_ptr p_role, role_ptr p_enemy);

	// �����������
	static bool role_fast_revive(role_ptr p_role);

	// ������������ս��
	static void attack_long_add(role_ptr p_role);

	// gm����ս��
	static void gm_add_exploit(role_ptr p_role, uint32_t val);

public:
	static country_scene_info_ptr get_country_info_by_id(uint32_t country_id);

	static king_war_role_ptr get_king_war_role_ex(uint32_t country_id, uint64_t uid);
	static king_war_role_ptr get_king_war_role(uint32_t country_id, uint64_t uid);

	// ��ɱ��õĽ���
	static std::pair<uint32_t, uint32_t> get_reward_by_self_title(em_kill_title title);
	// �ս��õĽ���
	static std::pair<uint32_t, uint32_t> get_reward_by_other_title(em_kill_title title);
	// ��ɱ��Ա�Ľ���
	static std::pair<uint32_t, uint32_t> get_reward_by_other_offer(uint32_t offer);

	// �õ�title
	static em_kill_title get_kill_title(uint32_t kill_num);

public:
	static bool exploit_rank_sort(const king_war_role_ptr& h1, const king_war_role_ptr& h2);

	// �ص����ھ�̬�࣬��ֹtime cancelʱ�򣬿��ܻ����cancelʧ�ܣ���Ϊ��ʱ���Ѿ��ڵ��ö�������
public:
	static void on_long_revive_timer_call_back(const boost::system::error_code& ec, uint64_t object_id);

	static void on_long_occupied_timer_call_back(const boost::system::error_code& ec, uint64_t object_id);

public:
	static country_scene_map m_country_scene_map;
};

#endif
