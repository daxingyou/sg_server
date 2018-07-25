#pragma once

#include "role/role.hpp"

class country_manager_t
{
public:
	typedef std::map<uint32_t, proto::common::country_common> country_common_map;

public:
	// 服务器更新国家信息
	static void update_country_common(const proto::server::eg_update_country_notify& notify);
	// 得到国家信息
	static void peek_country_common(uint32_t id, proto::common::country_common* p_data);

public:
	// 修改国家名称扣钱
	static uint32_t change_country_name_flag(const role_ptr& p_role, const std::string& name, uint32_t flag);
	// 修改国家年号扣钱
	static uint32_t change_country_year_name(const role_ptr& p_role, const std::string& name);

protected:
	static country_common_map m_country_common_map;
};