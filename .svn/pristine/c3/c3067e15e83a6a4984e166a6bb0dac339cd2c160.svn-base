#include "simple_data_manager.hpp"
#include "user/global_user_manager.hpp"
#include "main/center_server.hpp"
#include "utility.hpp"
#include "redis_client.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "rank/rank_manager.hpp"
#include "country/country_mgr.hpp"
#include "family/family_manager.hpp"
#include "redis/global_user_data_mgr.hpp"
#include "common/global_id.hpp"

bool simple_data_manager_t::save_simle_info_to_redis(const uint64_t uid, const proto::common::role_blob_data& notify, proto::common::role_data_type type)
{
	switch (type) {
		case proto::common::simple_info_type: {
			return redis_client_t::set_protobuf("role_simple_data", "simple_role_data", uid, notify);
			break;
		}
		default: {
			break;
		}
	}

	return false;
}

bool simple_data_manager_t::load_simle_info_from_redis(const uint64_t uid, proto::common::role_blob_data& notify,proto::common::role_data_type type)
{
	switch (type) {
		case proto::common::simple_info_type: {
			bool result = redis_client_t::get_protobuf("role_simple_data", "simple_role_data", uid, notify);
			return result;
			break;
		}
		default: {
			break;
		}
	}

	return false;
}

bool simple_data_manager_t::get_other_role_info(const uint64_t uid, const uint64_t other_uid,uint32_t type /* = 0 */)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user) {
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_other_role_info_reply reply;

	proto::common::role_blob_data notify;
	if (!load_simle_info_from_redis(other_uid, notify, proto::common::simple_info_type)) {
		log_error("user[%lu] look other user[%lu] fail !!!",uid, other_uid);
		return false;
	}
	
	reply.set_uid(string_util_t::uint64_to_string(other_uid));
	reply.set_did(common::user_did_generator::gen_user_did(other_uid));
	proto::common::role_base_data* p_role_base = reply.mutable_base_info();
	p_role_base->CopyFrom(notify.simple().base());
	global_user_ptr p_other_user = global_user_manager_t::get_global_user_by_uid(other_uid);
	if (NULL != p_other_user)
	{
		if (0 != p_other_user->get_troop_id())
		{
			p_role_base->set_team_id(string_util_t::uint64_to_string(p_other_user->get_troop_id()));
		}
		country_ptr p_country = country_mgr_t::get_country(p_other_user->get_country_id());
		if (NULL != p_country)
		{
			p_role_base->set_country_name(p_country->get_country_name());
			p_role_base->set_country_flag(p_country->get_country_flag());
		}
		p_role_base->set_fighting(p_other_user->get_fighting_value());
		p_role_base->set_name(p_other_user->get_name());
		p_role_base->set_head_info(p_other_user->get_hero_plugin());
		p_role_base->set_role_level(p_other_user->get_level());
		p_role_base->set_charm(p_other_user->get_charm());
		const family_ptr& p_family = family_manager_t::get_family(p_other_user->get_family_id());
		if (NULL != p_family)
		{
			p_role_base->set_family_name(p_family->get_name());
		}
	}
	else
	{
		uint32_t country_id = global_user_data_mgr_t::get_user_country_id(other_uid);
		if (0 != country_id)
		{
			country_ptr p_country = country_mgr_t::get_country(country_id);
			if (NULL != p_country)
			{
				p_role_base->set_country_name(p_country->get_country_name());
				p_role_base->set_country_flag(p_country->get_country_flag());
			}
		}
		p_role_base->set_fighting(global_user_data_mgr_t::get_user_fighting_value(other_uid));
		p_role_base->set_name(global_user_data_mgr_t::get_name(other_uid));
		p_role_base->set_head_info(global_user_data_mgr_t::get_user_hero_plugin(other_uid));
		p_role_base->set_role_level(global_user_data_mgr_t::get_user_level(other_uid));
		p_role_base->set_charm(global_user_data_mgr_t::get_user_charm(other_uid));
		const family_ptr& p_family = family_manager_t::get_family(global_user_data_mgr_t::get_user_family_id(other_uid));
		if (NULL != p_family)
		{
			p_role_base->set_family_name(p_family->get_name());
		}
	}
	 
	if(other_info_type::other_info_all == type)
	{
		proto::common::hero_data* p_hero = reply.mutable_hero();
		proto::common::item_data* p_item = reply.mutable_equip();
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

		for (const auto& it : param_list)
		{
			rank_sort_ptr p_rank = rank_manager_t::get_rank_sort_by_type(it);
			if (p_rank) {
				uint32_t rank_value = p_rank->get_rank_value(other_uid);
				if (rank_value != 0) {
					proto::common::other_rank_data* p_other_rank_data = reply.add_rank();
					if (p_other_rank_data) {
						p_other_rank_data->set_type(it);
						p_other_rank_data->set_value(p_rank->get_rank_score(other_uid));
						p_other_rank_data->set_level(rank_value);
					}
				}				
			}
		}
	}
	
	p_user->send_msg_to_client(op_cmd::ec_other_role_info_reply, reply);

	return true;
}



