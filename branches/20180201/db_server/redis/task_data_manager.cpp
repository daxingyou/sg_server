#include "task_data_manager.hpp"
#include "common/redis_client.hpp"
#include "config/config_manager.hpp"
#include "tblh/Task.tbls.h"

task_data_manager::task_data_manager()
{
}

task_data_manager::~task_data_manager()
{
}

bool task_data_manager::save_task_data_to_redis(uint64_t role_uid, const proto::common::task_state& task)
{
	uint64_t uid = task.uid();
	redis_client_t::set_uint64("task", "role_uid", uid, role_uid);
	redis_client_t::set_uint32("task", "task_id", uid, task.id());
	redis_client_t::set_uint32("task", "type", uid, task.type());
	redis_client_t::set_string("task", "progress", uid, task.progress());
	redis_client_t::set_uint32("task", "state", uid, task.state());
	redis_client_t::set_uint32("task", "circle_count", uid, task.circle_count());
	redis_client_t::set_uint32("task", "circle_id", uid, task.circle_id());
	redis_client_t::set_uint32("task", "accept_level", uid, task.accept_level());
	redis_client_t::set_uint32("task", "star", uid, task.star());

	return true;
}

bool task_data_manager::load_task_data_from_redis(uint64_t uid, proto::common::task_data* data)
{
	if (data == NULL)
	{
		log_error("load_task_data_from_redis [%lu] data == null", uid);
		return false;
	}

	std::vector<uint64_t> task_uids;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint64_t, std::vector<uint64_t>>("task", "role_uid", uid, task_uids);

	for (auto task_uid : task_uids)
	{
		uint32_t task_tid = redis_client_t::get_uint32("task", "task_id", task_uid);
		auto conf = GET_CONF(Task, task_tid);
		if (NULL == conf)
		{
			continue;
		}

		proto::common::task_state* task = data->add_tasks();
		if (task == NULL)
			continue;
		task->set_uid(task_uid);
		task->set_id(task_tid);
		task->set_progress(redis_client_t::get_string("task", "progress", task_uid));
		task->set_type(redis_client_t::get_uint32("task", "type", task_uid));
		task->set_state(redis_client_t::get_uint32("task", "state", task_uid));
		task->set_circle_count(redis_client_t::get_uint32("task", "circle_count", task_uid));
		task->set_circle_id(redis_client_t::get_uint32("task", "circle_id", task_uid));
		task->set_accept_level(redis_client_t::get_uint32("task", "accept_level", task_uid));
		task->set_star(redis_client_t::get_uint32("task", "star", task_uid));
	}
	//试炼数据在这里
	data->set_shilian_count(redis_client_t::get_uint32("role_ex", "shilian_count", uid));
	std::string seq_info = redis_client_t::get_string("role_ex", "shilian_seq", uid);
	if (!seq_info.empty())
	{
		std::vector<uint32_t> seq;
		string_util_t::split<uint32_t>(seq_info, seq, "$");
		for (auto star : seq)
		{
			data->add_shilian_seq(star);
		}
	}

	//环任务数据
	std::string circle_info_str = redis_client_t::get_string("role_ex", "circle_task_comp_list", uid);
	if (!circle_info_str.empty())
	{
		std::vector<uint32_t> seq;
		string_util_t::split<uint32_t>(circle_info_str, seq, "$");
		for (auto it: seq)
		{
			data->add_circle_task_comp_list(it);
		}
	}

	return true;
}

bool task_data_manager::save_shilian_data_to_redis(uint64_t uid, const proto::common::task_data& data)
{
	//试炼数据存放在role_ex
	redis_client_t::set_uint32("role_ex", "shilian_count", uid, data.shilian_count());
	std::ostringstream sss;
	for (auto star : data.shilian_seq())
	{
		if (!sss.str().empty())
		{
			sss << "$";
		}

		sss << star;
	}

	redis_client_t::set_string("role_ex", "shilian_seq", uid, sss.str());

	std::ostringstream sss2;
	for (auto id: data.circle_task_comp_list())
	{
		if (!sss2.str().empty())
		{
			sss2 << "$";
		}

		sss2 << id;
	}

	redis_client_t::set_string("role_ex", "circle_task_comp_list", uid, sss2.str());
	return true;
}
