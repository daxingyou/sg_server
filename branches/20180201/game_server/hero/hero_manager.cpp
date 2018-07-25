#include "hero_manager.hpp"
#include "global_id.hpp"
#include "tblh/HeroFate.tbls.h"
#include "tblh/HeroFateLv.tbls.h"
#include "tblh/HeroJobTransfer.tbls.h"
#include "tblh/HeroGrade.tbls.h"
#include "tblh/Hero.tbls.h"
#include "tblh/LevelDemad.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "cultivate_manager.hpp"
#include "log/log_wrapper.hpp"
#include "config_mgr.h"
#include "task/bounty_manager.hpp"
#include "item/item_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "achieve/achieve_common.hpp"
#include "tblh/mailcode_enum.hpp"
#include "mail/mail_manager.hpp"
#include "tblh/HeroPlugin.tbls.h"
#include "role/role_manager.hpp"
#include "role/money_manager.hpp"
//#include "achieve/achieve_manager.hpp"

void hero_manager_t::load_all_datas(role_ptr p_role, const proto::common::hero_data& data)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL!");
		return;
	}

	p_role->m_hero_list.clear();
	for (int32_t i = 0; i < data.hero_list_size(); ++i)
	{
		proto::common::hero_single single = data.hero_list(i);
		create_hero(p_role, single);
	}
	//HERO_LOG("[fj hero]role[%lu] load all hero from db: size[%d]", p_role->get_uid(), data.hero_list_size());
}

void hero_manager_t::peek_all_datas(role_ptr p_role, proto::common::hero_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL!");
		return;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}

	auto it = p_role->m_hero_list.begin();
	auto it_end = p_role->m_hero_list.end();
	hero_ptr p_hero;
	int32_t send_num = 0;
	for (; it != it_end; ++it)
	{
		p_hero = it->second;
		if (NULL != p_hero)
		{
			proto::common::hero_single* p_single = p_data->add_hero_list();
			p_hero->peek_data(p_single);
			++send_num;
		}
	}
	//HERO_LOG("role[%lu] send all heros to client: size[%d]", p_role->get_uid(), send_num);
}

void hero_manager_t::save_all_datas(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL!");
		return;
	}

	hero_map::iterator it = p_role->m_hero_list.begin();
	hero_map::iterator it_end = p_role->m_hero_list.end();
	hero_ptr p_hero;
	for (; it != it_end; ++it)
	{
		p_hero = it->second;
		if (NULL != p_hero)
		{
			p_hero->save_self(p_role->get_uid());
		}
	}
	//HERO_LOG("role[%lu] save all heros to db: size[%d]", p_role->get_uid(), p_role->m_hero_list.size());
}

void hero_manager_t::create_hero(role_ptr p_role, const proto::common::hero_single& single)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL!");
		return;
	}
	if (single.base_attr().delete_flag() == 1)
	{
		return;
	}
	hero_ptr p_hero(new hero_t(p_role->get_uid()));
	p_hero->load_data(single);
// wys注释 2017.10.30 该段代码会在add_hero的时候执行 此处没必要做
// 	if (p_hero->get_unique_id() == p_role->get_main_hero_uid())
// 	{
// 		p_role->set_main_hero_tid(p_hero->get_tid());
// 		//p_role->set_main_hero_plugin(p_hero->get_current_plugin());
// 	}
	add_hero(p_role, p_hero, 0, NULL);

	p_hero->calc_fighting();
}

// 添加主角武将
void hero_manager_t::add_role_hero(role_ptr p_role, uint32_t hero_job, bool is_notify)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL!");
		return;
	}

	std::map<uint32_t, HeroJobTransfer*> confs;
	GET_ALL_CONF(HeroJobTransfer, confs);
	HeroJobTransfer *conf = NULL;

	std::map<uint32_t, HeroJobTransfer*>::iterator iter_conf = confs.begin();

	for (iter_conf = confs.begin(); iter_conf != confs.end(); ++iter_conf)
	{
		conf = iter_conf->second;
		if (NULL == conf)
		{
			log_error("role[%lu] add hero fail! no hero_job[%u]!", p_role->get_uid(), hero_job);
			continue;
		}
		if (hero_job == conf->id())
		{
			add_hero(p_role, conf->initial_id(), log_enum::source_type_role_create, NULL, 0, proto::common::hero_state_type_none, is_notify);
		}
		else
		{
			add_hero(p_role, conf->initial_id(), log_enum::source_type_role_create, NULL, 1, proto::common::hero_state_type_lock, is_notify);
		}
	}
}

