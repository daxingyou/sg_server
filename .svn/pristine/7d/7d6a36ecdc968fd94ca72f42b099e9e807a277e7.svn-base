#pragma once

#include <map>

#include "global_id.hpp"
#include "partner.hpp"

// 伙伴管理器
class partner_manager_t
{
public:
	// 关服清理
	static void close_clear();

	// 查找伙伴
	static partner_ptr find_partner(uint64_t uid);
	// 删除生物
	static void delete_partner(uint64_t uid);
	// 添加生物
	static bool add_partner(partner_ptr p_partner);

public:
	typedef std::map<uint64_t, partner_ptr> partner_map;

protected:
	static partner_map m_partner_map;
};
