#ifndef __LOG_FAMILY_LOG_HPP__
#define __LOG_FAMILY_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class family_log_t
{
 public:
    family_log_t( uint64_t family_uid, 
		          uint64_t role_uid,
		          uint64_t target_uid,
                  uint32_t source_type,
		          uint32_t param,
				  uint32_t param2,
                  uint32_t log_time)
        : m_family_uid(family_uid)
        , m_role_uid(role_uid)
        , m_target_uid(target_uid)
        , m_source_type(source_type)
		, m_source_param(param)
		, m_source_param2(param2)
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
                "insert into `family_log` set \
                `family_uid`=?, \
				`role_uid`=?, \
				`target_uid`=?, \
                `source_type`=?, \
				`source_param`=?, \
				`source_param2`=?, \
                `create_time`=from_unixtime(?), \
                `source_type_desc`=?;");
			PreparedStatement_setLLong(pre, 1, m_family_uid);
			PreparedStatement_setLLong(pre, 2, m_role_uid);
			PreparedStatement_setLLong(pre,	3, m_target_uid);
			PreparedStatement_setInt(pre,	4, m_source_type);
			PreparedStatement_setInt(pre, 5, m_source_param);
			PreparedStatement_setInt(pre, 6, m_source_param2);
			PreparedStatement_setInt(pre, 7, m_log_time);
			PreparedStatement_setString(pre, 8, sourec_type_str.c_str());
            PreparedStatement_execute(pre);
        }
        CATCH(SQLException)
        {
            log_error("family[%lu] insert log to db error: %s", m_family_uid, Exception_frame.message);
        }
        FINALLY
        {
        }
        END_TRY;
    }

private:
	uint64_t            m_family_uid;
    uint64_t            m_role_uid;
	uint64_t            m_target_uid;
	uint32_t			m_source_type;
	uint32_t			m_source_param;
	uint32_t			m_source_param2;
	uint32_t            m_log_time;
};
typedef boost::shared_ptr<family_log_t> family_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class family_log_manager_t
{
public:
    void add_log(family_log_ptr log_ptr) 
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
    typedef std::vector<family_log_ptr> family_log_vec_type;
    family_log_vec_type m_logs_list;
};

#endif
