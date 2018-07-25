#ifndef __LOG_HERO_LOG_HPP__
#define __LOG_HERO_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class hero_log_t
{
 public:
	 hero_log_t( uint64_t user_uid, uint32_t role_level, uint64_t hero_uid, uint32_t hero_tid, uint32_t op_type, 
                 uint32_t source_type, uint32_t source_param, uint32_t log_time, uint32_t old_num, uint32_t up_num, uint32_t new_num)
        : m_user_uid(user_uid)
		, m_role_level(role_level)
        , m_hero_uid(hero_uid)
        , m_hero_tid(hero_tid)
        , m_op_type(op_type)
        , m_source_type(source_type)
		, m_source_param(source_param)
		, m_old_num(old_num)
		, m_up_num(up_num)
		, m_new_num(new_num)
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
                "insert into `hero_log` set \
                `role_uid`=?, \
                `role_level`=?, \
                `hero_uid`=?, \
                `hero_tid`=?, \
                `op_type` =?, \
                `source_type`=?, \
                `source_param`=?, \
                `source_type_desc`=?, \
				`old_num`=?, \
				`up_num`=?, \
				`new_num`=?, \
                `create_time`=from_unixtime(?);");
			PreparedStatement_setLLong(pre,  1, m_user_uid);
			PreparedStatement_setInt(pre,    2, m_role_level);
            PreparedStatement_setLLong(pre,  3, m_hero_uid);
            PreparedStatement_setInt(pre,    4, m_hero_tid);
            PreparedStatement_setInt(pre,    5, m_op_type);
			PreparedStatement_setInt(pre,    6, m_source_type);
			PreparedStatement_setInt(pre,    7, m_source_param);
			PreparedStatement_setString(pre, 8, sourec_type_str.c_str());
			PreparedStatement_setInt(pre,	 9, m_old_num);
			PreparedStatement_setInt(pre,	 10, m_up_num);
			PreparedStatement_setInt(pre,	 11, m_new_num);
            PreparedStatement_setInt(pre,    12, m_log_time);
            PreparedStatement_execute(pre);
            }
                CATCH(SQLException)
            {
                log_error("user[%lu] insert hero log[%d] to db error: %s", m_user_uid, m_source_type, Exception_frame.message);
            }
            FINALLY
            {
            }
            END_TRY;
    }

private:
    uint64_t            m_user_uid;
    uint64_t            m_hero_uid;
	uint32_t			m_role_level;
    uint32_t            m_hero_tid;
    uint32_t            m_op_type;
	uint32_t            m_source_type;
	uint32_t            m_source_param;
	uint32_t			m_old_num;
	uint32_t			m_up_num;
	uint32_t			m_new_num;
    uint32_t            m_log_time;
};
typedef boost::shared_ptr<hero_log_t> hero_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class hero_log_manager_t
{
public:
    void add_log(hero_log_ptr log_ptr)
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
    typedef std::vector<hero_log_ptr> hero_log_vec_type;
	hero_log_vec_type m_logs_list;
};

#endif
