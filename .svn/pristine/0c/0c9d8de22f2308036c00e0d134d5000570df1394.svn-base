#include "user_manager.hpp"
#include "log.hpp"
#include "main/account_server.hpp"
#include "main/account_server_fwd.hpp"
#include "common/global_id.hpp"
#include "common/common_struct.hpp"
#include "login/login_manager.hpp"
#include "zdb_ex.hpp"
#include "utility.hpp"
#include "log/log_wrapper.hpp"
#include "serverlist/server_info_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include <boost/regex.hpp>

USING_NS_NETWORK;
USING_NS_COMMON;

list_t<network::tcp_socket_wptr>::type user_manager_t::m_socket_list;

bool user_manager_t::find_socket(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return false;
    }

    for (auto itr = m_socket_list.begin(); itr != m_socket_list.end(); ++itr)
    {
        const tcp_socket_ptr p_socket = (*itr).lock();
        if (NULL != p_socket && s == p_socket)
        {
            return true;
        }
    }

    return false;
}

void user_manager_t::add_socket(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return;
    }

    m_socket_list.push_front(s);

    s->m_iter = new ITER_T(m_socket_list.begin());

    socket_data_t* p_socket_data = new socket_data_t();
    if (0 != env::xml->timeout.session)
    {
        if (NULL == p_socket_data->p_session_timer)
        {
            p_socket_data->p_session_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
        }
        p_socket_data->p_session_timer->expires_from_now(boost::posix_time::seconds(env::xml->timeout.session));
        p_socket_data->p_session_timer->async_wait(boost::bind(&user_manager_t::socket_session_timeout_callback, boost::asio::placeholders::error, s));
    }
    p_socket_data->reset_msg_bomb();
    s->set_bind_data(p_socket_data);

    try
    {
		ROLE_LOG("add new socket[%s: %d] success!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
    }
    catch (boost::system::system_error &ec)
    {
        log_error("throw exception: %s", ec.what());
    }
}

void user_manager_t::del_socket(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return;
    }

    if (!find_socket(s))
    {
        log_error("not find socket[%d]!", s->socket().native());
        return;
    }

    socket_data_t* p_socket_data = (socket_data_t*)s->get_bind_data();
    if (NULL != p_socket_data)
    {
        if (NULL != p_socket_data->p_session_timer)
        {
            p_socket_data->p_session_timer->cancel();
        }
        delete p_socket_data;
        p_socket_data = NULL;
        s->set_bind_data(NULL);
    }
    
    ITER_T* p_iter = (ITER_T*)s->m_iter;
    if (NULL != p_iter)
    {
        m_socket_list.erase(p_iter->i);
        delete p_iter;
        p_iter = NULL;
    }

    env::server->post_network_close_socket(s);
}

void user_manager_t::clear_socket()
{
    for (auto itr = m_socket_list.begin(); itr != m_socket_list.end(); ++itr)
    {
        tcp_socket_ptr p_socket = (*itr).lock();
        if (NULL != p_socket)
        {
            socket_data_t* p_socket_data = (socket_data_t*)p_socket->get_bind_data();
            if (NULL != p_socket_data)
            {
                if (NULL != p_socket_data->p_session_timer)
                {
                    p_socket_data->p_session_timer->cancel();
                }
                delete p_socket_data;
                p_socket_data = NULL;
            }
            p_socket->set_bind_data(NULL);

            ITER_T* it = (ITER_T*)p_socket->m_iter;
            if (NULL != it)
            {
                delete it;
                it = NULL;
            }

            env::server->post_network_close_socket(p_socket);
        }
    }
    m_socket_list.clear();
}