// 添加武将 根据索引添加的武将一定是新武将
void hero_manager_t::add_hero(role_ptr p_role, uint32_t hero_tid, uint32_t source_type, proto::common::role_change_data* p_data, uint32_t type, uint32_t state, bool is_notify, proto::common::luckydraw_award* la)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL!");
		return;
	}
	auto p_hero_conf = GET_CONF(Hero, hero_tid);
	if (NULL == p_hero_conf)
	{
		log_error("NULL == p_hero_conf[%d]", hero_tid);
		return;
	}
	hero_ptr p_hero = get_hero(p_role, hero_tid);
	if (NULL == p_hero)
	{
		//没有这个武将就添加武将
		uint64_t unique_id = gen_hero_uid();
		p_hero = hero_ptr(new hero_t(hero_tid, unique_id, p_role->get_uid()));
		if (type == 0)
		{
			p_role->set_main_hero_uid(unique_id);
			p_role->set_main_hero_tid(hero_tid);
			p_role->set_main_hero_plugin(p_hero->get_current_plugin());
		}
		p_hero->set_hero_state(state);

		add_hero(p_role, p_hero, source_type, p_data, true);
		bounty_mgr_ptr p_bounty_mgr = p_role->get_bounty_mgr();
		if (NULL != p_bounty_mgr)
		{
			p_bounty_mgr->calc_star(hero_tid);
		}
		//成就通知
		if (0 != type)
		{
			//成就完成立即通知
			achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER,0,1);
		}
		
		if (NULL != la)
		{
			la->set_hero_tid(hero_tid);
			la->set_item_tid(0);
			la->set_item_count(0);
		}
	}
	else
	{
		CULTI_LOG("add hero[%d] exist, add material", hero_tid);
		//有这个武将就添加碎片
		if (item_manager_t::check_add_item(p_role, p_hero_conf->material_id(), p_hero_conf->smelt_count(), proto::common::package_type_main))
		{
			item_manager_t::add_item(p_role, p_hero_conf->material_id(), p_hero_conf->smelt_count(), source_type, 0, p_data);
		}
		else
		{
			std::map<uint32_t, uint32_t> mail_item_info;
			mail_item_info.insert(std::make_pair(p_hero_conf->material_id(), p_hero_conf->smelt_count()));
			mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, mailcode_enum::sys_mail_hero,
				mail_item_info, proto::common::MAIL_GET_TYPE_ADD_HERO);
		}
		
		if (NULL != la)
		{
			la->set_hero_tid(hero_tid);
			la->set_item_tid(p_hero_conf->material_id());
			la->set_item_count(p_hero_conf->smelt_count());
		}
	}
}

// 添加武将
void hero_manager_t::add_hero(role_ptr p_role, hero_ptr p_hero, uint32_t source_type, proto::common::role_change_data* p_data, bool is_new/* = false*/)
{
	if (p_role == NULL)
	{
		log_error("add hero fail role is null");
		return;
	}
	if (p_hero == NULL)
	{
		log_error("role [%lu] add hero fail role is null", p_role->get_uid());
		return;
	}
	auto binsert = p_role->m_hero_list.insert(std::pair<uint64_t, hero_ptr>(p_hero->get_unique_id(), p_hero));
	if (binsert.second && is_new)
	{
		p_hero->save_self(p_role->get_uid(), is_new);
	}
	if (p_hero->get_unique_id() == p_role->get_main_hero_uid())
	{
		p_hero->set_type(0);
		p_role->set_main_hero_tid(p_hero->get_tid());
		p_role->set_main_hero_plugin(p_hero->get_current_plugin());
	}
	else
	{
		p_hero->set_type(1);
		if (is_new)
		{
			//calc_fate_info(p_role, p_hero);
			log_wrapper_t::send_hero_log(p_role->get_uid(), p_role->get_level(), p_hero->get_unique_id(), p_hero->get_tid(), log_enum::opt_type_add, source_type);
		}
	}
	if (NULL != p_data)
	{
		proto::common::hero_data* p_hero_data = p_data->mutable_hero();
		proto::common::hero_single* p_hero_single = p_hero_data->add_hero_list();
		p_hero->peek_data(p_hero_single);
	}
}

