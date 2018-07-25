#include "admin_func_manager.hpp"
#include "main/gm_server.hpp"
#include "log.hpp"
#include <string>
#include "protocpp/protos_fwd.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <common/utility.hpp>
#include "admin/admin_manager.hpp"
#include "redis_client.hpp"

USING_NS_COMMON;

bool admin_func_manager_t::send_mail_to_center( std::string &json_data )
{
	try {
		log_debug("json_data:[%s] ", json_data.c_str() );

		std::stringstream ss(json_data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);

		//读取玩家列表		
		std::string oper_name = pt.get<std::string>("oper_name");
		uint64_t uid		= pt.get<uint64_t>("id");
		uint32_t type		= pt.get<uint32_t>("mail_data.type");
		std::string title	= pt.get<std::string>("mail_data.title");
		std::string content	= pt.get<std::string>("mail_data.content");
		std::string send_name  = pt.get<std::string>("mail_data.send_name");
		uint32_t send_time  = pt.get<uint32_t>("mail_data.send_time");
		uint32_t end_time	= pt.get<uint32_t>("mail_data.end_time");
		uint32_t on_time	= pt.get<uint32_t>("mail_data.on_time");
		std::string items	= pt.get<std::string>("mail_data.items");
		uint32_t limit_level = pt.get<uint32_t>("mail_data.limit_level");
		uint32_t limit_cond = pt.get<uint32_t>("mail_data.limit_cond");

		proto::gm::me_send_mail_request request;
		proto::gm_common::global_mail_single *p_mail = request.mutable_mail_data();
		p_mail->set_uid(uid);
		p_mail->set_type(type);
		p_mail->set_send_name(send_name);
		p_mail->set_title(title);
		p_mail->set_content(content);
		p_mail->set_send_time(send_time);
		p_mail->set_over_time(end_time);
		p_mail->set_send_hour(on_time);
		p_mail->set_items(items);
		p_mail->mutable_limit()->set_level(limit_level);
		p_mail->mutable_limit()->set_regist_type(limit_cond);
		
		env::server->send_msg_to_center(op_cmd::me_send_mail_request, 0, request);
	}
	catch (boost::property_tree::ptree_error& e)
	{
		log_error("\033[throw error when parse json[%s], [%s]\n", e.what(), json_data.c_str() );
		return false;
	}

	return true;
}

bool admin_func_manager_t::get_user_num_from_center( uint32_t admin_id )
{
	proto::gm::me_get_user_ol_num_request request;
	request.set_admin_id( admin_id );
	env::server->send_msg_to_center(op_cmd::me_get_user_ol_num_request, 0,  request);
	return true;
}

bool admin_func_manager_t::send_user_num_to_tools(uint32_t admin_id, uint32_t num)
{
	admin_ptr p_admin = admin_manager_t::get_admin(admin_id);
	if (!p_admin)
	{
		log_debug("send user_num to tools , admin has offline");
		return false;
	}
	std::string num_str = string_util_t::uint32_to_string(num);
	network::msg_buf_ptr p_ret_buf(new network::msg_buf_t(num_str.c_str(), num_str.size()));
	env::server->post_msg_to_gm(p_admin->get_socket(), p_ret_buf);
	return true;
}