void user_manager_t::socket_session_timeout_callback(const boost::system::error_code& ec, const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return;
    }

	if (!ec)
	{
        log_error("socket[%d] session timeout!", s->socket().native());
        del_socket(s);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void user_manager_t::channel_login_respone(const account_channel_ptr& p_account_channle)
{
    uint64_t account_uid = 0;
    bool is_new = true;
    bool rlt = false;
	uint32_t err_code = errcode_enum::error_ok;
    do 
    {
        if (NULL == p_account_channle)
        {
            log_error("NULL == p_account_channle");
			err_code = 1; 
            break;
        }

		// 检查特殊字符
		boost::regex reg("^[\u4e00-\u9fa5a-zA-Z0-9]+$");
		bool is_match = boost::regex_match(p_account_channle->m_login_respone_data.m_guid, reg);
		if (!is_match)
		{
			log_error("guid[%s] has special char!", p_account_channle->m_login_respone_data.m_guid.c_str());
			err_code = errcode_enum::notice_name_special;
			break;
		}

        boost::tie(account_uid, is_new) = db_check_guid(p_account_channle->m_login_respone_data.m_guid, p_account_channle->m_channel_id);
        if (0 == account_uid)
        {
            log_error("account_uid[%d:%s] is 0 !", p_account_channle->m_channel_id, p_account_channle->m_login_respone_data.m_guid.c_str());
			err_code = 1; 
            break;
        }
        
        rlt = true;

    } while (0);

    proto::client::nc_server_info_reply reply;
    if (rlt)
    {
        load_all_roles_data(p_account_channle->m_login_respone_data.m_guid, account_uid, p_account_channle->m_channel_id, p_account_channle->m_login_mark_type, reply);
		//生成一个唯一登陆串给客户端
        uint32_t source_type = is_new ? log_enum::source_type_account_registered : log_enum::source_type_account_login;
        log_wrapper_t::send_account_log(account_uid, p_account_channle->m_channel_id, 0, p_account_channle->m_mobile_uuid, source_type);
    }
    else
    {
        reply.set_reply_code(err_code);
    }

	//设置服务端版本号 ADD_BY_HY	
	reply.set_server_version(env::xml->version);
    env::server->send_msg_to_client(p_account_channle->get_client_socket(), op_cmd::nc_server_info_reply, reply);
}

boost::tuple<uint64_t, bool> user_manager_t::db_check_guid(std::string const& guid, uint32_t channel_id)
{
    uint64_t account_uid = 0;
    bool is_new = true;

    Connection_T db_con = zdb_util_t::get_zdb_conn(env::conn_zdb_pool);
    if (NULL == db_con)
    {
        log_error("db_con is NULL!");
        return boost::make_tuple(account_uid, is_new);
    }

    TRY
    {
        ResultSet_T result = Connection_executeQuery(db_con, "SELECT `id` FROM `account` WHERE `guid`=\'%s\';", guid.c_str());
        if (ResultSet_next(result))
        {
            //老账户
            account_uid = ResultSet_getLLong(result, 1);
            is_new = false;
        }
        else
        {
            //获得当前数据库中account_id的最大值
            result = Connection_executeQuery(db_con, "SELECT MAX(`id`) FROM `account`;");
            if (ResultSet_next(result))
            {
                uint64_t account_uid_max = ResultSet_getLLong(result, 1);
                uint64_t account_uid_next = account_uid_max + 1;
                if (account_uid_next > ACCOUNT_ID_MAX)
                {
                    log_error("account_uid[%lu] > max[%u]", account_uid_next, ACCOUNT_ID_MAX);
                    return boost::make_tuple(account_uid, is_new);
                }
            }

            //新账户
            PreparedStatement_T pre = Connection_prepareStatement(db_con, "INSERT INTO `account` SET `guid`=?, `channel_id`=?, `regist_time`=NOW();");
            PreparedStatement_setString(pre,    1, guid.c_str());
            PreparedStatement_setInt(pre,		2, channel_id);
            PreparedStatement_execute(pre);

            result = Connection_executeQuery(db_con, "SELECT `id` FROM `account` WHERE `guid`=\'%s\';", guid.c_str());
            if (ResultSet_next(result))
            {
                account_uid = ResultSet_getLLong(result, 1);
                is_new = true;
            }
        }
    }
    CATCH(SQLException)
    {
        log_error("check user[guid=%s] from db error: %s", guid.c_str(), Exception_frame.message);
    }
    FINALLY
    {
        Connection_close(db_con);
    }
    END_TRY;

    return boost::make_tuple(account_uid, is_new);
}

void user_manager_t::load_all_roles_data(std::string &guid, uint64_t account_uid, uint32_t channel_id, const std::string login_server_mark_type, proto::client::nc_server_info_reply& reply)
{
	Connection_T db_con = zdb_util_t::get_zdb_conn(env::conn_zdb_pool);
	if (NULL == db_con)
	{
		log_error("db_con is NULL!");
		reply.set_reply_code(1);
		return;
	}

    bool ret = false;
	std::map<uint32_t, std::vector<proto::common::account_role_data>> role_map;
	TRY
	{
		ResultSet_T result = Connection_executeQuery(db_con, "SELECT `uid`,`name`,`create_time`,`species`,`level`,`plugin`, `server_id` FROM `role` WHERE `account_uid`=%lu;", account_uid);
		while (ResultSet_next(result))
		{
			uint64_t role_id = ResultSet_getLLong(result, 1);
			
			proto::common::account_role_data ard;
			ard.set_uid(string_util_t::uint64_to_string(role_id));
			ard.set_name(ResultSet_getString(result, 2));
			ard.set_create_time(ResultSet_getInt(result, 3));
			ard.set_species(ResultSet_getInt(result, 4));
			ard.set_level(ResultSet_getInt(result, 5));
			ard.set_plugin(ResultSet_getInt(result, 6));
			uint32_t srv_group_id = ResultSet_getInt(result, 7);

            //uint32_t srv_group_id = user_uid_generator::get_srv_group_id(role_id);
			std::map<uint32_t, std::vector<proto::common::account_role_data>>::iterator it = role_map.find(srv_group_id);
			if (it == role_map.end())
			{
				std::vector<proto::common::account_role_data> role_list;
				role_list.push_back(ard);
				role_map.insert(std::make_pair(srv_group_id, role_list));
			}
			else
			{
				std::vector<proto::common::account_role_data>& role_list = it->second;
				role_list.push_back(ard);
			}
		}

        ret = true;
	}
	CATCH(SQLException)
	{
		log_error("load user[account_id=%lu] from db error: %s", account_uid, Exception_frame.message);
	}
	FINALLY
	{
		Connection_close(db_con);
	}
	END_TRY;

	if (!ret)
	{
		reply.set_reply_code(1);
		return;
	}
	reply.set_reply_code(0);


	//从web服务器中获取登录服列表信息
	auto login_list_iter_f = server_info_manager_t::m_server_info_map.find(login_server_mark_type);
	if (login_list_iter_f == server_info_manager_t::m_server_info_map.end())
	{
		reply.set_reply_code(1);
		log_error("can't find mark_type :[%s]", login_server_mark_type.c_str());
		return;
	}

	auto &login_server_list = login_list_iter_f->second;
	auto it_login = login_server_list.begin();
	auto it_end = login_server_list.end();
	for (; it_login != it_end; ++it_login)
	{
		uint32_t server_id = it_login->first;
		server_info_ptr p_cfg_server = it_login->second;
		if (NULL == p_cfg_server)
		{
			continue;
		}
		
		//打包服务器信息
		proto::common::server_info* p_server = reply.add_server_list();
		p_server->set_server_id(server_id);
		p_server->set_server_state(proto::common::SERVER_STATE(p_cfg_server->m_status));
		p_server->set_server_name(p_cfg_server->m_name);
		p_server->set_label( p_cfg_server->m_label );
		
		uint32_t can_create_new_role = 0;	//是否可以创建新角色
		auto login_iter_f = login_manager_t::m_login_map.find(it_login->first);
		if (login_iter_f != login_manager_t::m_login_map.end())
		{
			login_ptr p_login = login_iter_f->second;
			if (NULL == p_login )
			{ 
				continue;
			}
			p_server->set_login_ip(p_login->get_ip());
			p_server->set_login_port(p_login->get_port());
			p_server->set_server_name(p_login->get_name());	//如果是活的login取login传过来的名字
			can_create_new_role = p_login->get_can_create_new_role();	//获取是否可以创建新角色

			//如果断开的连接并不会删除这个map,也需要设置为维护状态, 如果是还连接着，但是表里已经修改了状态为close也需要close 
			if ( p_login->get_state() == proto::common::SERVER_STATE::SERVER_STATE_CLOSE || p_cfg_server->m_status == uint32_t(proto::common::SERVER_STATE::SERVER_STATE_CLOSE) )
			{
				p_server->set_server_state(proto::common::SERVER_STATE::SERVER_STATE_CLOSE);
			}
		}
		else
		{
			//如果找不到默认用CLOSE状态
			p_server->set_server_state(proto::common::SERVER_STATE::SERVER_STATE_CLOSE);
		}

		//打包玩家列表
		uint32_t new_index = 0;
		std::string name = "";
		std::map<uint32_t, std::vector<proto::common::account_role_data>>::iterator it_role = role_map.find(server_id);
		if (it_role != role_map.end())
		{
			std::vector<proto::common::account_role_data>& role_list = it_role->second;
			for (uint16_t i = 0; i < role_list.size(); ++i)
			{
				proto::common::account_role_data* p_role = p_server->add_role_list();
				const proto::common::account_role_data& ard = role_list[i];
				p_role->CopyFrom(ard);
				uint64_t role_id = string_util_t::string_to_uint64(ard.uid());
				uint32_t role_index = user_uid_generator::get_role_index(role_id);
				if (role_index > new_index)
				{
					new_index = role_index;
				}
				name = p_role->name();
			}
			new_index += 1;
		}
		uint64_t new_role_id = 0;
		if (new_index < ROLE_INDEX_MAX)
		{
			new_role_id = user_uid_generator::gen_user_uid(env::xml->srv_group.platform, channel_id, server_id, new_index, account_uid);
		}
		else
		{
			//log_error("role_account_uid[%lu] at server[%u] has role_num[%d] >= max_role_num[%d] set new_role_id=[%d]", account_uid, p_server->server_id(), new_index, ROLE_INDEX_MAX, new_role_id );
		}

		p_server->set_new_role_id(string_util_t::uint64_to_string(new_role_id));
		p_server->set_can_create_new_role(can_create_new_role);
		//log_info("package_server_list id:%d, state:%d, name:%s, guid[%s], account_id[%lu] role_name[%s]", p_server->server_id(), p_server->server_state(),
		//								p_server->server_name().c_str(),  guid.c_str(), account_uid,  name.c_str() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
bool socket_data_t::check_msg_bomb(uint32_t cmd)
{
    if (0 == env::xml->timeout.msg_bomb_interval)
    {
        return true;
    }
    if (cmd == op_cmd::ca_pingpong_request || cmd == op_cmd::cg_move_request)
    {
        return true;
    }

    if (0 == m_msg_bomb_count)
    {
        m_last_msg_time = time_util_t::now_msecs();
    }

    ++m_msg_bomb_count;

    if (m_msg_bomb_count >= env::xml->timeout.msg_bomb_count)
    {
        uint64_t curr_time = time_util_t::now_msecs();
        if (curr_time - m_last_msg_time <= env::xml->timeout.msg_bomb_interval)
        {
            return false;
        }
        else
        {
            reset_msg_bomb();
        }
    }
    return true;
}
