#include "buff_manager.hpp"
#include "tblh/BuffTable.tbls.h"
#include "tblh/BuffBigType.tbls.h"
#include "tblh/BuffSmallType.tbls.h"
#include "common/config_mgr.h"
#include "fight_hero.hpp"

std::map<uint32_t, elenment_func> buff_manager_t::m_funclist;

buff_manager_t::buff_manager_t()
{
}

buff_manager_t::~buff_manager_t()
{
	close();
}

bool buff_manager_t::init()
{
	return buff_element_t::regist(m_funclist);
}

void buff_manager_t::close()
{
	m_buffs.clear();
	m_special_buffs.clear();
	m_remove_set.clear();
	m_owner.reset();
}

void buff_manager_t::set_owner(fight_hero_ptr owner)
{
	m_owner = owner;
}

void buff_manager_t::change_buff_round(buff_ptr buff, const int change)
{
	if (change == 0)
		return;
	if (change > 0)
	{
		buff->add_round(change);
		get_owner()->add_combat_buff_act(proto::common::combat_act_type_buff_update, buff->get_id(), buff->get_tid(), buff->get_layer(), buff->get_round(), buff->get_attacker());
	}
	else
	{
		buff->sub_round(-change);
		// �жϻغ����Ƿ�û�У�û����ɾ��
		if (buff->get_round() == 0)
		{
			remove_buff(buff,buff->get_attacker());
		}
		else
		{
			get_owner()->add_combat_buff_act(proto::common::combat_act_type_buff_update, buff->get_id(), buff->get_tid(), buff->get_layer(), buff->get_round(), buff->get_attacker());
		}
	}
}

bool buff_manager_t::buff_safe_check()
{
	if(get_owner() != NULL && get_owner()->get_combat() != NULL)
	if (get_owner()->get_combat()->get_round_add_buff_count() > 100)
	{
		return false;
	}
	return true;
}

void buff_manager_t::change_buff_round_by_type(const int change, buff_effect_type type)
{
	if (change == 0)
		return;
	buff_vec copy(m_buffs);

	buff_ptr p_buff = buff_ptr();
	buff_vec::iterator it;
	for (it = copy.begin(); it != copy.end(); ++it)
	{
		p_buff = *it;
		if (p_buff == NULL)
			continue;
		if (p_buff->get_effect_type() != type)
			continue;
		change_buff_round(p_buff,change);
	}
}

void buff_manager_t::change_debuff_round(const int change)
{
	change_buff_round_by_type( change, buff_effect_type_debuff );
}

void buff_manager_t::change_profit_buff_round(const int change)
{
	change_buff_round_by_type(change, buff_effect_type_gain);
}

void buff_manager_t::detonate_buff(const int element_type)
{
	buff_vec copy(m_buffs);

	buff_ptr buff = buff_ptr();
	buff_vec::iterator it;
	for (it = copy.begin(); it != copy.end(); ++it)
	{
		buff = *it;
		if (buff == NULL)
			continue;
		// �ж��Ƿ���ڸ�Ԫ��
		if(!buff->has_element(element_type))
			continue;
		// �����׶�
		buff->set_step(buff_step_unload);
		run_buff(buff);
		// �������Ƴ�
		remove_buff(buff, buff->get_attacker());
	}
}

void buff_manager_t::change_buff_round(const int buff_tid, const int change)
{
	if (change == 0)
		return;
	buff_vec copy(m_buffs);

	buff_ptr p_buff = buff_ptr();
	buff_vec::iterator it;
	for (it = copy.begin(); it != copy.end(); ++it)
	{
		p_buff = *it;
		if (p_buff == NULL)
			continue;
		if ((int)p_buff->get_tid() != buff_tid)
			continue;
		change_buff_round(p_buff, change);
	}
}

void buff_manager_t::on_dead()
{
	remove_buff_when_dead();
}

