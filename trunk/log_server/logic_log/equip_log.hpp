#ifndef __LOG_EQUIP_LOG_HPP__
#define __LOG_EQUIP_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class equip_log_t
{
 public:
	 equip_log_t( uint64_t user_uid, uint32_t	role_level,
                uint64_t item_uid, uint32_t item_tid, uint64_t hero_uid, const std::string& attr, 
				const std::string& exattr, const std::string& remake_exattr, uint32_t op_type,
                uint32_t source_type, uint32_t source_param, uint32_t log_time)
        : m_user_uid(user_uid)
		, m_role_level(role_level)
        , m_item_uid(item_uid)
        , m_item_tid(item_tid)
        , m_hero_uid(hero_uid)
        , m_attr(attr)
        , m_exattr(exattr)
		, m_remake_exattr(remake_exattr)
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
                log_error("can not find a description of the source_type[%d]!", m_source_type);
                return;
            }

            PreparedStatement_T pre = Connection_prepareStatement(conn,
                "insert into `equip_log` set \
                `role_uid`=?, \
                `role_level`=?, \
                `item_uid`=?, \
                `item_tid`=?, \
                `hero_uid` =?, \
                `attr`  =?, \
                `exattr`  =?, \
				`remake_exattr`=?, \
                `op_type` =?, \
                `source_type`=?, \
                `source_param`=?, \
                `source_type_desc`=?, \
                `create_time`=from_unixtime(?);");
			PreparedStatement_setLLong(pre,  1, m_user_uid);
			PreparedStatement_setLLong(pre,  2, m_role_level);
            PreparedStatement_setLLong(pre,  3, m_item_uid);
            PreparedStatement_setInt(pre,    4, m_item_tid);
			PreparedStatement_setLLong(pre,  5, m_hero_uid);
			PreparedStatement_setString(pre, 6, m_attr.c_str());
			PreparedStatement_setString(pre, 7, m_exattr.c_str());
			PreparedStatement_setString(pre, 8, m_remake_exattr.c_str());
            PreparedStatement_setInt(pre,    9, m_op_type);
			PreparedStatement_setInt(pre,	 10, m_source_type);
			PreparedStatement_setInt(pre,	 11, m_source_param);
            PreparedStatement_setString(pre, 12, sourec_type_str.c_str());
            PreparedStatement_setInt(pre,    13, m_log_time);
            PreparedStatement_execute(pre);
            }
                CATCH(SQLException)
            {
                log_error("user[%lu] insert item log[%d] to db error: %s", m_user_uid, m_source_type, Exception_frame.message);
            }
            FINALLY
            {
            }
            END_TRY;
    }

private:
    uint64_t            m_user_uid;
    uint64_t            m_item_uid;
	uint32_t			m_role_level;
    uint32_t            m_item_tid;
	uint64_t            m_hero_uid;
    std::string         m_attr;
	std::string         m_exattr;
	std::string         m_remake_exattr;
    uint32_t            m_op_type;
	uint32_t            m_source_type;
	uint32_t            m_source_param;
    uint32_t            m_log_time;
};
typedef boost::shared_ptr<equip_log_t> equip_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class equip_log_manager_t
{
public:
    void add_log(equip_log_ptr log_ptr)
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
    typedef std::vector<equip_log_ptr> equip_log_vec_type;
	equip_log_vec_type m_logs_list;
};

#endif
