#include "global_id.hpp"
#include "utility.hpp"
#include "log.hpp"
#include <boost/format.hpp>
#include "utility.hpp"


NS_COMMON_BEGIN

id_allocator_t::id_allocator_t()
{

}

id_allocator_t::~id_allocator_t()
{

}

bool id_allocator_t::init(uint32_t srv_group_id, uint32_t srv_idx, std::string start_time, bool is_global_server)
{
    /*
    srv_id(11B): support the max number of server_id is 2047
    srv_index(gate\game 3B): support the max number of server_index is 7
    */
    m_srv_group_id = srv_group_id;
    m_srv_index = srv_idx;
    m_seq = 0;

	start_time += " 00:00:00";

    if (is_global_server)
    {
        if (srv_group_id <= 1500 || srv_group_id > 2047)
        {
            log_error("id allocator error, server id is [1501,2047], now is [%d]!", m_srv_group_id);
            return false;
        }
    }
    else
    {
        if (srv_group_id == 0 || srv_group_id > 1500)
        {
            log_error("id allocator error, server id is [0,1500], now is [%d]!", m_srv_group_id);
            return false;
        }
    }

    if (m_srv_index > 7)
    {
        log_error("id allocator,max server index is [%d], now is [%d]", 7, m_srv_index);
        return false;
    }

    m_start_time = time_util_t::FormatTime_to_time_t(start_time);
    if (m_start_time == 0)
    {
        log_error("m_start_time[%d] <= 0", m_start_time);
        return false;
    }

    uint32_t cur_time = time_util_t::now_time();
    if (cur_time <= m_start_time)
    {
        log_error("cur_time[%d] <= m_start_time[%d]", cur_time, m_start_time);
        return false;
    }

    m_is_init = true;

    log_info("init global id success!");
    return true;
}

bool id_allocator_t::is_init()
{
    return m_is_init;
}

#if 0
uint64_t id_allocator_t::gen_new_id(ID_ALLOCATOR_TYPE type)
{
    /*
    type(5B) + srv_id(3B) + srv_index(2B) + time(27B) + seq(15B) = 52B
    type(5B): support max allocator type num is 31
    srv_id(11B): support the max number of server_id is 2047
    srv_index(gate\game 3B): support the max number of server_index is 7
    time(27B): support the max number of years is 4 years
    seq(15): support the max number of allocate num per second is 32767
    */

    if (type >= ID_ALLOCATOR_TYPE_MAX)
    {
        log_error("type[%d] error!", type);
        return 0;
    }

    if (!is_init())
    {
        log_error("id allocator is not init");
        return 0;
    }

    uint64_t id = 0;
    time_t cur_time = time(0);

    ++m_seq;
    if (m_seq >= 0x7FFF)
    {
        m_seq = 0;
    }

    if (cur_time < m_start_time)
    {
        return 0;
    }

    id |= ((uint64_t)(0x1F & type) << 47);
    id |= ((uint64_t)(0x7 & m_srv_group_id) << 44);
    id |= ((uint64_t)(0x3 & m_srv_index) << 42);
    id |= ((uint64_t)(0x7FFFFFF & (cur_time - m_start_time)) << 15);
    id |= ((uint64_t)(0x7FFF) & m_seq);

    return id;
}
#endif

