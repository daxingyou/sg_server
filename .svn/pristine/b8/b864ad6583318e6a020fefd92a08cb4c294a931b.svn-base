#ifndef __LOG_OFFLINE_ARENA_RANK_LOG_HPP__
#define __LOG_OFFLINE_ARENA_RANK_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"
#include "common.pb.h"

class offline_arena_rank_log_t
{

 public:
	 offline_arena_rank_log_t( uint64_t role_uid, const std::string& role_name, uint32_t role_level, uint32_t op_type, 
		 uint32_t old_class, uint32_t old_rank, uint32_t new_class, uint32_t new_rank, uint32_t log_time)
        : m_role_uid(role_uid)
		, m_role_name(role_name)
        , m_role_level(role_level)
        , m_op_type(op_type)
        , m_old_class(old_class)
		, m_old_rank(old_rank)
        , m_new_class(new_class)
        , m_new_rank(new_rank)
        , m_log_time(log_time)
    {
    }

    void update_to_db(const Connection_T& conn)
    {
        TRY
        {
            PreparedStatement_T pre = Connection_prepareStatement(conn,
                "insert into `offline_arena_rank_log` set \
                `role_uid`=?, \
                `role_name`=?, \
                `role_level`=?, \
                `op_type`=?, \
                `old_class` =?, \
				`old_rank` =?, \
                `new_class`  =?, \
                `new_rank` =?, \
                `create_time`=from_unixtime(?);");
			PreparedStatement_setLLong(pre,  1, m_role_uid);
			PreparedStatement_setString(pre, 2, m_role_name.c_str());
			PreparedStatement_setInt(pre,  3, m_role_level);
            PreparedStatement_setInt(pre,    4, m_op_type);
            PreparedStatement_setInt(pre,    5, m_old_class);
			PreparedStatement_setInt(pre,	 6, m_old_rank);
            PreparedStatement_setInt(pre,    7, m_new_class);
            PreparedStatement_setInt(pre,    8, m_new_rank);
            PreparedStatement_setInt(pre,    9, m_log_time);
            PreparedStatement_execute(pre);
            }
                CATCH(SQLException)
            {
                log_error("user[%lu] insert offline_arena_rank log[%d] to db error: %s", m_role_uid, m_op_type, Exception_frame.message);
            }
            FINALLY
            {
            }
            END_TRY;
    }

private:
    uint64_t            m_role_uid;
    std::string         m_role_name;
	uint32_t			m_role_level;
    uint32_t            m_op_type;
    uint32_t            m_old_class;
	uint32_t            m_old_rank;
    uint32_t            m_new_class;
    uint32_t            m_new_rank;
    uint32_t            m_log_time;
};
typedef boost::shared_ptr<offline_arena_rank_log_t> offline_arena_rank_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class offline_arena_rank_log_manager_t
{
public:
    void add_log(offline_arena_rank_log_ptr log_ptr)
    { 
        m_logs_list.push_back(log_ptr); 
    }

    void update_to_db(const Connection_T& conn)
    {
        int32_t vec_size = m_logs_list.size();
        for (int32_t i = 0; i < vec_size; ++i)
        {
            if (m_logs_list[i])
            {
                m_logs_list[i]->update_to_db(conn);
            }
        }
        m_logs_list.clear();
    }

private:
    typedef std::vector<offline_arena_rank_log_ptr> offline_arena_rank_log_vec_type;
	offline_arena_rank_log_vec_type m_logs_list;
};

#endif
