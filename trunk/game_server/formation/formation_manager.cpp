#include "formation_manager.hpp"
#include "formation.hpp"
#include "global_id.hpp"
#include "hero/hero_manager.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "user_troop/user_troop_mgr.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"
#include "expedition/expedition.hpp"
#include "hero/hero_manager.hpp"
#include "tblh/attr_enum.hpp"
#include "object/object_manager.hpp"
#include "fight/fight_manager.hpp"

extern common::id_allocator_t common::g_id_allocator;

formation_manager_t::formation_manager_t(uint64_t owner)
	: m_owner(owner)
{
}

formation_manager_t::~formation_manager_t()
{
}

// load datas from db
void formation_manager_t::load_all_datas(const proto::common::formation_data& data)
{
	for (int32_t i = 0; i < data.formation_list_size(); ++i)
	{
		proto::common::formation_single single = data.formation_list(i);
		add_formation(single);
	}
}

// send datas to client
void formation_manager_t::peek_all_datas(proto::common::formation_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}

	formation_ptr p_formation;
	int32_t send_num = 0;
	p_data->set_cur_formation_id(m_cur_formation_id);
	for (auto it : m_formation_list)
	{
		p_formation = it.second;
		if (p_formation != NULL)
		{
			proto::common::formation_single* p_single = p_data->add_formation_list();
			p_formation->peek_data(p_single);
			++send_num;
		}
	}
}

void formation_manager_t::peek_datas(uint32_t formation_id, proto::common::formation_single* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}

	formation_ptr p_formation;
	auto it = m_formation_list.find(formation_id);
	if (it == m_formation_list.end())
		return;

	p_formation = it->second;
	if (p_formation != NULL)
	{
		p_formation->peek_data(p_data);
	}
}

void formation_manager_t::peek_cur_formation_datas(proto::common::formation_single* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}

	formation_ptr p_formation;
	auto it = m_formation_list.find(m_cur_formation_id);
	if (it == m_formation_list.end())
		return;

	p_formation = it->second;
	if (p_formation != NULL)
	{
		p_formation->peek_data(p_data);
	}	
}

void formation_manager_t::peek_fight_data(proto::common::hero_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}

	formation_ptr p_formation;
	auto it = m_formation_list.find(m_cur_formation_id);
	if (it == m_formation_list.end())
		return;

	p_formation = it->second;
	if (p_formation != NULL)
	{
		p_formation->peek_fight_data(p_data);
	}

}

// 添加布阵
bool formation_manager_t::add_formation(formation_ptr p_formation)
{
	if (NULL == p_formation)
	{
		log_error("[zqx formation] role[%lu] p_formation is NULL!", m_owner);
		return false;
	}

	auto res = m_formation_list.insert(std::make_pair(p_formation->get_index(), p_formation));
	if (!res.second)
	{
		log_error("[zqx formation] role[%lu] add new formation[%d] failed!", m_owner, p_formation->get_index());
		return false;
	}

	return true;
}

// 添加布阵
void formation_manager_t::add_formation(const proto::common::formation_single& single)
{
	formation_ptr p_formation(new formation_t(m_owner));
	p_formation->load_data(single);
	add_formation(p_formation);
}

bool formation_manager_t::check_hero_up_form(uint32_t fight_type, hero_ptr p_hero, role_ptr p_role )
{
	if (NULL == p_hero)
	{
		log_error("check_hero_up_form p_hero NULL");
		return false;
	}

	if (NULL == p_role) {
		log_error("check_hero_up_form p_role NULL");
		return false;
	}


	if (fight_type == proto::common::fight_type_expedition)
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::glory_hero_limit_level);
		uint32_t glory_hero_limit_level = GET_COMPREHENSIVE_VALUE_1(conf);
		if (p_hero->get_unique_id() != p_role->get_main_hero_uid() && p_hero->get_current_level() < glory_hero_limit_level) {
			log_error("formation hero[%lu] level[%u] < limit_level[%u] ", p_hero->get_unique_id(), p_hero->get_current_level(), glory_hero_limit_level);
			return false;
		}
	}
	if (p_hero->get_hero_state() == proto::common::hero_state_type_lock){
		log_error("formation hero[%lu] is lock", p_hero->get_unique_id());
		return false;
	}
	return true;
}

