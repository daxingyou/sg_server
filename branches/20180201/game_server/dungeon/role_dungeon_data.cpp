#include "role_dungeon_data.hpp"
#include "cache_key.hpp"
#include "log.hpp"
#include "role/role_unify_save.hpp"
#include "config/config_manager.hpp"
#include "dungeon_common.hpp"
#include "task/task_manager.hpp"
#include "role/role.hpp"

role_dungeon_data_t::role_dungeon_data_t(uint64_t uid)
	: m_role_uid(uid)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::dungeon);
}

role_dungeon_data_t::~role_dungeon_data_t()
{

}

void role_dungeon_data_t::load_data(const proto::common::role_all_dungeon_data& data)
{
	m_dungeon_data_map.clear();

	for (int32_t i = 0; i < data.datas_size(); ++i)
	{
		const proto::common::role_dungeon_data& dungeon_data = data.datas(i);

		dungeon_data_ptr p_dungeon_data(new dungeon_data_t());
		if (p_dungeon_data == NULL)
		{
			log_error("p_dungeon_data null error");
			continue;
		}

		p_dungeon_data->dungeon_id = dungeon_data.dungeon_id();
		p_dungeon_data->total_pass_time = dungeon_data.total_pass_time();
		p_dungeon_data->refresh_time = dungeon_data.refresh_time();

		//DUNGEON_LOG("role[%lu] dungeon[%u] total_time[%u] refresh_time[%u]", m_role_uid, p_dungeon_data->dungeon_id, p_dungeon_data->total_pass_time, p_dungeon_data->refresh_time);

		for (int32_t j = 0; j < dungeon_data.monsters_size(); ++j)
		{
			const proto::common::role_dungeon_monster& dungeon_data_monster = dungeon_data.monsters(j);

			p_dungeon_data->rwds.insert(std::make_pair(dungeon_data_monster.monster_tid(), dungeon_data_monster.num()));

			//DUNGEON_LOG("role[%lu] dungeon[%u] monster_tid[%u] num[%u]", m_role_uid, p_dungeon_data->dungeon_id, dungeon_data_monster.monster_tid(), dungeon_data_monster.num());
		}

		p_dungeon_data->best_score = dungeon_data.best_score();
		p_dungeon_data->best_pass_time = dungeon_data.best_pass_time();
		p_dungeon_data->best_death_value = dungeon_data.best_death_value();

		m_dungeon_data_map.insert(std::make_pair(p_dungeon_data->dungeon_id, p_dungeon_data));
	}
}

void role_dungeon_data_t::peek_data(const role_ptr& p_role, proto::common::role_all_client_dungeon_data* p_data)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return;
	}

	if (p_data == NULL) {
		log_error("p_data null error");
		return;
	}

	std::map<uint32_t, DungeonTable*> all_dungeons;
	GET_ALL_CONF(DungeonTable, all_dungeons);

	uint32_t now_time = common::time_util_t::now_time();

	for (std::map<uint32_t, DungeonTable*>::iterator itr = all_dungeons.begin();
		itr != all_dungeons.end(); ++itr) {
		proto::common::role_client_dungeon_data* p_client_dungeon_data = p_data->add_datas();
		if (!p_client_dungeon_data) {
			log_error("p_client_dungeon_data null error");
			continue;
		}

		p_client_dungeon_data->set_dungeon_id(itr->first);
		dungeon_data_ptr p_dungeon_data = get_dungeon_data(itr->first);
		if (p_dungeon_data != NULL) {
			proto::common::role_dungeon_data* p_data_one = p_client_dungeon_data->mutable_data();
			if (p_data_one) {
				p_data_one->set_dungeon_id(itr->first);
				p_data_one->set_total_pass_time(p_dungeon_data->total_pass_time);
				p_data_one->set_refresh_time(p_dungeon_data->refresh_time);

				//DUNGEON_LOG("role[%lu] dungeon[%u] total_time[%u] refresh_time[%u]", m_role_uid, p_dungeon_data->dungeon_id, p_dungeon_data->total_pass_time, p_dungeon_data->refresh_time);

				// 还在刷新期内
				if (now_time < p_dungeon_data->refresh_time) {
					for (const auto& rwd : p_dungeon_data->rwds) {
						proto::common::role_dungeon_monster* p_dungeon_monster = p_data_one->add_monsters();
						if (p_dungeon_monster) {
							p_dungeon_monster->set_monster_tid(rwd.first);
							p_dungeon_monster->set_num(rwd.second);
						}

						//DUNGEON_LOG("role[%lu] dungeon[%u] monster_tid[%u] num[%u]", m_role_uid, p_dungeon_data->dungeon_id, rwd.first, rwd.second);
					}
				}
			}
		} else {
			DungeonTable* p_dungeon_table = itr->second;
			if (p_dungeon_table) {
				for (const auto& item : p_dungeon_table->open_condition()) {
					proto::common::role_dungeon_condition* p_cond = p_client_dungeon_data->add_cond();
					if (!p_cond) {
						log_error("p_cond null error");
						continue;
					}

					p_cond->set_type(item.first);
					p_cond->set_param(item.second);
					switch (item.first) {
						case proto::common::dungeon_condition_type_task: {
							task_mgr_ptr p_task_mgr_ptr = p_role->get_task_mgr();
							if (p_task_mgr_ptr == NULL || !p_task_mgr_ptr->is_done(item.second)) {
								p_cond->set_is_pass(false);
							} else {
								p_cond->set_is_pass(true);
							}
							break;
						}
						case proto::common::dungeon_condition_type_pass: {
							if (get_dungeon_pass_time(item.second) == 0) {
								p_cond->set_is_pass(false);
							} else {
								p_cond->set_is_pass(true);
							}
							break;
						}
						default: {
							p_cond->set_is_pass(false);
							log_error("dungeon[%u] condition type[%u] not define", itr->first, item.first);
						}
					}
				}
			}
		}
	}
}

