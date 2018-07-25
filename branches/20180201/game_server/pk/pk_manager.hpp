#ifndef __GAME_PK_MANAGER_H__
#define __GAME_PK_MANAGER_H__

#include "role/role.hpp"
#include "item/item.hpp"
#include "config/config_manager.hpp"

class pk_manager_t
{
public:
	static uint32_t do_pk(role_ptr p_role, role_ptr p_target, proto::common::role_change_data* p_data);
	static bool attacker_pk_result(uint64_t self_uid, uint64_t enemy_uid, bool is_win, proto::common::role_change_data* p_data);
	static bool defenser_pk_result(uint64_t self_uid, uint64_t enemy_uid, bool is_win, proto::common::role_change_data* p_data);
	//Լս
	static uint32_t do_challenge(role_ptr p_role, uint64_t target_uid);
	//��ӦԼս
	static uint32_t process_challenge(role_ptr p_role, uint32_t process);
	//����
	static void challenge_fight(uint64_t challenger_uid, uint64_t target_uid);
public:
	//ͨ����PKֵ
	static uint32_t get_max_pk_value();
	//�Ƿ���ͨ����
	static bool is_criminal(uint32_t val);
private:
	//�Ƿ��ڰ�ȫ��
	static bool is_safe(role_ptr p_role);
	//�Ƿ���ͨ����
	static bool is_criminal(role_ptr p_role);
	//�Ƿ������ֱ�����
	static bool is_protected(role_ptr p_role);
	//PK����
	static uint32_t get_pk_item();
	static uint32_t get_pk_item_num();
	//ͨ����PKֵ
	static uint32_t get_criminal_value();
	
	//��ɱ���PKֵ
	static uint32_t get_pk_value_add();
	//���ȼ����
	static uint32_t get_delta_level();
	//PK������
	static uint32_t get_pk_distance();
	
public:
	//����PKֵ��ʱ��
	static uint32_t get_pk_value_reduce_time();
	//ÿ�ν��Ͷ���PKֵ
	static uint32_t get_pk_value_reduce();
	//ÿ��PK����
	static uint32_t get_pk_count();
	//ÿ�ձ�PK����
	static uint32_t get_target_count();
	//Լս�۽��
	static uint32_t get_challenge_gold();
	//Ӧս�۽��
	static uint32_t get_accept_challenge_gold();
	//�����������
	static uint32_t get_prison_rate();
	//�ʳ�PK���͵�ID
	static uint32_t get_challenge_point();
	//������ͼID
	static uint32_t get_prison_id();
	//�������͵�ID
	static uint32_t get_prison_point();
	//��ȡ����ֵ����(����Ŀ���PKֵ)
	static uint32_t get_errantry(uint32_t pk_value);
	//��ȡ����ͷ�(�����Լ���PKֵ)
	static uint32_t get_exp_reduce(uint32_t pk_value);
	//�������͵�ID(����ȥ��)
	static uint32_t get_prison_out_point();
	//��ȡ������ʧ�ܱ��ؽ������ļ���(�����Լ���PKֵ)
	static uint32_t get_attacker_lose_prison_rate(uint32_t pk_value);
	//��ȡ��������ʧ�ܱ��ؽ������ļ���(�����Լ���PKֵ)
	static uint32_t get_defenser_lose_prison_rate(uint32_t pk_value);
};

#endif
