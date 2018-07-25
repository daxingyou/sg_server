#ifndef __LUCK_GIFT_LOG_HPP__
#define __LUCK_GIFT_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"

class luck_gift_log_t
{
 public:
	 luck_gift_log_t( uint64_t src_role_uid, uint64_t grab_role_uid, uint64_t luck_gift_uid, uint32_t old_num, uint32_t up_num, uint32_t new_num, uint32_t log_time)
        : m_src_role_uid(src_role_uid)
		, m_grab_role_uid(grab_role_uid)
		, m_luck_gift_uid(luck_gift_uid)
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
            PreparedStatement_T pre = Connection_prepareStatement(conn,
                "insert into `luck_gift_log` set \
                `src_role_uid`=?, \
                `grab_role_uid`=?, \
                `luck_gift_uid`=?, \
                `old_num` =?, \
                `up_num`  =?, \
                `new_num` =?, \
                `create_time`=from_unixtime(?);");
			PreparedStatement_setLLong(pre,  1, m_src_role_uid);
			PreparedStatement_setLLong(pre,  2, m_grab_role_uid);
            PreparedStatement_setLLong(pre,  3, m_luck_gift_uid);
            PreparedStatement_setInt(pre,    4, m_old_num);
            PreparedStatement_setInt(pre,    5, m_up_num);
            PreparedStatement_setInt(pre,    6, m_new_num);
            PreparedStatement_setInt(pre,    7, m_log_time);
            PreparedStatement_execute(pre);
            }
                CATCH(SQLException)
            {
                log_error("insert luck_gift log to db error: %s", Exception_frame.message);
            }
            FINALLY
            {
            }
            END_TRY;
    }

private:
	uint32_t m_src_role_uid;
	uint32_t m_grab_role_uid;
	uint32_t m_luck_gift_uid;
	uint32_t m_old_num;
	uint32_t m_up_num;
	uint32_t m_new_num;
	uint32_t m_log_time;
};
typedef boost::shared_ptr<luck_gift_log_t> luck_gift_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class luck_gift_log_manager_t
{
public:
    void add_log(luck_gift_log_ptr log_ptr)
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
    typedef std::vector<luck_gift_log_ptr> luck_gift_log_vec_type;
	luck_gift_log_vec_type m_logs_list;
};

#endif
