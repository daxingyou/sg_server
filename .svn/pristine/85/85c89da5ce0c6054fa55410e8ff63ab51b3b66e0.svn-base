#include "channel.hpp"
#include "log.hpp"
#include "md5.hpp"
#include "main/account_server_fwd.hpp"
#include "log/log_wrapper.hpp"
#include "common.pb.h"

USING_NS_NETWORK;
USING_NS_COMMON;

bool account_channel_t::load_data(const channel_ptr& p_channel, const network::tcp_socket_ptr& s, const proto::client::cn_server_info_request& req)
{
    if (NULL == p_channel)
    {
        log_error("NULL == p_channel");
        return false;
    }

    const proto::common::channel_general& cg = req.general();
    //session_id
    std::string session_id = "";
    if (!cg.has_session_id())
    {
        log_error("client not send session_id!");
        return false;
    }
    if (cg.session_id().empty())
    {
        log_error("session_id[%s] error!", cg.session_id().c_str());
        return false;
    }
    session_id = cg.session_id();

    //uid
    std::string uid = "";
    if (cg.has_uid() && !cg.uid().empty())
    {
        uid = cg.uid();
    }
    else
    {
        uid = session_id;
    }
    m_login_request_data.m_session_id = session_id;
    m_login_request_data.m_uid = uid;
    m_post_login_fields = get_http_post_login_fields(p_channel);

    m_channel_id = p_channel->m_id;
    m_client_socket = s;
    
    ROLE_LOG("channel[%s:%d] account[uid=%s, session_id=%s] login......", p_channel->m_channel_name.c_str(), m_channel_id, m_login_request_data.m_uid.c_str(), m_login_request_data.m_session_id.c_str());
    ROLE_LOG("post fields: %s", m_post_login_fields.c_str());

    //设备信息 与具体的渠道登陆逻辑无关
    const proto::common::mobile_info& mobile = req.mobile();
    if (mobile.has_uuid() && !mobile.uuid().empty())
    {
        m_mobile_uuid = mobile.uuid();
        log_wrapper_t::send_mobile_log(mobile.uuid(), mobile.device_model(), mobile.device_sys_version(), mobile.cpu_type(), mobile.cpu_count(), mobile.memory_size(), mobile.grph_device_name());
    }
    //else
    //{
    //    log_error("mobile uuid is NULL !");
    //}

    return true;
}

std::string account_channel_t::get_http_post_login_fields(const channel_ptr& p_channel)
{
    if (NULL == p_channel)
    {
        log_error("NULL == p_channel");
        return "";
    }

	std::ostringstream ss;
	ss << "sid=" << m_login_request_data.m_uid;
	ss << "&channel_id=" << p_channel->m_id;
	ss << "&channel_name=" << p_channel->m_channel_name;
	ss << "&function=veritfy_session";
	return ss.str();
	/*
    //验证码
    std::string tmp_md5 = "";
    tmp_md5.append(m_login_request_data.m_uid);
    tmp_md5.append(m_login_request_data.m_session_id);
    tmp_md5.append(p_channel->m_login_sk);
    tmp_md5.append(p_channel->m_game_name);
    tmp_md5.append(p_channel->m_channel_name);
    common::MD5 md5(tmp_md5);
    std::string sign_key = md5.md5();

    //post fields
    char tmp[1024] = { 0 };
    snprintf(tmp, sizeof(tmp), "game=%s&channel=%s&sessionId=%s&uid=%s&sign=%s",
        p_channel->m_game_name.c_str(),
        p_channel->m_channel_name.c_str(),
        m_login_request_data.m_session_id.c_str(),
        m_login_request_data.m_uid.c_str(),
        sign_key.c_str());
    std::string post_fields(tmp);

    return post_fields;
	*/
}