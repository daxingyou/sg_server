#include "simple_data_manager.hpp"
#include "user/global_user_manager.hpp"
#include "main/center_server.hpp"
#include "utility.hpp"
#include "redis_client.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "rank/rank_manager.hpp"

bool simple_data_manager_t::save_simle_info_to_redis(const uint64_t uid,const proto::common::role_blob_data& notify, proto::common::role_data_type type)
{
	switch (type)
	{
	case proto::common::simple_info_type:
	{
		proto::common::role_simple_data data;
		data.CopyFrom(notify.simple());
		return redis_client_t::set_protobuf("role_simple_data", "simple_role_data", uid, data);
	}break;
	default:
		break;
	}

	return false;
}

bool simple_data_manager_t::load_simle_info_from_redis(const uint64_t uid, proto::common::role_blob_data& notify,proto::common::role_data_type type)
{
	switch (type)
	{
	case proto::common::simple_info_type:
	{
		proto::common::role_simple_data data;
		proto::common::role_simple_data* p_data = notify.mutable_simple();
		bool result = redis_client_t::get_protobuf("role_simple_data", "simple_role_data", uid, data);
		p_data->CopyFrom(data);
		return result;
	}break;
	default:
		break;
	}

	return false;
}

bool simple_data_manager_t::get_other_role_info(const uint64_t uid,const uint64_t other_uid)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	proto::client::ec_other_role_info_reply reply;

	proto::common::role_blob_data notify;
	if (!load_simle_info_from_redis(other_uid, notify, proto::common::simple_info_type))
	{
		log_error("user[%lu] look other user[%lu] fail !!!",uid, other_uid);
		return false;
	}

	reply.set_uid(string_util_t::uint64_to_string(other_uid));
	proto::common::hero_data* p_hero = reply.mutable_hero();
	proto::common::item_data* p_item = reply.mutable_equip();
	reply.set_role_level(notify.simple().base().role_level());
	reply.set_name(notify.simple().base().name());
	reply.set_family_name(notify.simple().base().family_name());
	p_hero->CopyFrom(notify.simple().hero());
	p_item->CopyFrom(notify.simple().item());

	std::vector<uint32_t> param_list;
	Comprehensive* p_config = GET_CONF(Comprehensive, comprehensive_common::player_rankid);
	if (NULL == p_config)
	{
		log_error("NULL == p_config player_rankid");
	}
	else
	{
		string_util_t::split<uint32_t>(p_config->parameter1(), param_list, ":");
	}

	for (auto& it : param_list)
	{
		proto::common::client_rank_single out;
		if (!rank_manager_t::get_rank_data(it, &out, other_uid))
			continue;
		proto::common::other_rank_data* p_other_rank_data = reply.add_rank();
		p_other_rank_data->set_type(it);
		p_other_rank_data->set_value(out.key());
		p_other_rank_data->set_level(out.index());
	}

	p_user->send_msg_to_client(op_cmd::ec_other_role_info_reply, reply);

	return true;
}

bool simple_data_manager_t::update_role_simle_info(const uint64_t uid,const proto::common::role_blob_data& notify,proto::common::role_data_type type)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	return save_simle_info_to_redis(uid, notify, type);
}
