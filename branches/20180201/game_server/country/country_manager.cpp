#include "country_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"
#include "log_enum.hpp"
#include "role/money_manager.hpp"

country_manager_t::country_common_map country_manager_t::m_country_common_map;

void country_manager_t::update_country_common(const proto::server::eg_update_country_notify& notify)
{
	for (int32_t i = 0; i < notify.countries_size(); ++i) {
		const proto::common::country_common& country = notify.countries(i);
		m_country_common_map[country.country_id()] = country;
	}
}

void country_manager_t::peek_country_common(uint32_t id, proto::common::country_common* p_data)
{
	country_common_map::const_iterator citr = m_country_common_map.find(id);
	if (citr != m_country_common_map.end() && p_data) {
		p_data->CopyFrom(citr->second);
	}
}

uint32_t country_manager_t::change_country_name_flag(const role_ptr& p_role, const std::string& name, uint32_t flag)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	money_ptr p_money = p_role->get_money();
	if (p_money == NULL) {
		log_error("role[%lu] p_money null error", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::country_edit_name_cost);
	uint32_t cost = GET_COMPREHENSIVE_VALUE_1(conf);

	uint32_t money = p_money->get_money(proto::common::MONEY_TYPE_YUANBAO);
	if (money < cost) {
		log_error("role[%lu] change country name, glod[%u] < %u", p_role->get_uid(), money, cost);
		return common::errcode_enum::country_error_gold_no_enough;
	}

	proto::server::ge_country_name_change_reply reply;
	// ¿ÛÇ®
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, cost, log_enum::source_type_country_change_name_flag,
		0, false, reply.mutable_change_data()))
	{
		log_error("role[%lu] use_money money error, when change_country_name_flag", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	if (reply.mutable_change_data()) p_role->get_personal_info(reply.mutable_change_data()->mutable_per_info());

	reply.set_reply_code(common::errcode_enum::error_ok);
	reply.set_name(name);
	reply.set_flag_id(flag);
	env::server->send_msg_to_center(op_cmd::ge_country_name_change_reply, p_role->get_uid(), reply);

	return common::errcode_enum::error_ok;
}

uint32_t country_manager_t::change_country_year_name(const role_ptr& p_role, const std::string& name)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	money_ptr p_money = p_role->get_money();
	if (p_money == NULL) {
		log_error("role[%lu] p_money null error", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::country_edit_titile_cost);
	uint32_t cost = GET_COMPREHENSIVE_VALUE_1(conf);

	uint32_t money = p_money->get_money(proto::common::MONEY_TYPE_YUANBAO);
	if (money < cost) {
		log_error("role[%lu] change country year_name, glod[%u] < %u", p_role->get_uid(), money, cost);
		return common::errcode_enum::country_error_gold_no_enough;
	}

	proto::server::ge_country_year_name_reply reply;
	// ¿ÛÇ®
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, cost, log_enum::source_type_country_modify_year_name,
		0, false, reply.mutable_change_data()))
	{
		log_error("role[%lu] use_money money error, when change_country_year_name", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	if (reply.mutable_change_data()) p_role->get_personal_info(reply.mutable_change_data()->mutable_per_info());

	reply.set_reply_code(common::errcode_enum::error_ok);
	reply.set_name(name);
	env::server->send_msg_to_center(op_cmd::ge_country_year_name_reply, p_role->get_uid(), reply);

	return common::errcode_enum::error_ok;
}
