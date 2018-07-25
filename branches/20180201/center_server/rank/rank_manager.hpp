#ifndef __RANK_MANAGER_H__
#define __RANK_MANAGER_H__
#include "rank_sort.hpp"
#include "user/global_user.hpp"

enum rank_type
{
	rank_type_level = 1,		//等级排行
	rank_type_criminal = 2,		//通缉令
};

enum clear_type
{
	clear_type_none = 0,
	clear_type_day = 1,		//每日清除
	clear_type_week = 2,	//每周清除
	clear_type_month = 3,	//每月清除
};

struct sort_key_t
{
	sort_key_t()
		: key(0)
		, time(0)
		, key2(0)
	{
	}

	sort_key_t(uint32_t _key, uint32_t _time, uint32_t _key2)
	{
		key = _key;
		time = _time;
		key2 = _key2;
	}

	sort_key_t& operator=(const sort_key_t& other)
	{
		if (&other == this)
		{
			return *this;
		}

		key = other.key;
		time = other.time;
		key2 = other.key2;

		return *this;
	}

	bool operator>(const sort_key_t& other) const
	{
		if (key == other.key && key2 == other.key2)
		{
			return time < other.time;
		}

		if (key > other.key)
		{
			return true;
		}

		if (key2 > other.key2)
		{
			return true;
		}

		return false;
	}

	uint32_t key;
	uint32_t time;
	uint32_t key2;
};

struct rank_record_t
{
	sort_key_t key;
	uint64_t owner;
	std::vector<std::string> custom_data;
};

class rank_t : public max_sort_rank<sort_key_t, rank_record_t>
{
public:
	rank_t();
	~rank_t();

	const char* getClassName() const
	{
		return "排行榜";
	}

public:

	void request_rank(global_user_ptr user);

	void rank_reward(uint32_t param = 0);
	void rank_reward(uint32_t index, uint64_t object_uid);
	void day_reward();
	void week_reward();
	void month_reward();

	void show();

	void fill_rank_info(proto::common::client_rank_single* add, const rank_record_t& info, const uint32_t index);
	bool get_rank_data(const uint32_t down, const uint32_t up, std::vector< rank_record_t >& list);
	bool get_rank_data(proto::common::client_rank_data* out, uint64_t self_uid, uint32_t num = 0, uint32_t condition = 0);
	bool get_rank_data(proto::common::client_rank_single* out, uint64_t self_uid);

	std::string get_rank_name();
	std::string get_owner_name(uint64_t owner);

	// 清理排行中的冗余数据
	void check_clean();
	void check_del(const uint64_t owner);

	// 临时写的，循环遍历获得排名
	uint32_t get_rank(uint64_t self_uid);

	// 获得各个国家排名前N的活跃玩家数量
	void get_actives(uint32_t rank, std::map<uint32_t, uint32_t>& actives);
};

typedef boost::shared_ptr<rank_t> rank_ptr;
typedef std::map<uint32_t, rank_ptr> rank_map;
typedef std::map<uint32_t, rank_ptr>::iterator rank_map_iter;

struct rank_reg
{
	rank_reg(const rank_owner_type _type, const std::string& _name)
		: type(_type)
		, name(_name)
	{
	}

	rank_owner_type type;
	std::string name;
};

class rank_manager_t
{
public:
	rank_manager_t();
	~rank_manager_t();

	typedef std::map< uint32_t, rank_reg > rank_register_map;
	typedef std::map< uint32_t, rank_reg >::iterator rank_register_map_iter;

public:

	static bool init();

	static bool request_rank(global_user_ptr user, uint32_t type);
	static void update_rank(const proto::server::ge_update_rank_notify& msg);
	static void update_rank(
		uint64_t owner, 
		uint32_t type,
		uint32_t key);	
	static void clear_rank(uint32_t type);
	static void delete_from_rank(uint64_t owner, uint32_t type);

	static void rank_reward(uint32_t type, uint32_t param = 0);
	static void rank_reward(uint32_t type, uint32_t index, uint64_t object_uid);
	static rank_owner_type get_owner_type(uint32_t type);
	static const std::string get_rank_name(uint32_t type);

	static void one_day();
	static void one_sec();
	static void one_week();
	static void one_month();

	static void check_save(bool immediately = false);

	static void show_rank(uint32_t type);

	static bool get_rank_data(uint32_t type, const uint32_t down, const uint32_t up, std::vector< rank_record_t >& list);
	static bool get_rank_data(uint32_t type, proto::common::client_rank_data* out, uint64_t self_uid, uint32_t num = 0, uint32_t condition = 0);
	static bool get_rank_data(uint32_t type, proto::common::client_rank_single* out, uint64_t self_uid);

	static bool request_self_rank(global_user_t* user, uint32_t type);

	static void final();

	static void check_clean(uint32_t type);
	static void check_del(uint32_t type, uint32_t id);

	static void beat_criminal_reward(uint64_t self_uid, uint64_t criminal_uid);

	// 临时用下获得排名
	static uint32_t get_rank_by_type(uint32_t type, uint64_t uid);

	// 获得各个国家排名前N的活跃玩家数量
	static void get_active_user(uint32_t rank, std::map<uint32_t, uint32_t>& actives);
private:
	static std::set< uint32_t > m_day_rank;		// 日排行，一天清除一次
	static std::set< uint32_t > m_week_rank;	// 周排行，一周清除一次
	static std::set< uint32_t > m_month_rank;	// 月排行，一月清除一次
	static rank_register_map	m_register;
	static rank_map 			m_ranks;
};

#endif//__RANK_MANAGER_H__