buff_ptr buff_manager_t::add_buff(const uint32_t id, const uint64_t attacker, const uint32_t skill, const uint32_t layer)
{
	if (NULL == get_owner())
	{
		log_error("get_owner is null");
		return NULL;
	}

	if (!buff_safe_check())
	{
		log_error("buff add dead cycle!!! id[%d] attacker[%lu] skill[%d]", id, attacker, skill);
		return NULL;
	}

	BuffTable* conf = GET_CONF(BuffTable, id);
	if (NULL == conf)
	{
		log_error("buff id invalid %u", id);
		return NULL;
	}

	if (!can_add_buff(conf, skill))
	{
		log_error("add_buff can't add buff id[%d] hero[%lu][%d]", id, get_owner()->get_uid(), get_owner()->get_tid());
		return NULL;
	}

	BuffBigType *p_big_type_conf = GET_CONF(BuffBigType, conf->buff_big_type());
	if (NULL == p_big_type_conf)
	{
		log_error("buff_manager_t p_big_type_conf NULL type[%d] buff_id[%d]", conf->buff_big_type(), id);
		return NULL;
	}

	BuffSmallType *p_small_type_conf = GET_CONF(BuffSmallType, conf->buff_small_type());
	if (NULL == p_small_type_conf)
	{
		log_error("buff_manager_t p_small_type_conf NULL id[%d] type[%d]", id, conf->buff_small_type());
		return NULL;
	}


	buff_ptr p_owner_buff = buff_ptr();
	buff_ptr p_same_owner_buff = buff_ptr();
	buff_ptr p_same_small_type_buff = buff_ptr();
	buff_ptr p_same_big_type_buff = buff_ptr();
	buff_ptr p_replace_buff = buff_ptr();

	for (uint32_t buff_pos = 0; buff_pos < m_buffs.size(); ++buff_pos)
	{
		p_owner_buff = m_buffs[buff_pos];
		if (NULL == p_owner_buff)
		{
			log_error("buff_manager_t p_owner_buff NULL pos[%d]", buff_pos);
			continue;
		}

		// �����û����ͬ�����͵�BUFF
		if (p_owner_buff->get_big_type() == conf->buff_big_type())
		{
			p_same_big_type_buff = p_owner_buff;
			// ��ͬС����
			if (p_owner_buff->get_small_type() == conf->buff_small_type())
			{
				p_same_small_type_buff = p_owner_buff;
				// ��ͬ��Դ
				if (p_owner_buff->get_attacker() == attacker)
				{
					p_same_owner_buff = p_owner_buff;
				}
			}
		}
		else
		{
			// �����û�л��� ���ⲻ���buff
			for (uint32_t i = 0; i < p_big_type_conf->buff_resist_types().size(); ++i)
			{
				if (p_owner_buff->get_big_type() == p_big_type_conf->buff_resist_type(i))
				{
					return buff_ptr();
				}
			}
			// �����û���滻
			for (uint32_t j = 0; j < p_big_type_conf->buff_replace_types().size(); ++j)
			{
				if (p_owner_buff->get_big_type() == p_big_type_conf->buff_replace_type(j))
				{
					p_replace_buff = p_owner_buff;
					break;
				}
			}

			if (p_replace_buff != NULL)
				break;
		}
	}

	bool b_can_add_new_buff = false;
	buff_ptr p_cover_buff = buff_ptr();

	// �ȼ���Ƿ��в�ͬ�����͵���� ��Ҫ�滻��buff
	if (p_replace_buff != NULL)
	{
		remove_buff(p_replace_buff, attacker);
		b_can_add_new_buff = true;
	}
	else
	{
		// ������ͬ������
		if (p_same_big_type_buff != NULL)
		{
			// ��ͬС����
			if (p_same_small_type_buff)
			{
				uint32_t add_type = 0;
				// ��ͬ��Դ
				if (p_same_owner_buff)
				{
					add_type = p_small_type_conf->same_source();
				}
				// ��ͬ��Դ
				else
				{
					add_type = p_small_type_conf->diff_source();
				}

				switch (add_type)
				{
				case buff_add_type_add_layer:
					{
						p_cover_buff = p_same_small_type_buff;
					}
					break;
				case buff_add_type_replace:
					{
						buff_ptr p_buff = buff_ptr();
						if (p_same_owner_buff)
							p_buff = p_same_owner_buff;
						else
							p_buff = p_same_small_type_buff;

						remove_buff(p_buff, attacker);
						b_can_add_new_buff = true;
					}
					break;
				case buff_add_type_add:
					{
						b_can_add_new_buff = true;
					}
					break;
				default:
					break;
				}
			}
			else
			{
				b_can_add_new_buff = true;
			}
		}
		// ��ͬ������
		else
		{
			b_can_add_new_buff = true;
		}
	}
	
	buff_ptr buff = buff_ptr();
	buff_ptr result = buff_ptr();

	uint32_t buff_id = 0;
	if (p_cover_buff)
	{
		buff_id = p_cover_buff->get_id();
		result = cover_buff(p_cover_buff, conf, layer);
		if (NULL == result)
		{
			m_remove_set.insert(buff);
			return result;
		}
	}


	if (b_can_add_new_buff)
	{
		combat_ptr p_combat = get_owner()->get_combat();
		if (NULL == p_combat)
		{
			log_error("buff_manager_t p_combat NULL");
			return result;
		}

		buff_id = p_combat->gen_buff_id();
		// ֻ���Լ����Լ���buffʱ��Ҫ��¼�佫��ǰ�غ�
		uint32_t hero_round = attacker == get_owner()->get_uid() ? get_owner()->get_round() : 0;
		buff.reset(new buff_t(buff_id, conf, attacker, skill, layer, hero_round));

		result = add_new_buff(buff);
	}

	if (NULL == result)
	{
		m_remove_set.insert(buff);
		return result;
	}

	if (NULL != get_owner())
	{
		get_owner()->on_add_buff(buff_id, id, result->get_layer(), result->get_round(), result->get_attacker());
	}

	return result;
}

