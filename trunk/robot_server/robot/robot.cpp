#include "robot.hpp"
#include "robot_manager.hpp"
#include "robot_msg_handler.hpp"
#include "name_manager.hpp"
#include "common/log.hpp"
#include "common/config_mgr.h"
#include "common/random_util.hpp"
#include "common/utility.hpp"
#include "protocpp/client.pb.h"
#include "protocpp/op_cmd.hpp"
#include "script/script_robot.h"
#include "tblh/country_enum.hpp"
#include "tblh/Task.tbls.h"
#include "tblh/TaskCircle.tbls.h"
#include "tblh/Bounty.tbls.h"
#include "tblh/DungeonTable.tbls.h"
#include "tblh/errcode_enum.hpp"

#define ENTER_THUNDERMONSTER_RANGE 6	// 明雷战斗范围

#define LOGIN_TIME_OUT 3600*1000 

USING_NS_COMMON;
USING_NS_NETWORK;

robot_t::robot_t(uint64_t uid)
	: m_robot_id(uid)
	, m_login_state(LOGIN_STATE_NONE)
	, m_login_start_time(0)
	, m_enter_success(false)
	, m_curr_time(0)
	, m_script_proxy(new script_robot_t(this))
	, m_init_lua(false)
	, m_task_done(false)
	, m_is_arena_matched(false)
{
	m_login_time_msec = m_last_login_state_time = common::time_util_t::now_msecs();
}

robot_t::robot_t(const std::string& name)
	: m_special_id(name)
	, m_login_state(LOGIN_STATE_NONE)
	, m_login_start_time(0)
	, m_enter_success(false)
	, m_curr_time(0)
	, m_script_proxy(new script_robot_t(this))
	, m_init_lua(false)
	, m_task_done(false)
	, m_is_arena_matched(false)
{
	m_login_time_msec = m_last_login_state_time = common::time_util_t::now_msecs();
}

robot_t::~robot_t()
{
	
}


const char* robot_t::name()
{
	return "robot";
}


void robot_t::on_connect(const network::tcp_socket_ptr& s)
{
	if (NULL == s)
	{
		log_error("robot[%lu] on connect error! socket is NULL", m_robot_id);
		return;
	}

	m_socket = s;

	if (m_login_state == LOGIN_STATE_NONE)
	{
		login_account_server();
	}
	else if (m_login_state == LOGIN_STATE_LOGIN_ACCOUNT)
	{
		login_login_server();
	}
	else if (m_login_state == LOGIN_STATE_LOGIN_LOGIN)
	{
		login_gate_server(0);
	}
	else
	{
		log_error("invalid robot[%lu]'s login_state[%d]", m_robot_id, m_login_state);
	}
}


void robot_t::on_new_msg(const network::tcp_socket_ptr& s,
						 const network::message_t& msg)
{
   auto msg_handler = boost::bind(&robot_msg_handler_t::handle_server_msg, shared_from_this(), msg.head.cmd, msg.body);
   env::server->get_ios().post(msg_handler);
}


void robot_t::on_close(const network::tcp_socket_ptr& s)
{
	// this function is useless
	//log_warn("robot[%lu] server close socket", m_robot_id);

	m_login_state = LOGIN_STATE_NONE;
}


//=============================================================================
bool robot_t::connect(const std::string& ip, uint16_t port)
{
	return env::server->connect(ip, port, shared_from_this().get());
}

bool robot_t::disconnect()
{
	return env::server->disconnect(get_socket());

	m_robot_id = 0;

	if (m_script_proxy)
	{
		m_script_proxy->dispose();
		m_script_proxy = NULL;
	}
}

bool robot_t::login_account_server()
{
	char guid_buf[256] = { '\0' };
	if (m_special_id.empty())
	{
		snprintf(guid_buf, sizeof(guid_buf), "%s%lu", env::robot_config->robot_name.c_str(), m_robot_id);
	}
	else
	{
		snprintf(guid_buf, sizeof(guid_buf), m_special_id.c_str());
	}

    set_guid(guid_buf);
    set_login_state(LOGIN_STATE_LOGIN_PRE1);

	proto::client::cn_server_info_request msg_req;
	proto::common::channel_general *p_general = msg_req.mutable_general();
	p_general->set_session_id(guid_buf);
	p_general->set_guid(guid_buf);
    proto::common::mobile_info *p_mobile = msg_req.mutable_mobile();
    p_mobile->set_uuid(guid_buf);
	msg_req.set_mark_type(env::robot_config->server_list_type);

    return send_msg(get_socket(), op_cmd::cn_server_info_request, msg_req);
}

bool robot_t::login_login_server()
{
	proto::client::cl_login_request msg_req;
	msg_req.set_uid(string_util_t::uint64_to_string(m_uid));
	msg_req.set_guid(m_guid);
	return send_msg(get_socket(), op_cmd::cl_login_request, msg_req);
}

bool robot_t::login_gate_server(int32_t reconnect)
{
	proto::client::ca_connect_request msg_req;
	msg_req.set_uid(string_util_t::uint64_to_string(m_uid));
	msg_req.set_guid(m_guid);
	msg_req.set_session_id(m_session_id);
	msg_req.set_reconnect(reconnect);
	return send_msg(get_socket(), op_cmd::ca_connect_request, msg_req);
}

bool robot_t::opt_gm_cmd(std::string cmd)
{
	proto::client::cg_gm_cmd_request msg_req;
	msg_req.set_cmd(cmd);
	return send_msg(get_socket(), op_cmd::cg_gm_cmd_request, msg_req);
}

