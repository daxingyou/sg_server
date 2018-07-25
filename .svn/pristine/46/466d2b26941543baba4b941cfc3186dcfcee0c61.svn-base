#ifndef __LUCKYDRAW_MGR_HPP__
#define __LUCKYDRAW_MGR_HPP__

#include <map>
#include "user/global_user.hpp"

typedef std::map<uint32_t, proto::common::wish_summon_info> wish_summon_map;
class luckydraw_mgr_t
{
public:
	static bool init();

	static bool clear();

	static bool save_wish_info( uint32_t country_id );

	static bool save_king_money( uint32_t country_id, uint32_t money );
public:
	static uint32_t open_king_wish_summon_req( const global_user_ptr &p_user , uint32_t summon_id);		//开启国王礼包

	static uint32_t buy_wish_summon_req(const global_user_ptr &p_user, uint32_t summon_id);				//购买国王礼包

	static uint32_t get_wish_summon_info_request(const global_user_ptr &p_user, proto::client::ec_get_wish_summon_info_reply &reply);

	static uint32_t get_king_money(const global_user_ptr &p_user, bool force = false);
public:
	static uint32_t on_buy_wish_summon( const global_user_ptr &p_user , uint32_t summon_id );		//购买国王礼包返回

	static uint32_t on_open_wish_summon(const global_user_ptr &p_user , uint32_t summon_id, proto::client::ec_open_summon_wish_reply &reply);
private:
	static wish_summon_map sys_wish_summon_map;

	static std::map<uint32_t, wish_summon_map> king_wish_summon_map;

	static std::map<uint32_t, uint32_t> king_money_map;	//国家对应的钱信息
};

#endif