// 是否是有效的布阵
bool formation_manager_t::is_validate_formation(
	role_ptr p_role, 
	formation_ptr p_formation, 
	uint32_t fight_type, 
	const std::map<uint32_t, PosHeroData> &hero_pos,
	uint32_t tactic_id)
{
	if (NULL == p_formation)
	{
		log_error("formation_manager_t p_formation NULL");
		return false;
	}
	if (NULL == p_role)
	{
		log_error("formation_manager_t p_role NULL");
		return false;
	}
// 	tactic_mgr_ptr p_tactic = p_role->get_tactic_mgr();
// 	if (NULL == p_tactic)
// 	{
// 		log_error("formation_manager_t p_tactic NULL role[%lu]", p_role->get_uid());
// 		return false;
// 	}
// 	if (!p_tactic->has_tactic(tactic_id))
// 	{
// 		log_error("formation_manager_t role[%lu] error tactic_id[%d]", p_role->get_uid(), tactic_id);
// 		return false;
// 	}

	if (hero_pos.size() > FORM_MAX_HERO)
	{
		return false;
	}

	std::map<uint64_t, uint32_t> check_map;
	std::map<uint64_t, uint32_t>::iterator iter_check;
	bool has_main_hero = false;
	hero_ptr p_hero = hero_ptr();

	//位置必须在1-9;英雄唯一id需合法
	for (auto ptr : hero_pos)
	{
// 		if (ptr.first == 0 || ptr.first > FORM_SIZE)
// 		{
// 			return false;
// 		}
		if (ptr.second.uid == 0)
			continue;
		p_hero = p_role->get_hero_by_unique_id(ptr.second.uid);
		if ( p_role != NULL && !check_hero_up_form(fight_type, p_hero, p_role))
		{
			return false;
		}
		// 不能重复上阵同一个武将
		iter_check = check_map.find(ptr.second.uid);
		if (iter_check != check_map.end())
		{
			log_error("formation_manager_t role[%lu] hero[%lu] repeat", p_role->get_uid(), ptr.second.uid);
			return false;
		}
		else
			check_map.insert(std::make_pair(ptr.second.uid, ptr.first));

		if (ptr.second.uid == p_role->get_main_hero_uid())
		{
			has_main_hero = true;
		}
	}

	// 判断远征 不能只上主公
	if(fight_type == proto::common::fight_type_expedition)
	{
		if (has_main_hero && hero_pos.size() == 1)
		{
			return false;
		}
	}
	
	//组队布阵，队员可以不上主角
	if ( p_role->get_team_member_size() > 0 && p_role->is_team_leader() == false )
		return true;

	if (has_main_hero == false) {
		log_error("user:[%lu],validate_formation:fight_type[%u]", p_role->get_uid(), fight_type );
	}
	return has_main_hero;
}

bool formation_manager_t::init_formation()
{
	auto role = role_manager_t::find_role(m_owner);
	if (NULL == role)
	{
		log_error("init formation owner[%lu] not valid", m_owner);
		return false;
	}
	uint32_t formation_id = 1;
	set_cur_formation_id(formation_id);

	m_formation_list.clear();
	std::map<uint32_t, PosHeroData> hero_pos;
	std::map<uint32_t, PosHeroData> hero_team_pos;
	hero_pos[DEFAULT_ROLE_POS] = PosHeroData(m_owner, proto::common::SCENEOBJECT_USER, role->get_main_hero_uid() );
	
	for (uint32_t index=1; index <= MAX_FORM_COUNT; ++index)
	{
		formation_ptr p_formation = formation_ptr(new formation_t(index, m_owner));
		if( index == proto::common::formation_type_troop_leader_2 ||
			index == proto::common::formation_type_troop_2 ||
			index == proto::common::formation_type_troop_leader_3 ||
			index == proto::common::formation_type_troop_3)
			p_formation->update_formation(hero_team_pos, 0, true);
		else
			p_formation->update_formation(hero_pos, 0, true);
		m_formation_list[index] = p_formation;
	}

	return true;
}

static std::map<uint32_t, PosHeroData> EMPTY_POS_HERO;