void robot_t::robot_in_game()
{
	//设置GM权限
	proto::client::cg_set_gm_level msg_req;
	msg_req.set_gm_level(env::robot_config->gm_level);
	send_msg(get_socket(), op_cmd::cg_set_gm_level, msg_req);

	//opt_gm_cmd("/funcopenall");
	//opt_gm_cmd("/setlevel 30");
	//opt_gm_cmd("/addhero 7004");
	//opt_gm_cmd("/addhero 7005");
	//opt_gm_cmd("/addhero 7008");
	//opt_gm_cmd("/addhero 7009");
	//opt_gm_cmd("/additem 140001 1");
	//opt_gm_cmd("/additem 140002 1");
	//opt_gm_cmd("/additem 140003 1");
	//opt_gm_cmd("/setlevel 30");

    //start_send_msg_timer();

    //for (uint32_t i = 0; i < 10; ++i)
    //{
    //    opt_gm_cmd("/additem 210001 1");
    //}
    //opt_gm_cmd("/setlevel 66");
}

void robot_t::enter_game()
{
	proto::client::ca_login_game_request msg_req;
	send_msg(get_socket(), op_cmd::ca_login_game_request, msg_req);

	if (!m_init_lua)
	{
		init_lua();
	}
}

void robot_t::create_new_role()
{
	std::string new_name("");
	int32_t job = random_util_t::randBetween(1, ROLE_JOB_SIZE);
	if (job >= 0)
	{
        if (!name_manager_t::create_new_name(job, new_name))
        {
            new_name = m_guid;
        }
	}
    else
    {
        new_name = m_guid;
    }

	proto::client::ca_create_role_request msg_req;
	msg_req.set_nickname(new_name);
	msg_req.set_role_type(job);
	send_msg(get_socket(), op_cmd::ca_create_role_request, msg_req);

	set_login_state(LOGIN_STATE_CREATE_USER);

	log_debug("robot[%lu] create user[%s]...", get_robot_id(), new_name.c_str());
}

void robot_t::enter_scene(uint64_t scene_id)
{
    proto::client::cg_enter_scene_request msg_req;
	msg_req.set_scene_id(common::string_util_t::uint64_to_string(scene_id));
    send_msg(get_socket(), op_cmd::cg_enter_scene_request, msg_req);
    //log_debug("robot[%lu] cg_enter_scene_request to game! map_tid=%u", m_robot_id, map_tid);
}

void robot_t::jump_scene(uint32_t map_tid)
{
	proto::client::cg_jump_scene_request msg_req;
	msg_req.set_map_tid(map_tid);
	msg_req.set_jump_id(0);
	send_msg(get_socket(), op_cmd::cg_jump_scene_request, msg_req);
	log_debug("robot[%lu] cg_jump_scene_request to game! map_tid=%u", m_robot_id, map_tid);
}

void robot_t::jump_id(uint32_t jump_tid)
{
	proto::client::cg_jump_scene_request msg_req;
	msg_req.set_map_tid(0);
	msg_req.set_jump_id(jump_tid);
	send_msg(get_socket(), op_cmd::cg_jump_scene_request, msg_req);
	log_debug("robot[%lu] cg_jump_scene_request to game! jump_id=%u", m_robot_id, jump_tid);
}

void robot_t::talking(proto::common::chat_channel chan, const char* str)
{
	proto::client::ch_channel_chat_request msg_req;
	msg_req.set_chat_info(common::string_util_t::convert_to_utf8(str));
	msg_req.set_chat_channel(chan);
	send_msg(get_socket(), op_cmd::ch_channel_chat_request, msg_req);
}

void robot_t::update_robot_info(const proto::client::gc_enter_game_reply& msg)
{
	if (msg.reply_code() > 0)
	{
		log_debug("Error:robot_t::update_robot_info...");
		return;
	}

	m_robot_info.role.CopyFrom(msg.role());
	m_robot_info.item.CopyFrom(msg.item());
	m_robot_info.hero.CopyFrom(msg.hero());
	m_robot_info.formation.CopyFrom(msg.formation());
	//m_robot_info.arm.CopyFrom(msg.arm());
	m_robot_info.task.CopyFrom(msg.task());
	m_robot_info.arena_data.CopyFrom(msg.arena_data());

	set_nickname(msg.role().name());

    log_debug("robot[%s] name[%s] login game success", m_robot_id ? common::string_util_t::uint64_to_string(m_robot_id).c_str() : m_special_id.c_str(), m_nickname.c_str());
}

void robot_t::update_scene_info(const proto::client::gc_enter_scene_notify& msg)
{
	m_robot_info.jump_scene_id = common::string_util_t::string_to_uint64(msg.scene_id());
	m_robot_info.jump_map_tid = msg.map_tid();

	m_robot_info.curr_pos.m_x = msg.pos_x();
	m_robot_info.curr_pos.m_z = msg.pos_y();

	if (m_enter_success) {
		enter_scene(m_robot_info.jump_scene_id);
	} else {
		m_delay_enter_scene = true;
        m_delay_time = time_util_t::now_msecs();
	}
}

void robot_t::enter_scene_reply(const proto::client::gc_enter_scene_reply& msg)
{
	m_robot_info.scene_id = m_robot_info.jump_scene_id;
	m_robot_info.map_id = m_robot_info.jump_map_tid;

	if (m_dungeon_map_id != 0 && m_dungeon_map_id != m_robot_info.map_id)
	{
		clear_dungeon();
	}

	// 清理9屏
	clear_object_info();
	update_into_vision(msg.vision());
}

