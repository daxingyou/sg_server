#include "offline_arena_record.hpp"
#include "log.hpp"
#include "utility.hpp"
USING_NS_COMMON;

void offline_arena_record_t::peek_data(proto::common::offline_arena_record* p_single)
{
	if (NULL == p_single)
	{
		log_error("NULL == p_single");
		return;
	}
	p_single->set_object_type((proto::common::EM_SCENEOBJECT_TYPE)m_enemy_type);
	p_single->set_role_uid(m_enemy_uid);
	p_single->set_role_name(m_enemy_name);
	p_single->set_fighting(m_enemy_fighting);
	p_single->set_plugin(m_enemy_plugin);
	p_single->set_level(m_enemy_level);
	p_single->set_record_time(m_record_time);
	p_single->set_is_win(m_is_win);
	p_single->set_old_rank(m_old_rank);
	p_single->set_new_rank(m_new_rank);
	p_single->set_side(m_side);
}