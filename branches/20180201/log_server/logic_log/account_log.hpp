#ifndef __LOG_ACCOUNT_LOG_HPP__
#define __LOG_ACCOUNT_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"
#include "common/global_id.hpp"

class account_log_t
{
public:
    account_log_t(uint64_t account_uid,
                uint32_t channel_id,
                uint64_t role_uid,
                const std::string& mobile_uuid,
                uint32_t source_type,
                uint32_t log_time)
		: m_account_uid(account_uid)
		, m_channel_id(channel_id)
        , m_role_uid(role_uid)
		, m_mobile_uuid(mobile_uuid)
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
            std::string log_date = time_util_t::FormatTime_to_string(m_log_time, false);
            uint64_t role_did = string_util_t::string_to_uint64(user_did_generator::gen_user_did(m_role_uid));
		    PreparedStatement_T pre = Connection_prepareStatement(conn,
			    "insert into `account_log` set \
                    `account_uid`=?, \
                    `channel_id`=?, \
                    `role_uid` =?, \
                    `role_did` =?, \
                    `mobile_uuid` =?, \
                    `source_type`=?, \
                    `source_type_desc`=?, \
                    `create_time`=from_unixtime(?), \
                    `create_date`=date(?);");
		    PreparedStatement_setLLong(pre,     1, m_account_uid);
            PreparedStatement_setInt(pre,       2, m_channel_id);
            PreparedStatement_setLLong(pre,     3, m_role_uid);
            PreparedStatement_setLLong(pre,     4, role_did);
            PreparedStatement_setString(pre,    5, m_mobile_uuid.c_str());
		    PreparedStatement_setInt(pre,	    6, m_source_type);
		    PreparedStatement_setString(pre,    7, sourec_type_str.c_str());
            PreparedStatement_setInt(pre,       8, m_log_time);
            PreparedStatement_setString(pre,    9, log_date.c_str());
		    PreparedStatement_execute(pre);
		}
        CATCH(SQLException)
		{
			log_error("account[%lu] insert account log[%d] to db error: %s", m_account_uid, m_source_type, Exception_frame.message);
		}
		FINALLY
		{
		}
		END_TRY;
	}

private:
	uint64_t            m_account_uid;
	uint32_t			m_channel_id;
    uint64_t            m_role_uid;
    std::string         m_mobile_uuid;
	uint32_t            m_source_type;
	uint32_t            m_log_time;
};
typedef boost::shared_ptr<account_log_t> account_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class account_log_manager_t
{
public:
	void add_log(account_log_ptr log_ptr)
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
	typedef std::vector<account_log_ptr> account_log_vec_type;
    account_log_vec_type m_logs_list;
};

#endif