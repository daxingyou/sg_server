#pragma once

#include "macros.hpp"
#include "protos_fwd.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <set>

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

/**
 * \brief µÈ¼¶½±Àø
 */

class role_level_reward_t
{
public:
	role_level_reward_t(uint64_t uid);
	~role_level_reward_t();

public:
	void load_data(const proto::common::level_reward_recived_list& data);
	void peek_data(proto::common::level_reward_recived_list* p_data);
	void save_data();

public:
	uint32_t get_level_reward(uint32_t id, role_ptr p_role, proto::common::role_change_data* p_data);
	bool is_recived_all_reward();

protected:
	uint64_t m_role_uid = 0;
	std::string	m_key = "";
	std::set<uint32_t> m_recived_map;
	proto::common::level_reward_recived_list m_recived_list;
};