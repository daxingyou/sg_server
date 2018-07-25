#ifndef __LOG_GOODS_LOG_HPP__
#define __LOG_GOODS_LOG_HPP__

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"
#include "common.pb.h"

class goods_log_t
{

 public:
	 goods_log_t( uint64_t role_uid, const std::string& role_name, uint64_t goods_uid,
		uint32_t item_tid, uint32_t item_num, uint32_t price, uint32_t selled_num, 
		uint32_t profit, uint32_t goods_state, uint32_t source_type, uint32_t log_time)
        : m_role_uid(role_uid)
		, m_role_name(role_name)
        , m_goods_uid(goods_uid)
        , m_item_tid(item_tid)
        , m_item_num(item_num)
        , m_price(price)
        , m_selled_num(selled_num)
        , m_profit(profit)
        , m_source_type(source_type)
		, m_goods_state(goods_state)
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
                "insert into `goods_log` set \
                `role_uid`=?, \
                `role_name`=?, \
                `goods_uid`=?, \
                `item_tid`=?, \
                `item_num` =?, \
                `price`  =?, \
                `selled_num` =?, \
                `profit` =?, \
				`goods_state`=?, \
                `source_type`=?, \
                `source_type_desc`=?, \
                `create_time`=from_unixtime(?);");
			PreparedStatement_setLLong(pre,  1, m_role_uid);
			PreparedStatement_setString(pre, 2, m_role_name.c_str());
            PreparedStatement_setLLong(pre,  3, m_goods_uid);
            PreparedStatement_setInt(pre,    4, m_item_tid);
            PreparedStatement_setInt(pre,    5, m_item_num);
            PreparedStatement_setInt(pre,    6, m_price);
            PreparedStatement_setInt(pre,    7, m_selled_num);
            PreparedStatement_setInt(pre,    8, m_profit);
			PreparedStatement_setInt(pre,	 9, m_goods_state);
			PreparedStatement_setInt(pre,	 10, m_source_type);
            PreparedStatement_setString(pre, 11, sourec_type_str.c_str());
            PreparedStatement_setInt(pre,    12, m_log_time);
            PreparedStatement_execute(pre);
            }
                CATCH(SQLException)
            {
                log_error("user[%lu] insert goods log[%d] to db error: %s", m_role_uid, m_source_type, Exception_frame.message);
            }
            FINALLY
            {
            }
            END_TRY;
    }

private:
    uint64_t            m_role_uid;
    std::string         m_role_name;
	uint64_t			m_goods_uid;
    uint32_t            m_item_tid;
    uint32_t            m_item_num;
    uint32_t            m_price;
    uint32_t            m_selled_num;
    uint32_t            m_profit;
	uint32_t            m_source_type;
	uint32_t            m_goods_state;
    uint32_t            m_log_time;
};
typedef boost::shared_ptr<goods_log_t> goods_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class goods_log_manager_t
{
public:
    void add_log(goods_log_ptr log_ptr)
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
    typedef std::vector<goods_log_ptr> goods_log_vec_type;
    goods_log_vec_type m_logs_list;
};

#endif
