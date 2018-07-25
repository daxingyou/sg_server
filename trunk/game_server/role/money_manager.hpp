#ifndef __GAME_MONEY_MANAGER_H__
#define __GAME_MONEY_MANAGER_H__

#include "money.hpp"
#include "protos_fwd.hpp"
#include "role/role.hpp"
class money_manager_t
{
public:
	//货币
	static uint32_t get_money(role_ptr p_role, proto::common::MONEY_TYPE type);
	static bool		add_money(role_ptr p_role, proto::common::MONEY_TYPE type, uint32_t count, uint32_t source_type, uint32_t source_param, bool notice = true, proto::common::role_change_data* p_data = NULL,bool is_notify = true);
	static bool		use_money(role_ptr p_role, proto::common::MONEY_TYPE type, uint32_t count, uint32_t source_type, uint32_t source_param, bool notice = true, proto::common::role_change_data* p_data = NULL);
	static bool		exchange_money(role_ptr p_role, proto::common::MONEY_TYPE source_type, uint32_t count, proto::common::MONEY_TYPE dest_type, proto::common::role_change_data* p_data = NULL);
	
    //红包
	static void		add_luck_gift_gold(role_ptr p_role, uint32_t luck_gift_gold);
	static void		subtract_luck_gift_gold(role_ptr p_role, uint32_t luck_gift_gold);
public:
	//荣誉
	static void		add_honor(role_ptr p_role, uint32_t count, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data = NULL);
	//友情点
	static void		add_friendship_point(role_ptr p_role, uint32_t amount, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data = NULL);
private:
	//声望
	static void		add_prestige(role_ptr p_role, uint32_t count, uint32_t source_type, uint32_t source_param);
	static bool		use_prestige(role_ptr p_role, uint32_t count, uint32_t source_type, uint32_t source_param);
	static uint32_t get_prestige_level(role_ptr p_role);
	//成就积分
	static void		add_achieve_point(role_ptr p_role, uint32_t count, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data = NULL);

	
	//活力上限
	static uint32_t get_energy_limit();
public:
	// 精力
	static uint32_t role_buy_vigour(const role_ptr& p_role);
};

#endif
