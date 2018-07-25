#include "field_boss_manager.hpp"

#include "server.pb.h"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "config/config_manager.hpp"
#include "tblh/BossBornTable.tbls.h"
#include "tblh/Monster.tbls.h"
#include "tblh/BossPersonalTable.tbls.h"
#include "tblh/BossFamilyTable.tbls.h"
#include "tblh/mailcode_enum.hpp"
#include "tblh/MailTable.tbls.h"
#include "mail/mail_manager.hpp"
#include "family/family_manager.hpp"
#include "family/family_member.hpp"
#include "family/family.hpp"

field_boss_manager_t::filed_boss_id_map field_boss_manager_t::m_filed_boss_id_map;

bool field_boss_manager_t::init()
{
	load_day(common::time_util_t::now_time());

	return true;
}

void field_boss_manager_t::close_clear()
{
	for (filed_boss_id_map::const_iterator citr = m_filed_boss_id_map.begin();
		citr != m_filed_boss_id_map.end(); ++citr) {
		for (const auto& p_filed_boss : citr->second) {
			if (p_filed_boss != NULL) {
				p_filed_boss->clear();
			}
		}
	}

	m_filed_boss_id_map.clear();
}

void field_boss_manager_t::reload()
{
	clear_all();

	load_day(common::time_util_t::now_time());
}

filed_boss_ptr field_boss_manager_t::find_field_boss(uint32_t id, uint64_t object_id)
{
	filed_boss_id_map::iterator itr = m_filed_boss_id_map.find(id);
	if (itr == m_filed_boss_id_map.end()) {
		return NULL;
	}

	filed_boss_vec::iterator itr2 = std::find_if(itr->second.begin(), itr->second.end(), 
		[object_id](const filed_boss_ptr& p_filed_boss)
	{
		if (p_filed_boss != NULL && p_filed_boss->get_object_id() == object_id) return true;
		return false;
	});

	if (itr2 != itr->second.end()) {
		return *itr2;
	}

	return NULL;
}

void field_boss_manager_t::clear_all()
{
	// 有boss，通知所有game删除boss
	proto::server::eg_clear_all_boss_notify msg;
	for (filed_boss_id_map::const_iterator citr = m_filed_boss_id_map.begin();
		citr != m_filed_boss_id_map.end(); ++citr) {
		for (const auto& p_filed_boss : citr->second) {
			if (p_filed_boss != NULL && p_filed_boss->get_state() == proto::common::FIELD_BOSS_STATE_START) {
				msg.add_object_ids(p_filed_boss->get_object_id());
			}
			p_filed_boss->clear();
		}
	}

	// 如果有需要删除的boss，只发静态场景服务器
	if (msg.object_ids_size() != 0)
		env::server->send_msg_to_game(common::GAME_SERVER_STATIC, op_cmd::eg_clear_all_boss_notify, 0, msg);

	m_filed_boss_id_map.clear();
}

void field_boss_manager_t::new_day(uint32_t cur_time)
{
	clear_all();

	load_day(cur_time);
}

void field_boss_manager_t::load_day(uint32_t cur_time)
{
	struct tm day_time;
	common::time_util_t::get_local_time(day_time, cur_time);

	uint32_t tm_wday = day_time.tm_wday;
	BOSS_LOG("field boss start init weekday[%u] config", tm_wday);

	std::map<uint32_t, BossBornTable*> all_confs;
	GET_ALL_CONF(BossBornTable, all_confs);

	for (std::map<uint32_t, BossBornTable*>::iterator itr = all_confs.begin();
		itr != all_confs.end(); ++itr) {
		BossBornTable* p_boss = itr->second;
		if (NULL != p_boss) {
			if (p_boss->is_in_today(tm_wday)) {
				const std::vector<boss_born_time>& born_time_vec = p_boss->get_boss_born_time();
				for (std::vector<boss_born_time>::const_iterator itr_time = born_time_vec.begin();
					itr_time != born_time_vec.end(); ++itr_time)
				{
					const boss_born_time& boss_time = *itr_time;
					uint32_t hour = std::get<0>(boss_time);
					uint32_t min = std::get<1>(boss_time);

					if (hour > 23 || min > 59)
					{
						log_error("field_boss[%u] time error", p_boss->id());
						continue;
					}

					day_time.tm_hour = hour;
					day_time.tm_min = min;
					day_time.tm_sec = 0;
					time_t target_time = common::time_util_t::get_utc_time(day_time);
					if (target_time == (time_t)-1) {
						log_error("field_boss[%u] invalid get_utc_time error", p_boss->id());
						continue;
					}

					filed_boss_ptr p_new_filed_boss(new field_boss_t());
					if (NULL == p_new_filed_boss) {
						log_error("p_new_filed_boss null error");
						continue;
					}

					if (!p_new_filed_boss->init(p_boss, target_time > cur_time ? proto::common::FIELD_BOSS_STATE_NONE : proto::common::FIELD_BOSS_STATE_GONE, target_time - cur_time)) {
						log_error("field_boss[%u] init error", p_boss->id());
						continue;
					}

					m_filed_boss_id_map[itr->first].push_back(p_new_filed_boss);
				}
			}
		}
	}
}

