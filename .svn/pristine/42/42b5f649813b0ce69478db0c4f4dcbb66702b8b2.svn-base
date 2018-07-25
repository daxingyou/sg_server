#include "center_msg_handler.hpp"
#include "main/log_server.hpp"
#include "main/log_server_fwd.hpp"
#include "network.hpp"
#include "main/log_manager.hpp"
USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::EO_BEGIN, op_cmd::EO_END, center_msg_handler_t::msg_handler_func> center_msg_handler_t::m_center_msg_handle;

bool center_msg_handler_t::init_msg_handler()
{
    bool ret = true;

    ret &= m_center_msg_handle.register_func(op_cmd::eo_batch_save_logs_notify, handle_batch_save_logs_notify);

    return ret;
}

bool center_msg_handler_t::handle_batch_save_logs_notify(const network:: tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG_NO_UID(proto::log::eo_batch_save_logs_notify);

    for (int32_t i = 0; i < (int32_t)msg.logs_size(); ++i)
    {
        uint16_t log_cmd = (uint16_t)msg.logs(i).cmd();
        add_logs(log_cmd, msg.logs(i).data());
    }

    return true;
}


void center_msg_handler_t::add_logs(uint16_t cmd, const std::string& msg_buf)
{
    log_data_t& log = log_manager_t::get_cur_thread_log_data();
    log.incre_log_num();

    switch (cmd)
    {
    case op_cmd::eo_country_log_notify:
        {
            PRE_S2LOG_MSG(proto::log::eo_country_log_notify);
            country_log_ptr new_log(new country_log_t(msg.country_id(), msg.source_type(), msg.source_param(), msg.source_param2(), msg.source_param3(), msg.log_time()));
            log.m_country_log_mgr.add_log(new_log);
        }
        break;
    case op_cmd::eo_family_log_notify:
        {
            PRE_S2LOG_MSG(proto::log::eo_family_log_notify);
            family_log_ptr new_log(new family_log_t(msg.family_uid(), msg.role_uid(), msg.target_uid(), msg.source_type(), msg.source_param(), msg.source_param2(), msg.log_time()));
            log.m_family_log_mgr.add_log(new_log);
        }
        break;
    case op_cmd::eo_friend_log_notify:
        {
            PRE_S2LOG_MSG(proto::log::eo_friend_log_notify);
            friend_log_ptr new_log(new friend_log_t(msg.role_uid(), msg.friend_uid(), msg.old_type(), msg.new_type(), msg.source_type(), msg.log_time(), msg.source_param()));
            log.m_friend_log_mgr.add_log(new_log);
        }
        break;
	case op_cmd::eo_mail_log_notify:
		{
			PRE_S2LOG_MSG(proto::log::eo_mail_log_notify);
			mail_log_ptr new_log(new mail_log_t(msg.role_uid(), msg.mail_id(), msg.mail_type(), msg.item_type(), msg.items(), msg.item_state(), msg.source_type(), msg.log_time(), msg.source_param()));
			log.m_mail_log_mgr.add_log(new_log);
		}
		break;
	case op_cmd::eo_goods_log_notify:
		{
			PRE_S2LOG_MSG(proto::log::eo_goods_log_notify);
			goods_log_ptr new_log(new goods_log_t(msg.role_uid(), msg.role_name(), msg.goods_uid(), 
				msg.item_tid(), msg.item_num(), msg.price(), msg.selled_num(), msg.profit(), 
				msg.state(), msg.source_type(), msg.log_time()));
			log.m_goods_log_mgr.add_log(new_log);
		}
		break;
	case op_cmd::eo_scene_log_notify:
        {
            PRE_S2LOG_MSG(proto::log::eo_scene_log_notify);
            scene_log_ptr new_log(new scene_log_t(msg.role_uid(), msg.old_map_tid(), msg.new_map_tid(), msg.source_type(), msg.log_time()));
            log.m_scene_log_mgr.add_log(new_log);
        }
        break;
	case op_cmd::eo_redbag_log_notify:
		{
			PRE_S2LOG_MSG(proto::log::eo_redbag_log_notify);
			redbag_log_ptr new_log(new redbag_log_t(msg.role_uid(), msg.role_name(), msg.redbag_uid(), msg.total_count(), 
				msg.total_money(), msg.draw_money(), msg.rest_count(), msg.rest_money(), msg.source_type(), msg.log_time()));
			log.m_redbag_log_mgr.add_log(new_log);
		}
		break;
    default:
        {
            log_error("op_cmd is unknown! cmd=%d", cmd);
            return;
        }
        break;
    }
}

//bool center_msg_handler_t::handle_eo_test_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
//{
//
//    return true;
//}