void hero_manager_t::add_story_hero(role_ptr p_role, uint32_t hero_tid, uint32_t level)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL!");
		return;
	}
	auto p_hero_conf = GET_CONF(Hero, hero_tid);
	if (NULL == p_hero_conf)
	{
		log_error("NULL == p_hero_conf[%d]", hero_tid);
		return;
	}
	if (p_hero_conf->hero_type() != hero_type_story)
	{
		log_error("add_story_hero role[%lu] hero_tid[%d] hero_type[%d] != hero_type_story", p_role->get_uid(), hero_tid, p_hero_conf->hero_type());
		return;
	}
	hero_ptr p_hero = get_hero(p_role, hero_tid);
	if (NULL == p_hero)
	{
		//没有这个武将就添加武将
		uint64_t unique_id = gen_hero_uid();
		p_hero = hero_ptr(new hero_t(hero_tid, unique_id, p_role->get_uid()));

		proto::client::gc_role_change_data_notify ntf;

		p_hero->set_current_level(level);
		add_hero(p_role, p_hero, log_enum::source_type_hero_story_add, ntf.mutable_change_data(), true);
		// 先通知client role_change_data
		p_role->send_msg_to_client(op_cmd::gc_role_change_data_notify, ntf);
		// 再更新布阵
		p_role->m_formation_mgr.add_hero_to_all_formation(p_role, unique_id);
		
		bounty_mgr_ptr p_bounty_mgr = p_role->get_bounty_mgr();
		if (NULL != p_bounty_mgr)
		{
			p_bounty_mgr->calc_star(hero_tid);
		}
	}
}

// 获取武将
hero_ptr hero_manager_t::get_hero(role_ptr p_role, uint32_t hero_id)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return hero_ptr(NULL);
	}

	for (auto it : p_role->m_hero_list)
	{
		hero_ptr p_hero = it.second;
		if (NULL == p_hero)
		{
			continue;
		}
		if (p_hero->get_tid() == hero_id)
		{
			return p_hero;
		}
	}
	return hero_ptr(NULL);
}

void hero_manager_t::get_all_heroes(role_ptr p_role, std::vector<hero_ptr>& res)
{
    if (p_role == NULL)
    {
        log_error("get hero fail role is null");
        return;
    }

	hero_ptr p_hero = NULL;
	hero_map::iterator iter = p_role->m_hero_list.begin();
	for (; iter != p_role->m_hero_list.end(); ++iter)
	{
		p_hero = iter->second;
		if (NULL == p_hero)
		{
			log_error("p_hero NULL uid:[%lu]",iter->first);
			continue;
		}
		if(p_hero->get_hero_state() == proto::common::hero_state_type_lock)
			continue;
		res.push_back(iter->second);
	}
}

void hero_manager_t::get_all_heroes_include_hide(role_ptr p_role, std::vector<hero_ptr>& res)
{
	if (p_role == NULL)
	{
		log_error("get hero fail role is null");
		return;
	}

	hero_ptr p_hero = NULL;
	hero_map::iterator iter = p_role->m_hero_list.begin();
	for (; iter != p_role->m_hero_list.end(); ++iter)
	{
		p_hero = iter->second;
		if (NULL == p_hero)
		{
			log_error("p_hero NULL uid:[%lu]", iter->first);
			continue;
		}
		res.push_back(iter->second);
	}
}

void hero_manager_t::get_all_heroes_by_fight_value(role_ptr p_role, std::vector<hero_ptr>& res)
{
	std::vector<hero_ptr> hero_list;
	hero_manager_t::get_all_heroes(p_role, hero_list);

	hero_ptr p_hero = NULL;
	for (uint32_t i = 0; i < hero_list.size(); ++i)
	{
		p_hero = hero_list[i];
		if (NULL == p_hero)
		{
			continue;
		}

		res.push_back(p_hero);
	}

	common::Sort::Qsort(res, 0, res.size() - 1, hero_manager_t::hero_fight_value_sort);
	//std::sort(res.begin(), res.end(), hero_manager_t::hero_fight_value_sort);
}

bool hero_manager_t::hero_fight_value_sort(const hero_ptr& h1, const hero_ptr& h2)
{
	if (NULL == h1 || NULL == h2)
	{
		return false;
	}

	return h1->get_fighting() < h2->get_fighting();
}

