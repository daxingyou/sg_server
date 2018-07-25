#include "family_member.hpp"
#include "utility.hpp"
#include "redis/global_user_data_mgr.hpp"
#include "user/global_user_manager.hpp"
#include "redis/family_data_mgr.hpp"
#include "tblh/time_type_enum.hpp"
#include "config_mgr.h"
#include "tblh/Comprehensive.tbls.h"
#include "time_manager.hpp"
#include "tblh/MailTable.tbls.h"
#include "mail/mail_manager.hpp"
#include "StringTool.h"
#include "tblh/Item.tbls.h"
#include "tblh/errcode_enum.hpp"
#include "tblh/mailcode_enum.hpp"
#include "tblh/FamilyPray.tbls.h"
#include "log/log_wrapper.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "redis/family_data_mgr.hpp"

#define WRITE_FAMILY_LOG_TO_DB(family_uid, role_uid, target_uid, type, param, param2) \
    log_wrapper_t::send_family_log(family_uid, role_uid, target_uid, type, param, param2);

family_member::family_member(uint64_t uid, uint64_t family_id)
	: m_uid(uid)
	, m_family_id(family_id)
{
	m_base.set_role_id(common::string_util_t::uint64_to_string(m_uid));
}

bool family_member::init(const global_user_ptr& p_user, uint32_t officer)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return false;
	}

	m_base.set_role_name(p_user->get_name());
	m_base.set_officer(officer);
	m_base.set_level(p_user->get_level());
	m_base.set_fighting(p_user->get_fighting_value());
	m_base.set_last_offline_time(0);
	m_base.set_total_family_contribution(0);
	m_base.set_left_family_contribution(p_user->get_family_contribution());
	m_base.set_plugin(p_user->get_hero_plugin());
	m_base.set_glory(0);
	m_base.set_prayer_contribution(0);

	m_join_time = common::time_util_t::now_time();
	m_is_impeach_ack = family_impeach_type_none;

	// 读取祈愿次数的信息
	family_data_mgr_t::load_prayer_info(shared_from_this());
	// 加载限购信息
	family_data_mgr_t::load_member_shop_info(shared_from_this());

	if (common::time_util_t::now_time() > m_last_prayer_day_time) {
		prayer_day_update(false);
	}

	if (common::time_util_t::now_time() > m_last_prayer_week_time) {
		prayer_week_update(false);
	}

	return true;
}

void family_member::update_attr(const global_user_ptr& p_user)
{
	if (NULL == p_user) {
		log_error("NULl == p_user");
		return;
	}

	m_base.set_level(p_user->get_level());
	m_base.set_fighting(p_user->get_fighting_value());
	m_base.set_left_family_contribution(p_user->get_family_contribution());
	m_base.set_plugin(p_user->get_hero_plugin());
}

bool family_member::init(uint32_t officer)
{
	if (!global_user_data_mgr_t::is_user_key_exsit(m_uid)) {
		log_error("user[%lu] not exsit", m_uid);
		return false;
	}

	m_base.set_role_name(global_user_data_mgr_t::get_name(m_uid));
	m_base.set_officer(officer);
	m_base.set_level(global_user_data_mgr_t::get_user_level(m_uid));
	m_base.set_fighting(global_user_data_mgr_t::get_user_fighting_value(m_uid));
	m_base.set_last_offline_time(global_user_data_mgr_t::get_last_logout_time(m_uid));
	m_base.set_total_family_contribution(0);
	m_base.set_left_family_contribution(global_user_data_mgr_t::get_family_contribution(m_uid));
	m_base.set_plugin(global_user_data_mgr_t::get_user_hero_plugin(m_uid));
	m_base.set_glory(0);
	m_base.set_prayer_contribution(0);

	m_join_time = common::time_util_t::now_time();
	m_is_impeach_ack = family_impeach_type_none;

	// 读取祈愿次数的信息
	family_data_mgr_t::load_prayer_info(shared_from_this());
	// 加载限购信息
	family_data_mgr_t::load_member_shop_info(shared_from_this());

	if (common::time_util_t::now_time() > m_last_prayer_day_time) {
		prayer_day_update(false);
	}

	if (common::time_util_t::now_time() > m_last_prayer_week_time) {
		prayer_week_update(false);
	}

	return true;
}

