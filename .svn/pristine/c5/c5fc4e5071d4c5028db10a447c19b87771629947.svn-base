#include "cdkey_manager.hpp"
#include "protocpp/protos_fwd.hpp"
#include "tblh/errcode_enum.hpp"
#include "mail/mail_manager.hpp"
#include "tblh/CDKeyTable.tbls.h"
#include "common/config_mgr.h"
#include "user/global_user_manager.hpp"

void cdkey_manager_t::do_exchange_cdkey_request(const global_user_ptr &p_user, uint32_t channel_id, const std::string &key_code)
{
	if (NULL == p_user)
	{
		log_error("p_user == NULL");
		return;
	}

	proto::gm::em_do_change_cdkey_request msg;
	msg.set_channel_id(0);
	msg.set_key_code( key_code );
	msg.set_role_uid( p_user->get_uid() );
	msg.set_channel_id( channel_id );
	env::server->send_msg_to_gm(op_cmd::em_do_change_cdkey_request, p_user->get_uid(), msg);
}

void cdkey_manager_t::do_exchange_cdkey_reply(const uint64_t role_uid, uint32_t key_id, uint32_t err_code)
{
	uint32_t reply_code = errcode_enum::notice_unknown;
	do {
		if (err_code != 0)
		{
			log_error("user[%lu] get_cdkey faild err_code[%d]", role_uid, err_code);
			reply_code = err_code;
			break;
		}

		auto p_cdkey_conf = GET_CONF(CDKeyTable, key_id);
		if ( NULL == p_cdkey_conf )
		{
			log_error("user[%lu] CDKeyTable Config[%d] not found", key_id);
			reply_code = errcode_enum::cdkey_not_found;
			break;
		}
		mail_manager_t::send_mail(role_uid, proto::common::MAIL_TYPE_SYS, p_cdkey_conf->mail_id(), p_cdkey_conf->drop_list(), proto::common::MAIL_GET_TYPE_CDKEY);
		reply_code = errcode_enum::error_ok;
	} while (0);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(role_uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", role_uid);
		return ;
	}
	proto::client::ec_exchange_cdkey_reply reply;
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_client(op_cmd::ec_exchange_cdkey_reply, reply);

	return;
}