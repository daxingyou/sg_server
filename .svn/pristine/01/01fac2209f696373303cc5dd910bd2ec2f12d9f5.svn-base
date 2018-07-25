#include "partner_shadow.hpp"
#include "log.hpp"
#include "country/country_manager.hpp"
#include "fight/fight_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"
#include "scene/scene.hpp"

partner_shadow_t::partner_shadow_t(uint64_t object_id)
	: partner_t(object_id)
{

}

partner_shadow_t::~partner_shadow_t()
{

}

void partner_shadow_t::get_object_info(proto::common::object_common_info* object_info)
{
	if (object_info) {
		partner_t::get_object_info(object_info);
		object_info->set_object_name(m_name);
		object_info->set_level(m_level);
		object_info->set_fighting_value(m_fighting);
		object_info->set_plugin(m_plugin);
		object_info->set_head(m_head);
		proto::common::object_state_info* object_state = object_info->mutable_object_state();
		if (object_state != NULL) {
			object_state->set_mode_state(proto::common::role_mode_state_normal);
		}

		if (0 != m_family_id) {
			object_info->set_family_id(string_util_t::uint64_to_string(m_family_id));
			object_info->set_family_name(m_family_name);
			object_info->set_family_officer(m_family_officer);
		}

		if (m_country != 0) {
			country_manager_t::peek_country_common(m_country, object_info->mutable_country());
			object_info->set_country_officer(m_country_officer);
		}
	}
}

void partner_shadow_t::on_leave_scene(scene_ptr p_scene)
{
	if (NULL == p_scene) {
		log_error("p_scene null error");
		return;
	}

	object_base_t::on_leave_scene(p_scene);

	// 离开副本从队伍中删除化身
	if (p_scene->get_scene_type() == proto::server::em_scene_type_dugeon) {
		proto::server::ge_troop_del_shadow_notify notify;
		notify.set_troop_id(m_troop_id);
		notify.set_member_id(get_object_id());
		env::server->send_msg_to_center(op_cmd::ge_troop_del_shadow_notify, 0, notify);
	}
}

bool partner_shadow_t::init_shadow(const troop_t::troop_member_ptr& p_member)
{
	if (!p_member) {
		log_error("troop_member null error");
		return false;
	}

	const proto::server::troop_extra_info& extra_info = p_member->get_member_extra();

	m_hero_data.CopyFrom(extra_info.shadow_info().blob().simple().hero());

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::team_ai_friend_name);
	if (p_conf && !p_conf->parameter1().empty()) {
		StringTool::format(m_name, common::string_util_t::convert_to_utf8(p_conf->parameter1()).c_str(), p_member->get_name().c_str());
	}

	//m_name = p_member->get_name();
	m_head = p_member->get_plugin();
	m_plugin = extra_info.shadow_info().plugin_modle();
	m_fighting = p_member->get_fighting();
	m_level = p_member->get_level();
	m_country = extra_info.shadow_info().country();
	m_country_officer = extra_info.shadow_info().country_officer();
	m_family_id = extra_info.shadow_info().family_id();
	m_family_name = extra_info.shadow_info().family_name();
	m_family_officer = extra_info.shadow_info().family_officer();

	return true;
}

hero_map* partner_shadow_t::get_hero_list()
{
	return &m_hero_list;
}

bool partner_shadow_t::init_hero_list()
{
	if (m_hero_list.size() != 0)
		return true;

	for (int32_t i = 0; i < m_hero_data.hero_list_size(); ++i) {
		hero_ptr p_hero(new hero_t(get_object_id_type()));
		p_hero->load_data(m_hero_data.hero_list(i));
		p_hero->calc_fighting();
		m_hero_list[p_hero->get_tid()] = p_hero;
	}

	return true;
}

bool partner_shadow_t::get_hero_list(uint32_t fight_type, uint32_t team_member_size, std::vector<uint64_t>& list)
{
	if (init_hero_list() == false)
		return false;

	uint32_t team_heroes_max_num = fight_manager_t::get_hero_count_by_member_size(team_member_size);
	hero_map::const_iterator it;
	uint32_t index = 0;
	for (it = m_hero_list.begin(); it != m_hero_list.end(); ++it)
	{
		list.push_back(it->first);
		++index;
		if (index >= team_heroes_max_num)
			break;
	}

	return true;
}

hero_ptr partner_shadow_t::get_hero_by_unique_id(uint64_t unique_id)
{
	// 判断是否要初始化管理器
	if (init_hero_list() == false)
		return NULL;

	auto it = m_hero_list.find(unique_id);
	if (it != m_hero_list.end())
	{
		return it->second;
	}

	return NULL;
}

bool partner_shadow_t::fill_fight_camp(
	uint32_t camp_index,
	uint64_t unique_offset,
	uint32_t team_member_size,
	proto::common::fight_camp* camp,
	proto::common::fight_type fight_type,
	proto::common::hero_state_data* state,
	int32_t team_member_pos)
{
	return fight_manager_t::fill_fight_camp(camp_index, get_camp_unique_offset(camp_index), team_member_size, team_member_pos, shared_from_this(), camp, state);
}

// 填充武将列表
bool partner_shadow_t::fill_hero_data(
	uint32_t camp_index,
	uint64_t unique_offset,
	uint32_t team_member_size,
	proto::common::fight_type fight_type,
	proto::common::hero_data* fhd,
	proto::common::hero_state_data* state,
	int32_t team_member_pos)
{
	return  fight_manager_t::fill_hero_data( camp_index, get_camp_unique_offset(camp_index), get_object_id(), team_member_size, team_member_pos, get_heros(), fhd, state);
}