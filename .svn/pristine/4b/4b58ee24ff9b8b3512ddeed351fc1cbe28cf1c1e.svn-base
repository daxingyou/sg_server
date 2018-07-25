#include "cultivate_manager.hpp"
#include "hero_manager.hpp"
#include "tblh/HeroGrade.tbls.h"
#include "tblh/HeroStar.tbls.h"
#include "tblh/Hero.tbls.h"
#include "tblh/HeroTalentAdd.tbls.h"
#include "tblh/HeroGet.tbls.h"
#include "tblh/LevelDemad.tbls.h"
#include "tblh/HeroTalent.tbls.h"
#include "tblh/HeroTalentLevel.tbls.h"
#include "tblh/HeroFate.tbls.h"
#include "tblh/HeroFateLv.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/SkillTable.tbls.h"
#include "tblh/SkillEffectTable.tbls.h"
#include <stdlib.h>
#include "utility.hpp"
#include "item/item_manager.hpp"
#include "log_enum.hpp"
#include "tblh/errcode_enum.hpp"
#include "log/log_wrapper.hpp"
#include "task/task_manager.hpp"
#include "role/money_manager.hpp"
#include "achieve/achieve_common.hpp"
#include "common/ContainerParse.h"
#include "sys_notice/sys_notice_manager.hpp"

USING_NS_COMMON;
USING_NS_CONFIG;
std::map<uint32_t, uint32_t> cultivate_manager_t::levelup_item;
bool cultivate_manager_t::init()
{
	auto p_levelup_item = GET_CONF(Comprehensive, comprehensive_common::level_up_mat);
	if (NULL == p_levelup_item)
	{
		log_error("star_prob_str null error");
		return false;
	}
	Dawn::parseMapContainer<uint32_t, uint32_t>(levelup_item, p_levelup_item->parameter1().c_str(), ":$");
	if (levelup_item.empty())
	{
		log_error("star_prob_str null error");
		return false;
	}
	return true;
}

uint32_t cultivate_manager_t::get_star_item()
{
	auto star_item = GET_CONF(Comprehensive, comprehensive_common::Star_item);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(star_item);
	return val;
}

uint32_t cultivate_manager_t::get_talent_stone()
{
	auto talent_stone = GET_CONF(Comprehensive, comprehensive_common::Talent_stone);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(talent_stone);
	return val;
}

uint32_t cultivate_manager_t::get_init_talent_count()
{
	auto talent_count = GET_CONF(Comprehensive, comprehensive_common::hero_talent_init);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(talent_count);
	return val;
}

uint32_t cultivate_manager_t::get_plugin_star()
{
	auto awake_star_num = GET_CONF(Comprehensive, comprehensive_common::awake_star_num);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(awake_star_num);
	return val;
}

uint32_t cultivate_manager_t::cultivate_hero(role_ptr p_role, const proto::client::cg_hero_cultivate_request& msg, uint32_t& out, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	CULTI_LOG("role[%lu] cultivate_hero begin", p_role->get_uid());
	hero_ptr p_hero = hero_manager_t::get_hero(p_role, msg.hero_id());
	if (NULL == p_hero)
	{
		log_error("hero_manager_t::get_hero[%d]  NULL!!", msg.hero_id());
		return errcode_enum::notice_hero_null;
	}
	if (p_hero->get_hero_state() == proto::common::hero_state_type_lock)
	{
		log_error("hero[%d] hero_state = lock", p_hero->get_tid());
		return false;
	}

	std::vector<uint32_t> va_list;
	for (int32_t i = 0; i < msg.value_size(); ++i)
	{
		va_list.push_back(msg.value(i));
	}
	uint32_t value1 = 0;
	uint32_t value2 = 0;
	if (va_list.size() >= 1)
	{
		value1 = va_list[0];
	}
	if (va_list.size() >= 2)
	{
		value2 = va_list[1];
	}
	uint32_t ret;
	switch (msg.type())
	{
	case proto::common::EMCUTYPE_LEVEL:
		ret = levelup_hero(p_role, p_hero, value1, value2, p_data);
		break;
	case proto::common::EMCUTYPE_STARUP:
		ret = starup_hero(p_role, p_hero, p_data);
		break;
	case proto::common::EMCUTYPE_ADVANCED:
		ret = advance_hero(p_role, p_hero, p_data);
		break;
	case proto::common::EMCUTYPE_SKILLUP:
	case proto::common::EMCUTYPE_SKILL_UNLOCK:
		{
			ret = levelup_skill(p_role, p_hero, value1, p_data);
			out = value1;
		}
		break;
	case proto::common::EMCUTYPE_WAKEUP:
		ret = wakeup_hero(p_role, p_hero, p_data);
		break;
	case proto::common::EMCUTYPE_SKILL_REPLACE:
		ret = replace_skill(p_role, p_hero, value1, value2, p_data);
		break;
	default:
		ret = errcode_enum::notice_unknown;
		break;
	}
	CULTI_LOG("role[%lu] cultivate_hero end", p_role->get_uid());
	proto::common::hero_data* p_hero_data = p_data->mutable_hero();
	proto::common::hero_single* p_hero_single = p_hero_data->add_hero_list();
	p_hero->peek_data(p_hero_single);
	return ret;
}