void hero_manager_t::remove_story_hero(role_ptr p_role, uint32_t hero_tid)
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL!");
		return;
	}
	Hero* p_hero_conf = GET_CONF(Hero, hero_tid);
	if (NULL == p_hero_conf)
	{
		log_error("NULL == p_hero_conf[%d]", hero_tid);
		return;
	}
	if (p_hero_conf->hero_type() != hero_type_story)
	{
		log_error("remove_story_hero role[%lu] hero_tid[%d] hero_type[%d] != hero_type_story", p_role->get_uid(), hero_tid, p_hero_conf->hero_type());
		return;
	}
	hero_ptr p_hero = get_hero(p_role, hero_tid);
	if (NULL == p_hero)
	{
		log_error("remove_story_hero role[%lu] hero_tid[%d] not find", p_role->get_uid(), hero_tid);
		return;
	}
	uint64_t old_hero_uid = p_hero->get_unique_id();
	proto::client::gc_role_change_data_notify ntf;
	remove_hero(p_role, hero_tid, log_enum::source_type_hero_story_remove, ntf.mutable_change_data());
	// 如果是剧情武将要从布阵中移除
	if (p_hero_conf->hero_type() == hero_type_story)
	{
		p_role->m_formation_mgr.remove_hero_from_all_formation(p_role, old_hero_uid);
	}
	p_role->send_msg_to_client(op_cmd::gc_role_change_data_notify, ntf);
}

void hero_manager_t::remove_hero(role_ptr p_role, uint32_t hero_tid, uint32_t source_type, proto::common::role_change_data* p_data)
{
 	if (NULL == p_role)
 	{
 		log_error("p_role is NULL!");
 		return;
 	}
 	hero_ptr p_hero = get_hero(p_role, hero_tid);
 	if (NULL != p_hero)
	{
		log_wrapper_t::send_hero_log(p_role->get_uid(), p_role->get_level(), p_hero->get_unique_id(), p_hero->get_tid(), log_enum::opt_type_del, source_type);
		p_hero->set_delete_flag(1);
		p_hero->save_self(p_role->get_uid());
		p_role->m_hero_list.erase(p_hero->get_unique_id());
	}
	if (NULL != p_data)
	{
		proto::common::hero_data* p_hero_data = p_data->mutable_hero();
		proto::common::hero_single* p_hero_single = p_hero_data->add_hero_list();
		p_hero->peek_data(p_hero_single);
	}
}

hero_ptr hero_manager_t::get_role_hero(role_ptr p_role)
{
    if (p_role == NULL)
    {
        log_error("get hero fail role is null");
        return hero_ptr(NULL);
    }
	hero_ptr p_hero = get_hero_by_unique_id(p_role, p_role->get_main_hero_uid());
	if (NULL == p_hero)
	{
		log_error("role[%lu] get_role_hero fail, hero[%lu] is null", p_role->get_uid(), p_role->get_main_hero_uid());
		return hero_ptr(NULL);
	}
    if (!p_hero->is_role())
    {
		log_error("role[%lu] get_role_hero fail, hero[%lu] is not role_hero", p_role->get_uid(), p_role->get_main_hero_uid());
		return hero_ptr(NULL);
    }

	return p_hero;
}

// 获取武将
hero_ptr hero_manager_t::get_hero_by_unique_id(role_ptr p_role, uint64_t unique_id)
{
	if (p_role == NULL)
	{
		log_error("get hero fail role is null hero id [%lu]", unique_id);
		return hero_ptr(NULL);
	}

	auto it = p_role->m_hero_list.find(unique_id);
	if (it != p_role->m_hero_list.end())
	{
		return it->second;
	}

	return hero_ptr(NULL);
}

// 获取武将唯一Id
uint64_t hero_manager_t::gen_hero_uid()
{
	uint64_t uid = common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_HERO);
	return uid;
}

bool hero_manager_t::has_hero(role_ptr p_role, uint64_t uid)
{
	return NULL != get_hero_by_unique_id(p_role, uid);
}

bool hero_manager_t::has_plugin(role_ptr p_role,uint32_t hero_id,uint32_t plugin_id)
{
	hero_ptr p_hero = hero_manager_t::get_hero(p_role, hero_id);
	if (NULL == p_hero)
	{
		log_error("p_hero NULL role[%lu] not exist hero[%d]", p_role->get_uid(), hero_id);
		return false;
	}
	const std::vector<uint32_t>& plugin_list = p_hero->get_plugin_list();
	for (auto& it : plugin_list)
	{
		if (it != plugin_id)
			continue;
		return true;
	}

	return false;
}

