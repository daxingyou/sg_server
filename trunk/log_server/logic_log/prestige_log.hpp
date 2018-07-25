#ifndef __LOG_PRESTIGE_LOG_HPP__
#define __LOG_PRESTIGE_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class prestige_log_t
{
 public:
	 prestige_log_t(uint64_t role_uid, uint32_t role_level, uint32_t old_prestige, uint32_t old_prestige_total, uint32_t old_prestige_level,
					uint32_t up_prestige, uint32_t new_prestige, uint32_t new_prestige_total, uint32_t new_prestige_level,
					uint32_t op_type, uint32_t source_type, uint32_t source_param, uint32_t log_time)
        : m_role_uid(role_uid)
		, m_role_level(role_level)
        , m_old_prestige(old_prestige)
        , m_old_prestige_total(old_prestige_total)
		, m_old_prestige_level(old_prestige_level)
        , m_up_prestige(up_prestige)
		, m_new_prestige(new_prestige)
		, m_new_prestige_total(new_prestige_total)
		, m_new_prestige_level(new_prestige_level)
        , m_op_type(op_type)
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
                log_error("[wys] can not find a description of the source_type[%d]!", m_source_type);
                return;
            }

            PreparedStatement_T pre = Connection_prepareStatement(conn,
                "insert into `prestige_log` set \
                `role_uid`=?, \
                `role_level`=?, \
                `old_prestige` =?, \
                `old_prestige_total`  =?, \
                `old_prestige_level` =?, \
				`up_prestige` =?, \
				`new_prestige` =?, \
                `new_prestige_total`  =?, \
                `new_prestige_level` =?, \
                `op_type` =?, \
                `source_type`=?, \
                `source_param`=?, \
                `source_type_desc`=?, \
                `create_time`=from_unixtime(?);");
			PreparedStatement_setLLong(pre,  1, m_role_uid);
			PreparedStatement_setInt(pre,	 2, m_role_level);
            PreparedStatement_setInt(pre,    3, m_old_prestige);
            PreparedStatement_setInt(pre,    4, m_old_prestige_total);
			PreparedStatement_setInt(pre,	 5, m_old_prestige_level);
			PreparedStatement_setInt(pre,	 6, m_up_prestige);
            PreparedStatement_setInt(pre,    7, m_new_prestige);
			PreparedStatement_setInt(pre,	 8, m_new_prestige_total);
			PreparedStatement_setInt(pre,	 9, m_new_prestige_level);
			PreparedStatement_setInt(pre,	 10, m_op_type);
			PreparedStatement_setInt(pre,	 11, m_source_type);
			PreparedStatement_setInt(pre,	 12, m_source_param);
            PreparedStatement_setString(pre, 13, sourec_type_str.c_str());
            PreparedStatement_setInt(pre,    14, m_log_time);
            PreparedStatement_execute(pre);
            }
                CATCH(SQLException)
            {
                log_error("[fangjian] user[%lu] insert prestige log[%d] to db error: %s", m_role_uid, m_source_type, Exception_frame.message);
            }
            FINALLY
            {
            }
            END_TRY;
    }

private:
    uint64_t            m_role_uid;
	uint32_t			m_role_level;
    uint32_t            m_old_prestige;
	uint32_t            m_old_prestige_total;
	uint32_t            m_old_prestige_level;
    uint32_t            m_up_prestige;
	uint32_t            m_new_prestige;
	uint32_t            m_new_prestige_total;
	uint32_t            m_new_prestige_level;
	uint32_t            m_op_type;
	uint32_t            m_source_type;
	uint32_t            m_source_param;
    uint32_t            m_log_time;
};
typedef boost::shared_ptr<prestige_log_t> prestige_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class prestige_log_manager_t
{
public:
    void add_log(prestige_log_ptr log_ptr)
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
    typedef std::vector<prestige_log_ptr> prestige_log_vec_type;
	prestige_log_vec_type m_logs_list;
};

#endif
