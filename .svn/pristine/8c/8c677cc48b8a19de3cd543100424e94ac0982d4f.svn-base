#ifndef  _BUSINESS_ACTIVITY_MSG_HANDLER_HPP__
#define  _BUSINESS_ACTIVITY_MSG_HANDLER_HPP__

#include "gate/gate_msg_handler.hpp"

class business_activity_msg_handler_t
{
public:
	static bool init_msg_handler();

	/*Ò¡Ç®Ê÷*/
	static bool handle_cg_get_money_tree_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_shake_money_tree_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_levelup_money_tree_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//ÆßÈÕµÇÂ¼
	static bool handle_cg_get_7d_login_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_7d_login_prize_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//ÔÂ¿¨
	static bool handle_cg_get_month_card_info_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_buy_month_card_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_get_month_card_prize_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	
	//»ù½ð
	static bool handle_cg_get_level_fund_info_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_get_level_fund_prize_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_buy_level_fund_prize_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );

	//¶Ò»»
	static bool handle_cg_get_exchange_info_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_get_exchange_prize_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );

	//ÇÀ¹º
	static bool handle_cg_get_limit_buy_info_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_buy_limit_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );

	//ÆßÈÕÄ¿±ê
	static bool handle_cg_get_sd_target_info_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_get_sd_target_red_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );
	static bool handle_cg_get_sd_target_prize_request( const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf );

	//ÀÛ³ä½±Àø
	static bool handle_cg_recharge_total_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_recharge_total_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//»Æ½ðÊ®Á¬
	static bool handle_cg_gold_luckydraw_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_gold_luckydraw_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//ÎäÊ¥±¦¿â
	static bool handle_cg_activity_shop_get_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_activity_shop_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