//////////////////////////////////////////////////////////////////////////



script_robot_t* robot_t::get_script_proxy() const
{
	return m_script_proxy;
}

bool robot_t::heartbeat(uint64_t ntime)
{
	uint64_t old = m_curr_time;
	m_curr_time = ntime;
	uint32_t gone_time = (uint32_t)(m_curr_time - old);

	if (!m_enter_success) {
		if (m_login_start_time + LOGIN_TIME_OUT < m_curr_time) {
			robot_manager_t::add_robot_failed_loginc(m_robot_id, m_uid, m_login_state);
			m_enter_success = true;	// 只输出一次
		}

		if (m_delay_enter_scene) 
        {
			if (m_curr_time - m_delay_time > ENTER_SCENE_DELAY_MSEC)
			{
                enter_scene(m_robot_info.jump_scene_id);
                m_delay_enter_scene = false;
			}
		}
	}
	
	if (m_login_state >= LOGIN_STATE_LOGIN_GATE)
	{
		if (m_heart_time.counting_timer(ntime)) game_heart_beat();

		if (m_is_open_lua && m_login_state == LOGIN_STATE_ENTER_SCENE) {
			if (m_script_proxy) m_script_proxy->onThink(m_robot_id, gone_time);
		}
	}

	return true;
}

void robot_t::move_to_postion(float x, float z)
{
	spos target_pos(x, z);

	float fDist = my_sqrt(m_robot_info.curr_pos, target_pos);
	if (fDist < 0.2f)
		return;

	m_robot_info.dir = my_angle(m_robot_info.curr_pos, target_pos);

	//log_info("robot_t::move_to_postion dir:%f", dir);
	//log_info("fDist:%f cx:%f cy:%f x:%f y:%f", fDist, m_robot_info.curr_pos.m_x, m_robot_info.curr_pos.m_z, x, z);

	if (fDist < 1.1f)
	{
		m_robot_info.curr_pos = target_pos;
	}
	else
	{
		m_robot_info.curr_pos.m_x += sin(m_robot_info.dir)*1.0f;
		m_robot_info.curr_pos.m_z += cos(m_robot_info.dir)*1.0f;
	}

	proto::client::cg_move_request msg_req;
	msg_req.set_pos_x((uint32_t)(m_robot_info.curr_pos.m_x * 100));
	msg_req.set_pos_y((uint32_t)(m_robot_info.curr_pos.m_z * 100));
	msg_req.set_direction((uint32_t)(m_robot_info.dir/ __PI * 180 * 100));
	msg_req.set_high(-4700); // 测试用高度
	msg_req.set_time_stamp(200);
	send_msg(get_socket(), op_cmd::cg_move_request, msg_req);

	// log_info("robot_t::move_to_postion x:%f y:%f", m_robot_info.curr_pos.m_x, m_robot_info.curr_pos.m_z);
}

bool robot_t::move_arrive(float x, float z, float dist /*= 0.2f*/)
{
	float fDist = my_sqrt(m_robot_info.curr_pos, spos(x, z));

	//log_info("fDist:%f cx:%f cy:%f x:%f y:%f", fDist, m_robot_info.curr_pos.m_x, m_robot_info.curr_pos.m_z, m_target_pos.m_x, m_target_pos.m_z);

	if (fDist < dist)
	{
		return true;
	}

	return false;
}

void robot_t::task_done()
{
    log_debug("robot[%lu] Task Done!!!!!!!!", m_robot_id);
	m_task_done = true;
}

void robot_t::fast_move(float x, float z)
{
	float fDist = my_sqrt(m_robot_info.curr_pos, spos(x, z));
	if (fDist < 0.2f)
		return;

	m_robot_info.curr_pos.m_x = x;
	m_robot_info.curr_pos.m_z = z;

	proto::client::cg_move_request msg_req;
	msg_req.set_pos_x((uint32_t)(m_robot_info.curr_pos.m_x * 100));
	msg_req.set_pos_y((uint32_t)(m_robot_info.curr_pos.m_z * 100));
	msg_req.set_direction((uint32_t)(m_robot_info.dir / __PI * 180 * 100));
	msg_req.set_time_stamp(0);
	send_msg(get_socket(), op_cmd::cg_move_request, msg_req);
}

void robot_t::game_heart_beat()
{
	proto::client::ca_pingpong_request msg_req;

	send_msg(get_socket(), op_cmd::ca_pingpong_request, msg_req);
}

void robot_t::init_lua()
{
	m_curr_time = common::time_util_t::now_msecs();
	m_heart_time.begin_timer(3000, m_curr_time);

	if (m_script_proxy)
		m_script_proxy->onInit(m_robot_id);

	m_init_lua = true;
}

float robot_t::get_currX()
{
	return m_robot_info.curr_pos.m_x;
}

float robot_t::get_currY()
{
	return m_robot_info.curr_pos.m_z;
}

void robot_t::talk_about(uint32_t type, const char* str)
{
	if (type < proto::common::chat_channel_system || type > proto::common::chat_channel_create_team)
	{
        log_debug("ERROR:robot[%lu] robot_t::talk_about error Type!!", m_robot_id);
		return;
	}

	talking((proto::common::chat_channel)type, str);
}