const std::map<uint32_t, PosHeroData>& formation_manager_t::get_pos_hero(uint32_t form_index)
{
	if (form_index <= 0 || form_index > MAX_FORM_COUNT)
	{
		log_error("update_formation role[%lu] form_index[%d] error", m_owner, form_index);
		return EMPTY_POS_HERO;
	}

	auto it = m_formation_list.find(form_index);
	if (it == m_formation_list.end())
	{
		log_error("[zqx formation] user [%lu] update formation fail! index is [%u]!", m_owner, form_index);
		return EMPTY_POS_HERO;
	}
	return it->second->get_pos_hero();
}

bool formation_manager_t::update_team_formation(
	role_ptr p_role,
	uint32_t team_member_size,
	const std::map<uint32_t, PosHeroData> &hero_pos,
	uint32_t tactic_id)
{
	uint32_t form_id = 0;
	uint32_t team_heroes_max_num = fight_manager_t::get_hero_count_by_member_size(team_member_size);
	if (hero_pos.size() > team_heroes_max_num)
	{
		return false;
	}
	if (team_member_size == 2)
	{
		if (p_role->is_team_leader())
			form_id = proto::common::formation_type_troop_leader_2;
		else
			form_id = proto::common::formation_type_troop_2;
	}
	else if (team_member_size == 3)
	{
		if (p_role->is_team_leader())
			form_id = proto::common::formation_type_troop_leader_3;
		else
			form_id = proto::common::formation_type_troop_3;
	}
	else
	{
		return false;
	}

	update_formation(p_role, form_id, proto::common::fight_type_none, hero_pos, 0);
	return true;
}

// 更新布阵
bool formation_manager_t::update_formation(
	role_ptr p_role, 
	uint32_t form_index, 
	uint32_t fight_type, 
	const std::map<uint32_t, PosHeroData> &hero_pos,
	uint32_t tactic_id)
{
	if (NULL == p_role)
	{
		log_error("formation_manager_t p_role NULL");
		return false;
	}
	
	if (form_index <= 0 || form_index > MAX_FORM_COUNT)
	{
		log_error("update_formation role[%lu] form_index[%d] error", p_role->get_object_id(), form_index);
		return false;
	}

	auto it = m_formation_list.find(form_index);
	if (it == m_formation_list.end())
	{
		log_error("[zqx formation] user [%lu] update formation fail! index is [%u]!", m_owner, form_index);
		return false;
	}

	formation_ptr p_formation = it->second;
	if (p_formation == NULL)
	{
		log_error("[zqx formation] user [%lu] update formation fail! p_formation[%u] is null!", m_owner, form_index);
		return false;
	}

	if (is_validate_formation(p_role, p_formation, fight_type, hero_pos, tactic_id))
	{
		p_formation->update_formation(hero_pos, tactic_id);

		p_role->update_role_info_to_center(proto::common::simple_info_type);

		p_role->calc_fighting();
		auto p_task_mgr = p_role->get_task_mgr();
		if (NULL != p_task_mgr)
		{
			uint32_t hero_count = get_cur_hero_count();
			proto::common::role_change_data rcd;
			p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_HERO_FORM, rcd, 1, hero_count, false);
		}
		if (form_index == proto::common::formation_type_offline_arena)
		{
			proto::server::ge_offline_arena_form_notify ntf;
			proto::common::mirror_fight_data* p_data = ntf.mutable_form_data();
			//竞技场布阵
			proto::common::obj_single* p_obj = p_data->mutable_obj();
			p_obj->set_object_uid(string_util_t::uint64_to_string(p_role->get_uid()));
			p_obj->set_name(p_role->get_name());
			p_obj->set_level(p_role->get_level());
			p_obj->set_object_type(proto::common::SCENEOBJECT_USER);
			p_obj->set_country(p_role->get_country_id());
			p_obj->set_life_level(p_role->get_lifestate_level());
			p_obj->set_fighting(p_formation->calc_fighting());
			p_obj->set_head_id(p_role->get_main_hero_plugin());
			p_formation->peek_fight_data(p_data->mutable_heros());
			p_data->set_formation_id(form_index);
			env::server->send_msg_to_center(op_cmd::ge_offline_arena_form_notify, p_role->get_uid(), ntf);
		}
		return true;
	}

	return false;
}

formation_ptr formation_manager_t::get_formation(uint32_t form_id)
{
	auto ptr = m_formation_list.find(form_id);
	if (m_formation_list.end() != ptr)
	{
		return ptr->second;
	}
	return NULL;
}

