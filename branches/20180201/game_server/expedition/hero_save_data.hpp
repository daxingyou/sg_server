#ifndef __GAME_HERO_SAVE_DATA_H__
#define __GAME_HERO_SAVE_DATA_H__

#include "protos_fwd.hpp"
#include "macros.hpp"
#include "hero/hero.hpp"

class hero_save_data_t
{
public:
	hero_save_data_t();
	~hero_save_data_t();
public:
	void load_data(const proto::common::hero_save_data& hsd);
	void peek_data(proto::common::hero_save_data* p_data);

	void save_data(const proto::common::hero_save_data& hsd);
public:
	uint64_t get_uid() { return m_hero_uid; }

	void set_hp(uint32_t val) { m_hp = val; }
	uint32_t get_hp() { return m_hp; }

	int32_t get_skill_cd(uint32_t skill_id);
	bool has_cd();
	void clear_cd();
private:
	uint64_t m_hero_uid = 0;
	uint32_t m_hp = 0;
	std::map<uint32_t, uint32_t> m_skill_cd;
};
typedef boost::shared_ptr<hero_save_data_t> hero_save_data_ptr;
#endif