#include "mail_msg_handle.hpp"

#include "center/center_msg_handler.hpp"
#include "common.pb.h"
#include "server.pb.h"
#include "client.pb.h"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "role/role_manager.hpp"
#include "role/role.hpp"
#include "role/money_manager.hpp"

#include <map>
#include "item/item_manager.hpp"
#include "tblh/errcode_enum.hpp"

bool mail_msg_handle_t::init_center_msg_handler()
{
	bool bresult = true;

	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_mail_get_addenda_request, handle_eg_mail_get_addenda_request);

	return bresult;
}

bool mail_msg_handle_t::handle_eg_mail_get_addenda_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_mail_get_addenda_request);

	uint64_t role_id = msg.uid();

	role_ptr p_role = role_manager_t::find_role(role_id);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null", role_id);
		return false;
	}

	// 发给client人物变化
	proto::client::gc_mail_get_addenda_reply msg_addenda;

	// 回给center
	proto::server::ge_mail_get_addenda_reply reply;
	reply.set_uid(role_id);

	bool have_get = false;
	bool is_full = false;

	for (int32_t i = 0; i < msg.addendas_size(); ++i)
	{
		const proto::server::mail_get_addenda& mail_info = msg.addendas(i);

		proto::server::mail_get_addenda_back* back_info = reply.add_backs();
		if (NULL == back_info)
		{
			log_error("back_info error null");
			continue;
		}

		back_info->set_mail_id(mail_info.mail_id());

		if (mail_info.objs_size() > 0)
		{
			if (is_full)
			{
				back_info->set_reply_code(errcode_enum::notice_main_bag_max_size);
				continue;
			}

			std::map<uint32_t, uint32_t> item_map;

			for (int32_t j = 0; j < mail_info.objs_size(); ++j)
			{
				const proto::common::mail_item_obj& item_obj = mail_info.objs(j);

				item_map.insert(std::make_pair(item_obj.item_id(), item_obj.item_num()));
			}

			if (item_manager_t::check_add_items(p_role, item_map))
			{
				item_manager_t::add_items(p_role, item_map, log_enum::source_type_mail_addenda, mail_info.item_type(), msg_addenda.mutable_change_data());

				back_info->set_reply_code(errcode_enum::error_ok);

				have_get = true;

				MAIL_LOG("role[%lu] mail[%lu] get addenda success", role_id, mail_info.mail_id());
			}
			else
			{
				is_full = true;

				back_info->set_reply_code(errcode_enum::notice_main_bag_max_size);
			}
		}
	}

	if (is_full)
	{
		MAIL_LOG("role[%lu] bag is max size", role_id);
	}

	if (have_get)
	{
		proto::common::item_data* items = msg_addenda.mutable_items();
		items->CopyFrom(msg_addenda.change_data().item());
		if (msg_addenda.mutable_change_data()) p_role->get_personal_info(msg_addenda.mutable_change_data()->mutable_per_info());
		p_role->send_msg_to_client(op_cmd::gc_mail_get_addenda_reply, msg_addenda);
	}

	if (NULL == env::server)
	{
		log_error("env::server null error");
		return false;
	}

	env::server->send_msg_to_center(op_cmd::ge_mail_get_addenda_reply, 0, reply);

	return true;
}