void robot_t::set_login_success()
{
	uint32_t use_time = (uint32_t)(time_util_t::now_msecs() - m_login_time_msec) - ENTER_SCENE_DELAY_MSEC;

    if (!m_enter_success) {
		m_enter_success = true;

		std::ostringstream str;
		for (const auto& pair : m_login_state_cost) {
			switch (pair.first) {
				case LOGIN_STATE_LOGIN_PRE1: {
					str << " ["
						<< "LOGIN_STATE_LOGIN_PRE1:"
						<< pair.second
						<< "] |";
						break;
				}
				case LOGIN_STATE_LOGIN_PRE2: {
					str << " ["
						<< "LOGIN_STATE_LOGIN_PRE2:"
						<< pair.second
						<< "] |";
						break;
				}
				case LOGIN_STATE_LOGIN_ACCOUNT: {
					str << " ["
						<< "LOGIN_STATE_LOGIN_ACCOUNT:"
						<< pair.second
						<< "] |";
						break;
				}
				case LOGIN_STATE_LOGIN_LOGIN: {
					str << " ["
						<< "LOGIN_STATE_LOGIN_LOGIN:"
						<< pair.second
						<< "] |";
						break;
				}
				case LOGIN_STATE_LOGIN_GATE: {
					str << " ["
						<< "LOGIN_STATE_LOGIN_GATE:"
						<< pair.second
						<< "] |";
						break;
				}
				case LOGIN_STATE_CREATE_USER: {
					str << " ["
						<< "LOGIN_STATE_CREATE_USER:"
						<< pair.second
						<< "] |";
						break;
				}
				case LOGIN_STATE_LOAD_ROLE: {
					str << " ["
						<< "LOGIN_STATE_LOAD_ROLE:"
						<< pair.second
						<< "] |";
						break;
				}
				case LOGIN_STATE_ENTER_SCENE: {
					str << " ["
						<< "LOGIN_STATE_ENTER_SCENE:"
						<< pair.second
						<< "] |";
						break;
				}
				default:
					break;
			}
		}

		log_info("robot[%lu:%lu] state cost %s", m_robot_id, m_uid, str.str().c_str());
		robot_manager_t::on_robot_enter_success(m_robot_id, m_uid, m_login_start_time , use_time );
		robot_in_game();
	}
}

void robot_t::get_mail_list()
{
	m_mail_lists.Clear();

	log_debug("send mail list request to center server");
	proto::client::ce_mail_list_request msg_req;
	send_msg(get_socket(), op_cmd::ce_mail_list_request, msg_req);
}

void robot_t::update_mail_list(const proto::client::ec_mail_list_notify& notify)
{
	log_debug("update mail list, size[%d]", notify.base_infos_size());

	m_mail_lists.CopyFrom(notify);

	for (int i = 0; i < m_mail_lists.base_infos_size(); ++i)
	{
		const ::proto::common::mail_base_info& base_info = m_mail_lists.base_infos(i);
		log_debug("mail[%s] mail_type[%d] mail_state[%d] mail_addenda[%d] from[%s] title[%s]", base_info.common_info().id().c_str(),
			base_info.common_info().type(), base_info.common_info().state(), base_info.common_info().addenda(), base_info.from_name().c_str(), base_info.title().c_str());
	}
}

bool robot_t::have_new_mail()
{
	for (int i = 0; i < m_mail_lists.base_infos_size(); ++i)
	{
		const ::proto::common::mail_base_info& base_info = m_mail_lists.base_infos(i);
		if (base_info.common_info().state() == proto::common::MAIL_STATE_NEW)
		{
			return true;
		}
	}

	return false;
}

void robot_t::open_one_mail()
{
	for (int i = 0; i < m_mail_lists.base_infos_size(); ++i)
	{
		const ::proto::common::mail_base_info& base_info = m_mail_lists.base_infos(i);
		if (base_info.common_info().state() == proto::common::MAIL_STATE_NEW)
		{
			proto::client::ce_mail_content_request msg_req;
			msg_req.set_m_id(base_info.common_info().id());
			send_msg(get_socket(), op_cmd::ce_mail_content_request, msg_req);
			break;
		}
	}
}

void robot_t::update_mail_content(const proto::client::ec_mail_content_reply& reply)
{
	if (reply.reply_code() != 0)
	{
		log_error("get mail content error, reply code:%u", reply.reply_code());
		return;
	}

	uint64_t mail_id = common::string_util_t::string_to_uint64(reply.base_info().common_info().id());

	proto::common::mail_base_info* base_info = get_mail_info(mail_id);
	if (NULL == base_info)
	{
		log_error("mail[%lu] null error", mail_id);
		return;
	}

	base_info->CopyFrom(reply.base_info());

	log_debug("mail[%lu] is opened,content[%s]", mail_id, reply.content().c_str());
}

void robot_t::update_mail_state_chage(const proto::client::ec_mail_state_change_notify& notify)
{
	if (notify.reply_code() != 0)
	{
		log_debug("update mail state change, reply code:%u", notify.reply_code());
	}

	for (int i = 0; i < notify.common_infos_size(); ++i)
	{
		const ::proto::common::mail_common_info& common_info = notify.common_infos(i);

		uint64_t mail_id = common::string_util_t::string_to_uint64(common_info.id());

		proto::common::mail_base_info* base_info = get_mail_info(mail_id);
		if (NULL != base_info)
		{
			::proto::common::mail_common_info* p_change_common = base_info->mutable_common_info();
			if (NULL != p_change_common)
			{
				p_change_common->CopyFrom(common_info);

				log_debug("mail[%lu] mail_state[%d] mail_addenda[%d] ", mail_id, p_change_common->state(), p_change_common->addenda());
			}
		}
	}
}

