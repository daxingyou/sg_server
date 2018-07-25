#ifndef __FRIENDLINESS_H__
#define __FRIENDLINESS_H__

#include "macros.hpp"
#include <map>
#include "friendlinessdata.h"
#include "object/object_position.hpp"
#include "server.pb.h"
#include "common.pb.h"

class Friendliness;

class friendliness_t
{
public:
	friendliness_t(uint64_t id) :m_my_role_id(id) {}
	void seek_data_from_center();
	void change_friendliness(uint64_t friend_role_id, int32_t change, proto::common::change_friendliness_reason reason);
	void on_get_friendliness_data_from_center(const ::google::protobuf::RepeatedPtrField<proto::server::player_friendliness >& data);
	int32_t get_friend_friendliness(uint64_t friend_role_id);
	static uint32_t exp_affected_by_friendliness(role_ptr role, const proto::common::fight_camp& camp_data);
	static uint32_t money_affected_by_friendliness(role_ptr role, const proto::common::fight_camp& camp_data);
	static void add_friendliness_when_gather_treasure(const std::vector<uint64_t>& team);
	static Friendliness* get_config_by_friendliness(int32_t friendliness_value);
	static void request_change_friendliness(uint64_t my_role_id, uint64_t friend_role_id, int32_t value, proto::common::change_friendliness_reason reason);
	bool inited = false;
private:
	std::map<uint64_t, friendlinessdata_t> m_all_my_friends_friendliness;
	uint64_t m_my_role_id;
};
typedef boost::shared_ptr<friendliness_t> friendliness_ptr;

#endif