bool family_member::is_online() const
{
	return m_base.last_offline_time() == 0;
}

bool family_member::update_officer(uint32_t new_officer)
{
	m_base.set_officer(new_officer);

	// 保存redis
	family_data_mgr_t::update_family_member_officer(m_uid, new_officer);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(m_uid);
	if (NULL != p_user) {
		p_user->set_family_officer(new_officer);
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(m_uid, proto::common::sys_notify_type_family, new_officer, p_user->get_name().c_str());
	} else {
		global_user_data_mgr_t::set_user_family_officer(m_uid, new_officer);
	}

	return true;
}

void family_member::set_online()
{
	m_base.set_last_offline_time(0);
}

void family_member::set_offline(uint32_t last_time)
{
	m_base.set_last_offline_time(last_time);
}

void family_member::peek_data(proto::common::family_member* p_data)
{
	if (p_data) {
		p_data->CopyFrom(m_base);
	}
}

void family_member::add_total_contribution(uint32_t val)
{
	uint32_t new_val = m_base.total_family_contribution() + val;

	m_base.set_total_family_contribution(new_val);

	// 保存redis
	family_data_mgr_t::update_total_family_contribution(m_uid, new_val);
}

void family_member::prayer_day_update(bool is_save /*= true*/)
{
	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_number);
	m_prayer_times = GET_COMPREHENSIVE_VALUE_1(conf);

	conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_reward2);
	uint32_t max_rwd_times = GET_COMPREHENSIVE_VALUE_1(conf);

	if (m_prayer_rwd_times < max_rwd_times) {
		conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_reward1);
		uint32_t pre_get_times = GET_COMPREHENSIVE_VALUE_1(conf);

		m_prayer_rwd_times += pre_get_times;
	}

	m_last_prayer_day_time = time_manager_t::get_next_refresh_time(common::time_type_enum::time_one_day);

	if (is_save)
		family_data_mgr_t::update_day_prayer_info(shared_from_this());

	WRITE_FAMILY_LOG_TO_DB(m_family_id, m_uid, 0, common::log_enum::source_type_family_prayer_day_time_refresh, m_prayer_times, m_prayer_rwd_times);
}

void family_member::prayer_week_update(bool is_save /*= true*/)
{
	m_prayer_gift_times = 0;

	// 用swap清理缓存 clear只会把内容清理
	proto::common::family_prayer_record_list m_new_list;
	m_record_list.Swap(&m_new_list);

	m_last_prayer_week_time = time_manager_t::get_next_refresh_time(common::time_type_enum::time_week);

	if (is_save)
		family_data_mgr_t::update_week_prayer_info(shared_from_this());

	WRITE_FAMILY_LOG_TO_DB(m_family_id, m_uid, 0, common::log_enum::source_type_family_prayer_week_time_refresh, m_last_prayer_week_time, 0);
}

void family_member::peek_prayer_data(proto::common::family_prayer_data* p_data)
{
	if (p_data) {
		p_data->set_role_id(m_base.role_id());
		p_data->set_role_name(m_base.role_name());
		p_data->set_officer(m_base.officer());
		p_data->set_plugin(m_base.plugin());
		p_data->set_level(m_base.level());
		p_data->set_prayer_contribution(m_base.prayer_contribution());
		p_data->set_hero_tid(m_prayer_hero_tid);
		p_data->set_hero_curr_num(m_prayer_hero_num);
		p_data->set_hero_max_num(m_prayer_hero_max);
		p_data->set_start_time(m_prayer_start_time);
		p_data->set_complete_time(m_prayer_complete_time);

		proto::common::family_gift_data_list* p_gift_data_list = p_data->mutable_list();
		if (p_gift_data_list) {
			p_gift_data_list->CopyFrom(m_gift_list);
		}
	}
}

void family_member::peek_prayer_times_data(proto::client::ec_family_prayer_list_reply& reply)
{
	// 先更新下
	if (common::time_util_t::now_time() > m_last_prayer_day_time) {
		prayer_day_update();
	}

	reply.set_prayer_times(m_prayer_times);
	reply.set_prayer_rwd_times(m_prayer_rwd_times);
}