proto::common::mail_base_info* robot_t::get_mail_info(uint64_t mail_id)
{
	for (int i = 0; i < m_mail_lists.base_infos_size(); ++i)
	{
		proto::common::mail_base_info* base_info = m_mail_lists.mutable_base_infos(i);
		if (NULL == base_info)
		{
			log_error("base_info null error");
			continue;
		}

		if (mail_id == common::string_util_t::string_to_uint64(base_info->common_info().id()))
		{
			return base_info;
		}

	}

	return NULL;
}

void robot_t::random_mail_send_self(uint32_t num)
{
	char tmp[32] = {0};
	snprintf(tmp, sizeof(tmp) - 1, "/randommail %u", num);

	opt_gm_cmd(tmp);
}

int32_t robot_t::get_mail_size()
{
	int32_t num = 0;

	// 删除的不算
	for (int i = 0; i < m_mail_lists.base_infos_size(); ++i)
	{
		const ::proto::common::mail_base_info& base_info = m_mail_lists.base_infos(i);
		if (base_info.common_info().state() == proto::common::MAIL_STATE_REMOVE)
			continue;

		// 没附件且被打开的不算
		if (base_info.common_info().state() == proto::common::MAIL_STATE_OPENED && base_info.common_info().addenda() == proto::common::MAIL_ADDENDA_NONE)
			continue;

		num++;
	}

	return num;
}

void robot_t::del_one_mail()
{
	for (int i = 0; i < m_mail_lists.base_infos_size(); ++i)
	{
		const ::proto::common::mail_base_info& base_info = m_mail_lists.base_infos(i);
		if (base_info.common_info().state() == proto::common::MAIL_STATE_REMOVE)
			continue;

		proto::client::ce_mail_remove_request msg_req;
		msg_req.set_m_id(base_info.common_info().id());
		send_msg(get_socket(), op_cmd::ce_mail_remove_request, msg_req);
		break;
	}
}

int32_t robot_t::get_addenda_mail_index()
{
	for (int i = 0; i < m_mail_lists.base_infos_size(); ++i)
	{
		const ::proto::common::mail_base_info& base_info = m_mail_lists.base_infos(i);
		if (base_info.common_info().state() == proto::common::MAIL_STATE_REMOVE)
			continue;

		if (base_info.common_info().addenda() == proto::common::MAIL_ADDENDA_NEW)
		{
			return i;
		}
	}

	return -1;
}

void robot_t::get_mail_addenda(int32_t index)
{
	proto::common::mail_base_info* p_base_info = m_mail_lists.mutable_base_infos(index);
	if (NULL != p_base_info)
	{
		proto::client::ce_mail_get_addenda_request msg_req;
		msg_req.set_m_id(p_base_info->common_info().id());
		send_msg(get_socket(), op_cmd::ce_mail_get_addenda_request, msg_req);
	}
}

void robot_t::one_key_del_all()
{
	proto::client::ce_mail_batch_request msg_req;
	msg_req.set_act(proto::common::MAIL_BATCH_CLEAR);
	send_msg(get_socket(), op_cmd::ce_mail_batch_request, msg_req);
}

void robot_t::finish_new_role_guide_notify(uint32_t guide_tid)
{
	proto::client::cg_finish_new_role_guide_notify msg_req;
	msg_req.set_tid(guide_tid);
	send_msg(get_socket(), op_cmd::cg_finish_new_role_guide_notify, msg_req);
}

void robot_t::update_robot_hero_info(const proto::client::gc_enter_game_for_hero_reply& msg)
{
	if (msg.has_hero())
	{
		const proto::common::hero_data& hero_list = msg.hero();
		for (int i = 0; i < hero_list.hero_list_size(); ++i)
		{
			proto::common::hero_single* p_single = m_robot_info.hero.add_hero_list();
			p_single->CopyFrom(hero_list.hero_list(i));
		}
		log_debug("role[%lu] hero num[%d]", m_robot_id, m_robot_info.hero.hero_list_size());
	}
}

void robot_t::update_robot_item_info(const proto::client::gc_enter_game_for_item_reply& msg)
{
	if (msg.has_item())
	{
		const proto::common::item_data& item_list = msg.item();
		for (int i = 0; i < item_list.item_list_size(); ++i)
		{
			proto::common::item_single* p_single = m_robot_info.item.add_item_list();
			p_single->CopyFrom(item_list.item_list(i));
		}
		log_debug("role[%lu] item num[%d]", m_robot_id, m_robot_info.item.item_list_size());
	}
}

void robot_t::update_into_vision(const proto::client::gc_role_into_vision_notify& msg)
{
	if (msg.common_list_size() == 0) return;

	for (int32_t i = 0; i < msg.common_list_size(); ++i) {
		const ::proto::common::object_common_info& object_common = msg.common_list(i);
		uint64_t object_id = common::string_util_t::string_to_uint64(object_common.object_id());

		if (object_id != m_uid) {
			m_object_vision_list.insert(std::make_pair(object_id, object_common));
		} else {
			proto::common::object_common_info* p_common_info = m_robot_info.role.mutable_obj_info();
			if (p_common_info) {
				p_common_info->CopyFrom(object_common);
			}
		}
	}
}