void role_dungeon_data_t::save_data(dungeon_data_ptr p_dungeon_data)
{
	if (p_dungeon_data == NULL)
	{
		log_error("p_dungeon_data null error");
		return;
	}

	//DUNGEON_LOG("role[%lu] dungeon[%u] total_time[%u] refresh_time[%u]", m_role_uid, p_dungeon_data->dungeon_id, p_dungeon_data->total_pass_time, p_dungeon_data->refresh_time);

	proto::common::role_dungeon_data data;
	data.set_dungeon_id(p_dungeon_data->dungeon_id);
	data.set_total_pass_time(p_dungeon_data->total_pass_time);
	data.set_refresh_time(p_dungeon_data->refresh_time);
	data.set_best_score(p_dungeon_data->best_score);
	data.set_best_pass_time(p_dungeon_data->best_pass_time);
	data.set_best_death_value(p_dungeon_data->best_death_value);
	for (const auto& rwd : p_dungeon_data->rwds)
	{
		proto::common::role_dungeon_monster* p_dungeon_monster = data.add_monsters();
		if (p_dungeon_monster != NULL)
		{
			p_dungeon_monster->set_monster_tid(rwd.first);
			p_dungeon_monster->set_num(rwd.second);

			//DUNGEON_LOG("role[%lu] dungeon[%u] monster_tid[%u] num[%u]", m_role_uid, p_dungeon_data->dungeon_id, rwd.first, rwd.second);
		}
	}

	role_unify_save::add_task(m_key, data, false, false, m_role_uid);
}

role_dungeon_data_t::dungeon_data_ptr role_dungeon_data_t::get_dungeon_data(uint32_t dungeon_id)
{
	dungeon_data_map::iterator itr = m_dungeon_data_map.find(dungeon_id);
	if (itr != m_dungeon_data_map.end())
	{
		return itr->second;
	}

	return NULL;
}

uint32_t role_dungeon_data_t::get_dungeon_pass_time(uint32_t dungeon_id)
{
	dungeon_data_ptr p_dungeon_data = get_dungeon_data(dungeon_id);
	if (p_dungeon_data != NULL)
	{
		return p_dungeon_data->total_pass_time;
	}

	return 0;
}

void role_dungeon_data_t::add_dungeon_pass(uint32_t dungeon_id)
{
	dungeon_data_ptr p_dungeon_data = get_dungeon_data(dungeon_id);
	if (p_dungeon_data == NULL)
	{
		p_dungeon_data.reset(new dungeon_data_t());

		if (p_dungeon_data == NULL)
		{
			log_error("p_dungeon_data null error");
			return;
		}

		p_dungeon_data->dungeon_id = dungeon_id;

		m_dungeon_data_map.insert(std::make_pair(dungeon_id, p_dungeon_data));
	}

	p_dungeon_data->total_pass_time += 1;

	save_data(p_dungeon_data);

	DUNGEON_LOG("role[%lu] add dungeon[%u] pass time", m_role_uid, dungeon_id);
}

