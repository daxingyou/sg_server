#ifndef __LOG_LIFEFORCE_LOG_HPP__
#define __LOG_LIFEFORCE_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class lifeforce_log_t
{
public:
	lifeforce_log_t(uint64_t role_uid, uint32_t old_lifestate_level, uint32_t old_lifestar_level,
		uint32_t up_lifestate, uint32_t up_lifestar,
		uint32_t new_lifestate_level, uint32_t new_lifestar_level,
		uint32_t source_type, uint32_t source_param,
		uint32_t log_time)
		: m_role_uid(role_uid)
		, m_old_lifestate_level(old_lifestate_level)
		, m_old_lifestar_level(old_lifestar_level)
		, m_up_lifestate(up_lifestate)
		, m_up_lifestar(up_lifestar)
		, m_new_lifestate_level(new_lifestate_level)
		, m_new_lifestar_level(new_lifestar_level)
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
			"insert into `lifeforce_log` set \
				`role_uid`=?, \
                `old_lifestate_level`=?, \
			   `old_lifestar_level`=?, \
			   `up_lifestate`=?, \
			   `up_lifestar`=?, \
				`new_lifestate_level`=?, \
			   `new_lifestar_level`=?, \
                `source_type`=?, \
			  `source_param`=?, \
                `create_time`=from_unixtime(?), \
                `source_type_desc`=?;");
		PreparedStatement_setLLong(pre,	1, m_role_uid);
		PreparedStatement_setInt(pre, 2, m_old_lifestate_level);
		PreparedStatement_setInt(pre, 3, m_old_lifestar_level);
		PreparedStatement_setInt(pre, 4, m_up_lifestate);
		PreparedStatement_setInt(pre, 5, m_up_lifestar);
		PreparedStatement_setInt(pre, 6, m_new_lifestate_level);
		PreparedStatement_setInt(pre, 7, m_new_lifestar_level);
		PreparedStatement_setInt(pre, 8, m_source_type);
		PreparedStatement_setInt(pre, 9, m_source_param);
		PreparedStatement_setInt(pre, 10, m_log_time);
		PreparedStatement_setString(pre, 11, sourec_type_str.c_str());
		PreparedStatement_execute(pre);
		}
			CATCH(SQLException)
		{
			log_error("user[%lu] insert lifeforce log to db error: %s", m_role_uid, Exception_frame.message);
		}
		FINALLY
		{
		}
		END_TRY;
	}

private:
	uint64_t            m_role_uid;
	uint32_t			m_old_lifestate_level;
	uint32_t			m_old_lifestar_level;
	uint32_t			m_up_lifestate;
	uint32_t			m_up_lifestar;
	uint32_t			m_new_lifestate_level;
	uint32_t			m_new_lifestar_level;
	uint32_t			m_source_type;
	uint32_t			m_source_param;
	uint32_t            m_log_time;
};
typedef boost::shared_ptr<lifeforce_log_t> lifeforce_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class lifeforce_log_manager_t
{
public:
	void add_log(lifeforce_log_ptr log_ptr)
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
	typedef std::vector<lifeforce_log_ptr> lifeforce_log_vec_type;
	lifeforce_log_vec_type m_logs_list;
};

#endif	//__LOG_LIFEFORCE_LOG_HPP__
