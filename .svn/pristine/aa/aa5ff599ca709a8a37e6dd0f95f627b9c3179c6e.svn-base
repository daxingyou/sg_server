#ifndef __GAME_LOG_WRAPPER_HPP__
#define __GAME_LOG_WRAPPER_HPP__

#include "main/game_server_fwd.hpp"
#include "protos_fwd.hpp"
#include "log_enum.hpp"
#include "item/item.hpp"
#include <vector>

class log_wrapper_t
{
public:
    // 角色ID gateID gameID
    static void send_role_login_log(uint64_t role_uid, uint32_t gate_id, uint32_t game_id);
    
	static void send_role_logout_log(uint64_t role_uid, uint32_t online_time, uint32_t gate_id, uint32_t game_id, 
									 uint32_t kick_reason);

    // 角色ID 角色等级 物品UID 物品type 旧数量 变化数量 新数量 操作类型 消耗钱币数 钱币类型 消耗源 消耗源参数
	static void send_item_log(uint64_t role_uid, uint32_t role_level, uint64_t item_uid, uint32_t item_tid, uint32_t old_num, 
							  uint32_t up_num, uint32_t new_num, uint32_t op_type, uint32_t source_type,
							  uint32_t source_param);

	// 角色ID 角色等级 旧数量 变化数量 新数量 钱币类型 操作类型 消耗源 消耗源参数
	static void send_money_log(uint64_t role_uid, uint32_t role_level, uint32_t old_num, uint32_t up_num, uint32_t new_num,
							   uint32_t money_type, uint32_t op_type, uint32_t source_type,
							   uint32_t source_param);

	// 角色ID 角色等级 充值渠道 充值获得的元宝 充值赠送的元宝 最新的元宝数 消费现金 充值订单号 游戏内充值的订单号 设备信息
	static void send_chart_success_log(uint64_t role_uid, uint32_t role_level, const std::string& pay_channel, 
									   uint32_t yuanbao, uint32_t free_yuanbao, uint32_t new_yuanbao, uint32_t cash, 
									   std::string &chart_order, std::string &game_chart_order, proto::common::mobile_info& mobile);

	// 发红包角色ID 抢红包角色ID 红包ID 金额(抢红包ID为0就是发的 不为0就是抢的) 红包剩余金额
	static void send_luck_gift_log(uint64_t src_role_uid, uint64_t grab_role_uid, uint64_t luck_gift_uid, uint32_t old_num,
									uint32_t up_num, uint32_t new_num);

	
	// 角色ID 角色等级 武将UID 武将tid  操作类型  操作来源
	static void send_hero_log(uint64_t role_uid, uint32_t role_level, uint64_t hero_uid, uint32_t hero_tid, uint32_t op_type,
							  uint32_t source_type, uint32_t old_num = 0, uint32_t up_num = 0, uint32_t new_num = 0, uint32_t source_param = 0);

	// 角色ID 任务ID 任务状态 任务进度 接取时间 操作类型
	static void send_task_log(uint64_t role_uid, uint32_t task_id, uint32_t task_state, const std::string& progress, 
								uint32_t accept_time, uint32_t op_type);

	// 角色ID 角色等级 变化前声望 变化前声望累计 变化前声望等级 变化的声望 变化后声望 变化后声望累计 变化后声望等级 操作类型 消耗源
	static void send_prestige_log(uint64_t role_uid, uint32_t role_level, uint32_t old_prestige, uint32_t old_prestige_total,
									uint32_t old_prestige_level, uint32_t up_prestige, uint32_t new_prestige, uint32_t new_prestige_total, 
									uint32_t new_prestige_level, uint32_t op_type, uint32_t source_type, uint32_t source_param);

	// 角色ID 角色等级 旧数量 变化数量 新数量 PK值类型 操作类型 消耗源 消耗源参数
	static void send_pk_log(uint64_t role_uid, uint32_t role_level, uint32_t old_num, uint32_t up_num, uint32_t new_num,
		uint32_t value_type, uint32_t op_type, uint32_t source_type,
		uint32_t source_param);

	// 角色ID 商品ID 商品数量 消耗货币类型 消耗货币数量	获得道具id 获得道具数量 操作类型  操作来源
	static void send_user_shop_log(uint64_t role_uid, uint32_t goods_tid, uint32_t goods_num, uint32_t cost_type, uint32_t cost_num,
		uint32_t item_id, uint32_t item_num, uint32_t source_type, uint32_t source_param);

	// 角色ID 商店ID 商品ID 消耗货币类型 消耗货币数量 消耗道具id 消耗道具数量 老的数量 变化后的数量 操作类型
	static void send_random_shop_log(uint64_t role_uid, uint32_t shop_tid, uint32_t goods_tid, uint32_t cost_type, uint32_t cost_value,
		uint32_t cost_item, uint32_t cost_item_value, uint32_t old_num, uint32_t new_num, uint32_t source_type);