uint32_t cultivate_manager_t::levelup_hero(role_ptr p_role, hero_ptr p_hero, uint32_t item_id, uint32_t item_num, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_hero)
	{
		log_error("NULL == p_hero");
		return errcode_enum::notice_hero_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	if (p_hero->get_hero_state() == proto::common::hero_state_type_lock)
	{
		log_error("hero[%d] hero_state = lock", p_hero->get_tid());
		return false;
	}
	CULTI_LOG("role[%lu] levelup_hero[%d] begin", p_role->get_uid(), p_hero->get_tid());
	//if (p_hero->get_unique_id() == p_role->get_main_hero_uid())
	//{
	//	log_error("main_hero can't levelup");
	//	return errcode_enum::notice_unknown;
	//}
	uint32_t add_exp = 0;
	if (levelup_item.find(item_id) == levelup_item.end())
	{
		log_error("levelup_item[%d] invalid", item_id);
		return errcode_enum::notice_item_type_not_match;
	}
	add_exp = levelup_item[item_id];
	
	if (!item_manager_t::has_item(p_role, item_id, item_num))
	{
		log_error("role[%lu] levelup_item[%d:%d] not enough", p_role->get_uid(), item_id, item_num);
		return errcode_enum::notice_item_not_enough;
	}
	// 等级判断
	uint32_t cur_level = p_hero->get_current_level();
	uint32_t old_level = cur_level;
	uint32_t old_exp = p_hero->get_current_exp();
	if (cur_level >= p_role->get_level())
	{
		log_error("role[%lu] hero[%d] level[%d] must less than role_level[%d]", p_role->get_uid(), p_hero->get_tid(), cur_level, p_role->get_level());
		return errcode_enum::notice_role_levle_not_enough;
	}
	if (cur_level >= hero_manager_t::get_hero_level_limit())
	{
		log_error("role[%lu] hero[%d] level[%d] must less than hero_level_limit[%d]", p_role->get_uid(), p_hero->get_tid(), cur_level, hero_manager_t::get_hero_level_limit());
		return errcode_enum::notice_role_levle_not_enough;
	}
	auto p_hero_config = GET_CONF(Hero, p_hero->get_tid());
	if (NULL == p_hero_config)
	{
		log_error("p_hero_config[%d] not found!!", p_hero->get_tid());
		return errcode_enum::notice_unknown;
	}
	for (uint32_t i = 0; i < item_num; ++i)
	{
		uint32_t reply_code = item_manager_t::remove_item(p_role, item_id, 1, log_enum::source_type_hero_levelup, p_hero->get_tid(), p_data);
		if (reply_code != errcode_enum::error_ok)
		{
			break;
		}
		hero_manager_t::add_exp(p_role, p_hero->get_unique_id(), add_exp, log_enum::source_type_hero_levelup, true);
		CULTI_LOG("role[%lu] hero[%lu] addexp[%d]", p_role->get_uid(), p_hero->get_unique_id(), add_exp);
		if (p_hero->get_current_level() >= p_role->get_level())
		{
			break;
		}
		if (cur_level >= hero_manager_t::get_hero_level_limit())
		{
			break;
		}
	}
	
	p_hero->save_self(p_role->get_uid());
	CULTI_LOG("role[%lu] levelup_hero[%d] success, level_exp from [%d:%d] to [%d:%d]", p_role->get_uid(), p_hero->get_tid(), old_level, old_exp, p_hero->get_current_level(), p_hero->get_current_exp());
	log_wrapper_t::send_hero_log( p_role->get_uid(), p_role->get_level(), p_hero->get_unique_id(), p_hero->get_tid(), log_enum::opt_type_set,
								  log_enum::source_type_hero_levelup, old_level, p_hero->get_current_level() - old_level, p_hero->get_current_level() );
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_LEVEL, p_hero->get_current_level());
	return errcode_enum::error_ok;
}

