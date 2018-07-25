#ifndef __LOG_MAIL_LOG_HPP__
#define __LOG_MAIL_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class mail_log_t
{
public:
	mail_log_t(uint64_t user_uid, uint64_t mail_id, uint32_t mail_type, uint32_t item_type, const std::string& items, uint32_t item_state, uint32_t source_type, uint32_t log_time, uint32_t source_param)
		: m_user_uid(user_uid), m_mail_id(mail_id), m_mail_type(mail_type), m_item_type(item_type), m_items(items), m_item_state(item_state), m_source_type(source_type), m_log_time(log_time), m_source_param(source_param)
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
				"insert into `mail_log` set \
				`role_uid`=?, \
				`mail_id`=?, \
				`type`=?, \
				`item_type`=?, \
				`items`=?, \
				`item_state`=?, \
				`source_type`=?, \
				`source_param`=?, \
				`source_type_desc`=?, \
				`create_time`=from_unixtime(?);");
			PreparedStatement_setLLong(pre, 1, m_user_uid);
			PreparedStatement_setLLong(pre, 2, m_mail_id);
			PreparedStatement_setInt(pre, 3, m_mail_type);
			PreparedStatement_setInt(pre, 4, m_item_type);
			PreparedStatement_setString(pre, 5, m_items.c_str());
			PreparedStatement_setInt(pre, 6, m_item_state);
			PreparedStatement_setInt(pre, 7, m_source_type);
			PreparedStatement_setInt(pre, 8, m_source_param);
			PreparedStatement_setString(pre, 9, sourec_type_str.c_str());
			PreparedStatement_setInt(pre, 10, m_log_time);
			PreparedStatement_execute(pre);
		}
		CATCH(SQLException)
		{
			log_error("user[%lu] insert mail log[%d] to db error: %s", m_user_uid, m_source_type, Exception_frame.message);
		}
		FINALLY
		{
		}
		END_TRY;
	}

private:
	uint64_t			m_user_uid;
	uint64_t			m_mail_id;
	uint32_t			m_mail_type;
	uint32_t			m_item_type;
	std::string		m_items;
	uint32_t			m_item_state;
	uint32_t			m_source_type;
	uint32_t			m_source_param;
	uint32_t			m_log_time;
};
typedef boost::shared_ptr<mail_log_t> mail_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class mail_log_manager_t
{
public:
	void add_log(mail_log_ptr log_ptr)
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
	typedef std::vector<mail_log_ptr> mail_log_vec_type;
	mail_log_vec_type m_logs_list;
};

#endif
