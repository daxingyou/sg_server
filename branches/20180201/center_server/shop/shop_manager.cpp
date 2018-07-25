#include "shop_manager.hpp"

#include "log.hpp"
#include "utility.hpp"
#include "config_mgr.h"
#include "time_manager.hpp"
#include "tblh/TimeTable.tbls.h"
#include "redis/shop_data_mgr.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/Item.tbls.h"
#include "tblh/sys_notice_enum.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "tblh/time_type_enum.hpp"

shop_manager_t::goods_map shop_manager_t::m_goods_map;
shop_manager_t::goods_list_map shop_manager_t::m_goods_list_map;
shop_manager_t::puton_timer_map	shop_manager_t::m_puton_time_map;

bool shop_manager_t::init()
{
	// 加载redis
	shop_data_mgr_t::load_shop_goods(m_goods_map);

	// 刷新时间回掉注册
	time_manager_t::register_func(time_type_enum::time_shop1, shop_manager_t::day_refresh_callback);
	time_manager_t::register_func(time_type_enum::time_shop2, shop_manager_t::week_refresh_callback);
	time_manager_t::register_func(time_type_enum::time_shop3, shop_manager_t::month_refresh_callback);

	return reload();
}

bool shop_manager_t::close()
{
	m_puton_time_map.clear();
	m_goods_map.clear();
	m_goods_list_map.clear();

	return true;
}

bool shop_manager_t::reload()
{
	// 清空数据
	m_goods_list_map.clear();

	// 清理上架时间回调
	clear_register_puton_timer();

	std::map<uint32_t, ShopTable*> all_confs;
	GET_ALL_CONF(ShopTable, all_confs);

	uint32_t time_now = common::time_util_t::now_time();

	for (std::map<uint32_t, ShopTable*>::iterator citr = all_confs.begin();
		citr != all_confs.end(); ++citr)
	{
		ShopTable* p_shop_conf = citr->second;

		if (p_shop_conf == NULL)
		{
			log_error("shop goods[%u] null error", citr->first);
			return false;
		}

		uint32_t shoptype = p_shop_conf->shop_type();

		proto::common::shop_goods* p_goods = m_goods_list_map[shoptype].add_goods_list();
		if (p_goods == NULL)
		{
			log_error("goods[%u] add list null error", citr->first);
			return false;
		}

		p_goods->set_goods_id(citr->first);

		// 服务器不限购，并且没有上架类型
		if (p_shop_conf->server_limit() == 0 && p_shop_conf->puton_type() == shop_puton_type::shop_puton_type_none)
		{
			continue;
		}

		shop_goods_ptr p_shop_goods = update_goods_time(citr->first, time_now, p_shop_conf);
		if (NULL == p_shop_goods)
		{
			log_error("p_shop_goods[%u] update error", citr->first);
			return false;
		}

		if (p_shop_goods->m_puton_time != 0)
		{
			// 注册上架时间
			m_puton_time_map[p_shop_goods->m_puton_time].insert(citr->first);
		}

		p_goods->set_buy_count(p_shop_goods->m_buy_count);
		p_goods->set_start_time(p_shop_goods->m_start_time);
		p_goods->set_end_time(p_shop_goods->m_end_time);
		p_goods->set_last_refresh_time(p_shop_goods->m_last_refresh_time);
	}

	register_puton_timer();

	return true;
}

shop_manager_t::shop_goods_ptr shop_manager_t::get_shop_goods(uint32_t id)
{
	goods_map::iterator itr = m_goods_map.find(id);
	if (itr != m_goods_map.end())
	{
		return itr->second;
	}

	return NULL;
}

