#include "role_level_reward_mail.hpp"
#include "cache_key.hpp"
#include "log.hpp"
#include "role/role_unify_save.hpp"
#include "config/config_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "config/config_manager.hpp"
#include "tblh/LevelRewardMailTable.tbls.h"
#include "role/role.hpp"
#include "mail/mail_manager.hpp"


role_level_reward_mail_t::role_level_reward_mail_t(uint64_t uid)
	:m_role_uid(uid)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::level_reward_mail);
}

role_level_reward_mail_t::~role_level_reward_mail_t()
{
}


void role_level_reward_mail_t::load_data(const proto::common::level_reward_mail_recived_list & data) 
{
	m_recived_set.clear();

	for(int32_t i = 0; i < data.data_size(); ++i) {
		m_recived_set.insert( data.data(i));
	}
}

void role_level_reward_mail_t::peek_data(proto::common::level_reward_mail_recived_list &p_data) 
{
	for (const auto &it : m_recived_set) 
	{
		p_data.add_data(it);
	}
}

void role_level_reward_mail_t::save_data() 
{
	proto::common::level_reward_mail_recived_list p_data;
	peek_data(p_data);
	role_unify_save::add_task(m_key, p_data, false, false, m_role_uid);
}


//检测是否需要发送邮件
void role_level_reward_mail_t::check_send_level_reward_mail( role_ptr p_role ) 
{
	if (NULL == p_role) {
		log_error("role[%lu] p_role == null", m_role_uid);
		return;
	}

	bool save_flag = false;
	std::map<uint32_t, LevelRewardMailTable*> config_map;
	GET_ALL_CONF(LevelRewardMailTable, config_map);
	for (auto iter : config_map) {
		auto p_conf = iter.second;
		if (NULL == p_conf) {
			continue;
		}

		uint32_t id = p_conf->id();

		//检测是否已经领取过
		if (has_send_level_reward_mail(id) == true ){
			continue;
		}

		//检测条件是否满足
		if (p_conf->level() > p_role->get_level()){
			continue;
		}

		//发送邮件	
		mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, p_conf->mail_id(), proto::common::MAIL_GET_TYPE_LEVEL_REWARD);

		//保存状态
		m_recived_set.insert(id);
		save_flag = true;
	}

	if (save_flag) { 
		save_data();
	}
}

bool role_level_reward_mail_t::has_send_level_reward_mail(uint32_t id) 
{
	auto iter_f = m_recived_set.find(id);
	if (iter_f == m_recived_set.end()) {
		return false;
	}
	return true;
}