uint32_t hero_manager_t::change_hero_plugin(role_ptr p_role, uint32_t hero_id, uint32_t plugin_id, proto::common::role_change_data* p_data /*= NULL*/)
{
	uint32_t reply_code = errcode_enum::error_ok;
	do 
	{
		if (NULL == p_role)
		{
			log_error("NULL == p_role");
			reply_code = errcode_enum::notice_unknown;
			break;
		}
		const uint64_t role_uid = p_role->get_uid();
		hero_ptr p_hero = get_hero(p_role, hero_id);
		if (NULL == p_hero)
		{
			log_error("NULL == p_hero role[%lu] not exist hero[%d]", role_uid, hero_id);
			reply_code = errcode_enum::notice_hero_null;//该英雄没有招募
			break;
		}

		if (!has_plugin(p_role, hero_id, plugin_id))
		{
			log_error("role[%lu] hero[%d] not exist plugin[%d]", role_uid, hero_id, plugin_id);
			reply_code = errcode_enum::notice_plugin_not_exist;
			break;
		}
		const uint32_t plugin_curr = p_hero->get_current_plugin();
		if (plugin_curr == plugin_id)
		{
			log_error("role[%lu] hero[%d] current use plugin[%d]", role_uid,hero_id,plugin_id);
			reply_code = errcode_enum::notice_plugin_useing;
			break;
		}

		p_hero->set_current_plugin(plugin_id);
		p_hero->save_self(p_role->get_uid());
		if (p_hero->get_unique_id() == p_role->get_main_hero_uid())
		{
			p_role->set_main_hero_plugin(p_hero->get_current_plugin());
		}

		HERO_LOG("role[%lu] hero[%d] set old plugin[%d] to new plugin[%d]", role_uid, hero_id,plugin_curr,plugin_id);
		proto::common::hero_data* p_hero_data = p_data->mutable_hero();
		proto::common::hero_single* p_hero_single = p_hero_data->add_hero_list();
		proto::common::base_attr* p_base_attr = p_hero_single->mutable_base_attr();
		p_base_attr->set_id(p_hero->get_tid());
		p_base_attr->set_current_plugin(p_hero->get_current_plugin());
		const std::vector<uint32_t>& plugin_list = p_hero->get_plugin_list();
		for (auto& it : plugin_list)
		{
			p_base_attr->add_plugins(it);
		}
		p_role->save_self(true);
	} while (false);
	
	return reply_code;
}

uint32_t hero_manager_t::get_hero_full_equip_count(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return 0;
	}
	uint32_t count = 0;
	for (auto it : p_role->m_hero_list)
	{
		auto p_hero = it.second;
		if (NULL == p_hero)
		{
			continue;
		}
		if (p_hero->get_equip_count() >= equip_pos_end)
		{
			++count;
		}
	}
	return count;
}

void hero_manager_t::calc_fate_info(role_ptr p_role, hero_ptr p_hero)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	if (NULL == p_hero)
	{
		log_error("NULL == p_hero");
		return;
	}
	for (auto it : p_role->m_hero_list)
	{
		hero_ptr p_temp_hero = it.second;
		if (NULL == p_temp_hero)
		{
			continue;
		}
		if (p_temp_hero == p_hero)
		{
			continue;
		}
		if (p_temp_hero->is_role())
		{
			continue;
		}
		for (int i = 0; i < FATE_NUM; ++i)
		{
			auto p_hero_fate_config = GET_CONF(HeroFate, hash_util_t::hash_to_uint32(p_temp_hero->get_tid(), i + 1));
			if (NULL == p_hero_fate_config)
			{
				log_error("hero[%d] fateCnf[%d] not found", p_temp_hero->get_tid(), i + 1);
				continue;
			}
			if (p_hero_fate_config->fate_hero() == p_hero->get_tid())
			{
				p_temp_hero->add_fate(p_hero_fate_config->fate_id());
				p_temp_hero->save_self(p_role->get_uid());
				break;
			}
		}
		for (int i = 0; i < FATE_NUM; ++i)
		{
			auto p_hero_fate_config = GET_CONF(HeroFate, hash_util_t::hash_to_uint32(p_hero->get_tid(), i + 1));
			if (NULL == p_hero_fate_config)
			{
				log_error("hero[%d] fateCnf[%d] not found", p_hero->get_tid(), i + 1);
				continue;
			}
			if (p_hero_fate_config->fate_hero() == p_temp_hero->get_tid())
			{
				p_hero->add_fate(p_hero_fate_config->fate_id());
				p_hero->save_self(p_role->get_uid());
				break;
			}
		}
	}
}