uint32_t family_member::get_prayer_gift_times()
{
	if (common::time_util_t::now_time() > m_last_prayer_week_time) {
		prayer_week_update();
	}

	return m_prayer_gift_times;
}

void family_member::get_prayer_record_list(proto::common::family_prayer_record_list* p_list)
{
	if (!p_list) {
		log_error("p_list null error");
		return;
	}

	if (common::time_util_t::now_time() > m_last_prayer_week_time) {
		prayer_week_update();
	}

	p_list->CopyFrom(m_record_list);
}

void family_member::ask_prayer_hero(uint32_t hero_tid, uint32_t num)
{
	if (m_prayer_times == 0) {
		log_error("user[%lu] m_prayer_times == 0", m_uid);
		return;
	}

	m_prayer_times -= 1;
	family_data_mgr_t::update_prayer_times(m_uid, m_prayer_times);

	m_prayer_hero_tid = hero_tid;
	m_prayer_hero_num = 0;
	m_prayer_hero_max = num;
	m_prayer_start_time = common::time_util_t::now_time();
	m_prayer_complete_time = 0;

	proto::common::family_gift_data_list m_new_list;
	m_gift_list.Swap(&m_new_list);
	m_gift_list_map.clear();

	family_data_mgr_t::update_prayer_hero_info(shared_from_this());

	WRITE_FAMILY_LOG_TO_DB(m_family_id, m_uid, 0, common::log_enum::source_type_family_prayer_distribute, hero_tid, num);
}

uint32_t family_member::get_prayer_times()
{
	// 先更新下
	if (common::time_util_t::now_time() > m_last_prayer_day_time) {
		prayer_day_update();
	}

	return m_prayer_times;
}

uint32_t family_member::get_prayer_rwd_times()
{
	// 先更新下
	if (common::time_util_t::now_time() > m_last_prayer_day_time) {
		prayer_day_update();
	}

	return m_prayer_rwd_times;
}