uint32_t cultivate_manager_t::starup_hero(role_ptr p_role, hero_ptr p_hero, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_hero)
	{
		log_error("NULL == p_hero");
		return errcode_enum::notice_hero_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	if (p_hero->get_hero_state() == proto::common::hero_state_type_lock)
	{
		log_error("hero[%d] hero_state = lock", p_hero->get_tid());
		return false;
	}
	CULTI_LOG("role[%lu] starup_hero[%d] begin", p_role->get_uid(), p_hero->get_tid());
	auto p_hero_config = GET_CONF(HeroStar, hash_util_t::hash_to_uint32(p_hero->get_tid(), p_hero->get_star()));
	if (NULL == p_hero_config)
	{
		log_error("HeroStar p_hero_config[%d:%d] not found!!", p_hero->get_tid(), p_hero->get_star());
		return errcode_enum::notice_unknown;
	}
	auto p_next_hero_config = GET_CONF(HeroStar, hash_util_t::hash_to_uint32(p_hero->get_tid(), p_hero->get_star()+1));
	if (NULL == p_next_hero_config)
	{
		log_error("role[%lu] hero[%d] star max!!", p_role->get_uid(), p_hero->get_tid());
		return errcode_enum::notice_unknown;
	}
	if ((int)p_hero->get_current_level() < p_hero_config->need_level())
	{
		log_error("role[%lu] hero[%d] level[%d] not enough[%d]!!", p_role->get_uid(), p_hero->get_tid(), p_hero->get_current_level(), p_hero_config->need_level());
		return errcode_enum::notice_unknown;
	}
	if ((int)money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_COPPER) < p_hero_config->consume_money())
	{
		log_error("role[%lu] hero[%d] can't starup_hero! copper[%d] not enough", p_role->get_uid(), p_hero->get_tid(), p_hero_config->consume_money());
		return errcode_enum::notice_copper_money_not_enough;
	}
	if ((int)money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_HEROSOUL) < p_hero_config->consume_herosoul())
	{
		log_error("role[%lu] hero[%d] can't starup_hero! herosoul[%d] not enough", p_role->get_uid(), p_hero->get_tid(), p_hero_config->consume_herosoul());
		return errcode_enum::notice_copper_money_not_enough;
	}
	if (errcode_enum::error_ok != item_manager_t::remove_items(p_role, p_hero_config->consume_material(), log_enum::source_type_hero_starup, p_hero->get_tid(), p_data))
	{
		log_error("role[%lu] hero[%d] star_item is not enough!!", p_role->get_uid(), p_hero->get_tid());
		return errcode_enum::notice_item_not_enough;
	}
	money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_COPPER, p_hero_config->consume_money(), log_enum::source_type_hero_starup, p_hero->get_tid());
	money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_HEROSOUL, p_hero_config->consume_herosoul(), log_enum::source_type_hero_starup, p_hero->get_tid());

	uint32_t old_star = p_hero->get_star();
	uint32_t new_star = old_star;
	++new_star;
	
	p_hero->set_star(new_star);
	p_hero->save_self(p_role->get_uid());
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	CULTI_LOG("role[%lu] starup_hero[%d] success,from[star:%d] to[star:%d]", p_role->get_uid(), p_hero->get_tid(), old_star, new_star);
	log_wrapper_t::send_hero_log(p_role->get_uid(), p_role->get_level(), p_hero->get_unique_id(), p_hero->get_tid(), log_enum::opt_type_set, log_enum::source_type_hero_starup, old_star, 1, new_star);

	//成就通知
	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_STAR, p_hero->get_star());
	return errcode_enum::error_ok;
}

