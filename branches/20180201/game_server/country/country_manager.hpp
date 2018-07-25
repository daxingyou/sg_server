#pragma once

#include "role/role.hpp"

class country_manager_t
{
public:
	typedef std::map<uint32_t, proto::common::country_common> country_common_map;

public:
	// ���������¹�����Ϣ
	static void update_country_common(const proto::server::eg_update_country_notify& notify);
	// �õ�������Ϣ
	static void peek_country_common(uint32_t id, proto::common::country_common* p_data);

public:
	// �޸Ĺ������ƿ�Ǯ
	static uint32_t change_country_name_flag(const role_ptr& p_role, const std::string& name, uint32_t flag);
	// �޸Ĺ�����ſ�Ǯ
	static uint32_t change_country_year_name(const role_ptr& p_role, const std::string& name);

protected:
	static country_common_map m_country_common_map;
};