void formation_manager_t::peek_expedition_formation_data(role_ptr p_role, proto::common::fight_special_hero_data *hero_data)
{
	if (NULL == p_role)
	{
		log_error("peek_expedition_formation_data p_role NULL");
		return;
	}
	if (NULL == hero_data)
	{
		log_error("peek_expedition_formation_data hero_data NULL");
		return;
	}

	expedition_ptr p_expedition = p_role->get_expedition();
	if (NULL == p_expedition)
		return;

	const std::map<uint64_t, hero_save_data_ptr> &hero_list = p_expedition->get_hero_list();

	std::map<uint64_t, hero_save_data_ptr>::const_iterator ibegin = hero_list.begin();
	std::map<uint64_t, hero_save_data_ptr>::const_iterator iend = hero_list.end();
	hero_save_data_ptr p_save_data = hero_save_data_ptr();
	hero_ptr p_hero = hero_ptr();
	proto::common::fight_special_hero_single *p_hero_data = NULL;

	for (; ibegin != iend; ++ibegin)
	{
		p_save_data = ibegin->second;
		if (NULL == p_save_data)
			continue;
		p_hero = p_role->get_hero_by_unique_id(p_save_data->get_uid());
		if (NULL == p_hero)
		{
			log_error("peek_expedition_formation_data p_hero NULL uid[%lu]", p_save_data->get_uid());
			continue;
		}
		p_hero_data = hero_data->add_hero_list();
		p_hero_data->set_uid(string_util_t::uint64_to_string(p_save_data->get_uid()));
		p_hero_data->set_tid(p_hero->get_tid());
		p_hero_data->set_save_hp(p_save_data->get_hp());
		p_hero_data->set_max_hp(p_hero->get_attr(attrtype_enum::max_hp));
	}

}

void formation_manager_t::peek_family_formation_data(role_ptr p_role, proto::common::fight_special_hero_data *hero_data)
{
	if (NULL == p_role)
	{
		log_error("peek_expedition_formation_data p_role NULL");
		return;
	}
	if (NULL == hero_data)
	{
		log_error("peek_expedition_formation_data hero_data NULL");
		return;
	}
	const std::set<uint64_t>& hero_list = p_role->get_family_war_hero();

	std::set<uint64_t>::const_iterator ibegin = hero_list.begin();
	std::set<uint64_t>::const_iterator iend = hero_list.end();
	hero_ptr p_hero = hero_ptr();
	uint64_t hero_uid = 0;
	proto::common::fight_special_hero_single *p_hero_data = NULL;

	for (; ibegin != iend; ++ibegin)
	{
		hero_uid = *ibegin;
		p_hero = p_role->get_hero_by_unique_id(hero_uid);
		if (NULL == p_hero)
		{
			log_error("peek_expedition_formation_data p_hero NULL uid[%lu]", hero_uid);
			continue;
		}

		p_hero_data = hero_data->add_hero_list();
		p_hero_data->set_uid(string_util_t::uint64_to_string(hero_uid));
		p_hero_data->set_tid(p_hero->get_tid());
	}
}

bool formation_manager_t::update_cur_form_id(uint32_t form_id)
{
	if (form_id > 0 && form_id <= MAX_FORM_COUNT)
	{
		m_cur_formation_id = form_id;
	}
	
	return true;
}

uint32_t formation_manager_t::calc_fighting()
{
	uint32_t fighting = 0;
	formation_ptr p_form = get_cur_formation();
	if (NULL == p_form)
	{
		log_error("get_cur_formation NULL == p_form[%u]", get_cur_formation_id());
		return fighting;
	}
	fighting = p_form->calc_fighting();
	return fighting;
}

bool formation_manager_t::is_in_cur_form(uint64_t hero_uid)
{
	formation_ptr p_form = get_cur_formation();
	if (NULL == p_form)
	{
		return false;
	}
	return p_form->has_hero(hero_uid);
}

