#pragma once
#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class patrol_hire_log_t
{
public:
	patrol_hire_log_t(uint64_t role_uid, uint32_t begin_time, uint32_t end_time,
		uint32_t count, uint32_t total_exp, uint32_t use_multi_point, 
		uint32_t map_id,uint32_t source_type,uint32_t log_time)
		: m_role_uid(role_uid)
		, m_begin_time(begin_time)
		, m_end_time(end_time)
		, m_count(count)
		, m_total_exp(total_exp)
		, m_use_multi_point(use_multi_point)
		, m_map_id(map_id)
		, m_source_type(source_type)
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
			"insert into `patrol_hire_log` set \
				`role_uid`=?, \
                `begin_time`=?, \
			   `end_time`=?, \
			   `count`=?, \
			   `total_exp`=?, \
			   `use_multi_point`=?, \
			   `map_id`=?, \
               `source_type`=?, \
				`create_time`=from_unixtime(?), \
                `source_type_desc`=?;");
		PreparedStatement_setLLong(pre,	1, m_role_uid);
		PreparedStatement_setInt(pre, 2, m_begin_time);
		PreparedStatement_setInt(pre, 3, m_end_time);
		PreparedStatement_setInt(pre, 4, m_count);
		PreparedStatement_setInt(pre, 5, m_total_exp);
		PreparedStatement_setInt(pre, 6, m_use_multi_point);
		PreparedStatement_setInt(pre, 7, m_map_id);
		PreparedStatement_setInt(pre, 8, m_source_type);
		PreparedStatement_setInt(pre, 9, m_log_time);
		PreparedStatement_setString(pre, 10, sourec_type_str.c_str());
		PreparedStatement_execute(pre);
		}
			CATCH(SQLException)
		{
			log_error("user[%lu] insert patrol_hire log to db error: %s", m_role_uid, Exception_frame.message);
		}
		FINALLY
		{
		}
		END_TRY;
	}

private:
	uint64_t            m_role_uid;
	uint32_t			m_begin_time;
	uint32_t			m_end_time;
	uint32_t			m_count;
	uint32_t			m_total_exp;
	uint32_t			m_use_multi_point;
	uint32_t			m_map_id;
	uint32_t			m_source_type;
	uint32_t            m_log_time;
};
typedef boost::shared_ptr<patrol_hire_log_t> patrol_hire_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class patrol_hire_log_manager_t
{
public:
	void add_log(patrol_hire_log_ptr log_ptr)
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
	typedef std::vector<patrol_hire_log_ptr> patrol_hire_log_vec_type;
	patrol_hire_log_vec_type m_logs_list;
};