bool admin_func_manager_t::send_manage_role_to_tools(const proto::server::em_manage_role_ntf& msg)
{
	admin_ptr p_admin = admin_manager_t::get_admin(msg.admin_id());
	if (!p_admin)
	{
		log_debug("send_manage_role_to_tools, admin has offline");
		return false;
	}
	if (0 == msg.reply_data_size())
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("1", 1));
		env::server->post_msg_to_gm(p_admin->get_socket(), p_ret_buf);
		return false;
	}
	std::ostringstream os;
	os << "{ ";
	os << "\"role_did\":\"";
	for (int32_t i = 0; i < msg.reply_data_size(); ++i)
	{
		const proto::gm_common::manage_reply_single& mrs = msg.reply_data(i);
		uint32_t did = redis_client_t::get_uint32("global_user", "did", mrs.role_uid());
		os << did << "-" << mrs.expired_time();
		if (i != msg.reply_data_size() - 1)
		{
			os << ";";
		}
	}
	os << "\", ";
	os << "\"manage_type\":\"" << msg.manage_type() << "\", ";
	os << "\"offset_time\":\"" << msg.offset_time() << "\", ";
	os << "\"oper_time\":\"" << time_util_t::now_time() << "\", ";
	os << "\"server_id\":\"" << env::xml->srv_group.group_id << "\", ";
	os << "\"reason\":\"" << msg.reason() << "\", ";
	os << "\"oper_name\":\"" << msg.oper_name() << "\" ";
	os << " }";
	network::msg_buf_ptr p_ret_buf(new network::msg_buf_t(os.str().c_str(), os.str().size()));
	env::server->post_msg_to_gm(p_admin->get_socket(), p_ret_buf);
	return true;
}


bool admin_func_manager_t::search_role(const network::tcp_socket_ptr& s, uint64_t role_uid)
{
	std::ostringstream os;
	os << "{ ";
	os << "\"role_uid\":\"" << role_uid << "\", ";

	os << "\"role_did\":\"" << redis_client_t::get_uint32("global_user", "did", role_uid) << "\", ";
	os << "\"role_name\":\"" << redis_client_t::get_string("global_user", "name", role_uid) << "\", ";
	os << "\"create_time\":\"" << redis_client_t::get_uint32("global_user", "create_time", role_uid) << "\", ";
	//os << "\"last_login_time\":\"" << redis_client_t::get_uint32("global_user", "last_login_time", role_uid) << "\", ";
	os << "\"last_logout_time\":\"" << redis_client_t::get_uint32("global_user", "last_logout_time", role_uid) << "\", ";
	os << "\"level\":\"" << redis_client_t::get_uint32("global_user", "level", role_uid) << "\", ";
	os << "\"fighting\":\"" << redis_client_t::get_uint32("global_user", "fighting", role_uid) << "\", ";
	os << "\"scene_id\":\"" << redis_client_t::get_uint32("global_user", "map_tid", role_uid) << "\", ";

	os << "\"total_recharge\":\"" << redis_client_t::get_uint32("role_ex", "total_pay", role_uid) << "\", ";
	os << "\"vip_level\":\"" << redis_client_t::get_uint32("role_ex", "vip_level", role_uid) << "\", ";

	os << "\"gold\":\"" << redis_client_t::get_uint32("role_money", "yuanbao", role_uid) << "\", ";
	os << "\"silver\":\"" << redis_client_t::get_uint32("role_money", "silver", role_uid) << "\", ";
	os << "\"copper\":\"" << redis_client_t::get_uint32("role_money", "copper", role_uid) << "\", ";
	os << "\"life_stone\":\"" << redis_client_t::get_uint32("role_money", "life_stone", role_uid) << "\" ";
	os << " }";
	network::msg_buf_ptr p_ret_buf(new network::msg_buf_t(os.str().c_str(), os.str().size()));
	env::server->post_msg_to_gm(s, p_ret_buf);
	return true;
}

// bool admin_func_manager_t::notify_server_reload_xml(const std::string& server_type)
// {
// 	proto::gm::mall_reload_xml_notify notify;
// 	env::server->send_msg_to_center(op_cmd::me_reload_xml_request, 0, notify);
// 	return true;
// }

