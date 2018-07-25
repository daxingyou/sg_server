#include "role_level_reward.hpp"
#include "cache_key.hpp"
#include "log.hpp"
#include "role/role_unify_save.hpp"
#include "config/config_manager.hpp"
#include "tblh/LevelRewardTable.tbls.h"
#include "tblh/errcode_enum.hpp"
#include "log_enum.hpp"
#include "item/drop_manager.hpp"

role_level_reward_t::role_level_reward_t(uint64_t uid)
	: m_role_uid(uid)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::level_reward);
}

role_level_reward_t::~role_level_reward_t()
{

}

void role_level_reward_t::load_data(const proto::common::level_reward_recived_list& data)
{
	m_recived_list.CopyFrom(data);

	for (int32_t i = 0; i < m_recived_list.data_size(); ++i) {
		m_recived_map.insert(m_recived_list.data(i));
	}
}

void role_level_reward_t::peek_data(proto::common::level_reward_recived_list* p_data)
{
	if (!p_data) {
		log_error("role[%lu] p_data null error", m_role_uid);
		return;
	}

	p_data->CopyFrom(m_recived_list);
}

void role_level_reward_t::save_data()
{
	role_unify_save::add_task(m_key, m_recived_list, false, false, m_role_uid);
}

uint32_t role_level_reward_t::get_level_reward(uint32_t id, role_ptr p_role, proto::common::role_change_data* p_data)
{
	if (NULL == p_role) {
		log_error("role[%lu] p_role == null", m_role_uid);
		return common::errcode_enum::notice_unknown;
	}

	if (!p_data) {
		log_error("role[%lu] p_data == null", m_role_uid);
		return common::errcode_enum::notice_unknown;
	}

	LevelRewardTable* p_conf = GET_CONF(LevelRewardTable, id);
	if (NULL == p_conf) {
		log_error("role[%lu] level_reward[%u] not find", m_role_uid, id);
		return common::errcode_enum::notice_unknown;
	}

	if (p_role->get_level() < p_conf->level()) {
		log_error("role[%lu] level[%u] < level_reward[%u] level[%u]", m_role_uid, p_role->get_level(), id, p_conf->level());
		return common::errcode_enum::notice_level_reward_level_insufficient;
	}

	std::set<uint32_t>::const_iterator citr = m_recived_map.find(id);
	if (citr != m_recived_map.end()) {
		log_error("role[%lu] level_reward[%u] is already recived", m_role_uid, id);
		return common::errcode_enum::notice_level_reward_recived;
	}

	drop_manager_t::drop(p_role, p_conf->drop(), log_enum::source_type_achieve_main_role_level,
		proto::common::drop_sys_type_level_reward, p_role->get_level(), p_data);

	p_role->get_personal_info(p_data->mutable_per_info());

	m_recived_map.insert(id);
	m_recived_list.add_data(id);

	save_data();

	return common::errcode_enum::error_ok;
}

bool role_level_reward_t::is_recived_all_reward()
{
	return m_recived_map.size() >= LevelRewardTableManager::getInstancePtr()->getSize();
}