shop_manager_t::shop_goods_ptr shop_manager_t::update_goods_time(uint32_t id, uint32_t time_now, ShopTable* p_shop_conf)
{
	if (p_shop_conf == NULL)
	{
		log_error("goods[%u] p_shop_conf null error", id);
		return NULL;
	}

	bool is_need_save = false;
	shop_goods_ptr p_shop_goods = get_shop_goods(id);
	// 表示数据库还没记录，增加一条
	if (p_shop_goods == NULL)
	{
		p_shop_goods = shop_goods_ptr(new shop_goods_t());
		if (NULL == p_shop_goods)
		{
			log_error("goods[%u] p_shop_goods new null error", id);
			return NULL;
		}

		p_shop_goods->m_id = id;
		// 类型
		p_shop_goods->m_shoptype = p_shop_conf->shop_type();
		// 限购刷新方式
		p_shop_goods->m_refresh_type = (proto::common::shop_refresh_type)p_shop_conf->refresh_type();
		// 记录一下刷新的id
		p_shop_goods->m_puton_time = p_shop_conf->puton_time();

		m_goods_map.insert(std::make_pair(id, p_shop_goods));

		is_need_save = true;
	}

	// 刷新限购
	if (p_shop_conf->refresh_type() > proto::common::shop_refresh_type_none && p_shop_goods->m_last_refresh_time < time_now)
	{
		p_shop_goods->m_buy_count = 0;

		p_shop_goods->m_last_refresh_time = get_next_refresh_time((proto::common::shop_refresh_type)p_shop_conf->refresh_type());

		// 发生错误
		if (p_shop_goods->m_last_refresh_time == 0)
		{
			log_error("goods[%u] get_next_refresh_time time 0 error", id);
		}

		is_need_save = true;
	}

	// 刷新上架时间
	if (p_shop_conf->puton_type() != shop_puton_type::shop_puton_type_none && p_shop_goods->m_end_time < time_now)
	{
		switch (p_shop_conf->puton_type())
		{
		case shop_puton_type::shop_puton_type_time:
			{
				std::pair<uint32_t, uint32_t> refresh_time = time_manager_t::get_refresh_time(p_shop_conf->puton_time());
				p_shop_goods->m_start_time = refresh_time.first;
				p_shop_goods->m_end_time = refresh_time.second;

				if (p_shop_goods->m_start_time == 0 && p_shop_goods->m_end_time == 0)
				{
					log_error("goods[%u] get_next_refresh_time time 0 error", id);
				}

				//log_warn("goods[%u] m_start_time[%u] m_end_time[%u]", p_shop_goods->m_id, p_shop_goods->m_start_time, p_shop_goods->m_end_time);

				is_need_save = true;
			}
			break;
		case shop_puton_type_gm:
			{
				// gm限购不需要在这里刷新
			}
			break;
		default:
			{
				log_error("goods[%u] p_shop_conf->puton_type[%u] error", id, p_shop_conf->puton_type());
			}
			break;
		}
	}

	// 保持数据库
	if (is_need_save)
	{
		shop_data_mgr_t::save_shop_goods(p_shop_goods);
	}

	return p_shop_goods;
}

uint32_t shop_manager_t::get_next_refresh_time(proto::common::shop_refresh_type type)
{
	switch (type)
	{
	case proto::common::shop_refresh_type_day:
		{
			std::pair<uint32_t, uint32_t> refresh_time = time_manager_t::get_refresh_time(time_type_enum::time_shop1);
			return refresh_time.first;
		}
		break;
	case proto::common::shop_refresh_type_week:
		{
			std::pair<uint32_t, uint32_t> refresh_time = time_manager_t::get_refresh_time(time_type_enum::time_shop2);
			return refresh_time.first;
		}
		break;
	case proto::common::shop_refresh_type_month:
		{
			std::pair<uint32_t, uint32_t> refresh_time = time_manager_t::get_refresh_time(time_type_enum::time_shop3);
			return refresh_time.first;
		}
		break;
	default:
		{
			log_error("shop_refresh_type[%u] can not find error", type);
		}
		break;
	}

	return 0;
}

void shop_manager_t::register_puton_timer()
{
	for (puton_timer_map::const_iterator citr = m_puton_time_map.begin();
		citr != m_puton_time_map.end(); ++citr)
	{
		time_manager_t::register_func(citr->first, shop_manager_t::puton_timer_callback);
	}
}

void shop_manager_t::clear_register_puton_timer()
{
	for (puton_timer_map::const_iterator citr = m_puton_time_map.begin();
		citr != m_puton_time_map.end(); ++citr)
	{
		time_manager_t::unregister_all(citr->first);
	}

	m_puton_time_map.clear();
}