uint32_t cultivate_manager_t::advance_hero(role_ptr p_role, hero_ptr p_hero, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_hero)
	{
		log_error("NULL == p_hero");
		return errcode_enum::notice_hero_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	if (p_hero->get_hero_state() == proto::common::hero_state_type_lock)
	{
		log_error("hero[%d] hero_state = lock", p_hero->get_tid());
		return false;
	}
	CULTI_LOG("role[%lu] advance_hero[%d] begin", p_role->get_uid(), p_hero->get_tid());
	uint32_t old_grade = p_hero->get_grade();
	auto p_hero_config = GET_CONF(HeroGrade, hash_util_t::hash_to_uint32(p_hero->get_tid(), old_grade));
	if (NULL == p_hero_config)
	{
		log_error("p_hero_config[%d:%d] not found!!", p_hero->get_tid(), old_grade);
		return errcode_enum::notice_unknown;
	}
	auto p_next_hero_config = GET_CONF(HeroGrade, hash_util_t::hash_to_uint32(p_hero->get_tid(), old_grade + 1));
	if (NULL == p_next_hero_config)
	{
		log_error("role[%lu] hero[%d] grade max!!", p_role->get_uid(), p_hero->get_tid());
		return errcode_enum::notice_unknown;
	}
	if (p_hero_config->need_level() > p_hero->get_current_level())
	{
		log_error("role[%lu] hero[%d] level[%d] not enough, need[%d]!!", p_role->get_uid(), p_hero->get_tid(), p_hero->get_current_level(), p_hero_config->need_level());
		return errcode_enum::notice_hero_levle_not_enough;
	}

	if (money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_COPPER) < p_hero_config->consume_money())
	{
		log_error("role[%lu] hero[%d] can't starup_hero! copper[%d] not enough", p_role->get_uid(), p_hero->get_tid(), p_hero_config->consume_money());
		return errcode_enum::notice_copper_money_not_enough;
	}

	if (errcode_enum::error_ok != item_manager_t::remove_items(p_role, p_hero_config->consume_material(), log_enum::source_type_hero_advanced, p_hero->get_tid(), p_data))
	{
		log_error("role[%lu] hero[%d] advanced_item is not enough!!", p_role->get_uid(), p_hero->get_tid());
		return errcode_enum::notice_item_not_enough;
	}
	money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_COPPER, p_hero_config->consume_money(), log_enum::source_type_hero_advanced, p_hero->get_tid());
	p_hero->set_grade(p_hero->get_grade() + 1);
	p_hero->save_self(p_role->get_uid());
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	CULTI_LOG("role[%lu] advance_hero[%d] success, grade from [%d] to [%d]", p_role->get_uid(), p_hero->get_tid(), old_grade, old_grade + 1);
	log_wrapper_t::send_hero_log(p_role->get_uid(), p_role->get_level(), p_hero->get_unique_id(), p_hero->get_tid(), log_enum::opt_type_set, log_enum::source_type_hero_advanced, old_grade, 1, p_hero->get_grade());

	//成就通知
	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_TYPE,p_hero->get_grade());
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL != p_task_mgr)
	{
		p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_HERO_GRADE, *p_data, 1, 1);
	}
	return errcode_enum::error_ok;
}

