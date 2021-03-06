#include "formation.hpp"
#include "formation_manager.hpp"
#include "hero/hero.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "hero/hero_manager.hpp"
#include "config/config_manager.hpp"
#include "role/role_manager.hpp"
#include "user_troop/user_troop.hpp"
#include "user_troop/user_troop_mgr.hpp"
#include "object/object_manager.hpp"


formation_t::formation_t(uint64_t owner) : m_owner(owner)
{

}

formation_t::formation_t(uint32_t index, uint64_t owner) : m_index(index), m_owner(owner)
{
}

formation_t::~formation_t()
{
}

void formation_t::load_data(const proto::common::formation_single& single)
{
	m_index = single.formation_index();
	m_tactic_id = single.tactic_id();
	for (auto pair : single.formations())
	{
		// 只有角色才会加载数据，这里简单处理下即可
		form(pair.pos(), string_util_t::string_to_uint64(pair.unique_id()));
	}
}

void formation_t::peek_data(proto::common::formation_single* p_single)
{
	if (NULL == p_single)
	{
		log_error("p_single is null");
		return;
	}

	p_single->set_formation_index(m_index);
	p_single->set_tactic_id(m_tactic_id);
	for (auto pos : m_pos_hero)
	{
		proto::common::formation_pair* p_pair = p_single->add_formations();
		p_pair->set_pos(pos.first);
		p_pair->set_unique_id(string_util_t::uint64_to_string(pos.second.uid));
		p_pair->set_owner_id(string_util_t::uint64_to_string(pos.second.id.first));
		hero_ptr p_hero = get_hero_by_pos(pos.first);
		if (NULL == p_hero)
		{
			//log_error("formation p_hero NULL pos[%d]", pos.first);
			continue;
		}
		p_pair->set_plugin(p_hero->get_current_plugin());
		p_pair->set_grade(p_hero->get_grade());
		p_pair->set_star(p_hero->get_star());
		p_pair->set_level(p_hero->get_current_level());
		
	}
}

void formation_t::save_self(bool is_new)
{
	auto role = role_manager_t::find_role(m_owner);
	if (NULL == role)
	{
		log_error("formation save self owner[%lu] not valid", m_owner);
		return;
	}

	role->save_self();
}

void formation_t::peek_fight_data(proto::common::hero_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("p_data is null");
		return;
	}

	for (auto pos : m_pos_hero)
	{
		object_base_ptr object = object_manager_t::find_object(pos.second.id);
		if (NULL == object)
			continue;
		hero_ptr p_hero = object->get_hero_by_unique_id(pos.second.uid);
		if (NULL == p_hero)
		{
			log_error("formation p_hero NULL pos[%d]", pos.first);
			continue;
		}
		proto::common::hero_single* p_single = p_data->add_hero_list();
		p_hero->peek_data(p_single, true);
	}
}

void formation_t::form(uint32_t pos, PosHeroData& data)
{
	if (pos > 0 && pos <= FORM_SIZE)
	{
		m_pos_hero[pos] = data;
	}
}

void formation_t::form(uint32_t pos, uint64_t unique_id)
{
	// 只有角色才会加载数据，这里简单处理下即可
	PosHeroData data(m_owner, proto::common::SCENEOBJECT_USER, unique_id);
	form(pos, data);
}

void formation_t::remove(uint32_t pos)
{
	m_pos_hero.erase(pos);
}

void formation_t::remove(uint64_t unique_id)
{
    auto ptr = std::find_if(
		m_pos_hero.begin(), 
		m_pos_hero.end(),
        [unique_id](std::map<uint32_t, PosHeroData>::value_type val)
        {
            if (val.second.uid == unique_id)
            {
                return true;
            }
            return false;
        });

    if( m_pos_hero.end() != ptr)
    {
        m_pos_hero.erase(ptr);
    }
}

