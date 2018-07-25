#include "money.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "role/role.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role_manager.hpp"
#include "tblh/Prestige.tbls.h"
#include "config_mgr.h"
#include "tblh/Prestige.tbls.h"
#include <limits>
#include "center/center_msg_handler.hpp"
USING_NS_COMMON;
money_t::money_t(uint64_t uid)
	: m_owner(uid)
{

}

money_t::~money_t()
{

}

void money_t::load_data(const proto::common::personal_info& info)
{
	m_copper = info.copper();
	m_silver = info.silver();
	m_yuanbao = info.yuanbao();
	m_energy = info.energy();
	m_luck_gift_gold = info.luck_gift_gold();
	m_prestige = info.prestige();
	m_prestige_total = info.prestige_total();
	m_prestige_level = info.prestige_level();
	m_achieve_point = info.achieve_point();
	m_lifeforce = info.lifeforce();
	m_prestige_today = info.prestige_today();
	m_life_stone = info.life_stone();
	m_herosoul = info.herosoul();
	m_honor = info.honor();
	m_honor_week = info.honor_week();
	m_honor_limit = info.honor_limit();
	m_contribution = info.contribution();
	m_multi_point = info.multi_point();
	m_role_unlock = info.role_unlock();
	m_feats = info.feats();
	m_friendship_point = info.friendship_point();
}

void money_t::peek_data(proto::common::personal_info* p_info)
{
	p_info->set_prestige(m_prestige);
	p_info->set_prestige_total(m_prestige_total);
	p_info->set_prestige_level(m_prestige_level);
	p_info->set_prestige_today(m_prestige_today);
	p_info->set_yuanbao(m_yuanbao);
	p_info->set_silver(m_silver);
	p_info->set_copper(m_copper);
	p_info->set_energy(m_energy);
	p_info->set_luck_gift_gold(m_luck_gift_gold);
	p_info->set_achieve_point(m_achieve_point);
	p_info->set_lifeforce(m_lifeforce);
	p_info->set_life_stone(m_life_stone);
	p_info->set_herosoul(m_herosoul);
	p_info->set_honor(m_honor);
	p_info->set_honor_week(m_honor_week);
	p_info->set_honor_limit(m_honor_limit);
	p_info->set_contribution(m_contribution);
	p_info->set_multi_point(m_multi_point);
	p_info->set_role_unlock(m_role_unlock);
	p_info->set_feats(m_feats);
	p_info->set_friendship_point(m_friendship_point);
}

uint32_t money_t::get_money(proto::common::MONEY_TYPE type)
{
	switch (type)
	{
	case proto::common::MONEY_TYPE_YUANBAO:
		return m_yuanbao;
	case proto::common::MONEY_TYPE_SILVER:
		return m_silver;
	case proto::common::MONEY_TYPE_COPPER:
		return m_copper;
	case proto::common::MONEY_TYPE_ENERGY:
		return m_energy;
    case proto::common::MONEY_TYPE_PRESTIGE:
        //只能得到声望等级，和声望相关的变量需要重新定义枚举
        return m_prestige_level;
	case proto::common::MONEY_TYPE_ACHIEVE_POINT:
		return m_achieve_point;
	case proto::common::MONEY_TYPE_LIFEFORCE:
		return m_lifeforce;
	case proto::common::MONEY_TYPE_LIFE_STONE:
		return m_life_stone;
	case proto::common::MONEY_TYPE_HEROSOUL:
		return m_herosoul;
	case proto::common::MONEY_TYPE_HONOR:
		return m_honor;
	case proto::common::MONEY_TYPE_CONTRIBUTION:
		return m_contribution;
	case proto::common::MONEY_TYPE_MULTI_POINT:
		return m_multi_point;
	case proto::common::MONEY_TYPE_ROLE_UNLOCK:
		return m_role_unlock;
	case proto::common::MONEY_TYPE_FEATS:
		return m_feats;
	case proto::common::MONEY_TYPE_FRIENDSHIP_POINT:
		return m_friendship_point;
	default:
		log_error("invalid money type[%d]!!!", type);
		break;
	}

	return 0;
}

void money_t::set_money(proto::common::MONEY_TYPE type, uint32_t val)
{
	switch (type)
	{
	case proto::common::MONEY_TYPE_YUANBAO:
		m_yuanbao = val;
		break;
	case proto::common::MONEY_TYPE_SILVER:
		m_silver = val;
		break;
	case proto::common::MONEY_TYPE_COPPER:
		m_copper = val;
		break;
	case proto::common::MONEY_TYPE_ENERGY:
		m_energy = val;
		break;
	case proto::common::MONEY_TYPE_ACHIEVE_POINT:
		m_achieve_point = val;
		break;
	case proto::common::MONEY_TYPE_LIFEFORCE:
		m_lifeforce = val;
		break;
	case proto::common::MONEY_TYPE_LIFE_STONE:
		m_life_stone = val;
		break;
	case proto::common::MONEY_TYPE_HEROSOUL:
		m_herosoul = val;
		break;
	case proto::common::MONEY_TYPE_HONOR:
		m_honor = val;
		break;
	case proto::common::MONEY_TYPE_CONTRIBUTION:
		m_contribution = val;
		// 同步家族贡献给center
		center_msg_handler_t::sync_data_to_center(m_owner, proto::server::data_sync_type_family_contribution, m_contribution, true);
		break;
	case proto::common::MONEY_TYPE_MULTI_POINT:
		m_multi_point = val;
		break;
	case proto::common::MONEY_TYPE_ROLE_UNLOCK:
		m_role_unlock = val;
		break;
	case proto::common::MONEY_TYPE_FEATS:
		m_feats = val;
		break;
	case proto::common::MONEY_TYPE_FRIENDSHIP_POINT:
		m_friendship_point = val;
		break;
	default:
        log_error("invalid money type[%d]!!!", type);
		break;
	}
}

void money_t::calc_prestige_level()
{
	while (true)
	{
		auto pres_conf = GET_CONF(Prestige, m_prestige_level);
		if (NULL == pres_conf)
		{
			log_error("role[%lu] not find prestige_level[%d] in the config", m_owner, m_prestige_level);
			return;
		}
		if (m_prestige_total < (uint32_t)pres_conf->exp())
		{
			return;
		}
		// 升级
		if (NULL == GET_CONF(Prestige, m_prestige_level+1))
		{
			//最高等级了不升级
			return;
		}
		++m_prestige_level;
	}
}
