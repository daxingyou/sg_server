#ifndef __LOG_FRIEND_LOG_HPP__
#define __LOG_FRIEND_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class friend_log_t
{
 public:
    friend_log_t( uint64_t role_uid,
				uint64_t friend_uid,
				uint32_t old_type,
				uint32_t new_type,
                  uint32_t source_type,
                  uint32_t log_time,
                  uint32_t param)
        : m_role_uid(role_uid)
		, m_friend_uid(friend_uid)
		, m_old_type(old_type)
		, m_new_type(new_type)
        , m_source_type(source_type)
        , m_log_time(log_time)
        , m_source_param(param)
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
                "insert into `friend_log` set \
                `role_uid`=?, \
			   `friend_uid`=?, \
			   `old_type`=?, \
			   `new_type`=?, \
                `source_type`=?, \
			  `source_param`=?, \
                `create_time`=from_unixtime(?), \
                `source_type_desc`=?;");
			PreparedStatement_setLLong(pre,	1, m_role_uid);
			PreparedStatement_setLLong(pre, 2, m_friend_uid);
			PreparedStatement_setInt(pre, 3, m_old_type);
			PreparedStatement_setInt(pre, 4, m_new_type);
			PreparedStatement_setInt(pre,	5, m_source_type);
			PreparedStatement_setInt(pre, 6, m_source_param);
            PreparedStatement_setInt(pre,	7, m_log_time);
			PreparedStatement_setString(pre,8, sourec_type_str.c_str());
            PreparedStatement_execute(pre);
        }
        CATCH(SQLException)
        {
            log_error("user[%lu] insert friend log to db error: %s", m_role_uid, Exception_frame.message);
        }
        FINALLY
        {
        }
        END_TRY;
    }

private:
    uint64_t            m_role_uid;
	uint64_t            m_friend_uid;
	uint32_t				m_old_type;
	uint32_t				m_new_type;
	uint32_t			m_source_type;
	uint32_t			m_source_param;
    uint32_t            m_log_time;
};
typedef boost::shared_ptr<friend_log_t> friend_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class friend_log_manager_t
{
public:
    void add_log(friend_log_ptr log_ptr) 
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
    typedef std::vector<friend_log_ptr> friend_log_vec_type;
    friend_log_vec_type m_logs_list;
};

#endif