uint32_t family_member::ask_gift_hero(const family_member_ptr& p_family_member, uint32_t hero_tid, uint32_t hero_material_id, uint32_t rarity, uint32_t hero_num)
{
	if (NULL == p_family_member) {
		log_error("NULL == p_family_member");
		return common::errcode_enum::notice_unknown;
	}

	if (hero_tid != m_prayer_hero_tid) {
		log_error("role[%lu] gift hero_tid[%u] != recive[%lu] prayer hero_tid[%u]", p_family_member->get_uid(), hero_tid, m_uid, m_prayer_hero_tid);
		return common::errcode_enum::notice_unknown;
	}

	if (m_prayer_hero_num >= m_prayer_hero_max) {
		FAMILY_LOG("user[%lu] recive[%lu] hero_tid[%u] is already enough", p_family_member->get_uid(), m_uid, hero_tid);
		return common::errcode_enum::family_error_prayer_is_over;
	}

	uint32_t max_gift_num = 0;
	switch (rarity) {
		case proto::common::hero_rarity_type_normal: {
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_hero_cnt0);
			max_gift_num = GET_COMPREHENSIVE_VALUE_1(conf);
		}
		break;
		case proto::common::hero_rarity_type_rare: {
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_hero_cnt1);
			max_gift_num = GET_COMPREHENSIVE_VALUE_1(conf);
		}
		break;
		case proto::common::hero_rarity_type_super_rare: {
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_hero_cnt2);
			max_gift_num = GET_COMPREHENSIVE_VALUE_1(conf);
		}
		break;
		case proto::common::hero_rarity_type_super_super_rare: {
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_hero_cnt3);
			max_gift_num = GET_COMPREHENSIVE_VALUE_1(conf);
		}
		break;
		default: {
			log_error("role[%lu] hero_tid[%u] rarity[%u] type not define", m_uid, hero_tid, rarity);
			return common::errcode_enum::notice_unknown;
		}
		break;
	}

	uint32_t user_gift_time = get_gift_time(p_family_member->get_uid());
	if (user_gift_time >= max_gift_num) {
		FAMILY_LOG("user[%lu] is gift max num", p_family_member->get_uid());
		return common::errcode_enum::family_error_prayer_gift_to_much;
	}

	if (m_prayer_hero_num + hero_num > m_prayer_hero_max) {
		FAMILY_LOG("user[%lu] recive[%lu] hero_tid[%u] is to much", p_family_member->get_uid(), m_uid, hero_tid);
		return common::errcode_enum::family_error_prayer_gift_once_to_much;
	}

	m_prayer_hero_num += hero_num;

	add_gift_time(p_family_member->get_uid());

	if (m_prayer_hero_num >= m_prayer_hero_max) {
		m_prayer_complete_time = time_manager_t::get_next_refresh_time(common::time_type_enum::time_one_day);
	}

	family_data_mgr_t::update_prayer_hero_info(shared_from_this());

	// 先清理下老的赠送和获得记录
	if (common::time_util_t::now_time() > m_last_prayer_week_time) {
		prayer_week_update();
	}

	// 增加一条获得奖励的记录
	proto::common::family_prayer_record_data* p_data = m_record_list.add_receive_list();
	if (p_data) {
		p_data->set_role_id(common::string_util_t::uint64_to_string(p_family_member->get_uid()));
		p_data->set_role_name(p_family_member->get_name());
		p_data->set_officer(p_family_member->get_officer());
		p_data->set_plugin(p_family_member->get_plugin());
		p_data->set_time(common::time_util_t::now_time());
		p_data->set_hero_tid(hero_tid);
		p_data->set_hero_num(hero_num);
		p_data->set_level(p_family_member->get_level());
	}
	family_data_mgr_t::save_prayer_record(m_uid, m_record_list);

	// 发邮件
	MailTable* p_mail = GET_CONF(MailTable, common::mailcode_enum::sys_mail_family_pray_2);
	if (NULL == p_mail) {
		log_error("user[%lu] p_mail[%u] error null", p_family_member->get_uid(), common::mailcode_enum::sys_mail_family_pray_2);
		return common::errcode_enum::notice_unknown;
	}

	Item* p_item = GET_CONF(Item, hero_material_id);
	if (NULL == p_item) {
		log_error("user[%lu] Item[%u] error null", p_family_member->get_uid(), hero_material_id);
		return common::errcode_enum::notice_unknown;
	}

	std::string content_text;
	StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_mail->content()).c_str(), p_family_member->get_name().c_str(), p_item->get_name().c_str());

	std::map<uint32_t, uint32_t> mail_item;
	mail_item.insert(std::make_pair(hero_material_id, hero_num));

	mail_manager_t::send_mail(m_uid, proto::common::MAIL_TYPE_NORMAL, p_family_member->get_uid(), p_family_member->get_name(), common::string_util_t::convert_to_utf8(p_mail->title()), content_text, 
		mail_item, proto::common::MAIL_GET_TYPE_FAMILY_PRAYER);

	WRITE_FAMILY_LOG_TO_DB(m_family_id, m_uid, p_family_member->get_uid(), common::log_enum::source_type_family_prayer_receive, hero_tid, hero_num);

	if (m_prayer_hero_num >= m_prayer_hero_max) {
		WRITE_FAMILY_LOG_TO_DB(m_family_id, m_uid, 0, common::log_enum::source_type_family_prayer_complete, hero_tid, hero_num);
	}

	return common::errcode_enum::error_ok;
}

uint32_t family_member::get_gift_time(uint64_t uid)
{
	std::map<uint64_t, uint32_t>::iterator itr = m_gift_list_map.find(uid);
	if (itr != m_gift_list_map.end()) {
		return itr->second;
	}

	return 0;
}

void family_member::add_gift_time(uint64_t uid)
{
	std::map<uint64_t, uint32_t>::iterator itr = m_gift_list_map.find(uid);
	if (itr != m_gift_list_map.end()) {
		itr->second = itr->second + 1;

		std::string str_uid = common::string_util_t::uint64_to_string(uid);
		for (int32_t i = 0; i < m_gift_list.gift_datas_size(); ++i) {
			proto::common::family_gift_data* p_data = m_gift_list.mutable_gift_datas(i);
			if (p_data->role_id() == str_uid) {
				p_data->set_gift_times(itr->second);
				return;
			}
		}
	} else {
		proto::common::family_gift_data* p_new_data = m_gift_list.add_gift_datas();
		if (p_new_data) {
			p_new_data->set_role_id(common::string_util_t::uint64_to_string(uid));
			p_new_data->set_gift_times(1);
		}
	}
}

