#include "log_wrapper.hpp"
#include "protos_fwd.hpp"
#include "utility.hpp"
#include "log_unify_save.hpp"
void log_wrapper_t::send_country_log(uint32_t country, uint32_t type, uint64_t param, uint32_t param2, uint32_t param3)
{
	proto::log::eo_country_log_notify msg;
	msg.set_country_id(country);
	msg.set_source_type(type);
	msg.set_source_param(param);
	msg.set_source_param2(param2);
	msg.set_source_param3(param3);

	msg.set_log_time(common::time_util_t::now_time());
	write_new_log(op_cmd::eo_country_log_notify, msg);
    
}

void log_wrapper_t::send_family_log(uint64_t family_uid, uint64_t role_uid, uint64_t target_uid, uint32_t type, uint32_t param, uint32_t param2 /*= 0*/)
{
	proto::log::eo_family_log_notify msg;
	msg.set_family_uid(family_uid);
	msg.set_role_uid(role_uid);
	msg.set_target_uid(target_uid);
	msg.set_source_type(type);
	msg.set_source_param(param);
	msg.set_source_param2(param2);
	msg.set_log_time(common::time_util_t::now_time());
	write_new_log(op_cmd::eo_family_log_notify, msg);
    
}

void log_wrapper_t::send_friend_log(uint64_t role_uid, uint64_t friend_uid, uint32_t old_type, uint32_t new_type, uint32_t type, uint32_t param)
{
	proto::log::eo_friend_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_friend_uid(friend_uid);
	msg.set_old_type(old_type);
	msg.set_new_type(new_type);
	msg.set_source_type(type);
	msg.set_source_param(param);

	msg.set_log_time(common::time_util_t::now_time());
	write_new_log(op_cmd::eo_friend_log_notify, msg);
    
}

void log_wrapper_t::send_mail_log(uint64_t role_uid, uint64_t mail_id, uint32_t mail_type, uint32_t item_type, const std::string& items, uint32_t item_state, uint32_t type, uint32_t param)
{
	proto::log::eo_mail_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_mail_id(mail_id);
	msg.set_mail_type(mail_type);
	msg.set_item_type(item_type);
	msg.set_items(items);
	msg.set_item_state(item_state);
	msg.set_source_type(type);
	msg.set_source_param(param);
	msg.set_log_time(common::time_util_t::now_time());
	write_new_log(op_cmd::eo_mail_log_notify, msg);
}

void log_wrapper_t::send_goods_log(uint64_t role_uid, const std::string& role_name, uint64_t goods_uid, uint32_t item_tid, uint32_t item_num, uint32_t price, uint32_t selled_num, uint32_t profit, uint32_t goods_state, uint32_t type)
{
	proto::log::eo_goods_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_role_name(role_name);
	msg.set_goods_uid(goods_uid);
	msg.set_item_tid(item_tid);
	msg.set_item_num(item_num);
	msg.set_price(price);
	msg.set_selled_num(selled_num);
	msg.set_profit(profit);
	msg.set_state(goods_state);
	msg.set_source_type(type);
	msg.set_log_time(common::time_util_t::now_time());
	write_new_log(op_cmd::eo_goods_log_notify, msg);
}

void log_wrapper_t::send_scene_log(uint64_t role_uid, uint32_t old_map, uint32_t new_map, uint32_t type)
{
	proto::log::eo_scene_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_old_map_tid(old_map);
	msg.set_new_map_tid(new_map);
	msg.set_source_type(type);
	msg.set_log_time(common::time_util_t::now_time());
	write_new_log(op_cmd::eo_scene_log_notify, msg);
}

void log_wrapper_t::send_redbag_log(uint64_t role_uid, const std::string& role_name, uint64_t redbag_uid, uint32_t total_count, uint32_t total_money, uint32_t draw_money, uint32_t rest_count, uint32_t rest_money, uint32_t type)
{
	proto::log::eo_redbag_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_role_name(role_name);
	msg.set_redbag_uid(redbag_uid);
	msg.set_total_count(total_count);
	msg.set_total_money(total_money);
	msg.set_draw_money(draw_money);
	msg.set_rest_count(rest_count);
	msg.set_rest_money(rest_money);
	msg.set_source_type(type);
	msg.set_log_time(common::time_util_t::now_time());
	write_new_log(op_cmd::eo_redbag_log_notify, msg);
}