bool buff_manager_t::can_add_buff(BuffTable* p_buff_conf, uint32_t skill)
{
	if (NULL == p_buff_conf)
	{
		log_error("can_add_buff p_buff_conf NULL");
		return false;
	}

	if (p_buff_conf->rounds() == 0)
	{
		log_error("buff[%d] rounds = 0", p_buff_conf->id());
		return false;
	}

	// �ж���BUFF�Ƿ�Ŀ��ָ��������Ա
	SkillEffectTable* skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill, 1));
	if (NULL != skill_conf)
	{
		if (skill_conf->target_type() != skill_target_dead_teammate && 
			skill_conf->target_type() != skill_target_dead_enemy &&
			skill_conf->target_type() != skill_target_dead_friend)
		{
			if (get_owner()->is_dead())
			{
				return false;
			}
		}
	}
	else
	{
		if (get_owner()->is_dead())
		{
			return false;
		}
	}

	if (get_owner()->get_attr(attrtype_enum::immuno) > 0)
	{
		const std::vector<uint32_t> immuno_types = get_owner()->get_immuno_buff_types();
		for (uint32_t i = 0; i < immuno_types.size(); ++i)
		{
			if (p_buff_conf->buff_big_type() == immuno_types[i])
			{
				get_owner()->on_buff_immuno();
				return false;
			}
		}
	}

	// �ж��Ƿ��ֹǿ��
	if (get_owner()->get_attr(attrtype_enum::forbid_strengthen) > 0)
	{
		if ((buff_effect_type)p_buff_conf->buff_effect_type() == buff_effect_type_gain)
			return false;
	}

	if (in_remove_set(p_buff_conf->id()))
	{
		log_warn("buff[%d] in_remove_set", p_buff_conf->id());
		return false;
	}
	return true;
}

// buff_ptr buff_manager_t::add_buff(buff_ptr buff)
// {
// 	if (NULL == buff)
// 	{
// 		log_error("buff is null");
// 		return buff_ptr();
// 	}
// 
// 	buff_ptr result = NULL;
// 	buff_ptr os = get_buff_by_id(buff->get_id());
// 	if (NULL == os)
// 	{
// 		result = add_new_buff(buff);
// 	}
// 	else
// 	{
// 		result = update_exist_buff(buff, os);
// 	}
// 
// 	return result;
// }

bool buff_manager_t::remove_buff_by_small_type(uint32_t small_type, uint64_t attacker_id )
{
	uint32_t notify = 0;
	buff_ptr buff = get_buff_by_small_type(small_type, attacker_id);
	if (buff)
	{
		notify |= remove_buff(buff, attacker_id);
		return true;
	}

	return false;
}

bool buff_manager_t::remove_buff_by_tid(uint32_t tid, uint64_t attacker_id)
{
	uint32_t notify = 0;
	buff_ptr buff = get_buff_by_tid(tid, attacker_id);
	if (buff)
	{
		notify |= remove_buff(buff, attacker_id);
		return true;
	}

	return false;
}

bool buff_manager_t::remove_buff(uint32_t id, uint64_t attacker_id)
{
	uint32_t notify = 0;
	buff_ptr buff = get_buff_by_id(id);
	if (buff)
	{
		notify |= remove_buff(buff, attacker_id);
		return true;
	}
		
	return false;
}

uint32_t buff_manager_t::remove_buff(buff_ptr buff, uint64_t attacker_id)
{
	if (NULL == buff)
	{
		log_error("remove_buff buff is null");
		return 0;
	}

	if (in_remove_set(buff))
	{
		log_warn("remove_buff buff[%d:%d] in remove_set", buff->get_id(), buff->get_tid());
		return 0;
	}

	if (m_buffs.size() == 0)
	{
		log_error("remove_buff m_buffs size = 0 but remove buff[%d:%d]", buff->get_id(), buff->get_tid());
		return 0;
	}

	buff->set_step(buff_step_unload);

	buff_vec::iterator iter = std::find(m_buffs.begin(), m_buffs.end(), buff);
	if (iter == m_buffs.end())
	{
		log_error("remove_buff not find buff[%d:%d]", buff->get_id(), buff->get_tid());
		return 0;
	}
	m_buffs.erase(iter);
	FIGHT_LOG("remove buff id[%d:%d]", buff->get_id(), buff->get_tid());

	if (NULL != get_owner())
	{
		get_owner()->on_remove_buff(buff->get_id(), buff->get_tid(), 0, 0, attacker_id);
	}

	return run_buff(buff);
}

uint32_t buff_manager_t::remove_buff_by_count(uint32_t count, buff_effect_type type)
{
	if (count == 0)
		return 0;
	buff_vec		remove_buffs;
	uint32_t		c = 0;
	for (auto buff : m_buffs)
	{
		if (NULL == buff)
			continue;

		if (buff->get_effect_type() != type)
			continue;
		remove_buffs.push_back(buff);
		++c;
		if (c >= count)
			break;
	}

	uint32_t	remove_count = 0;
	for (auto buff : remove_buffs)
	{

		++remove_count;
		remove_buff(buff);
		if (remove_count >= count)
			break;
	}


	return c > count ? count : c;
}

uint32_t buff_manager_t::remove_debuff_by_count(uint32_t count)
{
	return remove_buff_by_count(count, buff_effect_type_debuff);
}

uint32_t buff_manager_t::remove_profit_buff_by_count(uint32_t count)
{
	return remove_buff_by_count(count, buff_effect_type_gain);
	
}