void robot_t::update_out_of_vision(const proto::client::gc_role_outo_vision_notify& msg)
{
	if (msg.common_list_size() == 0) return;

	for (int32_t i = 0; i < msg.common_list_size(); ++i) {
		uint64_t object_id = common::string_util_t::string_to_uint64(msg.common_list(i).object_id());

		std::map<uint64_t, proto::common::object_common_info>::iterator itr = m_object_vision_list.find(object_id);
		if (itr != m_object_vision_list.end()) {
			m_object_vision_list.erase(itr);
		}
	}
}

void robot_t::update_vision_info(const proto::client::gc_scene_object_info_notify& msg)
{
	uint64_t object_id = common::string_util_t::string_to_uint64(msg.common_info().object_id());
	if (object_id != m_uid) {
		std::map<uint64_t, proto::common::object_common_info>::iterator itr = m_object_vision_list.find(object_id);
		if (itr != m_object_vision_list.end()) {
			itr->second.CopyFrom(msg.common_info());
		}
	} else {
		proto::common::object_common_info* p_common_info = m_robot_info.role.mutable_obj_info();
		if (p_common_info) {
			p_common_info->CopyFrom(msg.common_info());
		}
	}
}

bool robot_t::try_open_dungeon(uint32_t dungeon_id)
{
	if (m_dungeon_id == dungeon_id)
	{
		// 已经发送了开启副本的消息，等待返回
		return false;
	}

	DungeonTable* p_config = GET_CONF(DungeonTable, dungeon_id);
	if (NULL == p_config)
	{
		log_error("p_config null error, dungeon_id[%d]", dungeon_id);
		return false;
	}

	m_dungeon_map_id = p_config->map_tid();
	m_dungeon_id = dungeon_id;

	proto::client::cg_open_dungeon_request msg;
	msg.set_dungeon_id(m_dungeon_id);
	send_msg(get_socket(), op_cmd::cg_open_dungeon_request, msg);

	return true;
}

bool robot_t::is_in_dungeon(uint32_t dungeon_id)
{
	if (m_dungeon_id != dungeon_id)
		return false;

	return m_dungeon_map_id == m_robot_info.map_id;
}

bool robot_t::try_enter_fight()
{
	uint64_t object_id = get_object_id();
	if (object_id == 0)
	{
		log_error("try_enter_dungeon_fight fail, no target");
		return false;
	}

	proto::client::cg_enter_fight_request msg;
	msg.set_object_id(common::string_util_t::uint64_to_string(object_id));
	msg.set_mode(proto::common::EM_PK_MODE::PK_SLAUGHTER);
	msg.set_object_type(proto::common::EM_SCENEOBJECT_TYPE::SCENEOBJECT_MONSTER);
	send_msg(get_socket(), op_cmd::cg_enter_fight_request, msg);
	return true;
}

void robot_t::try_auto_fight()
{
	if (m_fight_uid == 0)
	{
		log_error("m_fight_uid == 0 error");
		return;
	}

	proto::client::cg_combat_auto_fight_request msg;
	msg.set_fight_uid(common::string_util_t::uint64_to_string(m_fight_uid));
	msg.set_auto_fight(1);
	send_msg(get_socket(), op_cmd::cg_combat_auto_fight_request, msg);

	proto::client::cg_combat_before_form_ready_request msg_request;
	msg_request.set_fight_uid(common::string_util_t::uint64_to_string(m_fight_uid));
	send_msg(get_socket(), op_cmd::cg_combat_before_form_ready_request, msg_request);
}

void robot_t::try_leave_dungeon()
{
	if (m_dungeon_map_id == m_robot_info.map_id)
	{
		proto::client::cg_dungeon_leave_request msg;
		send_msg(get_socket(), op_cmd::cg_dungeon_leave_request, msg);
	}
}

uint64_t robot_t::get_object_id()
{
	float fDist = 0.f;
	uint64_t object_id = 0;
	for (const auto& it : m_object_vision_list)
	{
		if (it.second.object_state().obj_state() == proto::common::object_state_death || it.second.object_state().obj_state() == proto::common::object_state_fighting)
			continue;

		float Dist = my_sqrt(m_robot_info.curr_pos, spos(it.second.current_x(), it.second.current_y()));
		if (Dist <= ENTER_THUNDERMONSTER_RANGE)
		{
			if (object_id == 0 || Dist < fDist)
			{
				object_id = it.first;
				fDist = Dist;
			}
		}
	}
	return object_id;
}

uint64_t robot_t::get_monster_by_tid(uint32_t monster_tid)
{
	for (const auto& it : m_object_vision_list) {
		if (it.second.object_type() != proto::common::SCENEOBJECT_MONSTER)
			continue;

		if (it.second.object_state().obj_state() == proto::common::object_state_death)
			continue;

		if (it.second.npc_tid() == monster_tid) {
			return common::string_util_t::string_to_uint64(it.second.object_id());
		}
	}

	return 0;
}

