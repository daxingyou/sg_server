#ifndef __LOG_REDBAG_LOG_HPP__
#define __LOG_REDBAG_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"
#include "common.pb.h"

class redbag_log_t
{

 public:
	 redbag_log_t( uint64_t role_uid, const std::string& role_name, uint64_t redbag_uid, uint32_t total_count, uint32_t total_money, 
		 uint32_t draw_money, uint32_t rest_count, uint32_t rest_money, uint32_t source_type, uint32_t log_time)
        : m_role_uid(role_uid)
		, m_role_name(role_name)
        , m_redbag_uid(redbag_uid)
        , m_total_count(total_count)
        , m_total_money(total_money)
		, m_draw_money(draw_money)
        , m_rest_count(rest_count)
        , m_rest_money(rest_money)
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
                "insert into `redbag_log` set \
                `role_uid`=?, \
                `role_name`=?, \
                `redbag_uid`=?, \
                `total_count`=?, \
                `total_money` =?, \
				`draw_money` =?, \
                `rest_count`  =?, \
                `rest_money` =?, \
                `source_type`=?, \
                `source_type_desc`=?, \
                `create_time`=from_unixtime(?);");
			PreparedStatement_setLLong(pre,  1, m_role_uid);
			PreparedStatement_setString(pre, 2, m_role_name.c_str());
            PreparedStatement_setLLong(pre,  3, m_redbag_uid);
            PreparedStatement_setInt(pre,    4, m_total_count);
            PreparedStatement_setInt(pre,    5, m_total_money);
			PreparedStatement_setInt(pre,	 6, m_draw_money);
            PreparedStatement_setInt(pre,    7, m_rest_count);
            PreparedStatement_setInt(pre,    8, m_rest_money);
			PreparedStatement_setInt(pre,	 9, m_source_type);
            PreparedStatement_setString(pre, 10, sourec_type_str.c_str());
            PreparedStatement_setInt(pre,    11, m_log_time);
            PreparedStatement_execute(pre);
            }
                CATCH(SQLException)
            {
                log_error("user[%lu] insert redbag log[%d] to db error: %s", m_role_uid, m_source_type, Exception_frame.message);
            }
            FINALLY
            {
            }
            END_TRY;
    }

private:
    uint64_t            m_role_uid;
    std::string         m_role_name;
	uint64_t			m_redbag_uid;
    uint32_t            m_total_count;
    uint32_t            m_total_money;
	uint32_t            m_draw_money;
    uint32_t            m_rest_count;
    uint32_t            m_rest_money;
	uint32_t            m_source_type;
    uint32_t            m_log_time;
};
typedef boost::shared_ptr<redbag_log_t> redbag_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class redbag_log_manager_t
{
public:
    void add_log(redbag_log_ptr log_ptr)
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
    typedef std::vector<redbag_log_ptr> redbag_log_vec_type;
	redbag_log_vec_type m_logs_list;
};

#endif