void buff_manager_t::dispel_buff(const buff_dispel_info_vec& buffs, uint32_t num, fight_hero_ptr p_attacker)
{
	if (NULL == p_attacker)
	{
		log_error("p_attacker NULL");
		return;
	}
	uint64_t attacker_id = p_attacker->get_uid();
	buff_vec temp;
	buff_vec::iterator it;
	buff_ptr p_buff = buff_ptr();
	for (it = m_buffs.begin(); it != m_buffs.end(); ++it)
	{
		p_buff = *it;
		if(NULL == p_buff)
			continue;

		for (uint32_t i = 0; i < buffs.size(); ++i)
		{
			if (p_buff->get_conf() != NULL && p_buff->get_big_type() == buffs[i])
			{
				temp.push_back(p_buff);
			}
		}
	}

	// ȫ����ɢ
	if (num >= temp.size())
	{
		p_attacker->m_buff_mgr.add_dispel_buff_count(temp.size());
		for (it = temp.begin(); it != temp.end(); ++it)
		{
			if (in_remove_set(*it))
				continue;
			remove_buff(*it, attacker_id);
		}
	}
	// �����ɢ
	else
	{
		uint32_t count = 0;
		uint32_t limit = 0;
		uint32_t max = 0;
		int32_t rand_pos = 0;
		while (limit++ < 200 && count < num)	// ����ѭ��
		{
			max = temp.size() - 1;
			rand_pos = random_util_t::randBetween(0, max);
			if (rand_pos == -1)
			{
				log_error("dispel_buff rand error min[%d] max[%d]", 0, max);
				continue;
			}
			if (in_remove_set(temp[rand_pos]))
				continue;
			remove_buff(temp[rand_pos], attacker_id);
			temp.erase(temp.begin() + rand_pos);
			++count;
		}
		p_attacker->m_buff_mgr.add_dispel_buff_count(count);
	}
}

uint32_t buff_manager_t::dispel_buff(uint32_t type, uint32_t num, uint64_t attacker_id)
{
	buff_vec temp;
	buff_vec::iterator it;
	buff_ptr p_buff = buff_ptr();
	for (it = m_buffs.begin(); it != m_buffs.end(); ++it)
	{
		p_buff = *it;
		if (NULL == p_buff)
			continue;

		if (p_buff->get_conf() != NULL && p_buff->get_big_type() == type)
		{
			temp.push_back(p_buff);
		}
	}

	// ȫ����ɢ
	if (num >= temp.size())
	{
		for (it = temp.begin(); it != temp.end(); ++it)
		{
			remove_buff(*it, attacker_id);
		}
	}
	// �����ɢ
	else
	{
		uint32_t count = 0;
		uint32_t limit = 0;
		uint32_t max = temp.size() - 1;
		int32_t rand_pos = 0;
		while (limit++ < 200 && count < num)	// ����ѭ��
		{
			rand_pos = random_util_t::randBetween(0, max);
			if (rand_pos == -1)
			{
				log_error("dispel_buff rand error min[%d] max[%d]", 0, max);
				continue;
			}
			if (in_remove_set(temp[rand_pos]))
				continue;
			remove_buff(temp[rand_pos], attacker_id);
			++count;
		}
	}

	return 0;
}

buff_ptr buff_manager_t::get_buff_by_id(const uint32_t id) const
{
	for (auto buff : m_buffs)
	{
		if (NULL != buff)
		{
			if (buff->get_id() == id)
			{
				return buff;
			}
		}
	}

	return buff_ptr();
}

buff_ptr buff_manager_t::get_buff_by_small_type(const uint32_t small_type, uint64_t attacker_id) const
{
	for (auto buff : m_buffs)
	{
		if (NULL == buff)
			continue;

		if (buff->get_small_type() != small_type)
			continue;

		if (attacker_id == 0)
			return buff;
		if (buff->get_attacker() == attacker_id)
			return buff;
	}

	return buff_ptr();
}

buff_ptr buff_manager_t::get_buff_by_tid(const uint32_t tid, uint64_t attacker_id) const
{
	for (auto buff : m_buffs)
	{
		if (NULL == buff)
			continue;
		
		if (buff->get_tid() != tid)
			continue;
		
		if (attacker_id == 0)
			return buff;
		if (buff->get_attacker() == attacker_id)
			return buff;
	}

	return buff_ptr();
}

buff_ptr buff_manager_t::get_random_buff_by_type(const uint32_t type)
{
	std::vector<buff_ptr> find;
	for (auto buff : m_buffs)
	{
		if (NULL != buff)
		{
			if (buff->get_small_type() == type)
			{
				find.push_back(buff);
			}
		}
	}

	if (find.empty())
	{
		return buff_ptr();
	}

	int32_t index = random_util_t::randMin(0, find.size());
	if (index == -1)
	{
		log_error("get_random_buff_by_type index = -1 min[%d] max[%d]", 0, find.size());
		return NULL;
	}
	return find[index];
}

buff_ptr buff_manager_t::add_new_buff(buff_ptr buff)
{
	if (NULL == buff)
	{
		log_error("add_new_buff buff is null");
		return buff_ptr();
	}

	if (get_owner() != NULL)
		get_owner()->add_combat_buff_act(proto::common::combat_act_type_buff, buff->get_id(), buff->get_tid(), buff->get_layer(), buff->get_round(), buff->get_attacker());

	uint32_t notify = 0;
	notify |= run_buff(buff);

	m_buffs.push_back(buff);

	get_owner()->get_combat()->add_round_buff_add_count();

	return buff;
}

