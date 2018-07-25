#ifndef __LOG_ROLE_BUFF_LOG_HPP__
#define __LOG_ROLE_BUFF_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class role_buff_log_t
{
 public:
	 role_buff_log_t( uint64_t user_uid, uint32_t buff_id, uint32_t op_type, uint32_t speed,
                uint32_t source_type, uint32_t source_param, uint32_t log_time)
        : m_user_uid(user_uid)
		, m_buff_id(buff_id)
        , m_op_type(op_type)
		, m_speed(speed)
        , m_source_type(source_type)
		, m_source_param(source_param)
        , m_log_time(log_time)
    {
    }

    void update_to_db(const Connection_T& conn)
    {
        TRY
        {
            std::string sourec_type_str;
            if (!source_type_converter_t::find_desc(m_source_type, sourec_type_str))
            {
                log_error("can not find a description of the source_type[%d]!", m_source_type);
                return;
            }

            PreparedStatement_T pre = Connection_prepareStatement(conn,
                "insert into `role_buff_log` set \
                `role_uid`=?, \
                `buff_id`=?, \
                `op_type` =?, \
				`speed` =?, \
                `source_type`=?, \
                `source_param`=?, \
                `source_type_desc`=?, \
                `create_time`=from_unixtime(?);");
			PreparedStatement_setLLong(pre,  1, m_user_uid);
			PreparedStatement_setInt(pre,	 2, m_buff_id);
            PreparedStatement_setInt(pre,    3, m_op_type);
			PreparedStatement_setInt(pre,	 4, m_speed);
			PreparedStatement_setInt(pre,	 5, m_source_type);
			PreparedStatement_setInt(pre,	 6, m_source_param);
            PreparedStatement_setString(pre, 7, sourec_type_str.c_str());
            PreparedStatement_setInt(pre,    8, m_log_time);
            PreparedStatement_execute(pre);
            }
                CATCH(SQLException)
            {
                log_error("user[%lu] insert role_buff log[%d] to db error: %s", m_user_uid, m_source_type, Exception_frame.message);
            }
            FINALLY
            {
            }
            END_TRY;
    }

private:
    uint64_t            m_user_uid;
    uint32_t            m_buff_id;
    uint32_t            m_op_type;
	uint32_t            m_speed;
	uint32_t            m_source_type;
	uint32_t            m_source_param;
    uint32_t            m_log_time;
};
typedef boost::shared_ptr<role_buff_log_t> role_buff_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class role_buff_log_manager_t
{
public:
    void add_log(role_buff_log_ptr log_ptr)
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
    typedef std::vector<role_buff_log_ptr> role_buff_log_vec_type;
	role_buff_log_vec_type m_logs_list;
};

#endif
