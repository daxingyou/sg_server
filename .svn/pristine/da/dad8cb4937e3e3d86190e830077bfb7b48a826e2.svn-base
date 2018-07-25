#ifndef __CHALLENGE_DATA_MGR_H__
#define __CHALLENGE_DATA_MGR_H__

#include "common.pb.h"
#include "challenge/challenge_manager.hpp"

class challenge_data_mgr_t
{
public:
	static void add_challenge(challenge_ptr p_challenge);

	static void update_challenge_state(uint64_t uid, uint32_t state);

	static void load_challenge_by_day(uint32_t day, challenge_map& challenge_data);

	static void load_challenge_record(uint32_t day_count, uint32_t record_count, challenge_record& record);
};
#endif