uint32_t role_dungeon_data_t::get_best_score(uint32_t dungeon_id)
{
	dungeon_data_ptr p_dungeon_data = get_dungeon_data(dungeon_id);
	if (p_dungeon_data != NULL)
	{
		return p_dungeon_data->best_score;
	}

	return 0;
}

uint32_t role_dungeon_data_t::get_best_pass_time(uint32_t dungeon_id)
{
	dungeon_data_ptr p_dungeon_data = get_dungeon_data(dungeon_id);
	if (p_dungeon_data != NULL)
	{
		return p_dungeon_data->best_pass_time;
	}

	return 0;
}

uint32_t role_dungeon_data_t::get_best_death_value(uint32_t dungeon_id)
{
	dungeon_data_ptr p_dungeon_data = get_dungeon_data(dungeon_id);
	if (p_dungeon_data != NULL)
	{
		return p_dungeon_data->best_death_value;
	}

	return 0;
}

void role_dungeon_data_t::set_best_score(uint32_t dungeon_id, uint32_t best_score)
{
	dungeon_data_ptr p_dungeon_data = get_dungeon_data(dungeon_id);
	if (p_dungeon_data != NULL)
	{
		p_dungeon_data->best_score = best_score;

		save_data(p_dungeon_data);
	}
}

void role_dungeon_data_t::set_best_pass_time(uint32_t dungeon_id, uint32_t best_pass_time)
{
	dungeon_data_ptr p_dungeon_data = get_dungeon_data(dungeon_id);
	if (p_dungeon_data != NULL)
	{
		p_dungeon_data->best_pass_time = best_pass_time;

		save_data(p_dungeon_data);
	}
}

void role_dungeon_data_t::set_best_death_value(uint32_t dungeon_id, uint32_t best_death_value)
{
	dungeon_data_ptr p_dungeon_data = get_dungeon_data(dungeon_id);
	if (p_dungeon_data != NULL)
	{
		p_dungeon_data->best_death_value = best_death_value;

		save_data(p_dungeon_data);
	}
}

uint32_t role_dungeon_data_t::get_monster_kill_times(uint32_t dungeon_id, uint32_t monster_tid)
{
	dungeon_data_ptr p_dungeon_data = get_dungeon_data(dungeon_id);
	if (p_dungeon_data != NULL)
	{
		if (common::time_util_t::now_time() < p_dungeon_data->refresh_time)
		{
			std::map<uint32_t, uint32_t>::const_iterator citr = p_dungeon_data->rwds.find(monster_tid);
			if (citr != p_dungeon_data->rwds.end())
			{
				return citr->second;
			}
		}
	}

	return 0;
}

void role_dungeon_data_t::add_one_kill_times(DungeonTable* p_dungeontable, uint32_t monster_tid)
{
	if (NULL == p_dungeontable)
	{
		log_error("p_dungeontable null error");
		return;
	}

	dungeon_data_ptr p_dungeon_data = get_dungeon_data(p_dungeontable->id());
	if (p_dungeon_data == NULL)
	{
		p_dungeon_data.reset(new dungeon_data_t());

		if (p_dungeon_data == NULL)
		{
			log_error("p_dungeon_data null error");
			return;
		}

		p_dungeon_data->dungeon_id = p_dungeontable->id();

		m_dungeon_data_map.insert(std::make_pair(p_dungeon_data->dungeon_id, p_dungeon_data));
	}

	uint32_t now_time = common::time_util_t::now_time();

	if (p_dungeon_data->refresh_time < now_time)
	{
		p_dungeon_data->rwds.clear();

		switch (p_dungeontable->refresh_type())
		{
		case dungeon::refresh_type_day:
			{
				p_dungeon_data->refresh_time = (common::time_util_t::get_local_day(now_time) + 1) * DAY_SECOND + common::time_util_t::get_timezone();
			}
			break;
		case dungeon::refresh_type_week:
			{
				p_dungeon_data->refresh_time = (common::time_util_t::get_local_week(now_time) + 1) * WEEK_SECOND + common::time_util_t::get_timezone() + 4 * DAY_SECOND;
			}
			break;
		default:
			{
				log_error("dungeon[%u] refresh type[%u] error", p_dungeontable->id(), p_dungeontable->refresh_type());
				return;
			}
			break;
		}
	}

	p_dungeon_data->rwds[monster_tid] += 1;

	save_data(p_dungeon_data);

	DUNGEON_LOG("role[%lu] add dungeon[%u] monster[%u] kill time, curr_time[%u]", m_role_uid, p_dungeontable->id(), monster_tid, p_dungeon_data->rwds[monster_tid]);
}