void robot_t::fight_end_update_info(proto::client::gc_combat_result_ntf& msg)
{
    log_debug("robot_name[%s],fight end,result[%d],result_type[%d]", get_nickname().c_str(), msg.result().win_camp(), msg.result().result());

	// 重置战斗id
	m_fight_uid = 0;
	m_is_auto_fight = false;

//	暂时不更新机器人信息
// 	const proto::common::role_change_data& info = msg.rcd();
// 	if (info.has_level())
// 	{
// 		m_robot_info.role.set_level(info.level());
// 	}
// 	if (info.has_exp())
// 	{
// 		m_robot_info.role.set_exp(info.exp());
// 	}
// 	
// 	
// 	for (int32_t i = 0; i < info.item().item_list_size();++i)
// 	{
// 		
// 		proto::common::item_single* p_item = m_robot_info.item.add_item_list();
// 		p_item->CopyFrom(info.item().item_list(i));
// 	}
// 	proto::common::personal_info* personal_info = m_robot_info.role.mutable_per_info();
// 	if (info.per_info().has_gold())
// 	{
// 		personal_info->set_gold(info.per_info().gold());
// 	}
// 
// 	if (info.per_info().has_silver())
// 	{
// 		personal_info->set_silver(info.per_info().silver());
// 	}
// 
// 	if (info.per_info().has_copper())
// 	{
// 		personal_info->set_copper(info.per_info().copper());
// 	}
// 
// 	if (info.per_info().has_pk_value())
// 	{
// 		personal_info->set_pk_value(info.per_info().pk_value());
// 	}
// 
// 	if (info.per_info().has_achieve_point())
// 	{
// 		personal_info->set_achieve_point(info.per_info().achieve_point());
// 	}
// 
// 	if (info.per_info().has_achieve_level())
// 	{
// 		personal_info->set_achieve_level(info.per_info().achieve_level());
// 	}
// 
// 	if (info.per_info().has_lifeforce())
// 	{
// 		personal_info->set_lifeforce(info.per_info().lifeforce());
// 	}
// 
// 	if (info.per_info().has_lifeforce())
// 	{
// 		personal_info->set_lifeforce(info.per_info().lifeforce());
// 	}
}

void robot_t::open_dungeon_failed(uint32_t back_code)
{
	log_error("robot[%lu] open_dungeon_failed, back_code:%u", m_robot_id, back_code);

	if (m_script_proxy)
	{
		m_script_proxy->onError(m_robot_id);
	}
}

void robot_t::clear_dungeon()
{
	m_dungeon_id = 0;
	m_dungeon_map_id = 0;

	m_dungeon_state = proto::common::dungeon_state_none;
	m_schedule_datas.clear();
	m_schedule_index = -1;
}

void robot_t::notify_dungeon_schedule(proto::client::gc_dungeon_schedule_data_notify &msg)
{
	m_dungeon_state = msg.state_type();

	m_schedule_datas.clear();

	for (int32_t i = 0; i < msg.datas_size(); ++i)
	{
		m_schedule_datas.push_back(msg.datas(i));
	}
}

void robot_t::notify_dungeon_schedule(proto::client::gc_notify_dungeon_result_notify &msg)
{
	m_dungeon_state = proto::common::dungeon_state_stop;

    log_debug("robot[%lu] dungeon[%u] result:%u", m_robot_id, m_dungeon_id, msg.type());
}

void robot_t::set_fight_uid(uint64_t fight_id)
{
	m_fight_uid = fight_id;

	m_is_auto_fight = false;
}

bool robot_t::is_all_killed()
{
	return m_dungeon_state == proto::common::dungeon_state_stop;
}

int32_t robot_t::get_fight_index()
{
	int32_t index = -1;
	for (int32_t i = 0; i < (int32_t)m_schedule_datas.size(); ++i)
	{
		const proto::common::dungeon_schedule_data& data = m_schedule_datas[i];
		if (data.curr_num() < data.max_num() && (data.pos_x() != 0 && data.pos_y() != 0 ))
		{
			index = i;
			break;
		}
	}

	m_schedule_index = index;

	return m_schedule_index;
}

void robot_t::dungeon_fast_move_to(int32_t index)
{
	if (m_schedule_index != index)
	{
		log_error("m_schedule_index[%d] != index[%d]", m_schedule_index, index);
		return;
	}

	const proto::common::dungeon_schedule_data& data = m_schedule_datas.at(index);

	fast_move(data.pos_x(), data.pos_y());
}

bool robot_t::is_near_dungeon_target(int32_t index)
{
	if (m_schedule_index != index)
	{
		log_error("m_schedule_index[%d] != index[%d]", m_schedule_index, index);
		return false;
	}

	const proto::common::dungeon_schedule_data& data = m_schedule_datas.at(index);

	float fDist = my_sqrt(m_robot_info.curr_pos, spos(data.pos_x(), data.pos_y()));

	if (fDist < 0.2f)
		return true;

	return false;
}

void robot_t::toggle_mount(uint32_t state)
{
	// 已经是这个状态了
	if (state == m_robot_info.role.obj_info().object_state().mode_state()) {
		return;
	}

	proto::client::cg_change_sys_state_notify notify;
	switch (state) {
		case proto::common::role_mode_state_none: {
			if (m_robot_info.role.obj_info().object_state().mode_state() == proto::common::role_mode_state_normal) {
				notify.set_mode_state(proto::common::role_mode_state_mount);
			} else if (m_robot_info.role.obj_info().object_state().mode_state() == proto::common::role_mode_state_mount) {
				notify.set_mode_state(proto::common::role_mode_state_normal);
			} else {
				log_error("robot[%lu] mode state[%u] error", m_uid, state);
			}
			break;
		}
		case proto::common::role_mode_state_normal: {
			notify.set_mode_state(proto::common::role_mode_state_mount);
			break;
		}
		case proto::common::role_mode_state_mount: {
			notify.set_mode_state(proto::common::role_mode_state_normal);
			break;
		}
		default: {
			log_error("mode state[%u] not define", state);
			return;
		}
	}

	send_msg(get_socket(), op_cmd::cg_change_sys_state_notify, notify);
}

