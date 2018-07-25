#pragma once

#include "common/macros.hpp"
#include "protos_fwd.hpp"

class level_reward_mail_manager
{
public:
	static bool save_level_reward_mail_to_redis(uint64_t uid, const proto::common::level_reward_mail_recived_list& list);
	static bool load_level_reward_mail_to_redis(uint64_t uid, proto::common::level_reward_mail_recived_list* p_list);
};

