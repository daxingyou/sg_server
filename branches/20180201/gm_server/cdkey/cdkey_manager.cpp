#include "main/gm_server.hpp"
#include "cdkey_manager.hpp"
#include "log.hpp"
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include "protocpp/protos_fwd.hpp"
#include "gm_curl_wrapper.hpp"

bool cdkey_manager_t::get_use_cdkey_requeset(uint64_t role_uid, uint32_t channel_id, const std::string &key_code)
{
	char url[512];
	bzero(url, sizeof(url));
	snprintf(url, sizeof(url), "%s/cdkey.php?role_uid=%lu&key_code=%s", env::xml->cdkey_url.c_str(), role_uid, key_code.c_str());
	log_info("[%lu] user get cdkey url:=%s", role_uid, url);
	
	gm_curl_wapper_ptr p_curl(new gm_curl_wapper_t());
	if (!p_curl->init(url))
	{
		p_curl->clean();
		return false;
	}
	std::string json_data = p_curl->get_http_request();
	p_curl->clean();
	return cdkey_manager_t::do_cdkey_to_user( role_uid, json_data );
}


bool cdkey_manager_t::do_cdkey_to_user( uint64_t role_uid, std::string &json_data )
{
	if (json_data.empty())
	{
		log_error("user[%lu] do_cdkey json_data is empty", role_uid);
		return false;
	}
	
	try
	{
		std::stringstream ss(json_data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);

		std::string status = pt.get<std::string>("status");
		uint32_t err_code = pt.get<uint32_t>("message.code");

		proto::gm::me_do_change_cdkey_reply reply;
		reply.set_role_uid(role_uid);
		reply.set_reply_code(err_code);
		if (err_code == 0) {
			uint32_t key_id = pt.get<uint32_t>("key_id");
			reply.set_key_type(key_id);
		}
		env::server->send_msg_to_center(op_cmd::me_do_change_cdkey_reply, role_uid, reply);
	}
	catch (boost::property_tree::ptree_error& e)
	{
		printf("\033[31mthrow error[%s]\033[0m\n", e.what());
		return false;
	}
	return true;
}


