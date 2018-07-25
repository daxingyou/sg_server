#include "gm_func.hpp"
#include "utility.hpp"
#include "zdb_ex.hpp"
#include "redis_client.hpp"
#include <boost/lexical_cast.hpp>
#include "mail/global_mail_manager.hpp"
#include "common/global_id.hpp"
#include "user/global_user_manager.hpp"
#include "redis/global_user_data_mgr.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "mail/mail_manager.hpp"
USING_NS_COMMON;
void gm_func::ban_account(std::string account, uint32_t flag)
{
// 	if (0 == flag)
// 	{
// 		redis_client_t::add_key("account_ban", account);
// 	}
// 	else if (1 == flag)
// 	{
// 		redis_client_t::remove_key("account_ban", account);
// 	}

}

void gm_func::access_ip(std::string ip_address, uint32_t ip_end, uint32_t flag)
{
// 	std::vector<uint32_t> tmp_vec;
// 	string_util_t::split(ip_address, tmp_vec, ".");
// 	if (4 != tmp_vec.size())
// 	{
// 		return;
// 	}
// 	if (tmp_vec[3] > ip_end)
// 	{
// 		return;
// 	}
// 	for (uint32_t i = tmp_vec[3]; i < ip_end+1; ++i)
// 	{
// 		std::ostringstream oss;
// 		oss << tmp_vec[0] << "." << tmp_vec[1] << "." << tmp_vec[2] << "." << i;
// 		access_ip(oss.str(), flag);
// 	}
}

void gm_func::access_ip(std::string ip_address, uint32_t flag)
{
// 	if (0 == flag)
// 	{
// 		redis_client_t::add_key("ip_access", ip_address);
// 	}
// 	else if (1 == flag)
// 	{
// 		redis_client_t::remove_key("ip_access", ip_address);
// 	}
}

void gm_func::send_mail(proto::gm::me_send_mail_request &request)
{
	const proto::gm_common::global_mail_single &mail_data = request.mail_data();
	
	uint32_t role_did = (uint32_t)mail_data.uid();
	if (role_did != 0){
		
		std::vector<uint64_t> users;
		redis_client_t::get_keys_by_field_condition<uint64_t, uint32_t, std::vector<uint64_t>>("global_user", "did", role_did, users);
		if (users.size() > 0)
		{
			std::vector<std::string> item_vec;
			common::string_util_t::split<std::string>(mail_data.items(), item_vec, "$");
			std::vector<uint32_t> item_v;
			std::map<uint32_t, uint32_t> items;
			for (uint32_t i = 0; i < item_vec.size(); ++i)
			{
				item_v.clear();
				common::string_util_t::split<uint32_t>(item_vec[i], item_v, ":");
				if (item_v.size() == 2)
				{
					items[item_v[0]] = item_v[1];
				} else
				{
					log_error("wrong items syntax:%s", mail_data.items().c_str());
					break;
				}
			}
			mail_manager_t::send_mail(users[0], proto::common::MAIL_TYPE_SYS, 0, mail_data.send_name(), mail_data.title(),
				mail_data.content(), items, proto::common::MAIL_GET_TYPE_SYS);
		} else
		{
			log_error("wrong did:%u", role_did);
		}
		
	}
	else{
		std::set<uint64_t> uids;

		//邮件限制条件
		const proto::gm_common::mail_recv_limit &limit = mail_data.limit();
		global_mail_manager_t::add_new_global_mail(EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_GM, limit.level(),limit.regist_type(), uids,
							mail_data.send_time(), mail_data.over_time(),
							mail_data.send_name(),
							mail_data.title(),
							mail_data.content(),
							mail_data.items(),
							"");
	}
}


void gm_func::send_ol_nun(uint32_t admin_id)
{
	proto::gm::em_get_user_ol_num_reply request;
	request.set_admin_id(admin_id);
	request.set_ol_num( global_user_manager_t::get_all_user_num() );
	env::server->send_msg_to_gm(op_cmd::em_get_user_ol_num_reply, 0 , request);
}

void gm_func::send_notice(proto::gm::me_send_notice_request &request)
{
	sys_notice_manager_t::add_gm_system_notice( request.notice_data() );
}

void gm_func::transport(proto::gm::me_transport_ntf &request)
{
	uint64_t role_uid = 0;
	std::vector<uint64_t> users;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint32_t, std::vector<uint64_t>>("global_user", "did", request.role_uid(), users);
	if (users.size() > 0)
	{
		role_uid = users[0];
	}

	global_user_ptr p_src_user = global_user_manager_t::get_global_user_by_uid(role_uid);
	if (NULL == p_src_user)
	{
		log_error("NULL == p_src_user");
		return;
	}

	proto::server::eg_gm_transport_ntf msg;
	msg.set_role_uid(role_uid);
	msg.set_scene_id(request.scene_id());
	p_src_user->send_msg_to_game(op_cmd::eg_gm_transport_ntf, msg);
}

void gm_func::manage_role(const proto::server::me_manage_role_ntf& msg)
{
	proto::server::em_manage_role_ntf ntf;
	ntf.set_admin_id(msg.admin_id());
	ntf.set_manage_type(msg.manage_type());
	ntf.set_offset_time(msg.offset_time());
	ntf.set_reason(msg.reason());
	ntf.set_oper_name(msg.oper_name());
	
	for (int32_t i = 0; i < msg.role_uid_size(); ++i)
	{
		manage_role(msg.role_uid(i), msg.manage_type(), msg.offset_time(), ntf);
	}
	env::server->send_msg_to_gm(op_cmd::em_manage_role_ntf, 0, ntf);
}

void gm_func::manage_role(uint64_t role_uid, uint32_t manage_type, uint32_t offset_time, proto::server::em_manage_role_ntf& ntf)
{
	if (!global_user_data_mgr_t::is_user_key_exsit(role_uid))
	{
		log_error("role[%lu] not exist", role_uid);
		return;
	}
	proto::gm_common::manage_reply_single* p_single = ntf.add_reply_data();
	p_single->set_role_uid(string_util_t::uint64_to_string(role_uid));
	switch (manage_type)
	{
	case 0:
		p_single->set_expired_time(global_user_data_mgr_t::ban_role(role_uid, offset_time));
		break;
	case 1:
		p_single->set_expired_time(global_user_data_mgr_t::unban_role(role_uid));
		break;
	case 2:
		p_single->set_expired_time(global_user_data_mgr_t::silence_role(role_uid, offset_time));
		break;
	case 3:
		p_single->set_expired_time(global_user_data_mgr_t::unsilence_role(role_uid));
		break;
	default:
		break;
	}
}