void field_boss_manager_t::get_all_boss_info(proto::client::ec_field_boss_info_notify& notify)
{
	for (filed_boss_id_map::const_iterator citr = m_filed_boss_id_map.begin();
		citr != m_filed_boss_id_map.end(); ++citr) {
		proto::common::filed_boss_state* boss_state = notify.add_today_boss();
		if (boss_state != NULL) {
			boss_state->set_id(citr->first);
			for (const auto& p_filed_boss : citr->second) {
				if (p_filed_boss != NULL) {
					p_filed_boss->peek_data(boss_state->add_states());
				}
			}
		}
	}
}

bool field_boss_manager_t::summon_boss_result(uint32_t id, uint64_t object_id, bool result)
{
	filed_boss_ptr p_field_boss = find_field_boss(id, object_id);
	if (p_field_boss == NULL) {
		log_error("p_field_boss[%u:%lu] null error", id, object_id);
		return false;
	}

	return p_field_boss->summon_result(result);
}

void field_boss_manager_t::field_boss_killed(uint32_t id, uint64_t object_id, const std::string& killer)
{
	filed_boss_ptr p_field_boss = find_field_boss(id, object_id);
	if (p_field_boss == NULL) {
		log_error("p_field_boss[%u:%lu] null error", id, object_id);
		return;
	}

	p_field_boss->set_killer(killer);
}