uint64_t id_allocator_t::gen_new_id(ID_ALLOCATOR_TYPE type)
{
    /*
    type(5B) + srv_type(2B) + srv_id(11B) + srv_index(3B) + time(27B) + seq(15B) = 61B
    type(5B): support max allocator type num is 31
    srv_id(11B): support the max number of server_id is 2047
    srv_index(gate\game 3B): support the max number of server_index is 7
    time(27B): support the max number of years is 4 years
    seq(15): support the max number of allocate num per second is 32767
    */

    if (type >= ID_ALLOCATOR_TYPE_MAX)
    {
        log_error("type[%d] error!", type);
        return 0;
    }

    if (!is_init())
    {
        log_error("id allocator is not init");
        return 0;
    }

    uint64_t id = 0;
    time_t cur_time = time(0);

    ++m_seq;
    if (m_seq >= 0x7FFF)
    {
        m_seq = 0;
    }

    if (cur_time < m_start_time)
    {
        log_error("cur_time[%u] < m_start_time[%u]", (uint32_t)cur_time, (uint32_t)m_start_time);
        return 0;
    }

    id |= ((uint64_t)(0x1F & type) << 56);
    id |= ((uint64_t)(0x7FF & m_srv_group_id) << 45);
    id |= ((uint64_t)(0x7 & m_srv_index) << 42);
    id |= ((uint64_t)(0x7FFFFFF & (cur_time - m_start_time)) << 15);
    id |= ((uint64_t)(0x7FFF) & m_seq);

    return id;
}
ID_ALLOCATOR_TYPE id_allocator_t::get_allocator_type(uint64_t uid)
{
    return (ID_ALLOCATOR_TYPE)((uid >> 56) & 0x1F);
}
uint32_t id_allocator_t::get_srv_group_id(uint64_t uid)
{
    return ((uid >> 45) & 0x7FF);
}
uint32_t id_allocator_t::get_srv_index(uint64_t uid)
{
    return ((uid >> 42) & 0x7);
}
uint32_t id_allocator_t::get_gen_time(uint64_t uid)
{
    return ((uid >> 15) & 0x7FFFFFF);
}
uint32_t id_allocator_t::get_seq(uint64_t uid)
{
    return ((uid)& 0x7FFF);
}

//-----------------------------------------------------------------------------------------------------------------------
uint64_t user_uid_generator::gen_user_uid(uint32_t platform, uint32_t channel_id, uint32_t srv_group_id, uint32_t role_index, uint64_t account_id)
{
    /*
    *@ platform(2B)         [0, 3]
    *@ channel_id(8B)       [0, 255]
    *@ srv_group_id(11B)    [0, 2047] 
    *@ role_index(3B)       [0, 7]
    *@ account_id(18B)      [0, 262143]
    *@ total: 42B
    */
    uint64_t uid = ((uint64_t)platform << 40) | ((uint64_t)channel_id << 32) | ((uint64_t)srv_group_id << 21) | ((uint64_t)role_index << 18) | account_id;
    return uid;
}
uint32_t user_uid_generator::get_platform(uint64_t uid)
{
    return (uint32_t)((uid >> 40) & 0x3);
}
uint32_t user_uid_generator::get_channel_id(uint64_t uid)
{
    return (uint32_t)((uid >> 32) & 0xFF);
}
uint32_t user_uid_generator::get_srv_group_id(uint64_t uid)
{
    return (uint32_t)((uid >> 21) & 0x7FF);
}
uint32_t user_uid_generator::get_role_index(uint64_t uid)
{
    return (uint32_t)((uid >> 18) & 0x7);
}
uint64_t user_uid_generator::get_account_id(uint64_t uid)
{
    return (uint64_t)((uid)& 0x3FFFF);
}

//------------------------------------------------------------------------------------------------------------
std::string user_did_generator::gen_user_did(uint64_t uid)
{
    /*
    *@ srv_group_id(11B)    [0, 2047]
    *@ account_uid(18B)     [0, 262143]
    *@ total: 29B
    */
    std::string str_did("");
    uint32_t srv_group_id = user_uid_generator::get_srv_group_id(uid);
    uint64_t account_id = user_uid_generator::get_account_id(uid);
    uint64_t did = ((uint64_t)srv_group_id << 18) | account_id;
    str_did.append((boost::format("%lu") % did).str());
    return str_did;
}
uint64_t user_did_generator::parse_user_uid_from_did(const std::string& str_did, uint32_t platform, uint32_t channel_id)
{
    uint32_t srv_group_id = get_srv_group_id(str_did);
    uint32_t account_uid = get_account_id(str_did);
    uint32_t role_index = 0;
    uint64_t uid = user_uid_generator::gen_user_uid(platform, channel_id, srv_group_id, role_index, account_uid);
    return uid;
}
uint32_t user_did_generator::get_srv_group_id(const std::string& str_did)
{
    uint64_t did = boost::lexical_cast<uint64_t>(str_did);
    return (uint32_t)((did >> 18) & 0x7FF);
}
uint64_t user_did_generator::get_account_id(const std::string& str_did)
{
    uint64_t did = boost::lexical_cast<uint64_t>(str_did);
    return (uint64_t)(did & 0x3FFFF);
}
id_allocator_t g_id_allocator;

NS_COMMON_END