buff_ptr buff_manager_t::cover_buff(buff_ptr os, BuffTable* ns, uint32_t layer)
{
	if (NULL == ns)
	{
		log_error("cover_buff ns is null");
		return buff_ptr();
	}

	if (NULL == os)
	{
		log_error("cover_buff os is null");
		return buff_ptr();
	}

	if (ns->overlay_type() == overlay_type_none)
	{
		log_error("overlay_type is 0");
		return buff_ptr();
	}

	uint32_t notify = 0;
	int numchange = 0;

	

	if (ns->overlay_type() == overlay_type_count)
	{
		os->set_round(os->get_round() + ns->rounds());
//		os->set_count(os->get_count() + ns->get_count());
	}
	else if (ns->overlay_type() == overlay_type_layer)
	{
		uint32_t max_layer = ns->max_layer();
		if (max_layer > os->get_layer() + layer)
		{
			numchange = layer;
			os->add_layer(numchange);
		}
		else
		{
			numchange = max_layer - os->get_layer();
			os->set_layer(max_layer);
		}

		os->set_step(buff_step_layer_change);
	}

	if (get_owner() != NULL)
		get_owner()->add_combat_buff_act(proto::common::combat_act_type_buff, os->get_id(), os->get_tid(), os->get_layer(), os->get_round(), os->get_attacker());

	notify |= run_buff(os, numchange);
	FIGHT_LOG("cover_buff buff_id[%d:%d] step[%d]", os->get_id(), os->get_tid(), os->get_step());
	return os;
}

uint32_t buff_manager_t::run_buff(buff_ptr buff, const int numchange /*= 0*/)
{
	if (NULL == buff)
	{
		log_error("run_buff buff is null");
		return 0;
	}

	BuffTable* conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("run_buff conf is null");
		return 0;
	}

	FIGHT_LOG("run_buff buff id[%d:%d] step[%d]", buff->get_id(), buff->get_tid(), buff->get_step());
	uint32_t notify = 0;
	std::vector<buff_element_data>::const_iterator it;
	for (it = conf->elements().begin(); it != conf->elements().end(); ++it)
	{
		//const buff_element_data& value = conf->elements();
		const buff_element_data& value = *it;
		uint32_t element_index = 0;
		if (m_funclist.find(value.id) != m_funclist.end())
		{
			element_index = value.id;
		}
		else
		{
			log_error("buff_manager_t::run_buff [%d:%d] can not find element %u", buff->get_id(), buff->get_tid(), value.id);
			return 0;
		}

		if (element_index > 0)
		{
			notify |= m_funclist[element_index](get_owner(), buff, value, numchange);

			FIGHT_LOG("run_buff deal_func buff id[%d:%d] func[%d]", buff->get_id(), buff->get_tid(), element_index);
		}
	}

// 	if (buff->get_step() == buff_step_load
// 		|| buff->get_step() == buff_step_layer_change)
// 	{
// 		buff->set_step(buff_step_trigger);
// 	}

	return notify;
}

uint32_t buff_manager_t::get_count_by_small_type(const int type)
{
	uint32_t count = 0;
	buff_ptr p_buff = buff_ptr();
	buff_vec::iterator it;
	for (it = m_buffs.begin(); it != m_buffs.end(); ++it)
	{
		p_buff = *it;
		if (p_buff == NULL)
			continue;
		if ((int)p_buff->get_small_type() == type)
			++count;
	}
	return count;
}

uint32_t buff_manager_t::get_count_by_big_type(const int type)
{
	uint32_t count = 0;
	buff_ptr p_buff = buff_ptr();
	buff_vec::iterator it;
	for (it = m_buffs.begin(); it != m_buffs.end(); ++it)
	{
		p_buff = *it;
		if (p_buff == NULL)
			continue;
		if ((int)p_buff->get_big_type() == type)
			++count;
	}
	return count;
}

uint32_t buff_manager_t::get_buff_count(buff_effect_type type)
{
	uint32_t count = 0;
	buff_ptr p_buff = buff_ptr();
	buff_vec::iterator it;
	for (it = m_buffs.begin(); it != m_buffs.end(); ++it)
	{
		p_buff = *it;
		if (p_buff == NULL)
			continue;
		if (p_buff->get_effect_type() == type)
			++count;
	}
	return count;
}

// ��ȡ����BUFF����
uint32_t buff_manager_t::get_debuff_count()
{
	return get_buff_count(buff_effect_type_debuff);
}
// ��ȡ����BUFF����
uint32_t buff_manager_t::get_profit_buff_count()
{
	return get_buff_count(buff_effect_type_gain);	
}

void buff_manager_t::add_passive_buff_cd(buff_ptr buff)
{
	// �жϼ����Ƿ񱻶�����
	SkillEffectTable* skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(buff->get_skill(), 1));
	if (NULL == skill_conf)
		return;
	// �ж��Ƿ񱻶�����
	if (skill_conf->type() != proto::common::skill_type_passive)
		return;
	get_owner()->add_skill_cd(buff->get_skill());
}