void field_boss_manager_t::field_boss_rank_reward(const proto::server::ge_field_boss_rank_notify& ntf)
{
	if (ntf.mode() == proto::server::FIELD_BOSS_NONE) {
		log_error("field boss mode == proto::server::FIELD_BOSS_NONE， no reward");
		return;
	}

	Monster* p_monster_conf = GET_CONF(Monster, ntf.monster_id());
	if (!p_monster_conf) {
		log_error("field boss[%u] config not find", ntf.monster_id());
		return;
	}

	uint32_t persional_first_mail_id = ntf.mode() == proto::server::FIELD_BOSS_KILL ? common::mailcode_enum::sys_mail_boss_1 : common::mailcode_enum::sys_mail_boss_3;
	MailTable* persional_first_mail = GET_CONF(MailTable, persional_first_mail_id);
	if (!persional_first_mail) {
		log_error("p_mail[%u] error null", persional_first_mail_id);
		return;
	}

	uint32_t persional_other_mail_id = ntf.mode() == proto::server::FIELD_BOSS_KILL ? common::mailcode_enum::sys_mail_boss_2 : common::mailcode_enum::sys_mail_boss_4;
	MailTable* persional_other_mail = GET_CONF(MailTable, persional_other_mail_id);
	if (!persional_other_mail) {
		log_error("p_mail[%u] error null", persional_other_mail);
		return;
	}

	for (int32_t i = 0; i < ntf.persion_ranks_size(); ++i) {
		if (ntf.persion_ranks(i).uids_size() == 0) continue;

		BossPersonalTable* p_conf = GET_CONF(BossPersonalTable, ntf.persion_ranks(i).rank_id());
		if (!p_conf) continue;

		std::string content_text;
		std::vector<uint64_t> uids;
		for (const auto& uid : ntf.persion_ranks(i).uids()) {
			uids.push_back(uid);
		}

		StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_conf->rank() == 1? persional_first_mail->content() : persional_other_mail->content()).c_str(), 
			common::string_util_t::convert_to_utf8(p_monster_conf->get_name()).c_str(), p_conf->rank());

		mail_manager_t::send_mail(uids, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(persional_first_mail->from_name()), common::string_util_t::convert_to_utf8(persional_first_mail->title()), content_text,
			ntf.mode() == proto::server::FIELD_BOSS_KILL ? p_conf->killed_reward() : p_conf->gone_reward(), proto::common::MAIL_GET_TYPE_FIELD_BOSS);
	}

	uint32_t family_first_mail_id = ntf.mode() == proto::server::FIELD_BOSS_KILL ? common::mailcode_enum::sys_mail_boss_5 : common::mailcode_enum::sys_mail_boss_7;
	MailTable* family_first_mail = GET_CONF(MailTable, family_first_mail_id);
	if (!family_first_mail) {
		log_error("p_mail[%u] error null", family_first_mail);
		return;
	}

	uint32_t family_other_mail_id = ntf.mode() == proto::server::FIELD_BOSS_KILL ? common::mailcode_enum::sys_mail_boss_6 : common::mailcode_enum::sys_mail_boss_8;
	MailTable* family_other_mail = GET_CONF(MailTable, family_other_mail_id);
	if (!family_other_mail) {
		log_error("p_mail[%u] error null", family_other_mail);
		return;
	}

	for (int32_t i = 0; i < ntf.family_ranks_size(); ++i) {
		if (ntf.family_ranks(i).uids_size() == 0) continue;

		BossFamilyTable* p_conf = GET_CONF(BossFamilyTable, ntf.family_ranks(i).rank_id());
		if (!p_conf) continue;

		std::string content_text;
		std::vector<uint64_t> uids;

		StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_conf->rank() == 1 ? family_first_mail->content() : family_other_mail->content()).c_str(),
			common::string_util_t::convert_to_utf8(p_monster_conf->get_name()).c_str(), p_conf->rank());

		for (const auto& uid : ntf.family_ranks(i).uids()) {
			family_ptr p_family = family_manager_t::get_family(uid);
			if (p_family != NULL) {
				uids.clear();
				const family::family_member_map& member_map = p_family->get_all_member();
				for (family::family_member_map::const_iterator citr = member_map.begin();
					citr != member_map.end(); ++citr) {
					const family_member_ptr& p_family_member = citr->second;
					if (NULL != p_family_member) {
						uids.push_back(p_family_member->get_uid());
					}
				}

				mail_manager_t::send_mail(uids, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(family_first_mail->from_name()), common::string_util_t::convert_to_utf8(family_first_mail->title()), content_text,
					ntf.mode() == proto::server::FIELD_BOSS_KILL ? p_conf->killed_reward() : p_conf->gone_reward(), proto::common::MAIL_GET_TYPE_FIELD_BOSS);
			}
		}
	}

	if (ntf.mode() == proto::server::FIELD_BOSS_KILL && ntf.killers_size() != 0) {
		BossBornTable* p_bossinfo = GET_CONF(BossBornTable, ntf.id());
		if (NULL == p_bossinfo) {
			log_error("field boss[%u] not find", ntf.id());
			return;
		}

		MailTable* kill_reward_mail = GET_CONF(MailTable, common::mailcode_enum::sys_mail_boss_9);
		if (!kill_reward_mail) {
			log_error("p_mail[%u] error null", common::mailcode_enum::sys_mail_boss_9);
			return;
		}

		std::vector<uint64_t> uids;
		for (const auto& uid : ntf.killers()) {
			uids.push_back(uid);
		}

		std::string content_text;
		StringTool::format(content_text, common::string_util_t::convert_to_utf8(kill_reward_mail->content()).c_str(),
			common::string_util_t::convert_to_utf8(p_monster_conf->get_name()).c_str());

		mail_manager_t::send_mail(uids, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(kill_reward_mail->from_name()), common::string_util_t::convert_to_utf8(kill_reward_mail->title()), content_text,
			p_bossinfo->get_kill_rwds(), proto::common::MAIL_GET_TYPE_FIELD_BOSS);
	}
}

void field_boss_manager_t::gm_summon_field_boss(uint32_t id)
{
	BossBornTable* p_conf = GET_CONF(BossBornTable, id);
	if (!p_conf) {
		log_error("field boss[%u] not find", id);
		return;
	}

	filed_boss_ptr p_new_filed_boss(new field_boss_t());
	if (NULL == p_new_filed_boss) {
		log_error("p_new_filed_boss null error");
		return;
	}

	if (!p_new_filed_boss->init(p_conf, proto::common::FIELD_BOSS_STATE_NONE, 5)) {
		log_error("field_boss[%u] init error", p_conf->id());
		return;
	}

	m_filed_boss_id_map[id].push_back(p_new_filed_boss);
}
