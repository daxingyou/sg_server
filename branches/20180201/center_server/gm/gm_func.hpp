#ifndef __CENTER_GM_FUNC_HPP__
#define __CENTER_GM_FUNC_HPP__

#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "protocpp/protos_fwd.hpp"

class gm_func
{
public:
	static void ban_account(std::string account, uint32_t flag);
	static void access_ip(std::string ip_address, uint32_t ip_end, uint32_t flag);
	static void access_ip(std::string ip_address, uint32_t flag);

	static void send_mail(proto::gm::me_send_mail_request &request);
	static void send_ol_nun(uint32_t admin_id);
};
#endif
