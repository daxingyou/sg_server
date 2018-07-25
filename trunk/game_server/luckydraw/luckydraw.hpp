#ifndef __GAME_LUCKYDRAW_H__
#define __GAME_LUCKYDRAW_H__

#include "protos_fwd.hpp"
#include "macros.hpp"

typedef std::vector<uint32_t> seq_list;
class luckydraw_t
{
public:
	luckydraw_t(uint64_t uid);
    ~luckydraw_t();
public:
	void load_data(const proto::common::luckydraw_data& ld);
	void peek_data(proto::common::luckydraw_data* p_data);
	void save_self();
	void peek_client_data(proto::common::luckydraw_client_data* p_data);
public:
	uint32_t get_senior_count(uint32_t type );
	void set_senior_count(uint32_t type, uint32_t val);
	uint32_t get_today_count(uint32_t type);
	void set_today_count(uint32_t type, uint32_t val);
	uint32_t get_today_count();

	uint32_t get_senior_level(uint32_t type, uint32_t index);
	uint32_t get_luckydraw_pool_size(uint32_t type);
	//气运值满了吗
	bool lucky_full();
	//增加气运值
	void add_lucky(uint32_t val) { m_lucky += val; }
	void add_lucky_ex(uint32_t val) { m_lucky_ex += val; }
	//使用气运值
	void use_lucky();
	//清空附加气运值
	void clear_lucky_ex() { m_lucky_ex = 0; }
public:
	void init_seq();

	void make_seq(uint32_t type);

	void one_day();

	static uint32_t get_senoir_seq( uint32_t type );

	static bool init_senoir_seq();
private:
	static uint32_t get_lucky_max();
private:
	std::string m_key = "";
	uint64_t m_owner = 0;

	std::map<uint32_t, uint32_t> m_senior_count;
	std::map<uint32_t, uint32_t> m_today_count;
	std::map<uint32_t, seq_list> m_senior_seq;
	uint32_t m_lucky = 0;
	uint32_t m_lucky_ex = 0;
	static std::map<uint32_t, seq_list> s_senior_seq;
};
typedef boost::shared_ptr<luckydraw_t> luckydraw_ptr;
#endif