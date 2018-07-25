#ifndef __GAME_OFFICE_H__
#define __GAME_OFFICE_H__

#include "protos_fwd.hpp"
#include "macros.hpp"

class office_t
{
public:
	office_t(uint64_t uid);
    ~office_t();
public:
	void load_data(const proto::common::office_data& od);
	void peek_data(proto::common::office_data* p_data);
	void save_self();

	void one_day();

	uint32_t get_red_num();

	void init();
public:
	void peek_client_data(proto::common::office_client_data* p_data);

	void check_levelup();

	uint32_t award(proto::common::role_change_data* p_data);

	void add_exp(uint32_t val);

	uint32_t get_level() { return m_level; }
private:
	std::string m_key = "";
	uint64_t m_owner = 0;

	uint32_t	m_level = 0;				///等级
	uint32_t	m_exp = 0;					///经验
	uint32_t	m_award = 0;				///领取标记0为未领取1为已领取
};
typedef boost::shared_ptr<office_t> office_ptr;
#endif