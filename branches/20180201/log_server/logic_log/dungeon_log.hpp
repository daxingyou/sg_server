#ifndef __LOG_DUNEGON_LOG_HPP__
#define __LOG_DUNEGON_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class dungeon_log_t
{
public:
	dungeon_log_t(uint64_t role_uid, uint32_t dungeon_id,
		uint32_t create_type, uint64_t create_id,
		uint32_t source_type, uint32_t source_param,
		uint32_t log_time)
		: m_role_uid(role_uid)
		, m_dungeon_id(dungeon_id)
		, m_create_type(create_type)
		, m_create_id(create_id)
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
			"insert into `dungeon_log` set \
				`role_uid`=?, \
                `dungeon_id`=?, \
			   `create_type`=?, \
			   `create_id`=?, \
                `source_type`=?, \
			  `source_param`=?, \
                `create_time`=from_unixtime(?), \
                `source_type_desc`=?;");
		PreparedStatement_setLLong(pre,	1, m_role_uid);
		PreparedStatement_setInt(pre, 2, m_dungeon_id);
		PreparedStatement_setInt(pre, 3, m_create_type);
		PreparedStatement_setLLong(pre, 4, m_create_id);
		PreparedStatement_setInt(pre, 5, m_source_type);
		PreparedStatement_setInt(pre, 6, m_source_param);
		PreparedStatement_setInt(pre, 7, m_log_time);
		PreparedStatement_setString(pre, 8, sourec_type_str.c_str());
		PreparedStatement_execute(pre);
		}
			CATCH(SQLException)
		{
			log_error("user[%lu] insert dungeon log to db error: %s", m_role_uid, Exception_frame.message);
		}
		FINALLY
		{
		}
		END_TRY;
	}

private:
	uint64_t            m_role_uid;
	uint32_t			m_dungeon_id;
	uint32_t			m_create_type;
	uint64_t			m_create_id;
	uint32_t			m_source_type;
	uint32_t			m_source_param;
	uint32_t            m_log_time;
};
typedef boost::shared_ptr<dungeon_log_t> dungeon_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class dungeon_log_manager_t
{
public:
	void add_log(dungeon_log_ptr log_ptr)
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
	typedef std::vector<dungeon_log_ptr> dungeon_log_vec_type;
	dungeon_log_vec_type m_logs_list;
};

#endif
