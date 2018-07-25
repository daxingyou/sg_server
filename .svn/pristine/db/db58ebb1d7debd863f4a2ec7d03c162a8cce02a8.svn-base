#include "partner.hpp"
#include "fight/fight_common.hpp"

partner_t::partner_t(uint64_t object_id)
	: object_base_t(object_id)
{

}

partner_t::~partner_t()
{

}

void partner_t::get_object_info(proto::common::object_common_info* object_info)
{
	if (object_info) {
		object_base_t::get_object_info(object_info);
		object_info->set_troop_id(common::string_util_t::uint64_to_string(m_troop_id));
	}
}

uint32_t partner_t::get_camp_unique_offset(uint32_t camp)
{
	if (camp == proto::common::combat_camp_1)
		return CAMP_SELF_ROBOT_UNIQUE_OFFSET;
	else
		return CAMP_ENEMY_ROBOT_UNIQUE_OFFSET;
}