void formation_manager_t::update_offline_arena_form(uint64_t hero_uid)
{
	formation_ptr p_form = get_formation(proto::common::formation_type_offline_arena);
	if (NULL == p_form)
	{
		log_error("role[%lu] not have offline_arena_form", m_owner);
		return;
	}
	if (p_form->has_hero(hero_uid))
	{
		role_ptr p_role = role_manager_t::find_role(m_owner);
		if (NULL == p_role)
		{
			log_error("NULL == p_role[%lu]", m_owner);
			return;
		}
		proto::server::ge_offline_arena_form_notify ntf;
		proto::common::mirror_fight_data* p_data = ntf.mutable_form_data();
		//竞技场布阵
		proto::common::obj_single* p_obj = p_data->mutable_obj();
		p_obj->set_object_uid(string_util_t::uint64_to_string(p_role->get_uid()));
		p_obj->set_name(p_role->get_name());
		p_obj->set_level(p_role->get_level());
		p_obj->set_object_type(proto::common::SCENEOBJECT_USER);
		p_obj->set_country(p_role->get_country_id());
		p_obj->set_life_level(p_role->get_lifestate_level());
		p_obj->set_fighting(p_form->calc_fighting());
		p_obj->set_head_id(p_role->get_main_hero_plugin());
		p_form->peek_fight_data(p_data->mutable_heros());
		p_data->set_formation_id(proto::common::formation_type_offline_arena);
		env::server->send_msg_to_center(op_cmd::ge_offline_arena_form_notify, p_role->get_uid(), ntf);
	}
}

void formation_manager_t::update_offline_arena_form()
{
	formation_ptr p_form = get_formation(proto::common::formation_type_offline_arena);
	if (NULL == p_form)
	{
		log_error("role[%lu] not have offline_arena_form", m_owner);
		return;
	}
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	proto::server::ge_offline_arena_form_notify ntf;
	proto::common::mirror_fight_data* p_data = ntf.mutable_form_data();
	//竞技场布阵
	proto::common::obj_single* p_obj = p_data->mutable_obj();
	p_obj->set_object_uid(string_util_t::uint64_to_string(p_role->get_uid()));
	p_obj->set_name(p_role->get_name());
	p_obj->set_level(p_role->get_level());
	p_obj->set_object_type(proto::common::SCENEOBJECT_USER);
	p_obj->set_country(p_role->get_country_id());
	p_obj->set_life_level(p_role->get_lifestate_level());
	p_obj->set_fighting(p_form->calc_fighting());
	p_obj->set_head_id(p_role->get_main_hero_plugin());
	p_form->peek_fight_data(p_data->mutable_heros());
	p_data->set_formation_id(proto::common::formation_type_offline_arena);
	env::server->send_msg_to_center(op_cmd::ge_offline_arena_form_notify, p_role->get_uid(), ntf);
}

uint32_t formation_manager_t::get_cur_hero_count()
{
	formation_ptr p_form = get_cur_formation();
	if (NULL == p_form)
	{
		return 0;
	}
	return p_form->get_hero_count();
}

void formation_manager_t::add_hero_to_all_formation(role_ptr p_role, uint64_t hero_uid)
{
	if (NULL == p_role)
	{
		log_error("add_hero_to_all_formation p_role NULL");
		return;
	}
	proto::client::gc_update_formation_reply reply;
	formation_ptr p_formation = NULL;

	for (uint32_t i = proto::common::formation_type_role_1; i < proto::common::formation_type_count; ++i)
	{
		p_formation = get_formation(i);
		if (p_formation != NULL)
		{
			p_formation->add_hero_to_formation(hero_uid);
			p_formation->peek_data(reply.mutable_formation());
		}

		p_role->send_msg_to_client(op_cmd::gc_update_formation_reply, reply);
	}
	p_role->update_role_info_to_center(proto::common::simple_info_type);
}

void formation_manager_t::remove_hero_from_all_formation(role_ptr p_role, uint64_t hero_uid)
{
	if (NULL == p_role)
	{
		log_error("remove_hero_from_all_formation p_role NULL");
		return;
	}
	proto::client::gc_update_formation_reply reply;
	formation_ptr p_formation = NULL;
	for (uint32_t i = proto::common::formation_type_role_1; i < proto::common::formation_type_count; ++i)
	{
		p_formation = get_formation(i);
		if (p_formation != NULL)
		{
			p_formation->remove_hero_from_formation(hero_uid);
			p_formation->peek_data(reply.mutable_formation());
		}

		p_role->send_msg_to_client(op_cmd::gc_update_formation_reply, reply);
	}
	p_role->update_role_info_to_center(proto::common::simple_info_type);
}