void shop_manager_t::day_refresh_callback(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		time_t cur_time = common::time_util_t::now_time();
		time_t next_time = cur_time + DAY_SECOND;	// 下次增加周期

		for (goods_map::iterator itr = m_goods_map.begin();
			itr != m_goods_map.end(); ++itr)
		{
			shop_goods_ptr p_shop_goods = itr->second;
			if (p_shop_goods == NULL)
			{
				log_error("shop goods[%u] null error", itr->first);
				continue;
			}

			if (p_shop_goods->m_refresh_type == proto::common::shop_refresh_type_day)
			{
				p_shop_goods->m_buy_count = 0;
				p_shop_goods->m_last_refresh_time = next_time;
				shop_data_mgr_t::save_shop_goods(p_shop_goods);
				update_list_info(p_shop_goods);
			}
		}

		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_19, 0, 0, 0);
	}
}

void shop_manager_t::week_refresh_callback(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		time_t cur_time = common::time_util_t::now_time();
		time_t next_time = cur_time + WEEK_SECOND;	// 下次增加周期

		for (goods_map::iterator itr = m_goods_map.begin();
			itr != m_goods_map.end(); ++itr)
		{
			shop_goods_ptr p_shop_goods = itr->second;
			if (p_shop_goods == NULL)
			{
				log_error("shop goods[%u] null error", itr->first);
				continue;
			}

			if (p_shop_goods->m_refresh_type == proto::common::shop_refresh_type_week)
			{
				p_shop_goods->m_buy_count = 0;
				p_shop_goods->m_last_refresh_time = next_time;
				shop_data_mgr_t::save_shop_goods(p_shop_goods);
				update_list_info(p_shop_goods);
			}
		}

		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_19, 0, 0, 0);
	}
}

void shop_manager_t::month_refresh_callback(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		// 月的时间不能直接加周期
		TimeTable* p_conf = GET_CONF(TimeTable, time_type_enum::time_shop3);
		if (NULL == p_conf)
		{
			log_error("invalid time id[%u]", time_type_enum::time_shop3);
			return;
		}
		const std::vector<uint32_t>& start_time_vec = p_conf->start_time_vec();

		time_t cur_time = common::time_util_t::now_time();
		tm date;
		localtime_r(&cur_time, &date);

		// 最后一个月
		if (date.tm_mon >= 11)
		{
			date.tm_year += 1;
			date.tm_mon = 0;
		}
		else
		{
			date.tm_mon += 1;
		}
		date.tm_mday = (int32_t)start_time_vec[0];
		date.tm_hour = (int32_t)start_time_vec[1];
		date.tm_min = (int32_t)start_time_vec[2];
		date.tm_sec = (int32_t)start_time_vec[3];

		time_t next_time = mktime(&date);
		if (next_time == (time_t)-1)
		{
			log_error("time id[%u] shop_manager_t::month_refresh_callback mktime error", time_type_enum::time_shop3);
			return;
		}

		for (goods_map::iterator itr = m_goods_map.begin();
			itr != m_goods_map.end(); ++itr)
		{
			shop_goods_ptr p_shop_goods = itr->second;
			if (p_shop_goods == NULL)
			{
				log_error("shop goods[%u] null error", itr->first);
				continue;
			}

			if (p_shop_goods->m_refresh_type == proto::common::shop_refresh_type_month)
			{
				p_shop_goods->m_buy_count = 0;
				p_shop_goods->m_last_refresh_time = next_time;
				shop_data_mgr_t::save_shop_goods(p_shop_goods);
				update_list_info(p_shop_goods);
			}
		}

		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_19, 0, 0, 0);
	}
}

