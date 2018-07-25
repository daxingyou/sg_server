#ifndef __LOG_ROLE_EXP_LOG_HPP__
#define __LOG_ROLE_EXP_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class role_exp_log_t
{
 public:
	 role_exp_log_t( uint64_t role_uid, uint32_t old_level, uint32_t old_exp, uint32_t up_exp, uint32_t new_level, 
		 uint32_t new_exp, uint32_t source_type, uint32_t source_param, uint32_t log_time)
        : m_role_uid(role_uid)
		, m_old_level(old_level)
        , m_old_exp(old_exp)
		, m_up_exp(up_exp)
		, m_new_level(new_level)
		, m_new_exp(new_exp)
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
                "insert into `role_exp_level_log` set \
                `role_uid`=?, \
                `old_level`=?, \
                `old_exp` =?, \
				`up_exp` =?, \
				`new_level`=?, \
                `new_exp` =?, \
                `source_type`=?, \
                `source_param`=?, \
                `source_type_desc`=?, \
                `create_time`=from_unixtime(?);");
			PreparedStatement_setLLong(pre,  1, m_role_uid);
			PreparedStatement_setInt(pre,	 2, m_old_level);
			PreparedStatement_setInt(pre,	 3,  m_old_exp);
            PreparedStatement_setInt(pre,    4, m_up_exp);
			PreparedStatement_setInt(pre,	 5, m_new_level);
			PreparedStatement_setInt(pre,	 6, m_new_exp);
			PreparedStatement_setInt(pre,	 7, m_source_type);
			PreparedStatement_setInt(pre,	 8, m_source_param);
            PreparedStatement_setString(pre, 9, sourec_type_str.c_str());
            PreparedStatement_setInt(pre,    10, m_log_time);
            PreparedStatement_execute(pre);
            }
                CATCH(SQLException)
            {
                log_error("role[%lu] insert role_exp log[%d] to db error: %s", m_role_uid, m_source_type, Exception_frame.message);
            }
            FINALLY
            {
            }
            END_TRY;
    }

private:
	uint64_t            m_role_uid;
	uint32_t            m_old_level;
	uint32_t            m_old_exp;
	uint32_t            m_up_exp;
	uint32_t            m_new_level;
	uint32_t            m_new_exp;
	uint32_t            m_source_type;
	uint32_t            m_source_param;
	uint32_t            m_log_time;
};

typedef boost::shared_ptr<role_exp_log_t> role_exp_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class role_exp_log_manager_t
{
public:
    void add_log(role_exp_log_ptr log_ptr)
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
    typedef std::vector<role_exp_log_ptr> role_exp_log_vec_type;
	role_exp_log_vec_type m_logs_list;
};

#endif
