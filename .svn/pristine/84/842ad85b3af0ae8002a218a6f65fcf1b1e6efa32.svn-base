#ifndef __GATE_USER_HPP__
#define __GATE_USER_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/weak_ptr.hpp>

#include "tcp_socket.hpp"
#include "log.hpp"
#include "protobuf.hpp"
#include "protos_fwd.hpp"
#include "common_struct.hpp"
#include "cmd_desc.hpp"
#include "network/ByteBuffer.hpp"

#define MOVE_INTERVAL_TIME 30

const unsigned int MOVE_CMD_SIZE = sizeof(struct gc_user_move_cmd);

class user_t : public boost::enable_shared_from_this<user_t>
{
public:
    user_t(uint64_t uid, std::string const& guid);

public:
    bool send_msg(const network::msg_buf_ptr& buf);

	// 批量发送移动9屏消息
	void send_batch_move_msg(const uint64_t& cur_time)
	{
		if (!m_batch_move_notify.rd_ready()) return;

		network::msg_buf_ptr send_buf = network::wrap_buffer_to_client<>(op_cmd::gc_batch_move_notify, m_batch_move_notify.rd_buf(), m_batch_move_notify.rd_size());
		if (NULL == send_buf) {
			log_error("send uid[%lu] cmd[%s:%d] to client filed: NULL == send_buf", m_uid, CMD_DESC(op_cmd::gc_batch_move_notify), op_cmd::gc_batch_move_notify);
			return;
		}

		send_msg(send_buf);

		m_move_next_time = cur_time + MOVE_INTERVAL_TIME;
		m_batch_move_notify.reset();
	}

    bool filter_cmd_print(uint16_t cmd)
    {
        switch (cmd)
        {
        case op_cmd::ac_pingpong_reply:
        case op_cmd::gc_role_outo_vision_notify:
        case op_cmd::gc_role_into_vision_notify:
        case op_cmd::gc_move_notify:
		case op_cmd::gc_batch_move_notify:
        case op_cmd::gc_field_boss_pos_info_notify:
        case op_cmd::gc_system_hint_notify:
        case op_cmd::gc_change_sys_state_notify:
        case op_cmd::gc_sys_notice_notify:
        case op_cmd::gc_scene_object_info_notify:
        case op_cmd::gc_update_long_info_notify:
        case op_cmd::gc_king_war_scene_start_notify:
        case op_cmd::gc_artifact_left_time_notify:
        case op_cmd::gc_object_hp_change_notify:
        case op_cmd::gc_dungeon_schedule_data_notify:
        case op_cmd::gc_notify_dungeon_result_notify:
        case op_cmd::gc_object_scene_jump_notify:
        case op_cmd::ec_king_war_notice_notify:
		case op_cmd::gc_server_time_reply:
		case op_cmd::ac_frames_check_reply:
            return false;
            break;
        default:
            break;
        }

        return true;
    }

    template<typename T_MSG>
    void send_msg_to_client(uint16_t cmd, const T_MSG& msg)
    {
#ifdef __ENCRYPT__
        network::msg_buf_ptr send_buf = network::wrap_msg_encrypt<T_MSG>(cmd, msg, SG_SERVER_TO_CLIENT_KEY);
#else
        network::msg_buf_ptr send_buf = network::wrap_msg_to_client<T_MSG>(cmd, msg);
#endif
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to client filed: NULL == send_buf", m_uid, CMD_DESC(cmd), cmd);
            return;
        }

        if (!send_msg(send_buf))
        {
            return;
        }

        if (filter_cmd_print(cmd))
        {
            log_trace("send user[%lu] cmd[%s:%d] size[%d] message to client", m_uid, CMD_DESC(cmd), cmd, (uint32_t)send_buf->size());
        }

		if (!m_batch_move_notify.rd_ready()) return;

		uint64_t curr_time = common::time_util_t::now_msecs();
		if (curr_time > m_move_next_time) { // 或者时间到了带出去
			send_batch_move_msg(curr_time);
		}
    }

    void send_msg_to_client(uint16_t cmd)
    {
#ifdef __ENCRYPT__
        network::msg_buf_ptr send_buf = network::wrap_msg_encrypt(cmd, SG_SERVER_TO_CLIENT_KEY);
#else
        network::msg_buf_ptr send_buf = network::wrap_msg_to_client(cmd);
#endif
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to client filed: NULL == send_buf", m_uid, CMD_DESC(cmd), cmd);
            return;
        }

        if (!send_msg(send_buf))
        {
            return;
        }

        if (filter_cmd_print(cmd))
        {
            log_trace("send user[%lu] cmd[%s:%d] size[%d] message to client", m_uid, CMD_DESC(cmd), cmd, (uint32_t)send_buf->size());
        }
    }

    void send_msg_to_client(uint16_t cmd, const network::msg_buf_ptr& buf)
    {
#ifdef __ENCRYPT__
        network::msg_buf_ptr send_buf = network::re_wrap_msg_encrypt(cmd, buf, SG_SERVER_TO_CLIENT_KEY);
#else
        network::msg_buf_ptr send_buf = network::re_wrap_buf_to_client(cmd, buf);
#endif
        if (NULL == send_buf)
        {
            log_error("send uid[%lu] cmd[%s:%d] to client filed: NULL == send_buf", m_uid, CMD_DESC(cmd), cmd);
            return;
        }

        if (!send_msg(send_buf))
        {
            return;
        }

        if (filter_cmd_print(cmd))
        {
            log_trace("send user[%lu] cmd[%s:%d] size[%d] message to client", m_uid, CMD_DESC(cmd), cmd, (uint32_t)send_buf->size());
        }

		if (!m_batch_move_notify.rd_ready()) return;

		uint64_t curr_time = common::time_util_t::now_msecs();
		if (curr_time > m_move_next_time) { // 或者时间到了带出去
			send_batch_move_msg(curr_time);
		}
    }

	// 9屏移动数据数据合并处理
	void send_move_to_client(const proto::client::gc_move_notify& notify, const uint64_t& cur_time)
	{
		m_batch_move_notify.wr_reserve(MOVE_CMD_SIZE);
		gc_user_move_cmd* p_move_cmd = (gc_user_move_cmd*)m_batch_move_notify.wr_buf();
		if (p_move_cmd) {
			p_move_cmd->object_id = notify.object_id();
			p_move_cmd->pos_x = notify.pos_x();
			p_move_cmd->pos_y = notify.pos_y();
			p_move_cmd->direction = notify.direction();
			p_move_cmd->speed = notify.speed();
			p_move_cmd->high = notify.high();
		}
		m_batch_move_notify.wr_flip(MOVE_CMD_SIZE);

		if (notify.object_id() == m_uid) { // 自己移动把在列表的都带出去
			send_batch_move_msg(cur_time);
		} else {
			if (cur_time > m_move_next_time) { // 或者时间到了带出去
				send_batch_move_msg(cur_time);
			}
		}
	}