void shop_manager_t::puton_timer_callback(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		puton_timer_map::const_iterator itr = m_puton_time_map.find(id);
		if (itr != m_puton_time_map.end())
		{
			const std::set<uint32_t>& m_time_set = itr->second;
			for (std::set<uint32_t>::const_iterator titr = m_time_set.begin();
				titr != m_time_set.end(); ++titr)
			{
				shop_goods_ptr p_shop_goods = get_shop_goods(*titr);
				if (p_shop_goods != NULL && p_shop_goods->m_puton_time == id)
				{
					std::pair<uint32_t, uint32_t> refresh_time = time_manager_t::get_refresh_time(id);
					p_shop_goods->m_start_time = refresh_time.first;
					p_shop_goods->m_end_time = refresh_time.second;

					if (p_shop_goods->m_start_time == 0 && p_shop_goods->m_end_time == 0)
					{
						log_error("goods[%u] get_refresh_time time[%u] error", p_shop_goods->m_id, id);
					}

					shop_data_mgr_t::save_shop_goods(p_shop_goods);
					update_list_info(p_shop_goods);

					//log_warn("goods[%u] m_start_time[%u] m_end_time[%u]", p_shop_goods->m_id, p_shop_goods->m_start_time, p_shop_goods->m_end_time);

					ShopTable* p_conf = GET_CONF(ShopTable, p_shop_goods->m_id);
					if (NULL == p_conf)
					{
						log_error("ShopTable id [%u] null error", p_shop_goods->m_id);
						continue;
					}

					Item* p_item_config = GET_CONF(Item, p_conf->item_id());
					if (NULL == p_item_config)
					{
						log_error("item item_tid[%d] not found!", p_conf->item_id());
						continue;
					}

					BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_20, 0, 0, 0, common::string_util_t::convert_to_utf8(p_item_config->name_desc()).c_str());
				}
			}
		}
	}
}

void shop_manager_t::update_list_info(const shop_goods_ptr p_shop_goods)
{
	if (p_shop_goods == NULL)
	{
		log_error("p_shop_goods null error");
		return;
	}

	for (int32_t i = 0; i < m_goods_list_map[p_shop_goods->m_shoptype].goods_list_size(); ++i)
	{
		if (m_goods_list_map[p_shop_goods->m_shoptype].goods_list(i).goods_id() == p_shop_goods->m_id)
		{
			proto::common::shop_goods* p_goods = m_goods_list_map[p_shop_goods->m_shoptype].mutable_goods_list(i);
			if (p_goods != NULL)
			{
				p_goods->set_buy_count(p_shop_goods->m_buy_count);
				p_goods->set_start_time(p_shop_goods->m_start_time);
				p_goods->set_end_time(p_shop_goods->m_end_time);
				p_goods->set_last_refresh_time(p_shop_goods->m_last_refresh_time);

				//log_warn("goods[%u, %u] time[%u, %u] last_refresh[%u]", p_shop_goods->m_id, p_shop_goods->m_buy_count, p_shop_goods->m_start_time, p_shop_goods->m_end_time, p_shop_goods->m_last_refresh_time);
			}
		}
	}
}

bool shop_manager_t::get_shop_list_by_type(uint32_t type, proto::client::ec_shop_goods_reply& reply)
{
	goods_list_map::const_iterator citr = m_goods_list_map.find(type);
	if (citr != m_goods_list_map.end())
	{
		proto::common::shop_goods_list* p_shop_list = reply.mutable_shop_list();
		if (p_shop_list != NULL)
		{
			p_shop_list->CopyFrom(citr->second);
		}

		return true;
	}

	return false;
}