void formation_manager_t::replace_hero_from_all_formation(role_ptr p_role, uint64_t old_hero_uid, uint64_t new_hero_uid)
{
	if (NULL == p_role)
	{
		log_error("remove_hero_from_all_formation p_role NULL");
		return;
	}

	proto::client::gc_update_all_formation_reply reply;
	 for (uint32_t i = proto::common::formation_type_role_1; i < proto::common::formation_type_count; ++i)
	 {
		 formation_ptr p_formation = get_formation(i);
		 if (p_formation != NULL)
		 {
			 p_formation->replace_hero_from_formation(old_hero_uid, new_hero_uid);
			 p_formation->peek_data(reply.add_formations());
		 }
	 }
	 p_role->send_msg_to_client(op_cmd::gc_update_all_formation_reply, reply);
	 p_role->update_role_info_to_center(proto::common::simple_info_type);
}


troop_formation_manager_t::troop_formation_manager_t(uint64_t owner)
	: formation_manager_t::formation_manager_t(owner)
{
	
}

troop_formation_manager_t::~troop_formation_manager_t()
{

}

bool troop_formation_manager_t::init_formation()
{
	set_cur_formation_id(1);
	m_formation_list.clear();

	troop_formation_ptr p_formation = troop_formation_ptr(new troop_formation_t(1, m_owner));
	
	m_formation_list[1] = p_formation;
	p_formation->clear_heroes();
	return true;
}

bool troop_formation_manager_t::is_validate_formation(
	role_ptr p_role, 
	formation_ptr p_formation, 
	uint32_t fight_type, 
	const std::map<uint32_t, PosHeroData> &hero_pos, 
	uint32_t tactic_id)
{
	if (NULL == p_formation)
	{
		log_error("troop_formation_manager_t p_formation NULL");
		return false;
	}

	hero_ptr p_old_hero = hero_ptr();
	hero_ptr p_new_hero = hero_ptr();
	uint32_t hero_count = 0;

	//object_id_type role_id;
	bool need_main_hero = false;
	if (p_role != NULL)
		need_main_hero = p_role->is_team_leader();

	bool has_main_hero = false;
	// 检查重复上阵武将
	std::map<uint64_t, uint32_t>	leader_check_map;
	std::map<uint64_t, uint32_t>::iterator iter_check;
	// 校验
	for (auto iter : hero_pos)
	{
		object_base_ptr object = object_manager_t::find_object(iter.second.id);
		if (NULL == object)
			continue;

		p_new_hero = object->get_hero_by_unique_id(iter.second.uid);
		if (NULL == p_new_hero)
		{
			continue;
		}

		if ( p_role != NULL && !check_hero_up_form(fight_type, p_new_hero,p_role))
		{
			return false;
		}

		++hero_count;

		if ( p_new_hero->get_tid() == object->get_main_hero_tid())
		{
			has_main_hero = true;
		}

		iter_check = leader_check_map.find(iter.second.uid);
		if (iter_check != leader_check_map.end())
		{
			log_error("troop_formation_manager_t role[%lu] hero[%lu] repeat", object->get_object_id(), iter.second.uid);
			return false;
		}
		else
			leader_check_map.insert(std::make_pair(iter.second.uid, iter.first));
	}

	// 校验队长是否上阵主公
	if (need_main_hero && !has_main_hero)
	{
		if(p_role != NULL)
			log_error("troop_formation_manager_t error leader[%lu] not use main_hero", p_role->get_uid());
		return false;
	}

	// 校验上阵的武将数量 超过允许的上限认为非法
	// 默认是1人上阵武将数
	//uint32_t team_heroes_max_num = fight_manager_t::get_hero_count_by_member_size(m_troop_member_num);
	
	if (hero_count > 6)
	{
		log_error("troop_formation_manager_t error hero_count[%d] > [%d]", hero_count, 6);
		return false;
	}
	return true;
}

bool troop_formation_manager_t::update_formation(
	role_ptr p_role, 
	uint32_t form_index, 
	uint32_t fight_type, 
	const std::map<uint32_t, PosHeroData> &hero_pos,
	uint32_t tactic_id)
{
	formation_ptr p_formation = get_cur_formation();
	if (NULL == p_formation)
	{
		log_error("troop_formation_manager_t cur_formation NULL");
		return false;
	}
	
	if (is_validate_formation(p_role, p_formation, fight_type, hero_pos, tactic_id))
	{
		p_formation->update_formation(hero_pos, tactic_id, false, false);
		return true;
	}
	return false;
}