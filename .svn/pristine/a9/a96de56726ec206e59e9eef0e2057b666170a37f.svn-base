#ifndef __LOG_COUNTRY_LOG_HPP__
#define __LOG_COUNTRY_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class country_log_t
{
 public:
    country_log_t( uint32_t country_id,
                  uint32_t source_type,
				  uint64_t param1,
				  uint32_t param2,
				  uint32_t param3,
                  uint32_t log_time)
        : m_country_id(country_id)
        , m_source_type(source_type)
		, m_source_param(param1)
		, m_source_param2(param2)
		, m_source_param3(param3)
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
                "insert into `country_log` set \
                `country_id`=?, \
                `source_type`=?, \
				`source_param1`=?, \
				`source_param2`=?, \
				`source_param3`=?, \
                `create_time`=from_unixtime(?), \
                `source_type_desc`=?;");
			PreparedStatement_setInt(pre, 1, m_country_id);
			PreparedStatement_setInt(pre, 2, m_source_type);
			PreparedStatement_setLLong(pre, 3, m_source_param);
			PreparedStatement_setInt(pre, 4, m_source_param2);
			PreparedStatement_setInt(pre, 5, m_source_param3);
            PreparedStatement_setInt(pre, 6, m_log_time);
			PreparedStatement_setString(pre, 7, sourec_type_str.c_str());
            PreparedStatement_execute(pre);
        }
        CATCH(SQLException)
        {
            log_error("country[%u] insert country log to db error: %s", m_country_id, Exception_frame.message);
        }
        FINALLY
        {
        }
        END_TRY;
    }

private:
    uint32_t            m_country_id;
	uint32_t			m_source_type;
	uint64_t			m_source_param;
	uint32_t			m_source_param2;
	uint32_t			m_source_param3;
	uint32_t            m_log_time;
};
typedef boost::shared_ptr<country_log_t> country_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class country_log_manager_t
{
public:
    void add_log(country_log_ptr log_ptr) 
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
    typedef std::vector<country_log_ptr> country_log_vec_type;
    country_log_vec_type m_logs_list;
};

#endif
