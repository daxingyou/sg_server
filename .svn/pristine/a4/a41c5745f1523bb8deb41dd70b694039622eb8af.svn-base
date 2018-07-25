#ifndef __LOG_TASK_LOG_HPP__
#define __LOG_TASK_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class task_log_t
{
 public:
    task_log_t( uint64_t role_uid, 
		uint32_t task_id,
		uint32_t task_state,
		const std::string& task_progress,
		uint32_t accept_time,
		uint32_t op_type,
		uint32_t log_time)
        : m_role_uid(role_uid)
		, m_task_id(task_id)
        , m_task_state(task_state)
        , m_task_progress(task_progress)
        , m_accept_time(accept_time)
        , m_op_type(op_type)
        , m_log_time(log_time)
    {
    }

    void update_to_db(const Connection_T& conn)
    {
        TRY
        {
            std::string sourec_type_str;
            if (!source_type_converter_t::find_desc(m_op_type, sourec_type_str))
            {
                log_error("can not find a description of the source_type[%d]!", m_op_type);
                return;
            }

            PreparedStatement_T pre = Connection_prepareStatement(conn,
                "insert into `task_log` set \
                `role_uid`=?, \
                `task_id`=?, \
                `task_state`=?, \
                `task_progress`=?, \
                `accept_time` =?, \
                `op_type`  =?, \
                `op_type_desc`=?, \
                `create_time`=from_unixtime(?);");
			PreparedStatement_setLLong(pre,	1, m_role_uid);
			PreparedStatement_setInt(pre,	2, m_task_id);
            PreparedStatement_setInt(pre,	3, m_task_state);
			PreparedStatement_setString(pre,4, m_task_progress.c_str());
            PreparedStatement_setInt(pre,   5, m_accept_time);
            PreparedStatement_setInt(pre,   6, m_op_type);
			PreparedStatement_setString(pre,7, sourec_type_str.c_str());
            PreparedStatement_setInt(pre,	8, m_log_time);
            PreparedStatement_execute(pre);
            }
                CATCH(SQLException)
            {
                log_error("user[%lu] insert task log[%d] [%d] to db error: %s", m_role_uid, m_task_id, m_op_type, Exception_frame.message);
            }
            FINALLY
            {
            }
            END_TRY;
    }

private:
    uint64_t            m_role_uid;
	uint32_t			m_task_id;
    uint32_t            m_task_state;
	std::string			m_task_progress;
    uint32_t            m_accept_time;
    uint32_t            m_op_type;
    uint32_t            m_log_time;
};
typedef boost::shared_ptr<task_log_t> task_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class task_log_manager_t
{
public:
    void add_log(task_log_ptr log_ptr) 
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
    typedef std::vector<task_log_ptr> task_log_vec_type;
    task_log_vec_type m_logs_list;
};

#endif