bool buff_manager_t::check_passive_buff_cd(buff_ptr buff)
{
	// �жϼ����Ƿ񱻶�����
	SkillEffectTable* skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(buff->get_skill(), 1));
	if (NULL == skill_conf)
		return true;
	// �ж��Ƿ񱻶�����
	if (skill_conf->type() != proto::common::skill_type_passive)
		return true;
	if( get_owner()->get_skill_cd( buff->get_skill() ) == 0 )
		return true;
	return false;
}

void buff_manager_t::on_round_start()
{
	clear_trigger_count();
	clear_dispel_buff_count();
}

void buff_manager_t::clear_trigger_count()
{
	buff_vec::iterator it;
	for (it = m_buffs.begin(); it != m_buffs.end(); ++it)
	{
		buff_ptr buff = *it;
		if (NULL == buff)
		{
			continue;
		}
		buff->set_trigger_count(0);
	}
	for (it = m_special_buffs.begin(); it != m_special_buffs.end(); ++it)
	{
		buff_ptr buff = *it;
		if (NULL == buff)
		{
			continue;
		}
		buff->set_trigger_count(0);
	}
}

bool buff_manager_t::update_buff(FIGHT_EVENT check_type)
{
	if (NULL == get_owner())
	{
		return false;
	}

	uint32_t notify = 0;
	buff_vec copy(m_buffs);
	buff_vec::iterator it;
	for (it = copy.begin(); it != copy.end(); ++it)
	{
		buff_ptr buff = *it;
		if (NULL == buff)
		{
			continue;
		}

		buff->set_step(buff_step_trigger);

		if (check_trigger(check_type, buff, notify))
		{
			buff->add_trigger_count();
		}
		check_remove_layer(check_type, buff, notify);
		check_remove(check_type, buff, notify);
	}

	// ������ȴ״̬�޷������Ч��
	if(get_owner()->get_attr( attrtype_enum::forget ) > 0 )
		return true;

	buff_vec copy_special(m_special_buffs);
	for (it = copy_special.begin(); it != copy_special.end(); ++it)
	{
		buff_ptr buff = *it;
		if (NULL == buff)
			continue;
		// ���BUFF�Ƿ���CD״̬
		if(!check_passive_buff_cd(buff))
			continue;

		buff->set_step(buff_step_trigger);

		if (check_trigger(check_type, buff, notify) == true)
		{
			// ��ӱ���BUFFCD
			add_passive_buff_cd(buff);
			buff->add_trigger_count();
			if( buff->get_attacker() == get_owner()->get_uid() )
				get_owner()->send_trigger_passive_to_user(buff->get_skill(), buff->get_id(), buff->get_tid());
		}
	}

	return true;
}

bool buff_manager_t::check_trigger(FIGHT_EVENT check_type, buff_ptr buff, uint32_t& notify, bool needprobability)
{
	if (NULL == buff)
	{
		log_error("buff is null");
		return false;
	}
	
	if (check_type == event_on_round_end)
	{
		// ���buff�м�¼�佫��ӻغ� ��ǰ�佫�غ�==buff�佫�غ�ʱ�����غ���
		if (buff->get_hero_round() == 0)
		{
			buff->sub_round();
		}
		else
		{
			if (buff->get_hero_round() != get_owner()->get_round())
				buff->sub_round();
		}

		if (NULL != get_owner())
		{
			get_owner()->add_combat_buff_act(proto::common::combat_act_type_buff_update, buff->get_id(), buff->get_tid(), buff->get_layer(), buff->get_round(), buff->get_attacker());
		}
	}

	if (buff->get_trigger_type() != check_type)
	{
		return false;
	}

	if (buff->get_max_trigger_count() != 0)
	{
		if (buff->get_trigger_count() >= buff->get_max_trigger_count())
			return false;
	}

	uint32_t rand_value = random_util_t::randBetween(1, 10000);
	BuffTable *p_conf = buff->get_conf();
	if (needprobability == true)
	{
		if (p_conf != NULL && p_conf->probability() < rand_value)
		{
			return false;
		}
	}

// 	if (check_type == buff_check_type_before_attack || check_type == buff_check_type_after_attack ||
// 		check_type == buff_check_type_before_be_attack || check_type == buff_check_type_after_be_attack)
// 	{
// 		buff->sub_layer();
// 	}

	notify |= run_buff(buff);
	FIGHT_LOG("check_trigger buff_id[%d:%d] step[%d] type[%d]", buff->get_id(), buff->get_tid(), buff->get_step(), check_type);

	return notify != (uint32_t)buff_notify_none;
}

bool buff_manager_t::check_remove_layer(FIGHT_EVENT check_type, buff_ptr buff, uint32_t& notify)
{
	if (NULL == buff)
	{
		log_error("buff is null");
		return false;
	}

	if (buff->get_layer_remove_type() != check_type)
	{
		return false;
	}

	sub_buff_layer(buff, 1);
	return true;
}