uint32_t shop_manager_t::user_want_buy_goods(global_user_ptr p_user, const proto::common::user_buy_goods& info, proto::client::ec_buy_goods_reply& reply)
{
	if (p_user == NULL)
	{
		log_error("p_user null error");
		return common::errcode_enum::notice_unknown;
	}

	uint32_t goods_id = info.goods_id();
	uint32_t goods_num = info.goods_num();

	if (goods_id == 0 || goods_num == 0)
	{
		log_error("user[%lu] goods id == 0 or number == 0", p_user->get_uid());
		return common::errcode_enum::notice_shop_error11;
	}

	ShopTable* p_conf = GET_CONF(ShopTable, goods_id);
	// 商品id错误
	if (NULL == p_conf)
	{
		log_error("user[%lu] ShopTable id[%u] not exsit", p_user->get_uid(), goods_id);	
		return common::errcode_enum::notice_shop_error9;
	}

	// 不限购商品center不处理，直接发给game判断就行
	if (p_conf->refresh_type() == proto::common::shop_refresh_type_none)
	{
		log_error("user[%lu] ShopTable id[%u] not limit goods", p_user->get_uid(), goods_id);
		return common::errcode_enum::notice_shop_error10;
	}

	// 先判断商店限购
	if (p_conf->server_limit() > 0)
	{
		shop_goods_ptr p_goods_ptr = get_shop_goods(goods_id);
		if (p_goods_ptr == NULL)
		{
			log_error("user[%lu] goods[%u] server limit null error", p_user->get_uid(), goods_id);
			return common::errcode_enum::notice_unknown;
		}

		// 有上架时间，不在上架时间内购买
		if (p_goods_ptr->m_start_time != 0 && p_goods_ptr->m_end_time != 0)
		{
			uint32_t curr_time = common::time_util_t::now_time();
			if (p_goods_ptr->m_start_time > curr_time || curr_time > p_goods_ptr->m_end_time)
			{
				log_error("user[%lu] goods[%u] is not in shop shelved", p_user->get_uid(), goods_id);
				return common::errcode_enum::notice_shop_error15;
			}
		}

		if (p_goods_ptr->m_buy_count >= p_conf->server_limit())
		{
			SHOP_LOG("user[%lu] goods[%u] curr[%u] is already max server limit number[%u]", p_user->get_uid(), goods_id, p_goods_ptr->m_buy_count, p_conf->server_limit());

			// 赋值告诉客户端当前限购数量
			proto::common::shop_goods* shop_goods_data = reply.mutable_shop_goods_data();
			if (shop_goods_data != NULL)
			{
				shop_goods_data->set_goods_id(goods_id);
				shop_goods_data->set_buy_count(p_goods_ptr->m_buy_count);
				shop_goods_data->set_start_time(p_goods_ptr->m_start_time);
				shop_goods_data->set_end_time(p_goods_ptr->m_end_time);
				shop_goods_data->set_last_refresh_time(p_goods_ptr->m_last_refresh_time);
			}

			return common::errcode_enum::notice_shop_error6;
		}

		if (p_goods_ptr->m_buy_count + goods_num > p_conf->server_limit())
		{
			SHOP_LOG("user[%lu] goods[%u] curr_num[%u] + buy_num[%u] > server_limit[%u]", p_user->get_uid(), goods_id, p_goods_ptr->m_buy_count, goods_num, p_conf->server_limit());

			// 赋值告诉客户端当前限购数量
			proto::common::shop_goods* shop_goods_data = reply.mutable_shop_goods_data();
			if (shop_goods_data != NULL)
			{
				shop_goods_data->set_goods_id(goods_id);
				shop_goods_data->set_buy_count(p_goods_ptr->m_buy_count);
				shop_goods_data->set_start_time(p_goods_ptr->m_start_time);
				shop_goods_data->set_end_time(p_goods_ptr->m_end_time);
				shop_goods_data->set_last_refresh_time(p_goods_ptr->m_last_refresh_time);
			}

			return common::errcode_enum::notice_shop_error11;
		}
	}

	// 再判断玩家限购
	if (p_conf->presonal_limit() > 0)
	{
		shop_user_data_t::shop_user_ptr person_limit_data = p_user->get_shop_user_data().get_presonal_limit_info(goods_id);
		// 为null 说明还没这个记录，可以购买
		if (person_limit_data != NULL)
		{
			// 判断是否在刷新时间内
			if (common::time_util_t::now_time() <= person_limit_data->m_user_goods.last_refresh_time())
			{
				if (person_limit_data->m_user_goods.buy_count() >= p_conf->presonal_limit())
				{
					SHOP_LOG("user[%lu] goods[%u] is already max presonal limit number", p_user->get_uid(), goods_id);

					return common::errcode_enum::notice_shop_error6;
				}

				if (person_limit_data->m_user_goods.buy_count() + goods_num > p_conf->presonal_limit())
				{
					SHOP_LOG("user[%lu] goods[%u] curr_num + user_num > presonal_limit", p_user->get_uid(), goods_id);

					return common::errcode_enum::notice_shop_error11;
				}
			}
		}
	}

	// 发给game验证
	proto::server::eg_check_user_buy_goods_request request;
	request.set_uid(p_user->get_uid());
	proto::common::user_buy_goods* goods_info = request.mutable_info();
	if (goods_info != NULL)
	{
		goods_info->CopyFrom(info);
	}
	p_user->send_msg_to_game(op_cmd::eg_check_user_buy_goods_request, request);

	return common::errcode_enum::error_ok;
}