void family_member::do_gift_reward(const family_member_ptr& p_family_member, uint32_t hero_tid, uint32_t hero_material_id, uint32_t rarity, uint32_t hero_num, uint32_t& gift_rwd_item)
{
	if (NULL == p_family_member) {
		log_error("NULL == p_family_member");
		return;
	}

	// 是否可以领取奖励
	uint32_t prayer_contribution = 0;
	switch (rarity) {
		case proto::common::hero_rarity_type_normal: {
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_contribution0);
			prayer_contribution = GET_COMPREHENSIVE_VALUE_1(conf);

			if (m_prayer_rwd_times > 0) {
				conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_drop0);
				gift_rwd_item = GET_COMPREHENSIVE_VALUE_1(conf);
			}
		}
		break;
		case proto::common::hero_rarity_type_rare: {
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_contribution1);
			prayer_contribution = GET_COMPREHENSIVE_VALUE_1(conf);

			if (m_prayer_rwd_times > 0) {
				conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_drop1);
				gift_rwd_item = GET_COMPREHENSIVE_VALUE_1(conf);
			}
		}
		break;
		case proto::common::hero_rarity_type_super_rare: {
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_contribution2);
			prayer_contribution = GET_COMPREHENSIVE_VALUE_1(conf);

			if (m_prayer_rwd_times > 0) {
				conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_drop2);
				gift_rwd_item = GET_COMPREHENSIVE_VALUE_1(conf);
			}
		}
		break;
		case proto::common::hero_rarity_type_super_super_rare: {
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_contribution3);
			prayer_contribution = GET_COMPREHENSIVE_VALUE_1(conf);

			if (m_prayer_rwd_times > 0) {
				conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_drop3);
				gift_rwd_item = GET_COMPREHENSIVE_VALUE_1(conf);
			}
		}
		break;
		default: {
			log_error("role[%lu] hero_tid[%u] rarity[%u] type not define", m_uid, hero_tid, rarity);
		}
		break;
	}

	if (prayer_contribution > 0) add_prayer_contribution(prayer_contribution);

	if (m_prayer_rwd_times > 0) {
		m_prayer_rwd_times -= 1;
		family_data_mgr_t::update_prayer_rwd_times(m_uid, m_prayer_times);
		WRITE_FAMILY_LOG_TO_DB(m_family_id, m_uid, 0, common::log_enum::source_type_family_prayer_gift_rwd, gift_rwd_item, 0);
	}

	// 先清理下老的赠送和获得记录
	if (common::time_util_t::now_time() > m_last_prayer_week_time) {
		prayer_week_update();
	}

	add_prayer_gift_times();

	// 增加一条获得奖励的记录
	proto::common::family_prayer_record_data* p_data = m_record_list.add_gift_list();
	if (p_data) {
		p_data->set_role_id(common::string_util_t::uint64_to_string(p_family_member->get_uid()));
		p_data->set_role_name(p_family_member->get_name());
		p_data->set_officer(p_family_member->get_officer());
		p_data->set_plugin(p_family_member->get_plugin());
		p_data->set_time(common::time_util_t::now_time());
		p_data->set_hero_tid(hero_tid);
		p_data->set_hero_num(hero_num);
		p_data->set_level(p_family_member->get_level());
	}
	family_data_mgr_t::save_prayer_record(m_uid, m_record_list);

	WRITE_FAMILY_LOG_TO_DB(m_family_id, m_uid, p_family_member->get_uid(), common::log_enum::source_type_family_prayer_gift, hero_tid, hero_num);
}

void family_member::add_prayer_contribution(uint32_t val)
{
	m_base.set_prayer_contribution(m_base.prayer_contribution() + val);

	family_data_mgr_t::update_prayer_contribution(m_uid, m_base.prayer_contribution());
}