public:
    uint64_t get_uid() const{ return m_uid; }
    void set_uid(uint64_t val) { m_uid = val; }

    std::string get_guid() const{ return m_guid; }
    void set_guid(std::string const& val){ m_guid = val; }

    std::string get_session_id() const{ return m_session_id; }
    void set_session_id(std::string const& val){ m_session_id = val; }

	uint32_t get_cross_id() const{ return m_cross_id; }
	void set_cross_id(uint32_t val) { m_cross_id = val; }

    network::tcp_socket_ptr get_client_socket() { return m_client_socket.lock(); }
	void reset_client_socket(network::tcp_socket_ptr const& s) { m_client_socket = s; }

	void cancel_timer();
    void start_pingpong_timer();
    void start_reconnect_timer();

    uint32_t get_kick_reason() const { return m_kick_reason; }
    void set_kick_reason(uint32_t val) { m_kick_reason = val; }

	// 玩家状态相关
public:
	common::ROLE_GAME_STATE get_game_state() const { return m_game_state; }

	// 设置离线状态
	void set_game_offline();
	// 设置登陆
	void set_login_gate();
	// 设置重连
	void set_login_regate();
	// 设置正在登陆场景
	void set_login_game();
	// 重新设置为初始化状态
	void set_init_state();
	// 设置游戏状态
	void set_scene_state();
	// 设置wait状态
	void set_wait_state();

	// 是否正在游戏
	bool is_scene_state() const;
	// 是否已经登陆
	bool is_login_game() const;
	// 是否已经离线
	bool is_offline_state() const;
	// 是否是初始化状态
	bool is_init_state() const;
	// 是否是登陆gate状态
	bool is_gate_state() const;
	// 是否是重连
	bool is_regate_state() const;
	// 是否是正在登陆场景状态
	bool is_login_state() const;
	// 是否是正在跳场景
	bool is_wait_state() const;
	// 是否正在游戏状态
	bool is_play_state() const;

	// 设置正在创角状态
	void set_create_state();
	// 清除创角状态
	void clear_create_state();
	// 是否正在创角状态
	bool is_create_state();

private:
    uint64_t                    m_uid = 0;
    std::string                 m_guid = 0;
	std::string					m_session_id = "";
    network::tcp_socket_wptr    m_client_socket;
    boost::shared_ptr<boost::asio::deadline_timer> m_timer;
    uint32_t m_kick_reason = common::KICK_ROLE_REASON_NONE;

	uint32_t					m_cross_id = 0;

	// 创角限制,防止多次点击创角
private:
	bool m_is_create_state = false;
	uint32_t m_create_time = 0;

	// 移动同步合并
private:
	network::BufferCmdQueue_t m_batch_move_notify;
	uint64_t m_move_next_time = 0;

private:
	volatile common::ROLE_GAME_STATE m_game_state = common::ROLE_GAME_STATE::ROLE_GAME_STATE_INIT;
};
typedef boost::shared_ptr<user_t> user_ptr;
typedef boost::weak_ptr<user_t> user_wptr;

//////////////////////////////////////////////////////////////////////////

class socket_data_t
{
public:
	socket_data_t() {};

	void init(const network::tcp_socket_ptr& s);
	void close();

	user_ptr get_user() const { return m_user_wptr.lock(); }
	void set_user(const user_ptr& p_user);

	bool check_msg_bomb(uint32_t cmd);
    void reset_msg_bomb()
    {
        m_last_msg_time = 0;
        m_msg_bomb_count = 0;
    }

private:
	user_wptr m_user_wptr;
    boost::shared_ptr<boost::asio::deadline_timer> m_session_timer = NULL;
	uint64_t					m_last_msg_time = 0;
	uint32_t					m_msg_bomb_count = 0;
    //uint32_t					m_last_cmd = 0;
};

#endif