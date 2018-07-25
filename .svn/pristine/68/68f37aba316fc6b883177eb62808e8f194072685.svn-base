#include "mirror_fight_data.hpp"
#include "log.hpp"
#include "fight/fight_manager.hpp"
#include "tblh/Monster.tbls.h"
#include "config/config_manager.hpp"
#include "tblh/attr_enum.hpp"
USING_NS_COMMON;

mirror_fight_data_t::mirror_fight_data_t()
{

}

mirror_fight_data_t::~mirror_fight_data_t()
{

}

void mirror_fight_data_t::load_data(const proto::common::mirror_fight_data& mfd)
{
	if (mfd.has_obj())
	{
		const proto::common::obj_single& os = mfd.obj();
		if (os.has_object_uid())
		{
			m_role_uid = os.object_uid();
		}
		if (os.has_name())
		{
			m_name = os.name();
		}
		if (os.has_monster_id())
		{
			m_monster_id = os.monster_id();
		}
		if (os.has_level())
		{
			m_level = os.level();
		}
		if (os.has_country())
		{
			m_country = os.country();
		}
		if (os.has_head_id())
		{
			m_head_id = os.head_id();
		}
		if (os.has_life_level())
		{
			m_life_level = os.life_level();
		}
		if (os.has_fighting())
		{
			m_fighting = os.fighting();
		}
		if (os.has_family_name())
		{
			m_family_name = os.family_name();
		}
		m_type = os.object_type();
	}
	if (mfd.has_heros())
	{
		const proto::common::hero_data& hd = mfd.heros();
		for (int32_t i = 0; i < hd.hero_list_size(); ++i)
		{
			const proto::common::hero_single& hs = hd.hero_list(i);
			uint64_t hero_uid = string_util_t::string_to_uint64(hs.base_attr().unique_id());
			m_hero_map.insert(std::make_pair(hero_uid, hs));
		}
	}
	
	m_formation_id = mfd.formation_id();
}

void mirror_fight_data_t::peek_data(proto::common::mirror_fight_data* p_data)
{
	p_data->mutable_obj()->set_object_uid(m_role_uid);
	p_data->mutable_obj()->set_name(m_name);
	p_data->mutable_obj()->set_level(m_level);
	p_data->mutable_obj()->set_object_type(m_type);
	p_data->mutable_obj()->set_monster_id(m_monster_id);
	p_data->mutable_obj()->set_country(m_country);
	p_data->mutable_obj()->set_head_id(m_head_id);
	p_data->mutable_obj()->set_fighting(m_fighting);
	p_data->mutable_obj()->set_life_level(m_life_level);
	p_data->mutable_obj()->set_family_name(m_family_name);
	for (auto it : m_hero_map)
	{
		const proto::common::hero_single& hs = it.second;
		p_data->mutable_heros()->add_hero_list()->CopyFrom(hs);
	}
	p_data->set_formation_id(m_formation_id);
}

void mirror_fight_data_t::fill_data()
{
	if (m_type == proto::common::SCENEOBJECT_MONSTER)
	{
		auto p_monster_conf = GET_CONF(Monster, m_monster_id);
		if (NULL == p_monster_conf)
		{
			log_error("NULL == p_monster_conf[%d]", m_monster_id);
			return;
		}
		uint32_t against_id = p_monster_conf->get_fight_id();
		auto p_against_conf = GET_CONF(FightAgainst, against_id);
		if (NULL == p_against_conf)
		{
			log_error("NULL == p_against_conf[%d]", against_id);
			return;
		}

		m_role_uid = string_util_t::uint32_to_string(against_id);

		proto::common::hero_data hd;
		fight_manager_t::fill_hero_data(proto::common::combat_camp_2, against_id, CAMP_ENEMY_UNIQUE_OFFSET, 0, 0, p_against_conf->get_against_ids(), &hd, 0);
		for (int32_t i = 0; i < hd.hero_list_size(); ++i)
		{
			const proto::common::hero_single& hs = hd.hero_list(i);
			uint64_t hero_uid = string_util_t::string_to_uint64(hs.base_attr().unique_id());
			m_hero_map.insert(std::make_pair(hero_uid, hs));
		}
	}

	for (auto& it : m_hero_map)
	{
		proto::common::hero_single& hs = it.second;
		hs.set_save_hp(100);
	}
}

void mirror_fight_data_t::save_data(const proto::common::fight_save_data& esd)
{
	for (int32_t i = 0; i < esd.hero_list_size(); ++i)
	{
		const proto::common::hero_save_data& hsd = esd.hero_list(i);
		uint64_t hero_uid = hsd.hero_uid();
		auto it_map = m_hero_map.find(hero_uid);
		if (it_map == m_hero_map.end())
		{
			continue;
		}
		proto::common::hero_single& hs = it_map->second;
		hs.set_save_hp(hsd.hp());
		for (int32_t j = 0; j < hsd.skill_list_size(); ++j)
		{
			const proto::common::skill_save_data& ssd = hsd.skill_list(j);
			uint32_t skill_id = ssd.skill_id();
			uint32_t skill_cd = ssd.cd();
			proto::common::skill_data* p_skill_data = hs.mutable_skill();
			for (int32_t k = 0; k < p_skill_data->skills_size(); ++k)
			{
				proto::common::skill_single* p_skill_single = p_skill_data->mutable_skills(k);
				if (skill_id == p_skill_single->skill_id())
				{
					p_skill_single->set_skill_cd(skill_cd);
				}
			}
		}
	}
}