uint32_t cultivate_manager_t::levelup_skill(role_ptr p_role, hero_ptr p_hero, uint32_t skill_id, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_hero)
	{
		log_error("NULL == p_hero");
		return errcode_enum::notice_hero_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	if (p_hero->get_hero_state() == proto::common::hero_state_type_lock)
	{
		log_error("hero[%d] hero_state = lock", p_hero->get_tid());
		return false;
	}

	Hero* p_hero_conf = GET_CONF(Hero, p_hero->get_tid());
	if (NULL == p_hero_conf)
	{
		log_error("NULL == p_hero_conf[%d]", p_hero->get_tid());
		return false;
	}
	if (p_hero_conf->wakeup_skill_type() == proto::common::hero_wakeup_type_unlock &&
		p_hero_conf->wakeup_skill_id() == skill_id)
	{
		log_error("levelup_skill error skill[%d] is wakeup_lock", skill_id);
		return false;
	}

	hero_skill hero_skill_data;
	if(!p_hero->find_hero_skill(skill_id, hero_skill_data))
	{
		log_error("role[%lu] hero[%d] can't levelup_skill! skill[%d] not found", p_role->get_uid(), p_hero->get_tid(), skill_id);
		return errcode_enum::notice_unknown;
	}
	auto p_skill_effect_config = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill_id, hero_skill_data.skill_level));
	if (NULL == p_skill_effect_config)
	{
		log_error("p_skill_effect_config[%d:%d] not found!!", skill_id, hero_skill_data.skill_level);
		return errcode_enum::notice_unknown;
	}
	auto p_next_skill_effect_config = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill_id, hero_skill_data.skill_level +1));
	if (NULL == p_next_skill_effect_config)
	{
		log_error("p_next_skill_effect_config[%d:%d] not found!!", skill_id, hero_skill_data.skill_level +1);
		return errcode_enum::notice_unknown;
	}
	if (money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_COPPER) < p_skill_effect_config->levelup_money())
	{
		log_error("role[%lu] hero[%d] can't levelup_skill[%d]! copper[%d] not enough", p_role->get_uid(), p_hero->get_tid(), skill_id, p_skill_effect_config->levelup_money());
		return errcode_enum::notice_copper_money_not_enough;
	}
	if (money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_HEROSOUL) < p_skill_effect_config->levelup_herosoul())
	{
		log_error("role[%lu] hero[%d] can't levelup_skill[%d]! herosoul[%d] not enough", p_role->get_uid(), p_hero->get_tid(), skill_id, p_skill_effect_config->levelup_herosoul());
		return errcode_enum::notice_copper_money_not_enough;
	}
	uint32_t consume_id = p_skill_effect_config->levelup_consume().first;
	uint32_t consume_num = p_skill_effect_config->levelup_consume().second;
	uint32_t reply_code = item_manager_t::remove_item(p_role, consume_id, consume_num, log_enum::source_type_hero_levelup_skill, skill_id, p_data);
	if (reply_code != errcode_enum::error_ok)
	{
		log_error("role[%lu] hero[%d] can't levelup_skill[%d]! item not enough", p_role->get_uid(), p_hero->get_tid(), skill_id);
		return errcode_enum::notice_item_not_enough;
	}
	money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_COPPER, p_skill_effect_config->levelup_money(), log_enum::source_type_hero_levelup_skill, skill_id, true, p_data);
	money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_HEROSOUL, p_skill_effect_config->levelup_herosoul(), log_enum::source_type_hero_levelup_skill, skill_id, true, p_data);

	p_hero->set_hero_skill_level(skill_id, hero_skill_data.skill_level + 1);
		
	p_hero->save_self(p_role->get_uid());
	proto::common::personal_info *p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	CULTI_LOG("role[%lu] hero[%d] levelup_skill[%d] success", p_role->get_uid(), p_hero->get_tid(), skill_id);
	log_wrapper_t::send_hero_log(p_role->get_uid(), p_role->get_level(), p_hero->get_unique_id(), p_hero->get_tid(), log_enum::opt_type_set, log_enum::source_type_hero_levelup_skill, hero_skill_data.skill_level,1, hero_skill_data.skill_level+1, skill_id);

	//成就通知
	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_SKILL_LEVEL, hero_skill_data.skill_level + 1);

	return errcode_enum::error_ok;
}