bool buff_manager_t::check_remove(FIGHT_EVENT check_type, buff_ptr buff, uint32_t& notify)
{
	if (NULL == buff)
	{
		log_error(" buff is null");
		return false;
	}

	if (buff->get_remove_type() == buff_remove_type_cur_round && check_type == event_on_round_end)
	{
		notify |= remove_buff(buff, buff->get_attacker());
		return true;
	}

	if (buff->get_round() == 0)
	{
		notify |= remove_buff(buff, buff->get_attacker());
		return true;
	}

// 	if (buff->get_count() == 0)
// 	{
// 		notify |= remove_buff(buff, get_owner()->get_attacker());
// 		return true;
// 	}

	if (buff->get_layer() == 0)
	{
		notify |= remove_buff(buff, buff->get_attacker());
		return true;
	}

	return false;
}

void buff_manager_t::remove_all_buff()
{
	buff_vec copy(m_buffs);
	buff_vec::iterator it;
	for (it = copy.begin(); it != copy.end(); ++it)
	{
		remove_buff(*it);
	}
}

void buff_manager_t::remove_buff_when_dead()
{
	buff_vec copy(m_buffs);
	buff_vec::iterator it;
	for (it = copy.begin(); it != copy.end(); ++it)
	{
		buff_ptr buff = *it;
		if (NULL == buff)
		{
			continue;
		}

		if (!buff->need_remove_when_dead())
		{
			continue;
		}

		remove_buff(buff);
	}
}

bool buff_manager_t::in_remove_set(buff_ptr buff) const
{
	if (NULL == buff)
	{
		log_error("in_remove_set buff is null");
		return false;
	}

	for (auto remove_buff : m_remove_set)
	{
		if (NULL == remove_buff)
		{
			continue;
		}

		if (remove_buff->get_id() == buff->get_id())
		{
			return true;
		}
	}

	return false;
}

bool buff_manager_t::in_remove_set(uint32_t buff_id) const
{
	for (auto remove_buff : m_remove_set)
	{
		if (NULL == remove_buff)
		{
			continue;
		}

		if (remove_buff->get_id() == buff_id)
		{
			return true;
		}
	}

	return false;
}

fight_hero_ptr buff_manager_t::get_owner()
{
	return m_owner.lock();
}

void buff_manager_t::peek_all_buff_data(proto::common::fight_buff_data *buff_data)
{
	if (NULL == buff_data)
	{
		log_error("buff_manager_t peek_all_buff_data buff_data NULL");
		return;
	}
	buff_ptr p_buff = buff_ptr();
	proto::common::fight_buff_single_data *single_data = NULL;
	for (uint32_t i = 0; i < m_buffs.size(); ++i)
	{
		p_buff = m_buffs[i];
		if (NULL == p_buff)
		{
			log_error("buff_manager_t peek_all_buff_data p_buff NULL pos[%d]", i);
			continue;
		}
		single_data = buff_data->add_buff_list();
		p_buff->peek_data(single_data);
	}
}

void buff_manager_t::add_trigger_count(const uint32_t id, uint32_t count /*= 1*/)
{
	buff_ptr buff = get_buff_by_id(id);
	if (NULL == buff)
	{
		log_error("buff is null %u", id);
		return;
	}

	buff->add_count(count);
}

void buff_manager_t::sub_trigger_count(const uint32_t id, uint32_t count /*= 1*/)
{
	buff_ptr buff = get_buff_by_id(id);
	if (NULL == buff)
	{
		log_error("buff is null %u", id);
		return;
	}

	buff->sub_count(count);
}

bool buff_manager_t::add_buff_layer(const uint32_t tid, uint64_t attacker, uint32_t count)
{
	buff_ptr buff = get_buff_by_tid(tid, attacker);
	if (NULL == buff)
		return false;
	
	return add_buff_layer(buff, count);
}

void buff_manager_t::sub_buff_layer(const uint32_t tid, uint64_t attacker, uint32_t count)
{
	buff_ptr buff = get_buff_by_tid(tid, attacker);
	if (NULL != buff)
	{
		sub_buff_layer(buff, count);
	}
}

bool buff_manager_t::add_buff_layer(buff_ptr buff, uint32_t count )
{
	if (NULL == buff)
	{
		log_error("buff is null");
		return false;
	}
	if (count == 0)
		return false;

	uint32_t real_count = count;
	if (buff->get_layer() + count >= buff->get_max_layer())
	{
		real_count = buff->get_max_layer() - buff->get_layer();
	}
	if (real_count > 0)
		return false;

	uint32_t notify = 0;
	int numchange = real_count;
	buff->add_layer(real_count);
	buff->set_step(buff_step_layer_change);

	if (NULL != get_owner())
	{
		get_owner()->add_combat_buff_act(proto::common::combat_act_type_buff_update, buff->get_id(), buff->get_tid(), buff->get_layer(), buff->get_round(), buff->get_attacker());
	}

	notify |= run_buff(buff, numchange);
	return real_count > 0;
}

bool buff_manager_t::sub_buff_layer(buff_ptr buff, uint32_t count /*= 1*/)
{
	if (NULL == buff)
	{
		log_error("buff is null");
		return false;
	}

	uint32_t real_count = count;
	if (buff->get_layer() < count)
	{
		real_count = buff->get_layer();
	}

	uint32_t notify = 0;
	int numchange = -real_count;
	buff->sub_layer(real_count);
	buff->set_step(buff_step_layer_change);

	if (NULL != get_owner())
	{
		get_owner()->add_combat_buff_act(proto::common::combat_act_type_buff_update, buff->get_id(), buff->get_tid(), buff->get_layer(), buff->get_round(), buff->get_attacker());
	}

	notify |= run_buff(buff, numchange);
	return buff->get_layer() <= 0 ;
}