bool admin_func_manager_t::parse_tower_trigger(uint64_t role_uid, proto::common::tower_trigger_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("role_tower", role_uid))
	{
		log_error("role_tower role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("role_tower", "tower_trigger_data", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_expedition_data(uint64_t role_uid, proto::common::expedition_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("expedition", role_uid))
	{
		log_error("expedition role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("expedition", "expedition_data", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_activity_data(uint64_t role_uid, proto::common::activities_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("activity", role_uid))
	{
		log_error("activity role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("activity", "activities_data", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_luckydraw_data(uint64_t role_uid, proto::common::luckydraw_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("luckydraw", role_uid))
	{
		log_error("luckydraw role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("luckydraw", "luckydraw_data", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_skill_data(uint64_t hero_uid, proto::common::hero_skill_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("heroes", hero_uid))
	{
		log_error("heroes hero[%lu] not exist", hero_uid);
		return false;
	}
	redis_client_t::get_protobuf("heroes", "skill_data", hero_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_talent_data(uint64_t hero_uid, proto::common::talent_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("heroes", hero_uid))
	{
		log_error("heroes hero[%lu] not exist", hero_uid);
		return false;
	}
	redis_client_t::get_protobuf("heroes", "talent_data", hero_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_cur_task(uint64_t role_uid, proto::common::cur_task_date* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("task", role_uid))
	{
		log_error("task role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("task", "cur_task_list", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_comp_task(uint64_t role_uid, proto::common::comp_task_date* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("task", role_uid))
	{
		log_error("task role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("task", "comp_task_list", role_uid, *p_data);
	return true;
}

bool admin_func_manager_t::parse_liandan_data(uint64_t role_uid, proto::common::liandan_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("liandan", role_uid))
	{
		log_error("liandan role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("liandan", "all_liandan_data", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_equip_plan_data(uint64_t role_uid, proto::common::equip_plan_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("equip_plan", role_uid))
	{
		log_error("equip_plan role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("equip_plan", "all_equip_plan_data", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_prayer_log(uint64_t role_uid, proto::common::family_prayer_record_list* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("family_member", role_uid))
	{
		log_error("family_member role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("family_member", "prayer_log", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_arena_fight_data(uint64_t role_uid, proto::common::mirror_fight_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("arena_fight_data", role_uid))
	{
		log_error("arena_fight_data role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("arena_fight_data", "fight_data", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_achieve_data(uint64_t role_uid, proto::common::achieve_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("achieve", role_uid))
	{
		log_error("achieve role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("achieve", "achieve_data", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_dungeon_times_data(uint64_t role_uid, proto::common::role_all_dungeon_times_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("role_dungeon_times", role_uid))
	{
		log_error("role_dungeon_times role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("role_dungeon_times", "times_data", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_family_skill_data(uint64_t role_uid, proto::common::family_skill_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("family_skill", role_uid))
	{
		log_error("family_skill role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("family_skill", "all_family_skill_data", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_goods_equip_data(uint64_t goods_uid, proto::common::equip_single* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("goods", goods_uid))
	{
		log_error("goods[%lu] not exist", goods_uid);
		return false;
	}
	redis_client_t::get_protobuf("goods", "equip_info", goods_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_redbag_draw_data(uint64_t redbag_uid, proto::common::draw_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("redbag", redbag_uid))
	{
		log_error("redbag[%lu] not exist", redbag_uid);
		return false;
	}
	redis_client_t::get_protobuf("redbag", "draw_data", redbag_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_role_simple_data(uint64_t role_uid, proto::common::role_simple_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("role_simple_data", role_uid))
	{
		log_error("role_simple_data role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("role_simple_data", "simple_role_data", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_offline_arena_fight_data(uint64_t role_uid, proto::common::mirror_fight_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("offline_arena", role_uid))
	{
		log_error("offline_arena role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("offline_arena", "fight_data", role_uid, *p_data);
	return true;
}


bool admin_func_manager_t::parse_offline_arena_record_data(uint64_t role_uid, proto::common::offline_arena_record_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (!redis_client_t::is_hash_exist("offline_arena", role_uid))
	{
		log_error("offline_arena role[%lu] not exist", role_uid);
		return false;
	}
	redis_client_t::get_protobuf("offline_arena", "record_data", role_uid, *p_data);
	return true;
}

bool admin_func_manager_t::send_notice_to_center(std::string &json_data)
{
	try {
		log_debug("json_data:[%s] ", json_data.c_str());

		std::stringstream ss(json_data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);

		//读取玩家列表		
		uint32_t type = pt.get<uint32_t>("type");
		uint32_t tick = pt.get<uint32_t>("tick");
		uint32_t start_time = pt.get<uint32_t>("start_time");
		uint32_t end_time = pt.get<uint32_t>("end_time");
		std::string notice = pt.get<std::string>("notice");
		std::string oper_name = pt.get<std::string>("oper_name");
		
		if (end_time == 0)
		{
			log_error( "gm_tools send notice with end_time == 0, is not allow");
			return false;
		}

		proto::gm::me_send_notice_request request;
		auto p_data = request.mutable_notice_data();
		p_data->set_type(type);
		p_data->set_tick(tick);
		p_data->set_start_time(start_time);
		p_data->set_end_time(end_time);
		p_data->set_notice(notice);
		env::server->send_msg_to_center(op_cmd::me_send_notice_request, 0, request);
	}
	catch (boost::property_tree::ptree_error& e)
	{
		log_error("\033[throw error when parse json[%s], [%s]\n", e.what(), json_data.c_str());
		return false;
	}
	return true;
}

bool admin_func_manager_t::transport_to_center(std::string &json_data)
{
	try {
		log_debug("json_data:[%s] ", json_data.c_str());

		std::stringstream ss(json_data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);
		
		uint32_t role_did = pt.get<uint32_t>("id");
		uint32_t scene_id = pt.get<uint32_t>("scene_id");

		proto::gm::me_transport_ntf ntf;
		ntf.set_role_uid(role_did);
		ntf.set_scene_id(scene_id);
		env::server->send_msg_to_center(op_cmd::me_transport_ntf, 0, ntf);

	}
	catch (boost::property_tree::ptree_error& e)
	{
		log_error("\033[throw error when parse json[%s], [%s]\n", e.what(), json_data.c_str());
		return false;
	}
	return true;
}

bool admin_func_manager_t::set_user_create_limit(std::string &json_data)
{
	try {
		log_debug("json_data:[%s] ", json_data.c_str());

		std::stringstream ss(json_data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);

		uint32_t flag = pt.get<uint32_t>("flag");

		proto::gm::me_send_gm_msg_to_account_request ntf;
		ntf.set_cmd_type( op_cmd::tm_set_user_create_limit_request );
		proto::common::gm_msg_to_account *p_data = ntf.mutable_msg();
		p_data->set_user_create_flag( flag );
		env::server->send_msg_to_center(op_cmd::me_send_gm_msg_to_account_request, 0, ntf);
	}
	catch (boost::property_tree::ptree_error& e)
	{
		log_error("\033[throw error when parse json[%s], [%s]\n", e.what(), json_data.c_str());
		return false;
	}
	return true;
}

bool admin_func_manager_t::refresh_ac_login_list()
{
	proto::gm::me_send_gm_msg_to_account_request ntf;
	ntf.set_cmd_type(op_cmd::tm_refresh_login_list);
	env::server->send_msg_to_center(op_cmd::me_send_gm_msg_to_account_request, 0, ntf);
	return true;
}

bool admin_func_manager_t::manage_role(uint32_t admin_id, std::string &json_data)
{
	try {
		log_debug("json_data:[%s] ", json_data.c_str());

		std::stringstream ss(json_data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);

		//读取玩家列表		
		std::string role_did = pt.get<std::string>("id");
		std::vector<uint32_t> did_vec;
		string_util_t::split(role_did, did_vec, ";");
		if (did_vec.empty())
		{
			log_error("gm_tools manage_role role_did empty");
			return false;
		}
		std::vector<uint64_t> uid_vec;
		for (auto did : did_vec)
		{
			std::vector<uint64_t> users;
			redis_client_t::get_keys_by_field_condition<uint64_t, uint32_t, std::vector<uint64_t>>("global_user", "did", did, users);
			if (users.size() > 0)
			{
				uid_vec.push_back(users[0]);
			}
		}
		if (uid_vec.empty())
		{
			log_error("gm_tools manage_role role_uid empty");
			return false;
		}
		uint32_t manage_type = pt.get<uint32_t>("manage_type");
		uint32_t offset_time = pt.get<uint32_t>("offset_time");
		std::string reason = pt.get<std::string>("reason");
		std::string oper_name = pt.get<std::string>("oper_name");
		
		proto::server::me_manage_role_ntf ntf;
		ntf.set_manage_type(manage_type);
		ntf.set_offset_time(offset_time);
		ntf.set_admin_id(admin_id);
		ntf.set_reason(reason);
		ntf.set_oper_name(oper_name);
		for (auto uid : uid_vec)
		{
			ntf.add_role_uid(uid);
		}
		env::server->send_msg_to_center(op_cmd::me_manage_role_ntf, 0, ntf);
	}
	catch (boost::property_tree::ptree_error& e)
	{
		log_error("\033[throw error when parse json[%s], [%s]\n", e.what(), json_data.c_str());
		return false;
	}
	return true;
}

bool admin_func_manager_t::search_role_byid(const network::tcp_socket_ptr& s, std::string &json_data)
{
	try {
		log_debug("json_data:[%s] ", json_data.c_str());

		std::stringstream ss(json_data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);

		//读取玩家列表		
		uint32_t role_did = pt.get<uint32_t>("id");
		uint64_t user_uid = 0;
		std::vector<uint64_t> users;
		redis_client_t::get_keys_by_field_condition<uint64_t, uint32_t, std::vector<uint64_t>>("global_user", "did", role_did, users);
		if (users.size() > 0)
		{
			user_uid = users[0];
		}
		if (0 == user_uid)
		{
			log_warn("role_did[%d] not found", role_did);
			network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("1", 1));
			env::server->post_msg_to_gm(s, p_ret_buf);
		}
		else
		{
			search_role(s, user_uid);
		}
	}
	catch (boost::property_tree::ptree_error& e)
	{
		log_error("\033[throw error when parse json[%s], [%s]\n", e.what(), json_data.c_str());
		return false;
	}
	return true;
}

bool admin_func_manager_t::search_role_byname(const network::tcp_socket_ptr& s, std::string &json_data)
{
	try {
		log_debug("json_data:[%s] ", json_data.c_str());

		std::stringstream ss(json_data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);

		//读取玩家列表		
		std::string role_name = pt.get<std::string>("role_name");
		uint64_t user_uid = 0;
		std::vector<uint64_t> users;
		redis_client_t::get_keys_by_field_condition<uint64_t, std::string, std::vector<uint64_t>>("global_user", "name", role_name, users);
		if (users.size() > 0)
		{
			user_uid = users[0];
		}
		if (0 == user_uid)
		{
			log_warn("role_name[%s] not found", role_name.c_str());
			network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("1", 1));
			env::server->post_msg_to_gm(s, p_ret_buf);
		}
		else
		{
			search_role(s, user_uid);
		}
	}
	catch (boost::property_tree::ptree_error& e)
	{
		log_error("\033[throw error when parse json[%s], [%s]\n", e.what(), json_data.c_str());
		return false;
	}
	return true;
}

bool admin_func_manager_t::reload_config(std::string &json_data)
{
	try {
		log_debug("json_data:[%s] ", json_data.c_str());

		std::stringstream ss(json_data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);

		uint32_t load_type = pt.get<uint64_t>("load_type");
		std::string file_name = pt.get<std::string>("file_name");
		
		proto::server::me_reload_config_notify ntf;
		ntf.set_load_type(load_type);
		ntf.set_file_name(file_name);
		env::server->send_msg_to_center(op_cmd::me_reload_config_notify, 0, ntf);
	}
	catch (boost::property_tree::ptree_error& e)
	{
		log_error("\033[throw error when parse json[%s], [%s]\n", e.what(), json_data.c_str());
		return false;
	}
	return true;
}
