#include "tower_data_manager.hpp"
#include "redis_client.hpp"
#include "utility.hpp"
USING_NS_COMMON;
tower_data_manager::tower_data_manager()
{
}

tower_data_manager::~tower_data_manager()
{
}

bool tower_data_manager::save_tower_data_to_redis(uint64_t uid, const proto::common::tower_data& td)
{
	REDIS_BATCH_START
	redis_client_t::set_uint32("role_tower", "record_tower", uid, td.record_tower());
	redis_client_t::set_uint32("role_tower", "current_tower", uid, td.current_tower());
	redis_client_t::set_uint32("role_tower", "reset_count", uid, td.reset_count());
	redis_client_t::set_uint32("role_tower", "max_reset_count", uid, td.max_reset_count());
	redis_client_t::set_uint32("role_tower", "auto_fight_time", uid, td.auto_fight_time());
	redis_client_t::set_uint32("role_tower", "current_achieve", uid, td.current_achieve());
	redis_client_t::set_uint32("role_tower", "first_day", uid, td.first_day());
	redis_client_t::set_protobuf("role_tower", "tower_trigger_data", uid, td.tower_trigger());
	redis_client_t::set_uint32("role_tower", "trigger_count", uid, td.trigger_count());
	redis_client_t::set_uint32("role_tower", "trigger_fight_count", uid, td.trigger_fight_count());
	redis_client_t::set_uint32("role_tower", "send_mail_flag", uid, td.send_mail_flag());
	REDIS_BATCH_DONE
	return true;
}

bool tower_data_manager::load_tower_data_from_redis(uint64_t uid, proto::common::tower_data* p_data)
{
	if (NULL == p_data)
	{
		return false;
	}

	p_data->set_record_tower(redis_client_t::get_uint32("role_tower", "record_tower", uid));
	p_data->set_current_tower(redis_client_t::get_uint32("role_tower", "current_tower", uid));
	p_data->set_reset_count(redis_client_t::get_uint32("role_tower", "reset_count", uid));
	p_data->set_max_reset_count(redis_client_t::get_uint32("role_tower", "max_reset_count", uid));
	p_data->set_auto_fight_time(redis_client_t::get_uint32("role_tower", "auto_fight_time", uid));
	p_data->set_current_achieve(redis_client_t::get_uint32("role_tower", "current_achieve", uid));
	p_data->set_first_day(redis_client_t::get_uint32("role_tower", "first_day", uid));
	proto::common::tower_trigger_data* p_tower_trigger_data = p_data->mutable_tower_trigger();
	redis_client_t::get_protobuf("role_tower", "tower_trigger_data", uid, *p_tower_trigger_data);
	p_data->set_trigger_count(redis_client_t::get_uint32("role_tower", "trigger_count", uid));
	p_data->set_trigger_fight_count(redis_client_t::get_uint32("role_tower", "trigger_fight_count", uid));
	p_data->set_send_mail_flag(redis_client_t::get_uint32("role_tower", "send_mail_flag", uid));
	return true;
}
