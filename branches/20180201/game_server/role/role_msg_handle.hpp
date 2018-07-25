#ifndef __GAME_ROLE_MSG_HANDLE_H__
#define __GAME_ROLE_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class role_msg_handle_t
{
public:
    static bool init_msg_handler();
    
	static bool handle_cg_gm_cmd_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// ��ɫ�ƶ�
	static bool handle_cg_role_move_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
   /* static bool handle_cg_cost_item_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);*/
	static bool handle_cg_hero_cultivate_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_hero_recruit_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// ����ս������
	static bool handle_cg_enter_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// ����PKģʽ
	static bool handle_cg_change_pk_mode_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// �һ�����
	static bool handle_cg_exchange_money_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// �ı����״̬
	static bool handle_cg_change_sys_state_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// ���󸴻�
	static bool handle_cg_object_revive_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// ����μ��������
	static bool handle_cg_world_cup_play_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// ����Ͷע
	static bool handle_cg_world_cup_bet_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// ������ȡͶע����
	static bool handle_cg_world_cup_bet_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    // �����������
    static bool handle_cg_finish_new_role_guide_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// �л�ģ������
	static bool handle_cg_change_hero_plugin_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// ��ֵ
	static bool handle_cg_recharge_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// Ѳ��
	static bool handle_cg_patrol_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// ��õȼ�����
	static bool handle_cg_get_level_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// �����
	static bool handle_cg_redbag_send_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//�������߾�������ս����
	static bool handle_cg_offline_arena_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// ��ֵ
	static bool handle_cg_recharge_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// �����佫����
	static bool handle_cg_role_main_hero_unlock_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// �����л�
	static bool handle_cg_role_main_hero_change_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// ������
	static bool handle_cg_buy_vigour_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
	
	//����GMȨ��
	static bool handle_cg_set_gm_level(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif // !__GAME_ROLE_MSG_HANDLE_H__