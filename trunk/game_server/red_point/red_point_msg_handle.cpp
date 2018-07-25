#include "red_point_msg_handle.hpp"
#include "gate/gate_msg_handler.hpp"
#include "center/center_msg_handler.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "red_point_manager.hpp"
#include "protocpp/client.pb.h"
#include "protocpp/server.pb.h"

bool red_point_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_red_point_request, handle_cg_red_point_request);
	
	return bresult;
}

bool red_point_msg_handle_t::init_center_msg_handler()
{
	bool bresult = true;

	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_red_point_notify, handle_eg_red_point_notify);

	return bresult;
}


bool red_point_msg_handle_t::handle_cg_red_point_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_red_point_request);

	uint64_t role_id = uid;

	role_ptr p_role = role_manager_t::find_role(role_id);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", role_id);
		return false;
	}

	proto::common::RED_POINT_TYPE red_point_type = msg.red_type();
	uint32_t param = msg.param();
	RED_POINT_LOG("role[%lu] red point typ[%d] param[%d]", uid, red_point_type, param);
	red_point_ptr p_red_point = red_point_manager_t::get_red_point(role_id, red_point_type);
	if (NULL == p_red_point)
	{
		log_warn("NULL == p_red_point role[%lu] red_point[%d]",role_id, red_point_type);
		return false;
	}
	p_red_point->remove_from_list(param);
	if (p_red_point->get_list().empty())
		red_point_manager_t::remove_role_type_red_point(role_id, p_red_point->get_type());
	return true;
}

bool red_point_msg_handle_t::handle_eg_red_point_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_red_point_notify);
	uint64_t role_id = msg.uid();

	role_ptr p_role = role_manager_t::find_role(role_id);
	if (NULL == p_role)
	{
		log_error("NULL == p_role [%lu]", role_id);
		return false;
	}

	proto::common::RED_POINT_TYPE red_point_type = msg.red_type();
	
	RED_POINT_LOG("role[%lu] add[%d] red point notify", role_id, red_point_type);

	std::string send_str;
	if (red_point_type != proto::common::RED_POINT_TYPE::RED_POINT_MAIL &&
		red_point_type != proto::common::RED_POINT_TYPE::RED_POINT_LIVENESS_GET_TREASURE &&
		red_point_type != proto::common::RED_POINT_TYPE::RED_POINT_PRIVATE_CHAT)
	{

		red_point_ptr p_red_point = red_point_ptr(new red_point_t(red_point_type, role_id));
		for (int32_t i = 0; i < msg.param_size(); ++i)
		{
			p_red_point->append_to_list(msg.param(i));
		}

		red_point_manager_t::add_red_point(role_id, p_red_point);
		send_str = p_red_point->get_list();
	}

	bool b_result = red_point_manager_t::send_notify_to_client(role_id, red_point_type, send_str);
	if (!b_result)
	{
		log_error("role[%lu] send red point[%d] to client fail",role_id,red_point_type);
		return false;
	}

	return true;
}