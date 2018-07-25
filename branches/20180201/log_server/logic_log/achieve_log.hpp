#ifndef __LOG_ACHIEVE_LOG_HPP__
#define __LOG_ACHIEVE_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class achieve_log_t
{
public:
	achieve_log_t(uint64_t role_uid, uint32_t achieve_id, uint32_t old_level,
		uint32_t up_level, uint32_t new_level,uint32_t old_count,uint32_t add_count,
		uint32_t new_count, uint32_t old_achieve_point, uint32_t add_achieve_point, 
		uint32_t new_achieve_point, uint32_t old_achieve_level, uint32_t up_achieve_level, 
		uint32_t new_achieve_level,uint32_t old_reward_get_level,uint32_t new_reward_get_level, 
		uint32_t source_type, uint32_t source_param,uint32_t log_time,const std::string& sourec_type_str)
		: m_role_uid(role_uid)
		, m_achieve_id(achieve_id)
		, m_old_level(old_level)
		, m_up_level(up_level)
		, m_new_level(new_level)
		, m_old_count(old_count)
		, m_add_count(add_count)
		, m_new_count(new_count)
		, m_old_achieve_point(old_achieve_point)
		, m_add_achieve_point(add_achieve_point)
		, m_new_achieve_point(new_achieve_point)
		, m_old_achieve_level(old_achieve_level)
		, m_up_achieve_level(up_achieve_level)
		, m_new_achieve_level(new_achieve_level)
		, m_old_reward_get_level(old_reward_get_level)
		, m_new_reward_get_level(new_reward_get_level)
		, m_source_type(source_type)
		, m_source_param(source_param)
		, m_log_time(log_time)
		, m_sourec_type_str(sourec_type_str)
	{
	}

	void update_to_db(const Connection_T& conn)
	{
		TRY
		{
			if (m_source_type == log_enum::source_type_achieve_begin)
			{
				std::string sourec_type_str;
				if (!source_type_converter_t::find_desc(m_source_type, sourec_type_str))
				{
					log_error("can not find a description of the source_type[%d]!", m_source_type);
					return;
				}
				m_sourec_type_str = sourec_type_str;
			}
			

		PreparedStatement_T pre = Connection_prepareStatement(conn,
			"insert into `achieve_log` set \
				`role_uid`=?, \
                `achieve_id`=?, \
			   `old_level`=?, \
			   `up_level`=?, \
			   `new_level`=?, \
			   `old_count`=?, \
			   `add_count`=?, \
			   `new_count`=?, \
			`old_achieve_point`= ? , \
			`add_achieve_point`= ? , \
			`new_achieve_point`= ? , \
			`old_achieve_level`= ? , \
			`up_achieve_level`= ? , \
			`new_achieve_level`= ? , \
			`old_reward_get_level`= ? , \
			`new_reward_get_level`= ? , \
               `source_type`=?, \
			  `source_param`=?, \
                `create_time`=from_unixtime(?), \
                `source_type_desc`=?;");
		PreparedStatement_setLLong(pre,	1, m_role_uid);
		PreparedStatement_setInt(pre, 2, m_achieve_id);
		PreparedStatement_setInt(pre, 3, m_old_level);
		PreparedStatement_setInt(pre, 4, m_up_level);
		PreparedStatement_setInt(pre, 5, m_new_level);
		PreparedStatement_setInt(pre, 6, m_old_count);
		PreparedStatement_setInt(pre, 7, m_add_count);
		PreparedStatement_setInt(pre, 8, m_new_count);
		PreparedStatement_setInt(pre, 9, m_old_achieve_point);
		PreparedStatement_setInt(pre, 10, m_add_achieve_point);
		PreparedStatement_setInt(pre, 11, m_new_achieve_point);
		PreparedStatement_setInt(pre, 12, m_old_achieve_level);
		PreparedStatement_setInt(pre, 13, m_up_achieve_level);
		PreparedStatement_setInt(pre, 14, m_new_achieve_level);
		PreparedStatement_setInt(pre, 15, m_old_reward_get_level);
		PreparedStatement_setInt(pre, 16, m_new_reward_get_level);
		PreparedStatement_setInt(pre, 17, m_source_type);
		PreparedStatement_setInt(pre, 18, m_source_param);
		PreparedStatement_setInt(pre, 19, m_log_time);
		PreparedStatement_setString(pre, 20, m_sourec_type_str.c_str());
		PreparedStatement_execute(pre);
		}
			CATCH(SQLException)
		{
			log_error("user[%lu] insert achieve log to db error: %s", m_role_uid, Exception_frame.message);
		}
		FINALLY
		{
		}
		END_TRY;
	}

private:
	uint64_t            m_role_uid;
	uint32_t			m_achieve_id;
	uint32_t			m_old_level;
	uint32_t			m_up_level;
	uint32_t			m_new_level;
	uint32_t			m_old_count;
	uint32_t			m_add_count;
	uint32_t			m_new_count;
	uint32_t			m_old_achieve_point;
	uint32_t			m_add_achieve_point;
	uint32_t			m_new_achieve_point;
	uint32_t			m_old_achieve_level;
	uint32_t			m_up_achieve_level;
	uint32_t			m_new_achieve_level;
	uint32_t			m_old_reward_get_level;
	uint32_t			m_new_reward_get_level;
	uint32_t			m_source_type;
	uint32_t			m_source_param;
	uint32_t            m_log_time;
	std::string			m_sourec_type_str;
};
typedef boost::shared_ptr<achieve_log_t> achieve_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class achieve_log_manager_t
{
public:
	void add_log(achieve_log_ptr log_ptr)
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
	typedef std::vector<achieve_log_ptr> achieve_log_vec_type;
	achieve_log_vec_type m_logs_list;
};

#endif	//__LOG_ACHIEVE_LOG_HPP__
