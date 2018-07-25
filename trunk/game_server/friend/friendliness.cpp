#include "friendliness.h"
#include "center/center_msg_handler.hpp"
#include "tblh/Friendliness.tbls.h"
#include "common/config_mgr.h"
#include "role/role.hpp"
#include "client.pb.h"
#include "role/role_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "main/game_server_fwd.hpp"
#include "friend/friend_manager.hpp"

void friendliness_t::change_friendliness(uint64_t friend_role_id, int32_t change, proto::common::change_friendliness_reason reason)
{
	//修改数据
	auto it = m_all_my_friends_friendliness.find(friend_role_id);
	if (it == m_all_my_friends_friendliness.end())
	{
		auto& save_data = m_all_my_friends_friendliness[friend_role_id];
		save_data.friendliness = change;
		save_data.friend_role_id = friend_role_id;
	}
	else
		it->second.friendliness += change;
}

void friendliness_t::seek_data_from_center()
{
	if(friend_manager_t::isClose())
		return;
	proto::server::ge_get_friendliness_request request;
	request.set_role_id(m_my_role_id);
	env::server->send_msg_to_center(op_cmd::ge_get_friendliness_request, m_my_role_id, request);
	FRIEND_LOG("friend[%lu] data seeked", m_my_role_id);
}

void friendliness_t::on_get_friendliness_data_from_center(const ::google::protobuf::RepeatedPtrField<proto::server::player_friendliness>& data)
{
	for (const auto& friendliness : data)
	{
		auto& save_data = m_all_my_friends_friendliness[friendliness.friend_role_id()];
		save_data.friendliness = friendliness.friendliness();
		save_data.friend_role_id = friendliness.friend_role_id();
	}
	inited = true;
	FRIEND_LOG("friend[%lu] data inited", m_my_role_id);
}

int32_t friendliness_t::get_friend_friendliness(uint64_t friend_role_id)
{
	auto it = m_all_my_friends_friendliness.find(friend_role_id);
	if (it == m_all_my_friends_friendliness.end()) {
		FRIEND_LOG("friend[%lu] is not in user's[%lu] m_all_my_friends_friendliness", friend_role_id, m_my_role_id);
		return 0;
	}
	return it->second.friendliness;
}

uint32_t friendliness_t::exp_affected_by_friendliness(role_ptr role, const proto::common::fight_camp& camp_data)
{
	if(friend_manager_t::isClose())
		return 0;
	if (role == NULL) {
		log_error("role is NULL");
		return 0;
	}
	if (role->get_friendliness_data().inited == false) {
		log_error("role[%lu] friendliness data not inited", role->get_uid());
	}
	uint32_t extra = 0;
	for (const proto::common::obj_single& obj : camp_data.objs().obj())
	{
		if (obj.object_type() != proto::common::SCENEOBJECT_USER)
			continue;
		
		uint64_t team_mate_role_id = string_util_t::string_to_uint64(obj.object_uid());
		if (team_mate_role_id == role->get_uid())
			continue;
		int32_t friendliness = role->get_friendliness_data().get_friend_friendliness(team_mate_role_id);
		if (friendliness > 0)
		{
			//一起战斗增加亲密度
			uint32_t val = 0;
			auto* p_config = GET_CONF(Comprehensive, comprehensive_common::qinmidu_add_fight);
			if (p_config != NULL) {
				val = GET_COMPREHENSIVE_VALUE_1(p_config);
			}
			else {
				log_error("qinmidu_add_fight not in Comprehensive table!");
			}
			friendliness_t::request_change_friendliness(role->get_uid(), team_mate_role_id, val, proto::common::change_friendliness_reason::change_reason_fight_together);
		}
		auto* p_config = friendliness_t::get_config_by_friendliness(friendliness);
		if (p_config == NULL)
			continue;

		//至此，在同阵营中发现了一位亲密度
		extra += p_config->extra_exp();
	}
	return extra;
	//role->get_friendliness_data().
}

void friendliness_t::add_friendliness_when_gather_treasure(const std::vector<uint64_t>& team)
{
	if(friend_manager_t::isClose())
		return;
	for (auto id : team)
	{
		role_ptr role = role_manager_t::find_role(id);
		if (role == NULL) {
			log_error("teammate[%lu] not found!", id);
			continue;
		}
		if (role->get_friendliness_data().inited == false) {
			log_error("role[%lu] friendliness data not inited", role->get_uid());
			continue;
		}
		for (auto teammate_id : team)
		{
			if (teammate_id == id)
				continue;
			int32_t friendliness = role->get_friendliness_data().get_friend_friendliness(teammate_id);
			if (friendliness > 0)
			{
				//一起战斗增加亲密度
				uint32_t val = 0;
				auto* p_config = GET_CONF(Comprehensive, comprehensive_common::qinmidu_add_treasure);
				if (p_config != NULL) {
					val = GET_COMPREHENSIVE_VALUE_1(p_config);
					friendliness_t::request_change_friendliness(id, teammate_id, val, proto::common::change_friendliness_reason::change_reason_treasure);
				}
			}
		}
	}
}

uint32_t friendliness_t::money_affected_by_friendliness(role_ptr role, const proto::common::fight_camp & camp_data)
{
	if(friend_manager_t::isClose())
		return 0;
	if (role == NULL) {
		log_error("role is NULL");
		return 0;
	}
	if (role->get_friendliness_data().inited == false) {
		log_error("role[%lu] friendliness data not inited", role->get_uid());
	}
	uint32_t extra = 0;
	for (const proto::common::obj_single& obj : camp_data.objs().obj())
	{
		if (obj.object_type() != proto::common::SCENEOBJECT_USER)
			continue;

		uint64_t team_mate_role_id = string_util_t::string_to_uint64(obj.object_uid());
		if (team_mate_role_id == role->get_uid())
			continue;
		int32_t friendliness = role->get_friendliness_data().get_friend_friendliness(team_mate_role_id);
		if(friendliness == 0)
			continue;
		auto* p_config = friendliness_t::get_config_by_friendliness(friendliness);
		if (p_config == NULL) {
			log_error("friendliness[%d] is out of config range", friendliness);
			continue;
		}
		extra += p_config->extra_gold();
	}
	return extra;
}

Friendliness* friendliness_t::get_config_by_friendliness(int32_t friendliness_value)
{
	if (friendliness_value == 0) {
		FRIEND_LOG("friendliness_value is 0");
		return NULL;
	}
	uint32_t size = FriendlinessManager::getInstancePtr()->getSize();
	for (uint32_t i = 1; i <= size; ++i)
	{
		auto* config = GET_CONF(Friendliness, i);
		if (config == NULL) {
			log_error("config[Friendliness] row %u is NULL", i);
			continue;
		}
		if (friendliness_value <= config->friendliness_range())
			return config;
	}
	log_error("friendliness[%d] is out of config range", friendliness_value);
	return NULL;
}

void friendliness_t::request_change_friendliness(uint64_t my_role_id, uint64_t friend_role_id, int32_t value, proto::common::change_friendliness_reason reason)
{
	proto::server::ge_change_friendliness_request request;
	request.set_my_role_id(my_role_id);
	request.set_friend_role_id(friend_role_id);
	request.set_change_value(value);
	request.set_reason(reason);
	env::server->send_msg_to_center(op_cmd::ge_change_friendliness_request, 0, request);

}