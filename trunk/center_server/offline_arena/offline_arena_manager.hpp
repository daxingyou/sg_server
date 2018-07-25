#ifndef __CENTER_OFFLINE_ARENA_MANAGER_H__
#define __CENTER_OFFLINE_ARENA_MANAGER_H__

#include "offline_arena.hpp"
#include "tblh/OfflineArenaClass.tbls.h"
#include "tblh/OfflineArenaMatch.tbls.h"
#include "tblh/OfflineArenaAward.tbls.h"

typedef std::map<uint64_t, offline_arena_ptr> offline_arena_map;
typedef std::map<uint32_t, offline_arena_ptr> offline_arena_map_by_rank;
typedef std::map<uint32_t, offline_arena_map_by_rank> offline_arena_map_by_class;
#define MAX_ARENA_CLASS 4

enum rank_op_type
{
	rank_op_type_new = 1,
	rank_op_type_class = 2,
	rank_op_type_up = 3,
	rank_op_type_down = 4,
};

class offline_arena_manager_t
{
public:
	offline_arena_manager_t(){}
	~offline_arena_manager_t(){}
public:
	static void load_data();

	static void one_day();

	static void award(uint32_t id, bool is_in_time);
private:
	static void add_arena(offline_arena_ptr p_arena);
	//�½�ɫ����
	static offline_arena_ptr add_new_arena(uint64_t role_uid);
	//�ı�����
	static void change_rank(offline_arena_ptr p_arena, uint32_t cur_class, uint32_t rank);
public:
	//��ȡ��������
	static void peek_data(uint64_t role_uid, proto::client::ec_offline_arena_data_reply& reply);
	//��ȡ�������
	static void peek_enemy_data(uint64_t role_uid, proto::common::mirror_fight_data* p_data);
	//��ȡ���а�����
	static void peek_rank_data(uint64_t role_uid, uint32_t cur_class, proto::client::ec_offline_arena_rank_reply& reply);
	//��ȡս������
	static void peek_record_data(uint64_t role_uid, proto::common::offline_arena_record_data* p_data);
	//����ɨ��
	static uint32_t sweep(uint64_t self_uid, uint32_t self_rank, uint32_t enemy_type, uint64_t enemy_id, uint32_t enemy_rank, uint32_t count);
	//���󹥴�
	static uint32_t fight(uint64_t self_uid, uint32_t self_rank, uint32_t enemy_type, uint64_t enemy_id, uint32_t enemy_rank);
	//������
	static void fight_result(uint64_t role_uid, uint32_t is_win);
	//��ȡ��ʷ�����������
	static void record_award(uint64_t role_uid);
	//��ȡ���ֽ���
	static void score_award(uint64_t role_uid, uint32_t score);
	//���򹥴����
	static uint32_t buy(uint64_t role_uid, uint32_t money);
	//����֪ͨ
	static void form_notify(uint64_t role_uid, const proto::common::mirror_fight_data& form_data);
	//��ҵȼ��仯
	static void update_role_level(uint64_t role_uid, uint32_t level);
	//GM����ı�����
	static void gm_rank(uint64_t role_uid, uint32_t rank);
	//��ȡ�������ݵ�ս����
	static uint32_t get_offline_arena_fighting(uint64_t role_uid);
private:
	//���ݵȼ���ȡ�ֶ�����
	static OfflineArenaClass* get_class_conf_by_level(uint32_t level);
	//����������ȡƥ������
	static OfflineArenaMatch* get_match_conf_by_rank(uint32_t rank);
	//��ȡָ�������ĸſ���Ϣ
	static void peek_general_data(uint32_t cur_class, uint32_t rank, proto::common::offline_arena_general* p_single);
	//��ȡָ�������Ļ����˸ſ���Ϣ
	static void peek_robot_general_data(uint32_t cur_class, uint32_t rank, proto::common::offline_arena_general* p_single);
	//��ȡָ�������Ļ�����
	static uint32_t get_robot_by_rank(uint32_t cur_class, uint32_t rank);
public:
	//���ݷֶ�������ȡ��������
	static OfflineArenaAward* get_award_conf_by_rank(uint32_t cur_class, uint32_t rank);
	//��󹥴����
	static uint32_t get_fight_count();
	//����Ĺ������
	static uint32_t get_buy_fight_count();
	//��սǰ10�����������
	static uint32_t get_fight_top_ten_rank();
	//ɨ����͵ȼ�
	static uint32_t get_sweep_level();
	//ʤ����û���
	static uint32_t get_win_score();
	//ʧ�ܻ�û���
	static uint32_t get_lose_score();
private:
	static offline_arena_map m_offline_arena_map;				//���е����� ��role_uidΪKEY
	static offline_arena_map_by_class m_offline_arena_map_by_class;	//������������ ��classΪһ��key����rankΪ����key
	static std::map<uint32_t, uint32_t> s_buy_info;				//������Ϣ
};

#endif