#ifndef __COMMON_GLOBAL_ID_HPP__
#define __COMMON_GLOBAL_ID_HPP__

#include "common_fwd.hpp"

NS_COMMON_BEGIN

enum ID_ALLOCATOR_TYPE
{
	ID_ALLOCATOR_TYPE_ROLE			= 0,
    ID_ALLOCATOR_TYPE_ITEM          = 1,
	ID_ALLOCATOR_TYPE_HERO          = 2,
	ID_ALLOCATOR_TYPE_FIGHT			= 3,
	ID_ALLOCATOR_TYPE_LUCK_GIFT		= 4,
	ID_ALLOCATOR_TYPE_MONSTER		= 6,
	ID_ALLOCATOR_TYPE_TASK			= 7,
	ID_ALLOCATOR_TYPE_MAIL			= 8,
	ID_ALLOCATOR_TYPE_COLLECT_POINT = 9,
	ID_ALLOCATOR_TYPE_CHALLENGE		= 10,
	ID_ALLOCATOR_TYPE_TROOP			= 11,
	ID_ALLOCATOR_TYPE_ACHIEVE	    = 12,
	ID_ALLOCATOR_TYPE_WORLD_CUP		= 13,
	ID_ALLOCATOR_TYPE_BET			= 14,
	//ID_ALLOCATOR_TYPE_SCENE			= 15,
	ID_ALLOCATOR_TYPE_GOODS			= 16,
    ID_ALLOCATOR_TYPE_GOODS_CARE    = 17,
	ID_ALLOCATOR_TYPE_FAMILY		= 18,
	ID_ALLOCATOR_TYPE_ACTIVE_REWARD = 19,
	ID_ALLOCATOR_TYPE_RECHARGE		= 20,
	ID_ALLOCATOR_TYPE_TOWER_TRIGGER = 21,
	ID_ALLOCATOR_TYPE_REDBAG		= 22,
	ID_ALLOCATOR_TYPE_PARTNER		= 23,
	ID_ALLOCATOR_TYPE_EQUIP_PLAN	= 24,
    ID_ALLOCATOR_TYPE_MAX           = 30,
};

/*DONOT support multi-thread*/
class id_allocator_t
{
public:
    id_allocator_t();
    ~id_allocator_t();

    bool init(uint32_t srv_group_id, uint32_t srv_idx, std::string start_time, bool is_global_server);
    uint64_t gen_new_id(ID_ALLOCATOR_TYPE type);

    static ID_ALLOCATOR_TYPE get_allocator_type(uint64_t uid);
    static uint32_t get_srv_group_id(uint64_t uid);
    static uint32_t get_srv_index(uint64_t uid);
    static uint32_t get_seq(uint64_t uid);
    static uint32_t get_gen_time(uint64_t uid);

private:
    bool is_init();

private:
    bool m_is_init = false;
    uint32_t m_srv_group_id = 0;
    uint32_t m_srv_index = 0;
    uint32_t m_seq = 0;
    uint32_t m_start_time = 0;
};

class user_uid_generator
{
public:
    static uint64_t gen_user_uid(uint32_t platform, uint32_t channel_id, uint32_t server_id, uint32_t role_index, uint64_t account_id);
    
public:
    static uint32_t get_platform(uint64_t uid);
    static uint32_t get_channel_id(uint64_t uid);
    static uint32_t get_srv_group_id(uint64_t uid);
    static uint32_t get_role_index(uint64_t uid);
    static uint64_t get_account_id(uint64_t uid);
};

class user_did_generator
{
public:
    static std::string gen_user_did(uint64_t uid);
    //static uint64_t parse_user_uid_from_did(const std::string& did);
    static uint64_t parse_user_uid_from_did(const std::string& did, uint32_t platform, uint32_t channel_id);

public:
    static uint32_t get_platform(const std::string& str_did);
    static uint32_t get_channel_id(const std::string& str_did);
    static uint32_t get_srv_group_id(const std::string& str_did);
    static uint32_t get_role_index(const std::string& str_did);
    static uint64_t get_account_id(const std::string& str_did);
};

extern id_allocator_t g_id_allocator;

NS_COMMON_END

#endif