void hero_manager_t::do_next_day(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("get hero fail role is null");
		return;
	}

	for (auto it : p_role->m_hero_list)
	{
		auto p_hero = it.second;
		if (NULL == p_hero)
		{
			log_error("get hero fail hero is null");
			continue;;
		}
		p_hero->set_exp_today(0);
	}
}

uint32_t hero_manager_t::get_hero_level_limit()
{
	auto hero_level_limit = GET_CONF(Comprehensive, comprehensive_common::hero_level_max);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(hero_level_limit);
	return val;
}

void hero_manager_t::check_hero_wakeup_add_plugin(const uint64_t role_uid, const uint32_t hero_id)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role [%lu]",role_uid);
		return;
	}
	hero_ptr p_hero = hero_manager_t::get_hero(p_role, hero_id);
	if (NULL == p_hero)
	{
		log_error("hero_manager_t::get_hero[%d]  NULL!!", hero_id);
		return;
	}
	bool is_update_attr = false;
	std::map<uint32_t, HeroPlugin*> confs;
	GET_ALL_CONF(HeroPlugin, confs);
	if (confs.empty())
	{
		log_error("HeroPlugin table empty!!!");
		return;
	}
	for (auto& it : confs)
	{
		HeroPlugin* p_config = it.second;
		if (NULL == p_config)
		{
			log_error("NULL == p_config , id[%d]", it.first);
			continue;
		}
		if (0 >= it.first)
		{
			log_error("HeroPlugin table id[%d]",it.first);
			continue;
		}
		if (p_config->hero_id() != p_hero->get_tid())
		{
			continue;
		}
		if (unlock_hero_plugin_type::unlock_plugin_wakeup != p_config->type())
		{
			continue;
		}
		if (has_plugin(p_role, p_hero->get_tid(), it.first))
		{
			HERO_LOG("role[%lu] hero[%d] already exist plugin[%d]!!!",role_uid, hero_id,it.first);
			continue;
		}
			
		p_hero->add_plugin(it.first);
		p_hero->set_current_plugin(it.first);
		HERO_LOG("role[%lu] hero[%d] add_plugin[%d] curr_plugin[%d]", role_uid, hero_id,it.first, it.first);
		const std::vector<uint32_t>& plugin_list = p_hero->get_plugin_list();
		for (auto& pl : plugin_list)
		{
			HERO_LOG("role[%lu] hero[%d] plugin[%d]",role_uid, hero_id, pl);
		}
		
		if (!is_update_attr)
		{
			is_update_attr = true;
		}
		
		if (p_hero->get_unique_id() == p_role->get_main_hero_uid())
		{
			p_role->set_main_hero_plugin(p_hero->get_current_plugin());
		}
		break;
	}
	if (is_update_attr)
	{
		p_hero->calc_fighting();
		p_hero->save_self(p_role->get_uid());
	}
}