uint32_t cultivate_manager_t::wakeup_hero(role_ptr p_role, hero_ptr p_hero, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_hero)
	{
		log_error("NULL == p_hero");
		return errcode_enum::notice_hero_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	if (p_hero->get_hero_state() == proto::common::hero_state_type_lock)
	{
		log_error("hero[%d] hero_state = lock", p_hero->get_tid());
		return false;
	}
	CULTI_LOG("role[%lu] wakeup_hero[%d] begin", p_role->get_uid(), p_hero->get_tid());
	if (p_hero->get_wakeup() != 0)
	{
		log_error("role[%lu] hero[%d] already wakeup!", p_role->get_uid(), p_hero->get_tid());
		return errcode_enum::notice_unknown;
	}
	Hero *p_hero_config = GET_CONF(Hero, p_hero->get_tid());
	if (NULL == p_hero_config)
	{
		log_error("Hero p_hero_config[%d] not found!!", p_hero->get_tid());
		return errcode_enum::notice_unknown;
	}
	
	if ((int)money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_COPPER) < p_hero_config->wakeup_consume_money())
	{
		log_error("role[%lu] hero[%d] can't wakeup! copper[%d] not enough", p_role->get_uid(), p_hero->get_tid(), p_hero_config->wakeup_consume_money());
		return errcode_enum::notice_copper_money_not_enough;
	}

	if (errcode_enum::error_ok != item_manager_t::remove_items(p_role, p_hero_config->wakeup_consume_material(), log_enum::source_type_hero_wakeup, p_hero->get_tid(), p_data))
	{
		log_error("role[%lu] hero[%d] can't wakeup! item is not enough!!", p_role->get_uid(), p_hero->get_tid());
		return errcode_enum::notice_item_not_enough;
	}
	money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_COPPER, p_hero_config->wakeup_consume_money(), log_enum::source_type_hero_wakeup, p_hero->get_tid());


	hero_manager_t::check_hero_wakeup_add_plugin(p_role->get_uid(), p_hero->get_tid());
	p_hero->set_wakeup(1);

	
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	CULTI_LOG("role[%lu] wakeup hero[%d] success", p_role->get_uid(), p_hero->get_tid());
	log_wrapper_t::send_hero_log(p_role->get_uid(), p_role->get_level(), p_hero->get_unique_id(), p_hero->get_tid(), log_enum::opt_type_set, log_enum::source_type_hero_wakeup);

	if (p_hero_config->wakeup_skill_type() == proto::common::hero_wakeup_type_add)
	{
		p_hero->add_hero_skill(p_hero_config->wakeup_skill_id());
	}
	else if (p_hero_config->wakeup_skill_type() == proto::common::hero_wakeup_type_replace)
	{
		p_hero->replace_hero_skill(p_hero_config->wakeup_skill_replace(), p_hero_config->wakeup_skill_id());
	}
	else if (p_hero_config->wakeup_skill_type() == proto::common::hero_wakeup_type_unlock)
	{
		hero_skill hero_skill_data;
		if (!p_hero->find_hero_skill(p_hero_config->wakeup_skill_id(), hero_skill_data))
		{
			log_error("wake_up unlock skill[%d] fail", p_hero_config->wakeup_skill_id());
			return errcode_enum::notice_unknown;
		}
		p_hero->set_hero_skill_level(p_hero_config->wakeup_skill_id(), hero_skill_data.skill_level + 1);
	}
	p_hero->save_self(p_role->get_uid());
	
	//成就通知
	//achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_STAR, p_hero->get_star());

	std::vector<std::string> param_str_list;
	param_str_list.push_back(common::string_util_t::uint32_to_string(p_hero->get_tid()));
	sys_notice_manager_t::sys_notice_notify(p_role->get_uid(), proto::common::sys_notify_type_hero_wakeup, 0, param_str_list);
	///BROADCAST_SYS_NOTICE_SYS_CHANNEL(p_role->get_uid(), proto::common::sys_notify_type_hero_wakeup, 0,p_hero_config->name().c_str());
	return errcode_enum::error_ok;
}

uint32_t cultivate_manager_t::unlock_skill(role_ptr p_role, hero_ptr p_hero, uint32_t skill_id, proto::common::role_change_data* p_data)
{
	return errcode_enum::error_ok;
}

