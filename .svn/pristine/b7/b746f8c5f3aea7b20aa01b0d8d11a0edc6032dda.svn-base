#include "equip_plan.hpp"
#include "main/game_server_fwd.hpp"
#include "utility.hpp"

USING_NS_COMMON;

equip_plan_t::equip_plan_t(uint64_t uid)
	:m_uid(uid)
{

}

equip_plan_t::~equip_plan_t()
{

}

void equip_plan_t::peek_data(proto::common::equip_plan_single* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	p_data->set_uid(string_util_t::uint64_to_string(m_uid));
	p_data->set_name(m_name);
	p_data->set_weapon(string_util_t::uint64_to_string(m_weapon));
	p_data->set_armor(string_util_t::uint64_to_string(m_armor));
	p_data->set_talisman(string_util_t::uint64_to_string(m_talisman));
	p_data->set_hero_uid(string_util_t::uint64_to_string(m_hero_uid));
}

void equip_plan_t::load_data(const proto::common::equip_plan_single& eps)
{
	m_uid = string_util_t::string_to_uint64(eps.uid());
	m_name = eps.name();
	m_weapon = string_util_t::string_to_uint64(eps.weapon());
	m_armor = string_util_t::string_to_uint64(eps.armor());
	m_talisman = string_util_t::string_to_uint64(eps.talisman());
	m_hero_uid = string_util_t::string_to_uint64(eps.hero_uid());
}

