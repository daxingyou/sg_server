#ifndef _SEVENDAY_LOGIN_HPP_
#define _SEVENDAY_LOGIN_HPP_

#include "macros.hpp"
#include "protos_fwd.hpp"
#include <set>
class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class sevenday_login_t 
{
public:
	sevenday_login_t(uint64_t role_id);

	void load_data(const proto::common::sevenday_login_save_data& data);
	void peek_data(proto::common::sevenday_login_save_data &data);
	void save_data();

	void oneday();
	void init_newbie();
public:
	uint32_t get_sevenday_list_info( proto::client::gc_get_7d_login_reply &reply );

	uint32_t get_sevenday_list_prize( uint32_t index, proto::client::gc_get_7d_login_prize_reply &reply );

	void  on_login();

	uint32_t get_prize_status(uint32_t day);

	uint32_t get_red_num();
private:
	std::string m_key = "";
	uint64_t m_role_uid = 0;
	uint8_t m_login_flag = 0;						//计数标记
	uint32_t m_day = 0;
	std::set<uint32_t>m_sevenday_list;	//七日登录奖励数据
};

#endif