// 添加经验
bool hero_manager_t::add_exp(role_ptr p_role, uint64_t hero_uid, uint32_t exp, uint32_t source_type, bool isNotify, proto::common::role_change_data* p_data)
{
	if (p_role == NULL)
	{
		log_error("role is null!");
		return false;
	}
	if (exp == 0)
	{
		log_error("role[%lu] can't add zero exp!", p_role->get_object_id());
		return false;
	}
	hero_ptr p_hero = get_hero_by_unique_id(p_role, hero_uid);
	if (p_hero == NULL)
	{
		log_error("hero [%lu] ptr is null!", hero_uid);
		return false;
	}
    auto p_hero_config = GET_CONF(Hero, p_hero->get_tid());
    if (NULL == p_hero_config)
    {
        log_error("p_hero_config[%d] not found!!", p_hero->get_tid());
		return false;
    }

	uint32_t current_exp = p_hero->get_current_exp();
	uint32_t current_level = p_hero->get_current_level();
	if (current_level >= p_role->get_level())
	{
        HERO_LOG("role[%lu] hero[%lu] add exp failed: hero_current_level[%d] >= role_level[%d]", p_role->get_uid(), hero_uid, current_level, p_role->get_level());
		return false;
	}
	if (current_level >= get_hero_level_limit())
	{
        HERO_LOG("role[%lu] hero[%lu] add exp failed: hero_current_level[%d] >= hero_level_limit[%d]", p_role->get_uid(), hero_uid, current_level, get_hero_level_limit());
		return false;
	}
	
    uint32_t old_exp = current_exp;
    current_exp += exp;
    uint32_t old_level = current_level;
	while (true)
	{
		auto p_level_demad = GET_CONF(LevelDemad, hash_util_t::hash_to_uint32(p_hero_config->level_curve(), current_level));
		if (NULL == p_level_demad)
		{
			log_error("role[%lu] can't add zero fail! Level FindCnf false", p_role->get_object_id());
			break;
		}
		auto p_next_level_demad = GET_CONF(LevelDemad, hash_util_t::hash_to_uint32(p_hero_config->level_curve(), current_level + 1));
		if (NULL == p_next_level_demad)
		{
			log_error("role[%lu] can't Level_up level max!!", p_role->get_object_id());
			break;
		}
		if (current_exp < p_level_demad->exp())
		{
			
			break;
		}
		// 升级
		current_level++;
		current_exp -= p_level_demad->exp();
        if (current_level >= get_hero_level_limit())
		{
			//允许一次增加经验让武将等级超过玩家等级但是不能超过武将等级上限
			break;
		}
		
		achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_HERO_UP_LEVEL_NUMBER);

	}
    
    HERO_LOG("role[%lu] hero[%d] add exp[%d] level_exp form old[%d:%d] to new[%d:%d]", p_role->get_uid(), p_hero->get_tid(), 
		exp, old_level, old_exp, current_level, current_exp);

	p_hero->set_current_exp(current_exp);
	p_hero->set_current_level(current_level);
	p_hero->save_self(p_role->get_uid());
	if (NULL != p_data)
	{
		proto::common::hero_data* p_hero_data = p_data->mutable_hero();
		proto::common::hero_single* p_hero_single = p_hero_data->add_hero_list();
		p_hero->peek_data(p_hero_single);
	}

	log_wrapper_t::send_hero_exp_log(p_role->get_uid(), p_role->get_level(), hero_uid, p_hero->get_tid(), old_level, old_exp, 
		exp, current_level, current_exp, source_type, 0);
	return true;
}

void hero_manager_t::add_pve_exp(role_ptr p_role, uint64_t hero_uid, uint32_t exp, proto::client::gc_combat_result_ntf* p_data /*= NULL*/)
{
	if (p_role == NULL)
	{
		log_error("role is null!");
		return;
	}
	if (exp == 0)
	{
		log_error("role[%lu] can't add zero exp!", p_role->get_object_id());
		return;
	}

	hero_ptr p_hero = get_hero_by_unique_id(p_role, hero_uid);
	if (p_hero == NULL)
	{
		log_error("hero [%lu] ptr is null!", hero_uid);
		return;
	}

	auto p_hero_config = GET_CONF(Hero, p_hero->get_tid());
	if (NULL == p_hero_config)
	{
		log_error("p_hero_config[%d] not found!!", p_hero->get_tid());
		return;
	}
	// 剧情武将不给经验
	if (p_hero_config->hero_type() == hero_type_story)
	{
		return;
	}
	auto p_level_demad = GET_CONF(LevelDemad, hash_util_t::hash_to_uint32(p_hero_config->level_curve(), p_hero->get_current_level()));
	if (NULL == p_level_demad)
	{
		log_error("role[%lu] can't add zero fail! Level FindCnf false");
		return;
	}
	if (exp > p_level_demad->exp_limit())
	{
		exp = p_level_demad->exp_limit();
	}
	if (NULL != p_data)
	{
		if (add_exp(p_role, hero_uid, exp, log_enum::source_type_monster_fight_pve, false, p_data->mutable_rcd()))
		{
			proto::common::pve_hero_exp* p_exp = p_data->add_hero_exp();
			p_exp->set_hero_uid(string_util_t::uint64_to_string(hero_uid));
			p_exp->set_exp(exp);
		}
	}
	else
	{
		add_exp(p_role, hero_uid, exp, log_enum::source_type_monster_fight_pve, false);
	}
}