uint32_t cultivate_manager_t::replace_skill(role_ptr p_role, hero_ptr p_hero, uint32_t old_skill_id, uint32_t new_skill_id, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_hero)
	{
		log_error("NULL == p_hero");
		return errcode_enum::notice_hero_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	if (p_hero->get_hero_state() == proto::common::hero_state_type_lock)
	{
		log_error("hero[%d] hero_state = lock", p_hero->get_tid());
		return false;
	}

	hero_skill old_hero_skill_data;
	if (!p_hero->find_hero_skill(old_skill_id, old_hero_skill_data))
	{
		log_error("role[%lu] hero[%d] old_skill_id[%d] not found", p_role->get_uid(), p_hero->get_tid(), old_skill_id);
		return errcode_enum::notice_unknown;
	}

	SkillEffectTable* p_skill_effect_config = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(old_skill_id, old_hero_skill_data.skill_level));
	if (NULL == p_skill_effect_config)
	{
		log_error("p_skill_effect_config[%d:%d] not found!!", old_skill_id, old_hero_skill_data.skill_level);
		return errcode_enum::notice_unknown;
	}

	if (p_skill_effect_config->type() == proto::common::skill_type_atk)
	{
		log_error("skill[%d:%d] type = skill_type_atk can't replace", old_skill_id, old_hero_skill_data.skill_level);
		return errcode_enum::notice_unknown;
	}

	hero_skill new_hero_skill_data;
	if (!p_hero->find_hero_skill(new_skill_id, new_hero_skill_data))
	{
		log_error("role[%lu] hero[%d] new_skill_id[%d] not found", p_role->get_uid(), p_hero->get_tid(), new_skill_id);
		return errcode_enum::notice_unknown;
	}
	// 给新技能设置位置
	p_hero->set_hero_skill_pos(new_skill_id, old_hero_skill_data.skill_pos);
	// 旧技能位置改成0
	p_hero->set_hero_skill_pos(old_skill_id, 0);

	p_hero->save_self(p_role->get_uid());
	proto::common::personal_info *p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	CULTI_LOG("role[%lu] hero[%d] replace_skill [%d] to [%d] success", p_role->get_uid(), p_hero->get_tid(), old_skill_id, new_skill_id);
	log_wrapper_t::send_hero_log(p_role->get_uid(), p_role->get_level(), p_hero->get_unique_id(), p_hero->get_tid(), log_enum::opt_type_set, log_enum::source_type_hero_replace_skill, old_skill_id, 0, new_skill_id);
	return errcode_enum::error_ok;
}

uint32_t cultivate_manager_t::recruit_hero(role_ptr p_role, uint32_t hero_tid, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	if (NULL != hero_manager_t::get_hero(p_role, hero_tid))
	{
		log_error("role[%lu] hero[%d] can't recruit! hero already exist", p_role->get_uid(), hero_tid);
		return errcode_enum::notice_hero_repeat;
	}
	auto p_hero_conf = GET_CONF(Hero, hero_tid);
	if (NULL == p_hero_conf)
	{
		log_error("hero[%d] can't recruit! p_hero_conf not found", hero_tid);
		return errcode_enum::notice_unknown;
	}
	
	if (errcode_enum::error_ok != item_manager_t::remove_item(p_role, p_hero_conf->material_id(), p_hero_conf->compose_count(), log_enum::source_type_hero_recruit, hero_tid, p_data))
	{
		log_error("role[%lu] hero[%d] can't recruit! hire_item[%d] not enough", p_role->get_uid(), hero_tid, p_hero_conf->material_id());
		return errcode_enum::notice_item_not_enough;
	}
	hero_manager_t::add_hero(p_role, hero_tid, log_enum::source_type_hero_recruit, p_data);
	auto p_task_mgr = p_role->get_task_mgr();
	if (NULL != p_task_mgr)
	{
		p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_RECRUIT, *p_data, hero_tid, 1);
	}
	proto::common::personal_info *p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	return errcode_enum::error_ok;
}

void cultivate_manager_t::get_unrecruit_hero(role_ptr p_role, std::vector<uint32_t>& tids)
{
	std::map<uint32_t, HeroGet*> hero_get_map;
	GET_ALL_CONF(HeroGet, hero_get_map);
	for (auto recruit_conf : hero_get_map)
	{
		auto conf = recruit_conf.second;
		hero_ptr p_hero = hero_manager_t::get_hero(p_role, conf->id());
		if (p_hero != NULL) // 已拥有
		{
			continue;
		}

		tids.push_back(conf->id());
	}
}