void robot_t::request_boss_info(uint32_t id)
{
	// 重新设置boss信息
	m_boss_id = id;
	m_filed_boss_data.Clear();

	proto::client::ce_field_boss_info_request msg_req;
	send_msg(get_socket(), op_cmd::ce_field_boss_info_request, msg_req);
	log_debug("robot[%lu] request boss[%d] info", m_robot_id, id);
}

void robot_t::reply_boss_info(const proto::client::ec_field_boss_info_notify& ntf)
{
	for (int32_t i = 0; i < ntf.today_boss_size(); ++i) {
		const proto::common::filed_boss_state& state = ntf.today_boss(i);
		if (m_boss_id != 0 && state.id() != m_boss_id) {
			continue;
		}

		for (int32_t j = 0; j < state.states_size(); ++j) {
			const proto::common::filed_boss_one_state& one_state = state.states(j);
			if (one_state.state() == proto::common::FIELD_BOSS_STATE_START) {
				m_filed_boss_data.CopyFrom(one_state);
				log_debug("robot[%lu] reply_boss_info, id: %u map_id: %u, x: %u, y: %u\n",
					m_robot_id, one_state.monster_tid(), one_state.map_id(), one_state.pos_x(), one_state.pos_y());

				m_field_boss_exist_flag = 1;//用于summon boss

				return;
			}
		}
	}
	m_field_boss_exist_flag = 0;//用于summon boss
}

bool robot_t::attack_field_boss(uint32_t monster_id)
{
	uint64_t monster_uid = get_monster_by_tid(monster_id);
	if (0 == monster_uid) 
	{
		log_debug("robot[%lu] attack_field_boss error, boss disappeared, monster_id[%d] monster_uid[0]", m_robot_id, monster_id);
		return false;
	}

	proto::client::cg_field_boss_attack_request request;
	request.set_object_id(common::string_util_t::uint64_to_string(monster_uid));
	send_msg(get_socket(), op_cmd::cg_field_boss_attack_request, request);
	log_debug("robot[%lu] send field boss attack request", m_robot_id);

	return true;
}

void robot_t::set_field_boss_exist_flag(char flag)
{
	if (flag > 1 || flag < -1)
	{
		log_error("robot[%lu] set field boss exist flag value[%d] error", m_robot_id, flag);
		return;
	}
	m_field_boss_exist_flag = flag;
}

char robot_t::get_field_boss_exist_flag()
{
	return m_field_boss_exist_flag;
}

void robot_t::clear_field_boss_exist_flag()
{
	m_filed_boss_data.Clear();
	m_field_boss_exist_flag = -1;
}

uint32_t robot_t::random_field_boss_id()
{
	return random_util_t::randBetween(1, 25);
}

void robot_t::arena_join()
{
	proto::client::cg_arena_join_request msg_req;
	send_msg(get_socket(), op_cmd::cg_arena_join_request, msg_req);
}

void robot_t::arena_match()
{
	proto::client::cg_arena_match_request msg_req;
	send_msg(get_socket(), op_cmd::cg_arena_match_request, msg_req);

	m_is_arena_matched = false;
}


void robot_t::arena_match_reply(proto::client::gc_arena_match_reply &msg)
{
	log_debug("robot[%lu] recv gc_arena_match_reply from game reply_code[%d] target[%s][%s] tar_score[%d] tar_lvl[%d]",
		get_robot_id(), msg.reply_code(), msg.target_user().rd().uid().c_str(), msg.target_user().rd().name().c_str(),
		msg.target_score(), msg.target_arena_level());

	if (msg.reply_code() == 0)
	{
		m_is_arena_matched = true;
	}
}

void robot_t::arena_battle_finish(proto::client::gc_arena_battle_result_notify &msg)
{
	log_debug("robot[%lu] battle_finish win_or_lose[%d] score[%d] level[%d] rank_up[%d] new_rank[%d] "
		"win_times[%d] lose_times[%d] target[%s][%s] ", get_robot_id(), msg.is_win(), msg.arena_data().score(),
		msg.arena_data().arena_level(), msg.own_rank_up(), msg.own_new_rank(), msg.arena_data().win_times(),
		msg.arena_data().lose_times(), msg.target_user().rd().uid().c_str(),
		msg.target_user().rd().name().c_str());

	m_robot_info.arena_data.set_score(msg.arena_data().score());
	m_robot_info.arena_data.set_arena_level(msg.arena_data().arena_level());
	m_robot_info.arena_data.set_win_times(msg.arena_data().win_times());
	m_robot_info.arena_data.set_lose_times(msg.arena_data().lose_times());

	m_robot_info.arena_data.set_is_start(msg.arena_data().is_start());

	m_is_arena_matched = false;//跨服战使用的m_fight_uid变量战斗结束后在gc_combat_result_ntf消息中置0
}

bool robot_t::is_join_arena()
{
	return m_robot_info.arena_data.is_start();
}

void robot_t::set_join_arena(bool is_join)
{
	m_robot_info.arena_data.set_is_start(is_join);
}

bool robot_t::is_arena_matched()
{
	return m_is_arena_matched;
}

void robot_t::buy_month_card(uint32_t id)
{
	proto::client::cg_buy_month_card_request msg_req;
	msg_req.set_card_id(id);
	send_msg(get_socket(), op_cmd::cg_buy_month_card_request, msg_req);
}

void robot_t::set_login_state(const login_state_em login_state)
{
	m_login_state = login_state;

	uint64_t curr = common::time_util_t::now_msecs();
	m_login_state_cost[m_login_state] = curr - m_last_login_state_time;
	m_last_login_state_time = curr;
}

