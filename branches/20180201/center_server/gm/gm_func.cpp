#include "gm_func.hpp"
#include "utility.hpp"
#include "zdb_ex.hpp"
#include "common/redis_client.hpp"
#include <boost/lexical_cast.hpp>
#include "mail/global_mail_manager.hpp"
#include "common/global_id.hpp"
#include "user/global_user_manager.hpp"

USING_NS_COMMON;
void gm_func::ban_account(std::string account, uint32_t flag)
{
	if (0 == flag)
	{
		redis_client_t::add_key("account_ban", account);
	}
	else if (1 == flag)
	{
		redis_client_t::remove_key("account_ban", account);
	}

}

void gm_func::access_ip(std::string ip_address, uint32_t ip_end, uint32_t flag)
{
	std::vector<uint32_t> tmp_vec;
	string_util_t::split(ip_address, tmp_vec, ".");
	if (4 != tmp_vec.size())
	{
		return;
	}
	if (tmp_vec[3] > ip_end)
	{
		return;
	}
	for (uint32_t i = tmp_vec[3]; i < ip_end+1; ++i)
	{
		std::ostringstream oss;
		oss << tmp_vec[0] << "." << tmp_vec[1] << "." << tmp_vec[2] << "." << i;
		access_ip(oss.str(), flag);
	}
}

void gm_func::access_ip(std::string ip_address, uint32_t flag)
{
	if (0 == flag)
	{
		redis_client_t::add_key("ip_access", ip_address);
	}
	else if (1 == flag)
	{
		redis_client_t::remove_key("ip_access", ip_address);
	}
}

void gm_func::send_mail(proto::gm::me_send_mail_request &request)
{
	const proto::gm_common::global_mail_single &mail_data = request.mail_data();
	
	std::set<uint64_t> uids;
	uint64_t role_did = mail_data.uid();
	if (role_did != 0)
	{
		std::vector<uint64_t> users;
		redis_client_t::get_keys_by_field_condition<uint64_t, uint32_t, std::vector<uint64_t>>("global_user", "did", role_did, users);
		if (users.size() > 0)
		{
			uids.insert(users[0]);
		}
		else {
			return;
		}
	}
	
	//邮件限制条件
	const proto::gm_common::mail_recv_limit &limit = mail_data.limit();
	global_mail_manager_t::add_new_global_mail(EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_GM, limit.level(),limit.regist_type(), uids,
						common::time_util_t::now_time(), mail_data.over_time(),
						mail_data.send_name(),
						mail_data.title(),
						mail_data.content(),
						mail_data.items(),
						"");
}


void gm_func::send_ol_nun(uint32_t admin_id)
{
	proto::gm::em_get_user_ol_num_reply request;
	request.set_admin_id(admin_id);
	request.set_ol_num( global_user_manager_t::get_all_user_num() );
	env::server->send_msg_to_gm(op_cmd::em_get_user_ol_num_reply, 0 , request);
}
