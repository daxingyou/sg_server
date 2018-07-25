#ifndef  _BUSINESS_ACTIVITY_MSG_HANDLER_HPP__
#define  _BUSINESS_ACTIVITY_MSG_HANDLER_HPP__

#include "gate/gate_msg_handler.hpp"

class business_activity_msg_handler_t
{
public:
	static bool init_msg_handler();

	/*ҡǮ��*/
	static bool handle_cg_get_money_tree_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_shake_money_tree_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_levelup_money_tree_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//���յ�¼
	static bool handle_cg_get_7d_login_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_7d_login_prize_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//�¿�
	static bool handle_cg_get_month_card_info_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_buy_month_card_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_get_month_card_prize_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	
	//����
	static bool handle_cg_get_level_fund_info_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_get_level_fund_prize_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_buy_level_fund_prize_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );

	//�һ�
	static bool handle_cg_get_exchange_info_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_get_exchange_prize_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );

	//����
	static bool handle_cg_get_limit_buy_info_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_buy_limit_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );

	//����Ŀ��
	static bool handle_cg_get_sd_target_info_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_get_sd_target_red_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_get_sd_target_prize_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );

	//�۳佱��
	static bool handle_cg_recharge_total_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_recharge_total_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//�ƽ�ʮ��
	static bool handle_cg_gold_luckydraw_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_gold_luckydraw_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//��ʥ����
	static bool handle_cg_activity_shop_get_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_activity_shop_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
