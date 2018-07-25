#include "buff.hpp"
#include "common/config_mgr.h"

buff_t::buff_t()
{
}

buff_t::buff_t(uint32_t id, BuffTable* conf, const uint64_t attacker, uint32_t skill, uint32_t level, const uint32_t layer, uint32_t hero_round, uint32_t count)
{
	m_skill = skill;
	m_skill_level = level ;
	if (NULL != conf)
	{
		m_run_attr.id = id;
		m_run_attr.tid = conf->id();
		m_run_attr.step = buff_step_load;
		m_run_attr.attacker = attacker;
		m_run_attr.round = count;
	//	m_run_attr.count = conf->count();
		m_run_attr.layer = layer;
		m_run_attr.hero_round = hero_round;
		m_run_attr.max_trigger_count = conf->max_trigger_count();
		m_run_attr.round_remove_type = conf->round_remove_type();
	}
}



buff_t::~buff_t()
{
}

uint32_t buff_t::get_max_trigger_count()
{
	return m_run_attr.max_trigger_count;
}

uint32_t buff_t::get_skill()
{
	return m_skill;
}

uint32_t buff_t::get_id()
{
	return m_run_attr.id;
}

uint32_t buff_t::get_tid()
{
	return m_run_attr.tid;
}

BuffTable* buff_t::get_conf()
{
	return GET_CONF(BuffTable, get_tid());
}

uint64_t buff_t::get_attacker()
{
	return m_run_attr.attacker;
}

uint32_t buff_t::get_hero_round()
{
	return m_run_attr.hero_round;
}

void buff_t::set_trigger_count(uint32_t count)
{
	m_run_attr.trigger_count = count;
}

void buff_t::add_trigger_count(uint32_t value)
{
	m_run_attr.trigger_count += value;
}

void buff_t::sub_trigger_count(uint32_t value )
{
	if (m_run_attr.trigger_count >= value)
		m_run_attr.trigger_count -= value;
	else
		m_run_attr.trigger_count = 0;
}

uint32_t buff_t::get_trigger_count()
{
	return m_run_attr.trigger_count;
}

uint32_t buff_t::get_layer()
{
	return m_run_attr.layer;
}

void buff_t::set_layer(uint32_t value)
{
	m_run_attr.layer = value;
}

void buff_t::add_layer(uint32_t value /*= 1*/)
{
	m_run_attr.layer += value;
}

void buff_t::sub_layer(uint32_t value /*= 1*/)
{
	if (m_run_attr.layer > value)
	{
		m_run_attr.layer -= value;
	}
	else
	{
		m_run_attr.layer = 0;
	}
}

uint32_t buff_t::get_overlay_type()
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return 0;
	}

	return conf->overlay_type();
}

uint32_t buff_t::get_max_layer()
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return 0;
	}

	return conf->max_layer();
}

uint32_t buff_t::get_step()
{
	return m_run_attr.step;
}

void buff_t::set_step(uint32_t step)
{
	m_run_attr.step = step;
	if (m_run_attr.step == buff_step_unload)
	{
		m_run_attr.damage_heal = 0;
	}
}

uint32_t buff_t::get_damage_heal()
{
	return m_run_attr.damage_heal;
}

void buff_t::set_damage_heal(uint32_t value)
{
	m_run_attr.damage_heal = value;
}

void buff_t::set_round(uint32_t round)
{
	m_run_attr.round = round;
}

void buff_t::sub_round(uint32_t value /*= 1*/)
{
	if (m_run_attr.round > value)
	{
		m_run_attr.round -= value;
	}
	else
	{
		m_run_attr.round = 0;
	}
}

void buff_t::add_round(uint32_t value /*= 1*/)
{
	m_run_attr.round += value;	
}

uint32_t buff_t::get_round()
{
	return m_run_attr.round;
}

bool buff_t::has_element(int id)
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		return false;
	}

	std::vector<buff_element_data>::const_iterator it;
	for (it = conf->elements().begin(); it != conf->elements().end(); ++it)
	{
		const buff_element_data& value = *it;
		if ((int)value.id == id)
			return true;
	}
	return false;
}

bool buff_t::is_valid()
{
	return m_run_attr.step != buff_step_unload;
}

void buff_t::set_count(uint32_t count)
{
	m_run_attr.count = count;
}

void buff_t::add_count(uint32_t value)
{
	m_run_attr.count += value;
}

void buff_t::sub_count(uint32_t value /*= 1*/)
{
	if (m_run_attr.count > value)
	{
		m_run_attr.count -= value;
	}
	else
	{
		m_run_attr.count = 0;
	}
}

uint32_t buff_t::get_count()
{
	return m_run_attr.count;
}

bool buff_t::is_persistence()
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return false;
	}

	return (conf->rounds() > 0);
} 

buff_effect_type buff_t::get_effect_type()
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return buff_effect_type_normal;
	}

	return (buff_effect_type)conf->buff_effect_type();
}

uint32_t buff_t::get_remove_type()
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return 0;
	}

	return conf->remove_type();
}

uint32_t buff_t::get_trigger_type()
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return 0;
	}

	return conf->get_first_trigger_type();
}

const std::set<uint32_t> *buff_t::get_triger_type_list()
{
	BuffTable* conf = get_conf();
	if (NULL == conf) {
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return NULL;
	}
	return &conf->trigger_type_list();
}


bool buff_t::is_trigger_type(uint32_t trigger_type) 
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return 0;
	}

	return conf->is_trigger_type( trigger_type );
}

bool buff_t::need_remove_when_dead()
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return false;
	}

	return (conf->remove_type() != buff_remove_type_dead_not_remove && conf->remove_type() != buff_remove_type_dead_not_remove_revive_remove);
}

bool buff_t::need_remove_when_revive()
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return false;
	}

	return conf->remove_type() == buff_remove_type_dead_not_remove_revive_remove;
}

uint32_t buff_t::get_small_type()
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return 0;
	}

	return conf->buff_small_type();
}

uint32_t buff_t::get_big_type()
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return 0;
	}

	return conf->buff_big_type();
}

uint32_t buff_t::get_worth()
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return 0;
	}

	if (conf->elements().size() == 0)
		return 0;

	const buff_element_data& data = conf->elements()[0];

	uint32_t worth = get_round() * data.value2;
	return worth;
}

void buff_t::peek_data(proto::common::fight_buff_single_data *buff_data)
{
	if (NULL == buff_data)
	{
		log_error("buff_t peek_data buff_data NULL");
		return;
	}
	buff_data->set_buff_id(get_id());
	buff_data->set_buff_tid(get_tid());
	buff_data->set_attacker(string_util_t::uint64_to_string(get_attacker()));
	buff_data->set_layer(get_layer());
	buff_data->set_round(get_round());
}

uint32_t buff_t::get_layer_remove_type()
{
	BuffTable* conf = get_conf();
	if (NULL == conf)
	{
		log_error("buff_t conf NULL tid[%d]", get_tid());
		return 0;
	}

	return conf->layer_remove_type();
}


int32_t buff_t::get_trigger_attr(uint32_t type) {
	auto iter_f = m_attr.find(type);
	if (iter_f == m_attr.end()) {
		return 0;
	}
	return iter_f->second;
}
