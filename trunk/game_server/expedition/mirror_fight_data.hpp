#ifndef __GAME_MIRROR_FIGHT_DATA_H__
#define __GAME_MIRROR_FIGHT_DATA_H__

#include "protos_fwd.hpp"
#include "macros.hpp"
#include "hero/hero.hpp"

class mirror_fight_data_t
{
public:
	mirror_fight_data_t();
	~mirror_fight_data_t();
public:
	void load_data(const proto::common::mirror_fight_data& mfd);
	void peek_data(proto::common::mirror_fight_data* p_data);
	void fill_data();
	void save_data(const proto::common::fight_save_data& esd);
public:
	const std::string& get_role_uid() { return m_role_uid; }
	void set_role_uid(const std::string& val) { m_role_uid = val; }

	const std::string& get_name() { return m_name; }
	void set_name(const std::string& val) { m_name = val; }

	void set_formation_id(uint32_t val) { m_formation_id = val; }
	uint32_t get_formation_id() { return m_formation_id; }

	proto::common::EM_SCENEOBJECT_TYPE get_type() { return m_type; }
	void set_type(proto::common::EM_SCENEOBJECT_TYPE type) { m_type = type; }

	void set_monster_id(uint32_t val) { m_monster_id = val; }
	uint32_t get_monster_id() { return m_monster_id; }
private:
	std::string m_role_uid = "";
	std::string m_name = "";
	uint32_t m_level = 0;
	uint32_t m_country = 0;
	uint32_t m_head_id = 0;
	uint32_t m_fighting = 0;
	uint32_t m_life_level = 0;
	std::string m_family_name = "";
	uint32_t m_formation_id = 0;
	std::map<uint64_t, proto::common::hero_single> m_hero_map;

	uint32_t m_monster_id = 0;
	proto::common::EM_SCENEOBJECT_TYPE m_type = proto::common::SCENEOBJECT_USER;
};
typedef boost::shared_ptr<mirror_fight_data_t> mirror_fight_data_ptr;
#endif