// 更新布阵
void formation_t::update_formation(
	const std::map<uint32_t, PosHeroData> &pos_hero, 
	uint32_t tactic_id, 
	bool is_new/* = false*/, 
	bool is_save)
{
	if (pos_hero.size() > FORM_SIZE)
	{
		log_error("update_formation pos_hero_size[%d] > FORM_SIZE[%d]", (int32_t)pos_hero.size(), FORM_SIZE);
		return;
	}
	m_pos_hero.clear();
	for (auto it : pos_hero)
	{
		m_pos_hero[it.first] = it.second;
	}
	m_tactic_id = tactic_id;
	if(is_save)
		save_self(is_new);
}

void formation_t::add_hero_to_formation(uint64_t unique_id)
{
	uint32_t free_pos = get_free_pos();
	if (free_pos == 0)
	{
		log_error("add_hero_to_formation not find free_pos");
		return;
	}
	form(free_pos, unique_id);
}

void formation_t::remove_hero_from_formation(uint64_t unique_id)
{
	std::map<uint32_t, PosHeroData>::iterator iter = m_pos_hero.begin();
	for (; iter != m_pos_hero.end(); ++iter)
	{
		if (iter->second.uid == unique_id)
		{
			m_pos_hero.erase(iter);
			return;
		}
	}
}

void formation_t::replace_hero_from_formation(uint64_t old_hero_uid, uint64_t new_hero_uid)
{
	uint32_t replace_pos = get_pos_by_unique_id(old_hero_uid);
	if (replace_pos == 0)
	{
		log_error("replace_hero_from_formation not find hero_uid[%lu]", old_hero_uid);
		return;
	}
	form(replace_pos, new_hero_uid);
}

bool formation_t::has_hero(uint64_t unique_id)
{
	for (auto ptr : m_pos_hero)
	{
		if (ptr.second.uid == unique_id)
		{
			return true;
		}
	}

	return false;
}

uint32_t formation_t::get_pos_by_unique_id(uint64_t unique_id)
{
	for (auto ptr : m_pos_hero)
	{
		if (ptr.second.id.second != proto::common::SCENEOBJECT_USER)
			continue;
		if (ptr.second.uid == unique_id)
		{
			return ptr.first;
		}
	}

	return 0;
}

hero_ptr formation_t::get_hero_by_pos(uint32_t pos)
{
	auto ptr = m_pos_hero.find(pos);
	if (ptr != m_pos_hero.end())
	{
		object_base_ptr object = object_manager_t::find_object(ptr->second.id);
		if (NULL == object)
			return NULL;
		return object->get_hero_by_unique_id(ptr->second.uid);
	}

	return NULL;
}

void formation_t::clear_heroes()
{
	m_pos_hero.clear();
}

void formation_t::reset_heroes()
{
	auto p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	m_pos_hero.clear();
	// 主公不能下阵
	form(DEFAULT_ROLE_POS, p_role->get_main_hero_uid());
	//m_pos_hero.insert(std::make_pair(DEFAULT_ROLE_POS, p_role->get_main_hero_uid()));
}

uint32_t formation_t::calc_fighting()
{
	auto p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return 0;
	}
	uint32_t fighting = 0;

	for (auto it : m_pos_hero)
	{
		object_base_ptr object = object_manager_t::find_object(it.second.id);
		if (NULL == object)
			continue;
		uint64_t hero_uid = it.second.uid;
		auto p_hero = object->get_hero_by_unique_id(hero_uid);
		if (NULL != p_hero)
		{
			fighting += p_hero->get_fighting();
			//log_warn("hero[%d] fighting[%d]", p_hero->get_tid(), p_hero->get_fighting());
		}
	}
	//log_warn("fighting[%d]", fighting);
	return fighting;
}

uint32_t formation_t::get_free_pos()
{
    for(auto i = 1; i < FORM_SIZE; i++)
    {
        auto it = m_pos_hero.find(i);
        if(it == m_pos_hero.end())
        {
            return i;
        }
    }

    return 0;
}

troop_formation_t::troop_formation_t(uint32_t index, uint64_t troop_id) :
	formation_t(index, troop_id), 
	m_troop_id(troop_id)
{
}

troop_formation_t& troop_formation_t::operator=(formation_t& form)
{
	m_pos_hero = form.get_pos_hero();
	return *this;
}
