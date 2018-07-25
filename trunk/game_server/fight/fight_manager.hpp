#ifndef __FIGHT_MANAGER_H__
#define __FIGHT_MANAGER_H__

#include "tblh/FightAgainst.tbls.h"
#include "tblh/MonsterHero.tbls.h"
#include "role/role.hpp"
#include "combat.hpp"
#include "user_troop/user_troop.hpp"
#include "fight_common.hpp"


#define MAX_CAMP_HERO 6
#define MAX_SUMMON_HERO 2

class fight_manager_t
{
public:
	fight_manager_t();
	~fight_manager_t();

	static bool init();
	static void close();

	// ����ս��
	static bool enter_fight_request(role_ptr p_role, const proto::client::cg_enter_fight_request& msg);

	// ==============���ڿͻ��˼����ս��==============
	//static bool fight_pve_c(role_ptr p_role, uint64_t against_id);
	//static bool fight_pve_c(role_ptr p_role, monster_ptr p_monster);

	// ����ս��
	static bool finish_fight(role_ptr p_role, const proto::common::fight_result& fight_result);

	// ==============���ڷ����������ս��==============
	// pveս��
	static uint64_t fight_pve(
		role_ptr p_role, 
		uint64_t against_id, 
		proto::common::fight_param* param = NULL);
//	static uint64_t fight_pve(role_ptr p_role, monster_ptr p_monster, proto::common::fight_param* param = NULL);
	static uint64_t fight_pve(
		role_ptr p_role, 
		const proto::common::mirror_fight_data& npc, 
		proto::common::fight_param* param = NULL);
	// pvpս��
	static uint64_t fight_pvp(
		role_ptr p_role, 
		role_ptr p_target, 
		proto::common::fight_param* param = NULL);

	// ����ս��
	static bool update_fight(uint64_t fight_uid, const proto::common::fight_state& state);

	//ͬ��ս�����˵�Ѫ��
	static bool sync_enemy_hp(uint64_t fight_uid, const proto::common::hero_state_data& state);

	// �Ƿ��������ս��
	static bool can_fight_with_team(uint32_t fight_type);

	// ����˳���ҵĹ�ս
	static bool check_close_role_watching_state(role_ptr p_role, proto::common::fight_type fight_type);
	// ���ݶ�������ȷ����������
	static uint32_t get_hero_count_by_member_size( uint32_t value );
	// ��ȡ�ٻ�����������
	static uint32_t get_max_summon_count();
	// ��ȡ��Ӫ��������
	static uint32_t get_max_camp_hero_count();

	static proto::common::hero_state* get_hero_state(proto::common::hero_state_data* state, uint32_t pos);
	static proto::common::hero_single* get_hero_single(proto::common::hero_data* data, uint64_t fight_uid);
	static void clear_hero_single_by_master_id(proto::common::hero_data* data, uint64_t master_uid);

	// ��Ⲣ�ر���һ��ս��
	static bool check_and_close_pre_combat( role_ptr p_role );

public:

	static bool fill_fight_camp(
		uint32_t camp_index, 
		FightAgainst* conf,
		uint32_t level, 
		uint32_t team_member_size,
		proto::common::fight_camp* camp,
		proto::common::hero_state_data* state = NULL,
		uint64_t role_uid = 0,
		int32_t team_member_pos = -1,
		proto::common::EM_SCENEOBJECT_TYPE object_type = proto::common::SCENEOBJECT_MONSTER,
		std::string* against_name = NULL );

	static bool fill_fight_camp(
		uint32_t camp_index,
		uint64_t unique_offset,
		uint32_t team_member_size,
		const proto::common::mirror_fight_data& npc,
		proto::common::fight_camp* camp,
		proto::common::hero_state_data* state = NULL);
	
	static bool fill_fight_camp(
		uint32_t camp_index,
		uint64_t unique_offset,
		uint32_t team_member_size,
		int32_t  team_member_pos,
		const object_base_ptr p_object_ptr,
		proto::common::fight_camp* camp,
		proto::common::hero_state_data* state = NULL);
///////////////////////////////
	// �����һ���npc��Ϣ
	static bool fill_obj_single(
		uint32_t camp_index, 
		uint64_t obj_id, 
		uint32_t monster_id, 
		uint32_t main_hero_tid, 
		uint32_t level,
		proto::common::obj_single* single,
		uint32_t team_member_size,
		int32_t team_member_pos,
		proto::common::EM_SCENEOBJECT_TYPE object_type = proto::common::SCENEOBJECT_MONSTER,
		std::string* against_name = NULL);

	static bool fill_obj_single(
		uint32_t camp_index, 
		const proto::common::obj_single& obj, 
		proto::common::obj_single* single,
		uint32_t team_member_size,
		int32_t team_member_pos,
		proto::common::EM_SCENEOBJECT_TYPE object_type = proto::common::SCENEOBJECT_MONSTER);

	static bool fill_obj_single(
		uint32_t camp_index,
		const object_base_ptr p_object_ptr,
		proto::common::obj_single* single,
		uint32_t team_member_size,
		int32_t team_member_pos);
//////////////////////////////////////////
	// ����佫�б�

	static bool fill_hero_data(
		uint32_t camp_index,
		uint64_t master_id,
		uint64_t unique_offset,
		uint32_t level,
		uint32_t show_type,	// ��ʾ���� 0 Ĭ�� Ҳ�������� 1 ������
		uint32_t team_member_size,
		const std::map<uint32_t, uint32_t> &against_map,
		proto::common::hero_data* fhd,
		proto::common::hero_state_data* state = NULL,
		int32_t team_member_pos = -1);

	static bool fill_hero_data(
		uint32_t camp_index,
		uint64_t unique_offset,
		uint64_t master,
		uint32_t team_member_size,
		const proto::common::hero_data& heros,
		proto::common::hero_data* fhd,
		proto::common::hero_state_data* state = NULL);

	static bool fill_hero_data(
		uint32_t camp_index,
		uint64_t unique_offset,
		uint64_t obj_id,
		uint32_t team_member_size,
		uint32_t team_member_pos,
		const proto::common::hero_data& heros,
		proto::common::hero_data* fhd,
		proto::common::hero_state_data* state = NULL);

//////////////////////////////////////
	// ����佫

	static bool fill_hero_single(
		uint32_t camp_index, 
		uint64_t unique_offset, 
		MonsterHero* conf, 
		uint32_t level, 
		proto::common::hero_single* single, 
		uint64_t master, 
		uint32_t pos,
		proto::common::hero_state* state = NULL);

	static bool fill_hero_single(
		uint32_t camp_index,
		uint64_t unique_offset,
		uint64_t obj_id,
		uint32_t pos,
		const proto::common::hero_single& hero,
		proto::common::hero_single* single,
		proto::common::hero_state* state = NULL);
public:
	// ��������
	static uint32_t calc_attr(
		hero_ptr p_hero, 
		attr_map& attrs, 
		proto::common::hero_single* single = NULL, 
		bool fight = false);
	static uint32_t calc_attr(
		const attr_map& add_attr, 
		const attr_map& add_percent, 
		attr_map& attrs, 
		uint32_t hero_tid, 
		uint32_t level, 
		uint32_t grade, 
		uint32_t star, 
		uint32_t wake_up);
	static void calc_base_attr(
		attr_map& attrs, 
		uint32_t hero_tid, 
		uint32_t level, 
		uint32_t grade, 
		uint32_t star, 
		uint32_t wake_up);
	static uint32_t calc_attr(
		uint32_t level, 
		MonsterHero* conf, 
		attr_map& attrs, 
		proto::common::hero_single* single = NULL);
	static void fill_fight_attr(attr_map& attrs, proto::common::attr_data* fa);

	// ��似��
	static bool fill_hero_skill(hero_ptr p_hero, proto::common::hero_single* single);
	static bool fill_hero_skill(MonsterHero* conf, proto::common::hero_single* single);
	static bool fill_hero_skill_data(proto::common::skill_single* skill, uint32_t skill_id, uint32_t skill_level);
	static bool check_skill_fill( uint32_t skill_id, uint32_t level);

	//// ���npc��������
	static bool fill_hero_base(MonsterHero* conf, proto::common::hero_single* single, uint32_t level );

public:

	static bool create_combat(
		role_ptr p_role, 
		const std::vector<object_base_ptr>* team_members,
		uint64_t fight_uid, 
		proto::common::fight_data& fd, 
		bool can_form_before_fight = true,
		combat_type type = combat_type_single_role,
		uint8_t fight_data_type = fight_data_type::fight_data_pvp );
	static combat_ptr get_combat(uint64_t fight_uid);
	static combat_ptr find_combat(uint64_t fight_uid);
	static combat_ptr check_combat(role_ptr p_role, uint64_t fight_uid);
	// loading����
	static bool combat_loading_end_ntf(role_ptr p_role, uint64_t fight_uid);
	// ��������
	static bool combat_disposition_request(role_ptr p_role, uint64_t fight_uid, const proto::common::combat_form_data& action);
	// ����ս������
	static bool combat_play_end_ntf(role_ptr p_role, uint64_t fight_uid, uint32_t hero_att_round);
	// սǰ����
	static bool combat_before_form_ready(role_ptr p_role, uint64_t fight_uid);
	// ����
	static bool request_escape(role_ptr p_role, uint64_t fight_uid);
	// �Զ�ս��
	static bool request_switch_auto_fight(role_ptr p_role, uint64_t fight_uid, uint32_t auto_fight, 
											const proto::client::hero_auto_fight_data* hero_data = NULL);
	static bool on_role_login(role_ptr p_role);
	static bool on_role_logout(role_ptr p_role);
	// ս�� ������
	static bool on_combat_after_attack(combat_ptr combat);
	//����غϽ�����
	static bool on_monster_round_end(combat_ptr combat);
	// ս�����
	static bool on_combat_result(const proto::common::combat_result& combat_result, combat_ptr combat, proto::client::gc_combat_result_ntf& ntf);
	// ս������
	static bool do_combat_reward();
	// ս������
	static bool on_combat_end(const proto::common::combat_result& combat_result, combat_ptr combat);
	// ��¼ս����־
	static void write_combat_db_log(const proto::common::combat_result& combat_result, combat_ptr combat);
	// ��ս
	static void watch_combat(role_ptr p_role, uint64_t watching_role_uid, uint64_t fight_uid = 0, bool b_is_troop_back = false);
	// ȡ����ս
	static void cancel_watch_combat(role_ptr p_role, bool b_send_msg = false);
	// ս����Ӫ����
	static bool on_combat_result_camp(
		const proto::common::combat_result& combat_result,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf,
		uint camp,
		const proto::common::fight_camp& camp_data);
	// ���׹ֻ�ʤ
	static uint32_t do_fight_type_dark_monster(
		role_ptr p_role,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf
		);
	// ������ﾭ��
	static void add_role_exp(
		role_ptr p_role,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf,
		uint32_t role_exp,
		uint32_t coeff
		);
	// ���Ӣ�۾���
	static void add_hero_exp(
		role_ptr p_role,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf,
		uint32_t hero_exp,
		uint32_t coeff
		);
protected:

	static uint64_t gen_fight_uid();
	static void check_remove();
	static void remove_all();
	static bool regist_callback();
	static int32_t check_find_empty_hero_pos(proto::common::hero_data *hero_data, uint32_t pos);

protected:
	typedef bool(*combat_round_callback)(
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat);

	typedef bool(*combat_result_callback)(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);

	typedef bool(*combat_end_notify)(const proto::common::combat_result& combat_result, combat_ptr combat);

	static bool on_attack_field_boss(
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat);

	static bool on_attack_family_long(
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat);

	static bool on_task_fight_result(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);

	static bool on_dark_fight_result(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);

	static bool on_family_long_result(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);

	static bool on_field_boss_result(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);

	static bool on_light_fight_result(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);

	static bool on_against_fight_result(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);
	
	static bool on_pk_result(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);

	static bool on_family_pk_result(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);

	static bool on_tower_result(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);

	static bool on_expedition_result(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);

	static bool on_page_pass_fight_result(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);

	static bool on_offline_arena_result(
		const proto::common::combat_result& result,
		const proto::common::obj_single& self,
		const proto::common::fight_common& common,
		const proto::common::fight_camp& self_camp,
		const proto::common::fight_camp& enemy_camp,
		combat_ptr combat,
		proto::client::gc_combat_result_ntf& ntf);
	
	static void get_win_lose_obj(const proto::common::combat_result& combat_result, combat_ptr combat, 
								 proto::common::obj_single* win_obj, proto::common::obj_single* lose_obj,
								 proto::common::hero_data *win_heros, proto::common::hero_data *lose_heros);
	static bool on_cross_fight_test_end(const proto::common::combat_result& combat_result, combat_ptr combat);
	static bool on_cross_arena_rank_end(const proto::common::combat_result& combat_result, combat_ptr combat);
	static bool on_fight_study_end(const proto::common::combat_result& combat_result, combat_ptr combat);
	static bool on_challenge_end(const proto::common::combat_result& combat_result, combat_ptr combat);
	static bool on_light_fight_end(const proto::common::combat_result& combat_result, combat_ptr combat);
	static bool on_family_long_end(const proto::common::combat_result& combat_result, combat_ptr combat);
	static bool on_field_boss_end(const proto::common::combat_result& combat_result, combat_ptr combat);
	// �ж���ӻ��Ǹ���ս�� ��ȡ���� ������Ϣ
	static bool check_fill_team_member(
		role_ptr p_role, 
		uint32_t fight_type, 
		uint32_t &level, 
		std::vector<object_base_ptr> &team_member_list);

	// ��ȡ����ȼ�
	static int get_team_level(const std::vector<object_base_ptr> &team_member_list);

	// ����ս������ӳ�
	static void get_exp_addition( uint32_t fight_type, uint32_t member_size, uint32_t& hero_exp, uint32_t& role_exp );

	//������Ӿ���ӳ�
	static uint32_t get_troop_exp_2();
	//������Ӿ���ӳ�
	static uint32_t get_troop_exp_3();
private:

	typedef std::map<uint64_t, combat_ptr> combat_map;
	static combat_map m_combats;
	static uint32_t create_combat_count;			// test
	static uint32_t remove_combat_count;			// test

	static std::set<uint64_t> m_remove_set;

	typedef std::map<proto::common::fight_type, combat_round_callback> combat_round_callback_map;
	typedef std::map<proto::common::fight_type, combat_result_callback> combat_result_callback_map;
	typedef std::map<proto::common::fight_type, combat_end_notify> combat_end_notify_map;

	static combat_round_callback_map m_combat_round_callback;
	static combat_result_callback_map m_combat_result_callback;
	static combat_end_notify_map m_combat_end_notify_map;
	static combat_round_callback_map m_combat_monster_round_callback_map;
};



#endif//__FIGHT_MANAGER_H__
