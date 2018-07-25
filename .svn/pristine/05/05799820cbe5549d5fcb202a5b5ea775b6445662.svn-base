#ifndef __LOG_MOBILE_LOG_HPP__
#define __LOG_MOBILE_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class mobile_log_t
{
public:
    mobile_log_t(const std::string& uuid,
                const std::string& device_model,
                const std::string& device_sys_version,
                const std::string& cpu_type,
                uint32_t cpu_count,
                uint32_t memory_size,
                const std::string& grph_device_name,
                uint32_t log_time)
		: m_uuid(uuid)
		, m_device_model(device_model)
        , m_device_sys_version(device_sys_version)
        , m_cpu_type(cpu_type)
		, m_cpu_count(cpu_count)
        , m_memory_size(memory_size)
        , m_grph_device_name(grph_device_name)
		, m_log_time(log_time)
	{
	}

	void update_to_db(const Connection_T& conn)
	{
		TRY
		{
            std::string log_date = time_util_t::FormatTime_to_string(m_log_time, false);
            PreparedStatement_T pre = Connection_prepareStatement(conn,
                "insert into `mobile_log` set \
                        `uuid`=?, \
                        `device_model`=?, \
                        `device_sys_version` =?, \
                        `cpu_type` =?, \
                        `cpu_count` =?, \
                        `memory_size`=?, \
                        `grph_device_name`=?, \
                        `create_time`=from_unixtime(?), \
                    `create_date`=date(?);");
            PreparedStatement_setString(pre,    1, m_uuid.c_str());
            PreparedStatement_setString(pre,    2, m_device_model.c_str());
            PreparedStatement_setString(pre,    3, m_device_sys_version.c_str());
            PreparedStatement_setString(pre,    4, m_cpu_type.c_str());
            PreparedStatement_setInt(pre,       5, m_cpu_count);
            PreparedStatement_setInt(pre,       6, m_memory_size);
            PreparedStatement_setString(pre,    7, m_grph_device_name.c_str());
            PreparedStatement_setInt(pre,       8, m_log_time);
             PreparedStatement_setString(pre,   9, log_date.c_str());
            PreparedStatement_execute(pre);
        }
        CATCH(SQLException)
		{
			log_error("mobile[%s] insert mobile log to db error: %s", m_uuid.c_str(), Exception_frame.message);
		}
		FINALLY
		{
		}
		END_TRY;
	}

private:
    std::string         m_uuid;
    std::string         m_device_model;
    std::string         m_device_sys_version;
    std::string         m_cpu_type;
    uint32_t            m_cpu_count;
    uint32_t            m_memory_size;
    std::string         m_grph_device_name;
	uint32_t            m_log_time;
};
typedef boost::shared_ptr<mobile_log_t> mobile_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class mobile_log_manager_t
{
public:
	void add_log(mobile_log_ptr log_ptr)
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
	typedef std::vector<mobile_log_ptr> mobile_log_vec_type;
    mobile_log_vec_type m_logs_list;
};

#endif