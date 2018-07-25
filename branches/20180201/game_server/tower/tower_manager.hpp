#ifndef __GAME_TOWER_MANAGER_H__
#define __GAME_TOWER_MANAGER_H__

#include "tower.hpp"
#include "protos_fwd.hpp"
#include "role/role.hpp"

class tower_manager_t
{

public:
	//�ֶ�����
	static uint32_t fight(role_ptr p_role);
	//�ֶ��������
	static void fight_result(role_ptr p_role, bool iswin, uint64_t trigger_uid, proto::common::role_change_data* rcd);
	//ɨ��
	static uint32_t auto_fight(role_ptr p_role, uint32_t& auto_fight_time);
	//����ɨ��
	static uint32_t auto_fight_immediately(role_ptr p_role, uint32_t& auto_fight_time, proto::common::role_change_data* p_data);
	//��ȡɨ������
	static uint32_t auto_fight_reward(role_ptr p_role, proto::common::tower_data* p_info, proto::common::role_change_data* p_data);
	//����
	static uint32_t reset(role_ptr p_role, uint32_t& current_tower, uint32_t& reset_count);
	//��ȡ�ɾͽ���
	static uint32_t achieve_reward(role_ptr p_role, uint32_t& current_achieve, proto::common::role_change_data* p_data);
	//�������عؿ�
	static uint32_t fight_trigger(role_ptr p_role, uint64_t trigger_uid);
private:
	static uint32_t get_auto_fight_immediately_gold();
};

#endif