uint32_t shop_manager_t::user_game_check_back(global_user_ptr p_user, const proto::common::user_buy_goods& info, proto::server::eg_give_user_goods_notify& notify)
{
	if (p_user == NULL)
	{
		log_error("p_user null error");
		return common::errcode_enum::notice_unknown;
	}

	uint32_t goods_id = info.goods_id();
	uint32_t goods_num = info.goods_num();

	if (goods_id == 0 || goods_num == 0)
	{
		log_error("user[%lu] goods id == 0 or number == 0", p_user->get_uid());
		return common::errcode_enum::notice_shop_error11;
	}

	ShopTable* p_conf = GET_CONF(ShopTable, goods_id);
	// 商品id错误
	if (NULL == p_conf)
	{
		log_error("user[%lu] ShopTable id[%u] not exsit", p_user->get_uid(), goods_id);
		return common::errcode_enum::notice_shop_error9;
	}

	// 不限购商品center不处理，直接发给game判断就行
	if (p_conf->refresh_type() == proto::common::shop_refresh_type_none)
	{
		log_error("user[%lu] ShopTable id[%u] not limit goods", p_user->get_uid(), goods_id);
		return common::errcode_enum::notice_shop_error10;
	}

	// 先判断商店限购
	if (p_conf->server_limit() > 0)
	{
		shop_goods_ptr p_goods_ptr = get_shop_goods(goods_id);
		if (p_goods_ptr == NULL)
		{
			log_error("user[%lu] goods[%u] server limit null error", p_user->get_uid(), goods_id);
			return common::errcode_enum::notice_unknown;
		}

		if (p_goods_ptr->m_buy_count >= p_conf->server_limit())
		{
			SHOP_LOG("user[%lu] goods[%u] is already max server limit number", p_user->get_uid(), goods_id);
			return common::errcode_enum::notice_shop_error6;
		}

		if (p_goods_ptr->m_buy_count + goods_num > p_conf->server_limit())
		{
			SHOP_LOG("user[%lu] goods[%u] curr_num + user_num > server_limit", p_user->get_uid(), goods_id);
			return common::errcode_enum::notice_shop_error11;
		}
	}

	// 再判断玩家限购
	if (p_conf->presonal_limit() > 0)
	{
		uint32_t back_code = p_user->get_shop_user_data().add_shop_goods_num(goods_id, goods_num, p_conf, notify);
		if (back_code != common::errcode_enum::error_ok)
		{
			log_error("user[%lu] goods[%u] add presonal_limit error", p_user->get_uid(), goods_id);
			return back_code;
		}
	}

	// 先判断商店限购
	if (p_conf->server_limit() > 0)
	{
		shop_goods_ptr p_goods_ptr = get_shop_goods(goods_id);
		if (p_goods_ptr == NULL)
		{
			log_error("user[%lu] goods[%u] server limit null error", p_user->get_uid(), goods_id);
			return common::errcode_enum::notice_unknown;
		}

		p_goods_ptr->m_buy_count = p_goods_ptr->m_buy_count + goods_num;
		shop_data_mgr_t::save_shop_goods(p_goods_ptr);
		update_list_info(p_goods_ptr);

		// 赋值告诉客户端当前限购数量
		proto::common::shop_goods* shop_goods_data = notify.mutable_shop_goods_data();
		if (shop_goods_data != NULL)
		{
			shop_goods_data->set_goods_id(goods_id);
			shop_goods_data->set_buy_count(p_goods_ptr->m_buy_count);
			shop_goods_data->set_start_time(p_goods_ptr->m_start_time);
			shop_goods_data->set_end_time(p_goods_ptr->m_end_time);
			shop_goods_data->set_last_refresh_time(p_goods_ptr->m_last_refresh_time);
		}
	}

	return common::errcode_enum::error_ok;
}
