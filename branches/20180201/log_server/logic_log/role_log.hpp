#ifndef __LOG_ROLE_LOG_DATA_HPP__
#define __LOG_ROLE_LOG_DATA_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class login_log_t
{
public:
    login_log_t(uint64_t in_uid, uint32_t gate_id, uint32_t game_id, uint32_t log_time)
        : m_role_uid(in_uid)
        , m_gate_id(gate_id)
        , m_game_id(game_id)
        , m_log_time(log_time)
    {

    }

    void update_to_db(const Connection_T& conn)
    {
        TRY
        {
            uint32_t source_type = log_enum::source_type_role_login;
            std::string sourec_type_str;
            if (!source_type_converter_t::find_desc(source_type, sourec_type_str))
            {
                log_error("role[%lu] insert role login log failed! reason: find desc[%d] failed!", m_role_uid, source_type);
                return;
            }

            PreparedStatement_T pre = Connection_prepareStatement(conn,
                "insert into `login_log` set \
                    `role_uid`=?, \
                    `gate_id`=?, \
                    `game_id`=?, \
                    `source_type`=?, \
                    `source_type_desc`=?, \
                    `create_time`=from_unixtime(?);");
            PreparedStatement_setLLong(pre,     1, m_role_uid);
            PreparedStatement_setInt(pre,       2, m_gate_id);
            PreparedStatement_setInt(pre,       3, m_game_id);
            PreparedStatement_setInt(pre,       4, source_type);
            PreparedStatement_setString(pre,    5, sourec_type_str.c_str());
            PreparedStatement_setInt(pre,       6, m_log_time);
            PreparedStatement_execute(pre);
        }
        CATCH(SQLException)
        {
            log_error("role[%lu] insert role login log to db error: %s", m_role_uid, Exception_frame.message);
        }
        FINALLY
        {
        }
        END_TRY;
    }

public:
    uint64_t                        m_role_uid;
    uint32_t                        m_gate_id;
    uint32_t                        m_game_id;
    uint32_t                        m_log_time;
};
typedef boost::shared_ptr<login_log_t> login_log_ptr;

class logout_log_t
{
public:
    logout_log_t(uint64_t in_uid, uint32_t in_online_time, uint32_t gate_id, uint32_t game_id, uint32_t kick_reason, uint32_t log_time)
        : m_role_uid(in_uid)
        , m_online_time(in_online_time)
        , m_gate_id(gate_id)
        , m_game_id(game_id)
        , m_kick_reason(kick_reason)
        , m_log_time(log_time)
    {

    }
    void update_to_db(const Connection_T& conn)
    {
        TRY
        {
            uint32_t source_type = log_enum::source_type_role_logout_none;
            switch (m_kick_reason)
            {
            case KICK_ROLE_REASON_NORMAL:
                source_type = log_enum::source_type_role_logout_normal;
                break;
            case KICK_ROLE_REASON_PINGPONG_TIMEOUT:
                source_type = log_enum::source_type_role_logout_pingpong_timeout;
                break;
            case KICK_ROLE_REASON_RECONNECT_TIMEOUT:
                source_type = log_enum::source_type_role_logout_reconnect_timeout;
                break;
            case KICK_ROLE_REASON_BY_OTHER:
                source_type = log_enum::source_type_role_logout_kick_by_others;
                break;
            case KICK_ROLE_REASON_BAN_ACCOUNT:
                source_type = log_enum::source_type_role_logout_ban_account;
                break;
            case KICK_ROLE_REASON_GATE_SERVER_CLOSED:
                source_type = log_enum::source_type_role_logout_gate_server_close;
                break;
            case KICK_ROLE_REASON_GAME_SERVER_CLOSED:
                source_type = log_enum::source_type_role_logout_game_server_close;
                break;
			case KICK_ROLE_REASON_MSG_BOMB:
				source_type = log_enum::source_type_role_logout_msg_bomb_kick;
				break;
			case KICK_ROLE_REASON_CENTER_UNREG:
				source_type = log_enum::source_type_role_logout_center_close;
				break;
			case KICK_ROLE_REASON_ERROR:
				source_type = log_enum::source_type_role_logout_error;
				break;
			case KICK_ROLE_REASON_CROSS_SCENE:
				source_type = log_enum::source_type_role_logout_cross_server;
				break;
            default:
                log_error("kick_reason[%d] error!", m_kick_reason);
				source_type = log_enum::source_type_role_logout_none;
				break;
            }

            std::string sourec_type_str;
            if (!source_type_converter_t::find_desc(source_type, sourec_type_str))
            {
                log_error("user[%lu] insert user login log failed! reason: find desc[%d] failed!", m_role_uid, source_type);
                return;
            }

            PreparedStatement_T pre = Connection_prepareStatement(conn,
                "insert into `login_log` set \
                `role_uid`=?, \
                `online_time`=?, \
                `gate_id`=?, \
                `game_id`=?, \
                `source_type`=?, \
                `source_type_desc`=?, \
                `create_time`=from_unixtime(?);");
            PreparedStatement_setLLong(pre,  1, m_role_uid);
            PreparedStatement_setInt(pre,    2, m_online_time);
            PreparedStatement_setInt(pre,    3, m_gate_id);
            PreparedStatement_setInt(pre,    4, m_game_id);
            PreparedStatement_setInt(pre,    5, source_type);
            PreparedStatement_setString(pre, 6, sourec_type_str.c_str());
            PreparedStatement_setInt(pre,    7, m_log_time);
            PreparedStatement_execute(pre);
        }
        CATCH(SQLException)
        {
            log_error("user[%lu] insert user login log to db error: %s", m_role_uid, Exception_frame.message);
        }
        FINALLY
        {
            //log_warn("inser user login log to db!");
        }
        END_TRY;
    }

public:
    uint64_t                        m_role_uid;
    uint32_t                        m_online_time;
    uint32_t                        m_gate_id;
    uint32_t                        m_game_id;
    uint32_t                        m_kick_reason;
    uint32_t                        m_log_time;
};
typedef boost::shared_ptr<logout_log_t> logout_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////
class role_log_manager_t
{
public:
    void update_to_db(const Connection_T& conn)
    {
        {//login
            int32_t vec_size = login_log_vec.size();
            for (int32_t i = 0; i < vec_size; ++i)
            {
                if (login_log_vec[i])
                {
                    login_log_vec[i]->update_to_db(conn);
                }
            }
            login_log_vec.clear();
        }

        {//logout
            int32_t vec_size = logout_log_vec.size();
            for (int32_t i = 0; i < vec_size; ++i)
            {
                if (logout_log_vec[i])
                {
                    logout_log_vec[i]->update_to_db(conn);
                }
            }
            logout_log_vec.clear();
        }
    }

    void add_role_login_log(login_log_ptr log_ptr)
    {
        login_log_vec.push_back(log_ptr);
    }

    void add_role_logout_log(logout_log_ptr log_ptr)
    {
        logout_log_vec.push_back(log_ptr);
    }

public:
    typedef std::vector<login_log_ptr> login_log_vec_type;
    login_log_vec_type login_log_vec;

    typedef std::vector<logout_log_ptr> logout_log_vec_type;
    logout_log_vec_type logout_log_vec;
};
#endif