void buff_manager_t::sub_buff_layer_or_remove(const uint32_t tid, uint64_t attacker, uint32_t count)
{
	buff_ptr buff = get_buff_by_tid(tid, attacker);
	if (NULL == buff)
		return;

	if (sub_buff_layer(buff, count) == true)
	{
		remove_buff(buff, attacker);
	}

}

uint32_t buff_manager_t::get_buff_layer(const uint32_t id)
{
	buff_ptr buff = get_buff_by_id(id);
	if (NULL == buff)
	{
		return 0;
	}

	return buff->get_layer();
}

uint32_t buff_manager_t::get_buff_layer_by_tid(uint32_t tid, uint64_t attacker)
{
	buff_ptr buff = get_buff_by_tid(tid, attacker);
	if (NULL == buff)
	{
		return 0;
	}

	return buff->get_layer();
}

uint32_t buff_manager_t::get_buff_round(const uint32_t id)
{
	buff_ptr buff = get_buff_by_id(id);
	if (NULL == buff)
	{
		return 0;
	}

	return buff->get_round();
}

uint32_t buff_manager_t::get_buff_round_by_tid(uint32_t tid, uint64_t attacker)
{
	buff_ptr buff = get_buff_by_tid(tid, attacker);
	if (NULL == buff)
	{
		return 0;
	}

	return buff->get_round();
}

uint64_t buff_manager_t::get_buff_attacker(const uint32_t id)
{
	buff_ptr buff = get_buff_by_id(id);
	if (NULL == buff)
	{
		return 0;
	}

	return buff->get_attacker();
}

buff_ptr buff_manager_t::add_special_buff(const uint32_t id, const uint64_t attacker, const uint32_t skill, const uint32_t layer)
{
	buff_ptr buff = buff_ptr();

	combat_ptr p_combat = get_owner()->get_combat();
	if (NULL == p_combat)
	{
		log_error("buff_manager_t p_combat NULL");
		return buff;
	}

	BuffTable* conf = GET_CONF(BuffTable, id);
	if (NULL == conf)
	{
		log_error("buff id invalid %u", id);
		return buff_ptr();
	}

	uint32_t buff_id = p_combat->gen_buff_id();
	buff.reset(new buff_t(buff_id, conf, attacker, skill, layer));

	add_new_special_buff(buff);
	return buff;
}

bool buff_manager_t::remove_special_buff(uint32_t id, uint64_t attacker_id)
{
	buff_ptr p_buff = buff_ptr();
	for (uint32_t i = 0; i < m_special_buffs.size(); ++i)
	{
		p_buff = m_special_buffs[i];
		if (NULL != p_buff)
		{
			log_error("remove_special_buff p_buff NULL pos[%d]", i);
			continue;
		}
		if (p_buff->get_id() == id)
		{
			return remove_special_buff(p_buff, attacker_id);
		}
	}
	return false;
}

bool buff_manager_t::remove_special_buff_by_tid(uint32_t tid, uint64_t attacker_id)
{
	buff_ptr p_buff = buff_ptr();
	for (uint32_t i = 0; i < m_special_buffs.size(); ++i)
	{
		p_buff = m_special_buffs[i];
		if (NULL != p_buff)
		{
			log_error("remove_special_buff p_buff NULL pos[%d]", i);
			continue;
		}
		if (p_buff->get_tid() == tid)
		{
			return remove_special_buff(p_buff, attacker_id);
		}
	}
	return false;
}

void buff_manager_t::remove_all_special_buff()
{
	buff_vec copy(m_special_buffs);
	buff_vec::iterator it;
	for (it = copy.begin(); it != copy.end(); ++it)
	{
		remove_special_buff(*it);
	}
}

void buff_manager_t::add_new_special_buff(buff_ptr buff)
{
	if (NULL == buff)
	{
		log_error("add_un_initiative_buff buff is null");
		return;
	}
	m_special_buffs.push_back(buff);
	run_buff(buff);
}

bool buff_manager_t::remove_special_buff(buff_ptr buff, uint32_t attacker_id /*= 0*/)
{
	if (NULL == buff)
	{
		log_error("remove_special_buff buff is null");
		return false;
	}
	if (m_special_buffs.size() == 0)
	{
		log_error("remove_special_buff m_special_buffs size = 0 but remove buff[%d:%d]", buff->get_id(), buff->get_tid());
		return false;
	}

	buff->set_step(buff_step_unload);

	buff_vec::iterator iter = std::find(m_special_buffs.begin(), m_special_buffs.end(), buff);
	if (iter == m_special_buffs.end())
	{
		log_error("remove_special_buff not find buff[%d:%d]", buff->get_id(), buff->get_tid());
		return 0;
	}
	m_special_buffs.erase(iter);
	FIGHT_LOG("remove_special_buff id[%d:%d]", buff->get_id(), buff->get_tid());

	return run_buff(buff);
}