uint32_t hero_manager_t::unlock_main_hero(role_ptr p_role, uint32_t hero_tid, proto::common::role_change_data* p_data)
{
	if (p_role == NULL)
	{
		log_error("role is null!");
		return errcode_enum::notice_unknown;
	}

	auto cost_conf = GET_CONF(Comprehensive, comprehensive_common::role_open_money);
	uint32_t cost = GET_COMPREHENSIVE_VALUE_1(cost_conf);

	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_ROLE_UNLOCK, cost, log_enum::source_type_hero_unlock, 0, false, p_data))
	{
		log_error("role[%lu] unlock_main_hero money_not_enough", p_role->get_uid());
		return errcode_enum::notice_gold_money_not_enough;
	}

	hero_ptr p_hero = get_hero(p_role, hero_tid);
	if (NULL == p_hero)
	{
		log_error("role[%lu] unlock_main_hero p_hero NULL hero_uid[%d]", p_role->get_uid(), hero_tid);
		return errcode_enum::notice_unknown;
	}

	p_hero->set_hero_state(proto::common::hero_state_type_none);
	p_hero->save_self(p_role->get_uid());

	return errcode_enum::error_ok;
}

void hero_manager_t::change_role_main_hero(role_ptr p_role, uint32_t hero_tid)
{
	proto::client::gc_role_main_hero_change_reply reply;
	if (p_role == NULL)
	{
		log_error("role is null!");
		return;
	}

	hero_ptr p_hero = get_hero(p_role, hero_tid);
	if (NULL == p_hero)
	{
		log_error("role[%lu] change_role_main_hero p_hero NULL hero_uid[%d]", p_role->get_uid(), hero_tid);
		reply.set_reply_code(errcode_enum::notice_unknown);
		p_role->send_msg_to_client(op_cmd::gc_role_main_hero_change_reply, reply);
		return;
	}

	if (p_hero->get_hero_state() == proto::common::hero_state_type_lock)
	{
		log_error("role[%lu] change_role_main_hero hero[%d] state = lock", p_role->get_uid(), hero_tid);
		reply.set_reply_code(errcode_enum::notice_unknown);
		p_role->send_msg_to_client(op_cmd::gc_role_main_hero_change_reply, reply);
		return;
	}

	uint64_t old_main_hero_uid = p_role->get_main_hero_uid();
	if (old_main_hero_uid == p_hero->get_unique_id())
	{
		log_error("role[%lu] change_role_main_hero old_main_hero_uid[%lu] = change_hero_uid[%lu] tid[%d]", old_main_hero_uid, p_hero->get_unique_id(), hero_tid);
		return;
	}
	hero_ptr p_old_hero = get_hero_by_unique_id(p_role, old_main_hero_uid);
	if (NULL == p_old_hero)
	{
		log_error("role[%lu] change_role_main_hero p_old_hero[%lu] NULL", p_role->get_uid(), old_main_hero_uid);
		reply.set_reply_code(errcode_enum::notice_unknown);
		p_role->send_msg_to_client(op_cmd::gc_role_main_hero_change_reply, reply);
		return;
	}

	p_hero->set_type(0);
	p_hero->save_self(p_role->get_uid());
	p_old_hero->set_type(1);
	p_old_hero->save_self(p_role->get_uid());

	p_role->set_main_hero_tid(p_hero->get_tid());
	p_role->set_main_hero_uid(p_hero->get_unique_id());
	p_role->set_main_hero_plugin(p_hero->get_current_plugin());
	p_role->save_self();
	p_role->get_personal_info(reply.mutable_rcd()->mutable_per_info());

	p_hero->peek_data(reply.mutable_new_main_hero());

	reply.set_reply_code(errcode_enum::error_ok);
	// 先发reply 再更新布阵 最后9屏广播
	p_role->send_msg_to_client(op_cmd::gc_role_main_hero_change_reply, reply);

	p_role->m_formation_mgr.replace_hero_from_all_formation(p_role, old_main_hero_uid, p_hero->get_unique_id());

	proto::client::gc_role_main_hero_change_ntf ntf;
	ntf.set_role_uid(string_util_t::uint64_to_string(p_role->get_uid()));
	ntf.set_plugin(p_hero->get_current_plugin());
	p_role->send_msg_to_nine(op_cmd::gc_role_main_hero_change_ntf, reply);
}