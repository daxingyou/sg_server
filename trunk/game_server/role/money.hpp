#ifndef __GAME_MONEY_H__
#define __GAME_MONEY_H__

#include "protos_fwd.hpp"
#include <boost/shared_ptr.hpp>

class money_t
{
public:
	money_t(uint64_t uid);
    ~money_t();

	void load_data(const proto::common::personal_info & info);

	void peek_data(proto::common::personal_info * p_info);

public:
	uint32_t	get_money(proto::common::MONEY_TYPE type);
	void		set_money(proto::common::MONEY_TYPE type, uint32_t val);
	
	void		set_luck_gift_gold(uint32_t val) { m_luck_gift_gold = val; }
	uint32_t	get_luck_gift_gold() { return m_luck_gift_gold; }

	void		set_prestige(uint32_t val) { m_prestige = val; }
	uint32_t	get_prestige() { return m_prestige; }
	void		set_prestige_total(uint32_t val) { m_prestige_total = val; }
	uint32_t	get_prestige_total() { return m_prestige_total; }
	void		set_prestige_today(uint32_t val) { m_prestige_today = val; }
	uint32_t	get_prestige_today() { return m_prestige_today; }

	void		set_prestige_level(uint32_t val) { m_prestige_level = val; }
	uint32_t	get_prestige_level() { return m_prestige_level; }
	void		calc_prestige_level();


	//成就
	void		set_achieve_point(uint32_t val) { m_achieve_point = val; }
	uint32_t	get_achieve_point() { return m_achieve_point; }	

	//荣誉
	void		set_honor_week(uint32_t val) { m_honor_week = val; }
	uint32_t	get_honor_week() { return m_honor_week; }
	void		set_honor_limit(uint32_t val) { m_honor_limit = val; }
	uint32_t	get_honor_limit() { return m_honor_limit; }
private:
	uint64_t m_owner = 0;
	uint32_t m_yuanbao = 0;                 //元宝
	uint32_t m_silver = 0;                  //银币
	uint32_t m_copper = 0;                  //铜币
	uint32_t m_energy = 0;					//活力
	uint32_t m_luck_gift_gold = 0;			//返还的聊天红包金币数

	uint32_t m_prestige = 0;				//声望值
	uint32_t m_prestige_total = 0;			//总声望值
	uint32_t m_prestige_level = 0;			//声望等级
	uint32_t m_prestige_today = 0;			//今天获得的声望（有每日上限）
	
	uint32_t m_achieve_point = 0;			//成就积分

	uint32_t m_lifeforce = 0;				//命力值

	uint32_t m_life_stone = 0;				//命石(充值币)

	uint32_t m_herosoul = 0;				//将魂

	uint32_t m_honor = 0;					//荣誉
	uint32_t m_honor_week = 0;				//本周获得的荣誉
	uint32_t m_honor_limit = 0;				//本周获得的荣誉上限
	uint32_t m_contribution = 0;			//功勋
	uint32_t m_multi_point = 0;				//多倍点数
	uint32_t m_role_unlock = 0;				//主角解锁

	uint32_t m_feats = 0;					//功绩

	uint32_t m_friendship_point = 0;		//友情点
};
typedef boost::shared_ptr<money_t> money_ptr;
#endif