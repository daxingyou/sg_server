#ifndef __GAME_LUCKYDRAW_MANAGER_H__
#define __GAME_LUCKYDRAW_MANAGER_H__

#include "protos_fwd.hpp"
#include "role/role.hpp"
#include "tblh/Luckydraw.tbls.h"

typedef std::vector<Luckydraw*> luckydraw_list;
typedef std::map<uint32_t, luckydraw_list> luckydraw_pool;
class luckydraw_manager_t
{

public:
	static bool make_luckydraw_pool();
	static void clear_luckydraw_pool();
public:
	//抽卡
	static uint32_t luckydraw(role_ptr p_role, uint32_t type, uint32_t count, proto::client::gc_luckydraw_reply& reply);

	static uint32_t buy_wish_summon(role_ptr p_role, uint32_t summon_id, proto::client::gc_buy_wish_summon_reply& reply);

	static uint32_t open_king_wish_summon(role_ptr p_role, uint32_t summon_id);
private:
	//普通抽卡
	static uint32_t luckydraw_junior(role_ptr p_role, uint32_t count, proto::client::gc_luckydraw_reply& reply);
	//高级抽卡
	static uint32_t luckydraw_senior(role_ptr p_role, uint32_t count, proto::client::gc_luckydraw_reply& reply);
	//获取抽卡收益
	static bool luckydraw_award(role_ptr p_role, Luckydraw* p_conf, proto::client::gc_luckydraw_reply& reply,uint32_t& reply_code);
private:
	//普通抽卡批量最大次数
	static uint32_t get_normal_draw_limit_number();
	//高级抽卡首抽ID
	static uint32_t get_first_draw_id();
	//普通抽卡消耗道具ID
	static uint32_t get_junior_item_id();
	//高级抽卡元宝消耗
	static uint32_t get_senior_yuanbao_consume();

	static uint32_t doluckydraw(role_ptr p_role, uint32_t level, uint32_t draw_type, proto::client::gc_luckydraw_reply& reply );
private:
	static luckydraw_pool s_luckydraw_pool;	//抽卡类型为key的抽卡池
};
#endif