	// 角色ID 角色等级 装备 操作类型  操作来源
	static void send_equip_log(uint64_t role_uid, uint32_t role_level, item_ptr p_equip, uint32_t op_type, uint32_t source_type, uint32_t source_param);


	/* 角色ID 成就ID  变化前成就等级 等级变化值 变化后成就等级 变化前成就进度 进度改变值 变化后进度值 
	 * 变化前成就积分 改变的成就积分 变化后成就积分 变化前成就等级 改变的成就等级 变化后成就等级
	 * 操作来源 成就类型
	*/
	static void send_achieve_log(uint64_t role_uid, uint32_t achieve_tid, uint32_t old_level, 
								uint32_t up_level,uint32_t new_level, uint32_t old_count,
								uint32_t add_count ,uint32_t new_count, uint32_t old_achieve_point, 
								uint32_t add_achieve_point, uint32_t new_achieve_point, 
								uint32_t old_achieve_level, uint32_t up_achieve_level, 
								uint32_t new_achieve_level, uint32_t old_reward_get_level,
								uint32_t new_reward_get_level,uint32_t source_type, uint32_t source_param,const std::string& source_src);

	// 角色ID 变化前境界等级 变化前命星等级 变化的境界等级 变化的命星等级 变化后的境界等级 变化后的命星等级 操作来源 消耗的命力值/完成的突破任务ID
	static void send_lifeforce_log(uint64_t role_uid, uint32_t old_lifestate_level, uint32_t old_lifestar_level,uint32_t up_lifestate,
				uint32_t up_lifestar, uint32_t new_lifestate_level,uint32_t new_lifestar_level,uint32_t source_type, uint32_t source_param);

	// 角色ID 副本ID 副本创建类型 副本创建id 操作类型  操作来源
	static void send_dungeon_log(uint64_t role_uid, uint32_t dungeon_id, uint32_t create_type, uint64_t create_id,
		uint32_t source_type, uint32_t source_param);

	// 角色ID buffID 操作类型  真实速度 操作来源 操作来源参数
	static void send_role_buff_log(uint64_t role_uid, uint32_t buff_id, uint32_t op_type, uint32_t speed,
		uint32_t source_type, uint32_t source_param);

	///角色ID new_guide_id 操作来源 操作来源参数
	static void send_new_guide_log(uint64_t role_uid, uint32_t new_guide_id, uint32_t source_type, uint32_t source_param);

	///角色ID new_guide_id 操作来源 操作来源参数
	static void send_trade_log(uint64_t role_uid, uint32_t item_tid,uint32_t price,uint32_t num,uint32_t tax,uint32_t total_num, uint32_t total_cost,
		uint32_t gain, uint32_t trade_id, uint32_t op_type, uint32_t source_type);

	///角色ID 订单号 充值ID 充值金额  渠道ID 日期
	static void send_recharge_log(uint64_t role_uid, const std::string& order_id,uint32_t recharge_tid,uint32_t recharge_rmb, uint32_t channel_id,const std::string& date_str);

	// 角色ID 角色等级 行为 参数
	static void send_expedition_log(uint64_t role_uid, uint32_t role_level, uint32_t source_type, uint32_t source_param);

	// 角色ID 角色旧等级 旧经验 增加/减少经验 新等级 新经验 行为 参数
	static void send_role_exp_log(uint64_t role_uid, uint32_t old_level, uint32_t old_exp, uint32_t up_exp, uint32_t new_level, uint32_t new_exp, uint32_t source_type, uint32_t source_param);

	// 角色ID 角色等级 战斗类型 战斗方式 队伍成员(可无) 目标(至少一个) 战斗UID 是否赢了 战斗开始时间 战斗结束时间
	static void send_role_combat_log(uint64_t role_uid, uint32_t role_level, uint32_t fight_type, uint32_t combat_type,
			const std::vector<uint64_t>& team_member, const std::vector<uint64_t>& targets, uint64_t fight_uid, uint32_t is_win,
			uint32_t start_time, uint32_t finish_time);

	// 角色ID 角色等级 武将唯一ID 武将策划表ID 武将旧等级 旧经验 增加经验 新等级 新经验 行为 参数
	static void send_hero_exp_log(uint64_t role_uid, uint32_t role_level, uint64_t hero_uid, uint32_t hero_tid, uint32_t old_level, uint32_t old_exp, uint32_t up_exp,
		uint32_t new_level, uint32_t new_exp, uint32_t source_type, uint32_t source_param);
};


#endif