void family_member::add_prayer_gift_times()
{
	m_prayer_gift_times += 1;

	family_data_mgr_t::update_prayer_gift_times(m_uid, m_prayer_gift_times);

	// 奖励
	FamilyPray* p_familypray = GET_CONF(FamilyPray, m_prayer_gift_times);
	if (NULL != p_familypray) {
		// 发邮件
		MailTable* p_mail = GET_CONF(MailTable, common::mailcode_enum::sys_mail_family_pray_1);
		if (NULL == p_mail) {
			log_error("user[%lu] p_mail[%u] error null", m_uid, common::mailcode_enum::sys_mail_family_pray_1);
			return;
		}

		std::string content_text;
		StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_mail->content()).c_str(), m_prayer_gift_times);

		mail_manager_t::send_mail(m_uid, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_mail->from_name()), common::string_util_t::convert_to_utf8(p_mail->title()), content_text,
			p_familypray->drop(), proto::common::MAIL_GET_TYPE_FAMILY_PRAYER);

		WRITE_FAMILY_LOG_TO_DB(m_family_id, m_uid, 0, common::log_enum::source_type_family_prayer_week_gift_rwd, m_prayer_gift_times, 0);
	}
}

bool family_member::is_have_prayer_hero()
{
	if (m_prayer_hero_tid != 0 && m_prayer_complete_time != 0) {
		if (common::time_util_t::now_time() > m_prayer_complete_time) {
			// 完成了并且在第二天了，清理数据
			m_prayer_hero_tid = 0;
			m_prayer_hero_num = 0;
			m_prayer_hero_max = 0;
			m_prayer_start_time = 0;
			m_prayer_complete_time = 0;

			proto::common::family_gift_data_list m_new_list;
			m_gift_list.Swap(&m_new_list);
			m_gift_list_map.clear();

			family_data_mgr_t::update_prayer_hero_info(shared_from_this());
		}
	}

	return m_prayer_hero_tid != 0;
}

void family_member::check_shop_time_out()
{
	if (m_shop_personal_data.next_refresh_time() != 0 && m_shop_personal_data.next_refresh_time() < common::time_util_t::now_time()) {
		m_shop_personal_data.Clear();
		family_data_mgr_t::clear_family_shop_personal(m_uid);
	}
}

void family_member::peek_family_shop_personal_data(proto::common::family_shop_personal_data* p_data)
{
	check_shop_time_out();

	if (p_data) p_data->CopyFrom(m_shop_personal_data);
}

uint32_t family_member::get_personal_buy_count(uint32_t index)
{
	check_shop_time_out();

	for (int32_t i = 0; i < m_shop_personal_data.items_size(); ++i) {
		const proto::common::family_shop_personal_item& item = m_shop_personal_data.items(i);
		if (item.shop_index() == index) {
			return item.buy_count();
		}
	}

	return 0;
}

void family_member::add_personal_buy_count(uint32_t refresh_time, uint32_t index, uint32_t num)
{
	m_shop_personal_data.set_next_refresh_time(refresh_time);

	bool is_find = false;
	for (int32_t i = 0; i < m_shop_personal_data.items_size(); ++i) {
		proto::common::family_shop_personal_item* p_item = m_shop_personal_data.mutable_items(i);
		if (p_item && p_item->shop_index() == index) {
			p_item->set_buy_count(p_item->buy_count() + num);
			is_find = true;
			break;
		}
	}

	if (!is_find) {
		proto::common::family_shop_personal_item* p_add_item = m_shop_personal_data.add_items();
		if (p_add_item) {
			p_add_item->set_shop_index(index);
			p_add_item->set_buy_count(num);
		}
	}

	family_data_mgr_t::save_family_shop_personal(m_uid, m_shop_personal_data);
}

void family_member::personal_failed_back(uint32_t refresh_time, uint32_t index, uint32_t num)
{
	if (m_shop_personal_data.next_refresh_time() == refresh_time) {
		for (int32_t i = 0; i < m_shop_personal_data.items_size(); ++i) {
			proto::common::family_shop_personal_item* p_item = m_shop_personal_data.mutable_items(i);
			if (p_item && p_item->shop_index() == index && p_item->buy_count() >= num) {
				p_item->set_buy_count(p_item->buy_count() - num);
				family_data_mgr_t::save_family_shop_personal(m_uid, m_shop_personal_data);
				break;
			}
		}
	}
}
