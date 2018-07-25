#include "item/drop_manager.hpp"
#include "tblh/DropTableConfig.hpp"
#include "log.hpp"
#include "task/task.hpp"
#include "task/task_manager.hpp"
#include "protos_fwd.hpp"
#include "role/money_manager.hpp"
#include "item/item_manager.hpp"
#include "tblh/DropWorldTable.tbls.h"
#include "tblh/mailcode_enum.hpp"
#include "mail/mail_manager.hpp"
#include "activity/holiday_manager.hpp"

USING_NS_COMMON;


// 掉落类型
enum drop_type
{
	drop_type_relative = 0,		// 相对概率
	drop_type_absolute = 1		// 绝对概率
};

// 掉落返还类型
enum drop_return_type
{
	drop_return_type_no = 0,	// 不可放回
	drop_return_type_yes = 1	// 可放回
};

// 掉落条件
enum drop_cond_type
{
	drop_cond_type_task = 1,
	drop_cond_type_level = 2,
	drop_cond_type_buff = 3
};

// 掉落物品类型
enum drop_item_type
{
	drop_item_type_item = 0,
	drop_item_type_mount = 1,

	drop_item_type_count
};

bool drop_manager_t::drop(role_ptr p_role, uint32_t drop_bag_id, uint32_t source_type, proto::common::drop_sys_type drop_sys_type,
						  uint32_t source_param, proto::common::role_change_data* rcd,bool is_notice,uint32_t drop_num, const proto::common::fight_camp* self_camp)
{
	if (NULL == p_role)
	{
		log_error("drop p_role NULL");
		return false;
	}
	DropBagTable *p_drop_bag = GET_CONF(DropBagTable, drop_bag_id);
	if (NULL == p_drop_bag)
	{
		log_error("drop[%d] p_drop_bag NULL", drop_bag_id);
		return false;
	}
	
	std::set<uint32_t> drop_ids;
	std::map<uint32_t, drop_info> drop_list;

	ITEM_LOG("role[%lu] drop num[%d]", p_role->get_uid(), drop_num);
	for (uint32_t num = 0; num < drop_num; ++num)
	{
		// 处理所有掉落组
		for (uint32_t i = 0; i < p_drop_bag->drop_ids_size(); ++i)
		{
			drop_ids.clear();
			DropTableConfig_t::get_drop_ids_by_group_id(p_drop_bag->drop_ids(i), drop_ids);

			if (!get_drop_list(p_role, p_drop_bag, drop_ids, drop_list))
			{
				log_error("drop get_drop_list error drop_ids[%s] drop_result_size[%d]", p_drop_bag->base_drop_ids().c_str(), (int32_t)drop_list.size());
				return false;
			}

			ITEM_LOG("role[%lu] start drop: drop_bag_id[%d] drop_group[%s]", p_role->get_uid(), p_drop_bag->id(), p_drop_bag->base_drop_ids().c_str());
		}
	}
	//处理节日掉落
	if (p_drop_bag->holiday_drop() > 0)
	{
		holiday_manager_t::drop(p_role, drop_list);
	}
	
	DropTable *p_drop = NULL;
	proto::common::item_limit_data *p_item_data = NULL;
	proto::server::ge_check_drop_items_request ge_msg;

	bool is_need_send_mail = false;
	bool is_need_syn_item_limit = false;

	std::map<uint32_t, uint32_t> mail_item_info;
	std::map<uint32_t, drop_info>::iterator iter = drop_list.begin();
	uint32_t real_drop_item_num = 0;
	// 校验物品发放
	for (; iter != drop_list.end(); ++iter)
	{
		p_drop = iter->second.pDropTable;
		real_drop_item_num = iter->second.real_drop_num;
		if (NULL == p_drop)
		{
			log_error("drop p_drop NULL");
			return false;
		}

		is_need_syn_item_limit = false;

		// 校验是否能发放
		if (item_manager_t::is_need_check(p_drop->drop_item_tid()))
		{
			if (!item_manager_t::check_limit_item(p_drop->drop_item_tid(), real_drop_item_num))
			{
				ITEM_LOG("drop check_limit_item fail drop_item_tid[%d], drop_item_num[%d]", p_drop->drop_item_tid(), real_drop_item_num);
				continue;
			}
			is_need_syn_item_limit = true;
		}

		switch (p_drop->drop_item_type())
		{
		case drop_item_type_item:// 添加道具
			proto::common::MONEY_TYPE money_type = item_manager_t::is_money_item(p_drop->drop_item_tid());
			if (money_type > proto::common::MONEY_TYPE_NONE && money_type < proto::common::MONEY_TYPE_END)
			{
				bool is_process_success = false;	// 是否加成功

				if (money_type == proto::common::MONEY_TYPE_EXP)
				{
					//经验道具
					is_process_success = p_role->add_exp(real_drop_item_num, source_type, source_param, rcd);
				}
				else if (money_type == proto::common::MONEY_TYPE_LIVENESS)
				{
					activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
                    if (NULL != p_activity_mgr)
                    {
                        is_process_success = p_activity_mgr->add_liveness_count(real_drop_item_num);
                    }
				}
				else
				{
					//货币道具
					if (money_type == proto::common::MONEY_TYPE_COPPER)
					{
						uint32_t added = 0;
						if (self_camp != NULL)
						{
							added = friendliness_t::money_affected_by_friendliness(p_role, *self_camp);
						}
						real_drop_item_num *= ((added * 1.0 / 10000) + 1);
					}
					is_process_success = money_manager_t::add_money(p_role, money_type, real_drop_item_num, source_type, source_param, is_notice, rcd);
				}

				if (rcd && is_process_success)
				{
					proto::common::item_data* p_item_data = rcd->mutable_item();
					if (p_item_data)
					{
						proto::common::item_single *p_item_single = p_item_data->add_item_list();
						if (p_item_single)
						{
							p_item_single->set_tid(p_drop->drop_item_tid());
							p_item_single->set_up_num(real_drop_item_num);
						}
					}
				}
			}
			else if (money_type > proto::common::MONEY_TYPE_END)
			{
				//真正的道具
				if (item_manager_t::check_add_item(p_role, p_drop->drop_item_tid(), real_drop_item_num, proto::common::package_type_main))
				{
					item_manager_t::add_item(p_role, p_drop->drop_item_tid(), real_drop_item_num, source_type, source_param, rcd);
				}
				// 组织邮件内容
				else
				{
					mail_item_info.insert(std::make_pair(p_drop->drop_item_tid(), real_drop_item_num));
					is_need_send_mail = true;
				}
			}
			else
			{
				log_error("item[%d] type[%d] error!", p_drop->drop_item_tid(), money_type);
			}
			break;
		}
		ITEM_LOG("role[%lu] really drop: drop_bag_id[%d] drop_id[%d]", p_role->get_uid(), drop_bag_id, p_drop->id());

		// 组包(发给center的)
		if (is_need_syn_item_limit)
		{
			p_item_data = ge_msg.add_item_list();
			if (NULL == p_item_data)
			{
				log_error("drop ge_check_drop_items_request add_item_list error");
				return false;
			}
			p_item_data->set_item_tid(p_drop->drop_item_tid());
			p_item_data->set_item_num(real_drop_item_num);
		}
	}
	

	// 需要发邮件
	if (is_need_send_mail)
	{
		mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, mailcode_enum::sys_mail_collect, 
								  mail_item_info, proto::common::MAIL_GET_TYPE_DROP);
	}

	// 发给center同步
    if (ge_msg.item_list_size() > 0)
    {
        env::server->send_msg_to_center(op_cmd::ge_check_drop_items_request, p_role->get_uid(), ge_msg);
    }

	p_role->save_self();

	if (rcd)
	{
		p_role->get_personal_info(rcd->mutable_per_info());
	}
	return true;
}


bool drop_manager_t::drop_ex(role_ptr p_role, const std::map<uint32_t, uint32_t>& drop_id_count, uint32_t source_type, proto::common::drop_sys_type drop_sys_type, uint32_t source_param /*= 0*/, proto::common::role_change_data* rcd /*= NULL*/, bool is_notice /*= true*/)
{
	if (NULL == p_role)
	{
		log_error("drop p_role NULL");
		return false;
	}

	std::set<uint32_t> drop_ids;
	std::map<uint32_t, drop_info> drop_list;

	for (auto it : drop_id_count)
	{
		uint32_t drop_bag_id = it.first;
		uint32_t drop_num = it.second;
		DropBagTable *p_drop_bag = GET_CONF(DropBagTable, drop_bag_id);
		if (NULL == p_drop_bag)
		{
			log_error("drop[%d] p_drop_bag NULL", drop_bag_id);
			continue;
		}

		ITEM_LOG("role[%lu] drop num[%d]", p_role->get_uid(), drop_num);
		for (uint32_t num = 0; num < drop_num; ++num)
		{
			// 处理所有掉落组
			for (uint32_t i = 0; i < p_drop_bag->drop_ids_size(); ++i)
			{
				drop_ids.clear();
				DropTableConfig_t::get_drop_ids_by_group_id(p_drop_bag->drop_ids(i), drop_ids);

				if (!get_drop_list(p_role, p_drop_bag, drop_ids, drop_list))
				{
					log_error("drop get_drop_list error drop_ids[%s] drop_result_size[%d]", p_drop_bag->base_drop_ids().c_str(), (int32_t)drop_list.size());
					return false;
				}

				ITEM_LOG("role[%lu] start drop: drop_bag_id[%d] drop_group[%s]", p_role->get_uid(), p_drop_bag->id(), p_drop_bag->base_drop_ids().c_str());
			}
		}

		//处理节日掉落
		if (p_drop_bag->holiday_drop() > 0)
		{
			holiday_manager_t::drop(p_role, drop_list);
		}
	}
	


	DropTable *p_drop = NULL;
	proto::common::item_limit_data *p_item_data = NULL;
	proto::server::ge_check_drop_items_request ge_msg;

	bool is_need_send_mail = false;
	bool is_need_syn_item_limit = false;

	std::map<uint32_t, uint32_t> mail_item_info;
	std::map<uint32_t, drop_info>::iterator iter = drop_list.begin();
	uint32_t real_drop_item_num = 0;
	// 校验物品发放
	for (; iter != drop_list.end(); ++iter)
	{
		p_drop = iter->second.pDropTable;
		real_drop_item_num = iter->second.real_drop_num;
		if (NULL == p_drop)
		{
			log_error("drop p_drop NULL");
			return false;
		}

		is_need_syn_item_limit = false;

		// 校验是否能发放
		if (item_manager_t::is_need_check(p_drop->drop_item_tid()))
		{
			if (!item_manager_t::check_limit_item(p_drop->drop_item_tid(), real_drop_item_num))
			{
				ITEM_LOG("drop check_limit_item fail drop_item_tid[%d], drop_item_num[%d]", p_drop->drop_item_tid(), real_drop_item_num);
				continue;
			}
			is_need_syn_item_limit = true;
		}

		switch (p_drop->drop_item_type())
		{
		case drop_item_type_item:// 添加道具
			proto::common::MONEY_TYPE money_type = item_manager_t::is_money_item(p_drop->drop_item_tid());
			if (money_type > proto::common::MONEY_TYPE_NONE && money_type < proto::common::MONEY_TYPE_END)
			{
				bool is_process_success = false;	// 是否加成功

				if (money_type == proto::common::MONEY_TYPE_EXP)
				{
					//经验道具
					is_process_success = p_role->add_exp(real_drop_item_num, source_type, source_param, rcd);
				}
				else if (money_type == proto::common::MONEY_TYPE_LIVENESS)
				{
					activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
                    if (NULL != p_activity_mgr)
                    {
                        is_process_success = p_activity_mgr->add_liveness_count(real_drop_item_num);
                    }
				}
				else
				{
					//货币道具
					is_process_success = money_manager_t::add_money(p_role, money_type, real_drop_item_num, source_type, source_param, is_notice, rcd);
				}

				if (rcd && is_process_success)
				{
					proto::common::item_data* p_item_data = rcd->mutable_item();
					if (p_item_data)
					{
						proto::common::item_single *p_item_single = p_item_data->add_item_list();
						if (p_item_single)
						{
							p_item_single->set_tid(p_drop->drop_item_tid());
							p_item_single->set_up_num(real_drop_item_num);
						}
					}
				}
			}
			else if (money_type > proto::common::MONEY_TYPE_END)
			{
				//真正的道具
				if (item_manager_t::check_add_item(p_role, p_drop->drop_item_tid(), real_drop_item_num, proto::common::package_type_main))
				{
					item_manager_t::add_item(p_role, p_drop->drop_item_tid(), real_drop_item_num, source_type, source_param, rcd);
				}
				// 组织邮件内容
				else
				{
					mail_item_info.insert(std::make_pair(p_drop->drop_item_tid(), real_drop_item_num));
					is_need_send_mail = true;
				}
			}
			else
			{
				log_error("item[%d] type[%d] error!", p_drop->drop_item_tid(), money_type);
			}
			break;
		}
		//ITEM_LOG("role[%lu] really drop: drop_bag_id[%d] drop_id[%d]", p_role->get_uid(), drop_bag_id, p_drop->id());

		// 组包(发给center的)
		if (is_need_syn_item_limit)
		{
			p_item_data = ge_msg.add_item_list();
			if (NULL == p_item_data)
			{
				log_error("drop ge_check_drop_items_request add_item_list error");
				return false;
			}
			p_item_data->set_item_tid(p_drop->drop_item_tid());
			p_item_data->set_item_num(real_drop_item_num);
		}
	}


	// 需要发邮件
	if (is_need_send_mail)
	{
		mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, mailcode_enum::sys_mail_collect,
			mail_item_info, proto::common::MAIL_GET_TYPE_DROP);
	}

	// 发给center同步
	if (ge_msg.item_list_size() > 0)
	{
		env::server->send_msg_to_center(op_cmd::ge_check_drop_items_request, p_role->get_uid(), ge_msg);
	}

	p_role->save_self();

	if (rcd)
	{
		p_role->get_personal_info(rcd->mutable_per_info());
	}
	return true;
}

bool drop_manager_t::check_bag_num(role_ptr p_role, uint32_t drop_bag_id)
{
	if (NULL == p_role)
	{
		log_error("drop p_role NULL");
		return false;
	}
	package_ptr p_package = item_manager_t::get_package(p_role, proto::common::package_type_main);
	if (NULL == p_package)
	{
		log_error("drop p_package NULL");
		return false;
	}
	DropBagTable *p_drop_bag_config = GET_CONF(DropBagTable, drop_bag_id);
	if (NULL == p_drop_bag_config)
	{
		log_error("drop p_drop_bag_config NULL drop_bag_id[%d]", drop_bag_id);
		return false;
	}

	uint32_t total_give_item_num = 0;
	DropTable *p_drop_table = NULL;
	std::set<uint32_t> drop_ids;
	// 预判空格字数 只计算drop表中策划填的drop组数 具体掉落道具数因随机性无法准确估算
	for (uint32_t i = 0; i < p_drop_bag_config->drop_ids_size(); ++i)
	{
		drop_ids.clear();
		DropTableConfig_t::get_drop_ids_by_group_id(p_drop_bag_config->drop_ids(i), drop_ids);
		std::set<uint32_t>::iterator iter = drop_ids.begin();
		if (iter == drop_ids.end())
		{
			continue;
		}

		p_drop_table = GET_CONF(DropTable, *iter);
		if (NULL == p_drop_table)
		{
			log_error("item p_drop_table NULL");
			return false;
		}
		total_give_item_num += p_drop_table->drop_num();
	}

	if (p_package->get_bag_can_use_num() < total_give_item_num)
	{
		ITEM_LOG("drop check_bag_num role_uid[%lu] package_free_num[%d] < give_num[%d] drop_bag_id[%d]",
			p_role->get_uid(), p_package->get_bag_can_use_num(), total_give_item_num, drop_bag_id);
		return false;
	}
	return true;
}

bool drop_manager_t::get_drop_list(role_ptr p_role, DropBagTable *p_drop_bag, const std::set<uint32_t> &drop_list,
								   std::map<uint32_t, drop_info> &drop_all_result)
{
	if (NULL == p_role)
	{
		log_error("[wys] p_role NULL");
		return false;
	}
	if (NULL == p_drop_bag)
	{
		log_error("drop p_drop_bag NULL");
		return false;
	}
	if (drop_list.size() == 0)
	{
		log_warn("drop drop_list_size = 0 drop_bag_id:%d", p_drop_bag->id());
		return false;
	}
	
	// 整组的随机参数都是一致的 所以取第一个就可以
	DropTable *p_drop_table = NULL;
	std::set<uint32_t>::iterator iter = drop_list.begin();
	if (iter != drop_list.end())
	{
		p_drop_table = GET_CONF(DropTable, *iter);
	}
	if (NULL == p_drop_table)
	{
		log_error("drop p_drop_table NULL");
		return false;
	}

	uint32_t drop_num = p_drop_table->drop_num();
	uint32_t prob_type = p_drop_table->prob_type();
	uint32_t rand_type = p_drop_table->rand_type();
	std::vector<DropTable*> drop_result;
	// 相对概率
	if (prob_type == drop_type_relative)
	{
		check_drop_relative(p_role, drop_num, rand_type, drop_list, drop_result);
	} // 绝对概率
	else if (prob_type == drop_type_absolute)
	{
		check_drop_absolute(p_role, drop_num, rand_type, drop_list, drop_result);
	}

	std::map<uint32_t, drop_info>::iterator iter_drop;
	// 把每组的掉落结果放到一个容器里 在外层打包发给center验证
	for (uint32_t i = 0; i < drop_result.size(); ++i)
	{
		p_drop_table = drop_result[i];
		if (NULL == p_drop_table)
		{
			log_error("drop p_drop_table NULL pos[%d]", i);
			return false;
		}
		iter_drop = drop_all_result.find(p_drop_table->drop_item_tid());
		if (iter_drop != drop_all_result.end())
		{
			iter_drop->second.real_drop_num += p_drop_table->drop_item_num();
		}
		else
		{
			drop_info info;
			info.pDropTable = p_drop_table;
			info.real_drop_num = p_drop_table->drop_item_num();
			drop_all_result.insert(std::make_pair(p_drop_table->drop_item_tid(), info));
		}
		ITEM_LOG("drop to_check_drop[%d]", p_drop_table->id());
	}

	return true;
}

bool drop_manager_t::check_drop_cond(role_ptr p_role, DropTable *p_drop)
{
	if (NULL == p_role)
	{
		log_error("drop p_role NULL");
		return false;
	}
	if (NULL == p_drop)
	{
		log_error("drop p_drop NULL");
		return false;
	}
	switch (p_drop->cond_type())
	{
	case drop_cond_type_task:
		{	// 检查任务状态 未接取和已完成两种状态认为不满足条件
			task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
			if (NULL == p_task_mgr)
			{
				log_error("drop task_mgr NULL");
				return false;
			}
			task_ptr p_task = p_task_mgr->get_task(p_drop->cond_param1());
			if (NULL == p_task || (p_task->get_state() != TASK_STATE_DOING && p_task->get_state() != TASK_STATE_CAN_FINISH))
			{
				ITEM_LOG("drop check_drop_cond fail role_uid[%lu] task[%d] state not finish", p_role->get_uid(), p_drop->cond_param1());
				return false;
			}
		}
		break;
	case drop_cond_type_level:
		{	// 检查等级
			if (p_role->get_level() < p_drop->cond_param1())
			{
				ITEM_LOG("drop check_drop_cond fail role_uid[%lu] role_level[%d] < drop_level[%d]", p_role->get_uid(), p_role->get_level(), p_drop->cond_param1());
				return false;
			}
			if (p_role->get_level() > p_drop->cond_param2())
			{
				ITEM_LOG("drop check_drop_cond fail role_uid[%lu] role_level[%d] > drop_level[%d]", p_role->get_uid(), p_role->get_level(), p_drop->cond_param2());
				return false;
			}
		}
		break;
	default:
		break;
	}
	return true;
}

bool drop_manager_t::check_drop_relative(role_ptr p_role, uint32_t drop_num, uint32_t rand_type, 
										 const std::set<uint32_t> &drop_list, std::vector<DropTable*> &drop_result)
{
	if (NULL == p_role)
	{
		log_error("drop p_role NULL");
		return false;
	}
	// 计算总概率值
	uint32_t total_prob = 0;
	DropTable *p_drop = NULL;
	std::vector<DropTable*> drop_cond_list;
	std::set<uint32_t>::iterator iter = drop_list.begin();
	std::set<uint32_t>::iterator iter_end = drop_list.end();

	// 筛选出满足条件的掉落
	for (; iter != iter_end; ++iter)
	{
		p_drop = GET_CONF(DropTable, *iter);
		if (NULL == p_drop)
		{
			log_error("drop config not find id[%d]", *iter);
			return false;
		}
		// 条件不满足排除掉
		if (!check_drop_cond(p_role, p_drop))
			continue;
		// 计算概率和
		total_prob += p_drop->drop_prob();
		drop_cond_list.push_back(p_drop);
		ITEM_LOG("drop check_drop_relative drop_cond[%d]", p_drop->id());
	}
	if (total_prob == 0)
	{
		log_error("drop total_prob = 0");
		return false;
	}
	if (drop_cond_list.size() == 0)
	{
		log_error("drop drop_cond_list size = 0");
		return false;
	}
	int32_t roll_pos = -1;
	uint32_t roll_limit = 50;	// 循环限制 防止死循环
	bool b_roll_result = false;

	// roll到足够的组数才停止 验证循环次数 防止死循环
	for (uint32_t roll_count = 0; roll_count < roll_limit; ++roll_count)
	{
		// 是否放回 不可放回就从列表里踢掉这次选出的重新roll
		if (rand_type == drop_return_type_no && roll_pos != -1)
		{
			// 只剩最后一个掉落不删 会一直掉落该项
			if (drop_cond_list.size() > 1)
			{
				p_drop = drop_cond_list[roll_pos];
				if(p_drop != NULL)
					total_prob -= p_drop->drop_prob();
				std::vector<DropTable*>::iterator iter_del = drop_cond_list.begin() + roll_pos;
				drop_cond_list.erase(iter_del);
			}
		}
		// 重置roll结果
		roll_pos = -1;
		b_roll_result = roll_drop_relative(drop_cond_list, drop_result, total_prob, roll_pos);
		if (!b_roll_result)
		{
			log_error("drop roll error drop_result[%d] roll_count[%d] roll_pos[%d]", (int32_t)drop_result.size(), roll_count, roll_pos);
			return false;
		}
		// 够数了 结束
		if(drop_result.size() >= drop_num)
			break;
	}

	return true;
}

bool drop_manager_t::roll_drop_relative(const std::vector<DropTable*> &drop_list, std::vector<DropTable*> &drop_result, 
										uint32_t total_prob, int32_t &roll_pos)
{
	// ！！！只有异常情况 程序无法继续 才renturn false ！！！
	if (total_prob <= 0)
	{
		log_error("drop total_prob[%d] <= 0", total_prob);
		return false;
	}
	// 随机
	int32_t rand_value = random_util_t::randBetween(1, total_prob);
    if (rand_value == -1 )
    {
		log_error("dispel_buff rand error min[%d] max[%d]", 1, total_prob );
        return false;
    }
	uint32_t prob_value = 0;
	DropTable *p_drop = NULL;
	DropTable *p_drop_result = NULL;

	// 对比满足条件的所有掉落的概率值
	for (uint32_t i = 0; i < drop_list.size(); ++i)
	{
		p_drop = drop_list[i];
		if (NULL == p_drop)
		{
			log_error("drop p_drop NULL pos[%d]", i);
			return false;
		}
		prob_value += p_drop->drop_prob();
		
		ITEM_LOG("drop roll_drop_relative rand_value[%d] drop_id[%d] prob[%d]", rand_value, p_drop->id(), prob_value);

		if ((uint32_t)rand_value <= prob_value)
		{
			p_drop_result = p_drop;
			roll_pos = (int32_t)i;
			ITEM_LOG("drop roll_drop_relative success drop_id[%d]", p_drop->id());
			break;
		}
	}

	if (NULL == p_drop_result)
	{
		log_error("drop not have match drop");
		return false;
	}
	drop_result.push_back(p_drop_result);

	ITEM_LOG("drop roll_drop_relative rand_value[%d] rand_drop_id[%d] prob[%d]", rand_value, p_drop_result->id(), p_drop_result->drop_prob());

	return true;
}

bool drop_manager_t::check_drop_absolute(role_ptr p_role, uint32_t drop_num, uint32_t rand_type,
										 const std::set<uint32_t> &drop_list, std::vector<DropTable*> &drop_result)
{
	if (NULL == p_role)
	{
		log_error("drop p_role NULL");
		return false;
	}
	DropTable *p_drop = NULL;
	std::vector<DropTable*> drop_cond_list;
	std::set<uint32_t>::iterator iter = drop_list.begin();
	std::set<uint32_t>::iterator iter_end = drop_list.end();

	// 筛选出满足条件的掉落
	for (; iter != iter_end; ++iter)
	{
		p_drop = GET_CONF(DropTable, *iter);
		if (NULL == p_drop)
		{
			log_error("drop DropTable not find id[%d]", *iter);
			return false;
		}
		// 条件不满足排除掉
		if (!check_drop_cond(p_role, p_drop))
			continue;
		drop_cond_list.push_back(p_drop);
		ITEM_LOG("drop check_drop_absolute drop_cond[%d]", p_drop->id());
	}
	if (drop_cond_list.size() == 0)
	{
		log_error("drop drop_cond_list size = 0");
		return false;
	}

	uint32_t roll_limit = 50;	// 循环限制 防止死循环
	bool b_roll_result = false;

	// roll到足够的组数才停止 验证循环次数 防止死循环
	for (uint32_t roll_count = 0; roll_count < roll_limit; ++roll_count)
	{
		b_roll_result = roll_drop_absolute(drop_num, drop_cond_list, drop_result);
		if (!b_roll_result)
		{
			log_error("drop roll error drop_result[%d] roll_count[%d]", (int32_t)drop_result.size(), roll_count);
			return false;
		}
		// 够数了 结束
		if (drop_result.size() >= drop_num)
			break;
	}
	return true;
}

bool drop_manager_t::roll_drop_absolute(uint32_t drop_num, const std::vector<DropTable*> &drop_list, std::vector<DropTable*> &drop_result)
{
	uint32_t rand_value = 0;
	DropTable *p_drop = NULL;
	// 逐个roll
	for (uint32_t i = 0; i < drop_list.size(); ++i)
	{
		p_drop = drop_list[i];
		if (NULL == p_drop)
		{
			log_error("drop p_drop NULL pos[%d]", i);
			return false;
		}
		//if (drop_result.size() >= drop_num)
		//	break;
		rand_value = random_util_t::randBetween(1, 10000);
		
		ITEM_LOG("drop roll_drop_absolute rand_value[%d] drop_id[%d] prob[%d]", rand_value, p_drop->id(), p_drop->drop_prob());

		if (rand_value > p_drop->drop_prob())
			continue;

		drop_result.push_back(p_drop);
		ITEM_LOG("drop roll_drop_absolute result_drop_id[%d]", p_drop->id());
	}
	return true;
}


bool drop_manager_t::drop_once(role_ptr p_role, uint32_t drop_bag_id, uint32_t source_type, proto::common::drop_sys_type drop_sys_type,
	uint32_t source_param, std::vector<proto::common::item_num_info> &item_vec, proto::common::role_change_data* rcd)
{
	if (NULL == p_role)
	{
		log_error("drop p_role NULL");
		return false;
	}
	DropBagTable *p_drop_bag = GET_CONF(DropBagTable, drop_bag_id);
	if (NULL == p_drop_bag)
	{
		log_error("drop[%d] p_drop_bag NULL", drop_bag_id);
		return false;
	}

	std::set<uint32_t> drop_ids;
	std::map<uint32_t, drop_info> drop_list;

	// 处理所有掉落组
	for (uint32_t i = 0; i < p_drop_bag->drop_ids_size(); ++i)
	{
		drop_ids.clear();
		DropTableConfig_t::get_drop_ids_by_group_id(p_drop_bag->drop_ids(i), drop_ids);

		if (!get_drop_list(p_role, p_drop_bag, drop_ids, drop_list))
		{
			log_error("drop get_drop_list error drop_ids[%s] drop_result_size[%d]", p_drop_bag->base_drop_ids().c_str(), (int32_t)drop_list.size());
			return false;
		}

		ITEM_LOG("role[%lu] start drop: drop_bag_id[%d] drop_group[%s]", p_role->get_uid(), p_drop_bag->id(), p_drop_bag->base_drop_ids().c_str());
	}

	DropTable *p_drop = NULL;
	proto::common::item_limit_data *p_item_data = NULL;
	proto::server::ge_check_drop_items_request ge_msg;

	bool is_need_send_mail = false;
	bool is_need_syn_item_limit = false;

	std::map<uint32_t, uint32_t> mail_item_info;
	std::map<uint32_t, drop_info>::iterator iter = drop_list.begin();
	uint32_t real_drop_item_num = 0;

	// 校验物品发放
	for (; iter != drop_list.end(); ++iter)
	{
		p_drop = iter->second.pDropTable;
		real_drop_item_num = iter->second.real_drop_num;
		if (NULL == p_drop)
		{
			log_error("drop p_drop NULL");
			return false;
		}

		is_need_syn_item_limit = false;

		// 校验是否能发放
		if (item_manager_t::is_need_check(p_drop->drop_item_tid()))
		{
			if (!item_manager_t::check_limit_item(p_drop->drop_item_tid(), real_drop_item_num))
			{
				ITEM_LOG("drop check_limit_item fail drop_item_tid[%d], drop_item_num[%d]", p_drop->drop_item_tid(), real_drop_item_num);
				continue;
			}
			is_need_syn_item_limit = true;
		}

		switch (p_drop->drop_item_type())
		{
		case drop_item_type_item:// 添加道具
			proto::common::MONEY_TYPE money_type = item_manager_t::is_money_item(p_drop->drop_item_tid());
			if (money_type > proto::common::MONEY_TYPE_NONE && money_type < proto::common::MONEY_TYPE_END)
			{
				bool is_process_success = false;	// 是否加成功

				if (money_type == proto::common::MONEY_TYPE_EXP)
				{
					//经验道具
					is_process_success = p_role->add_exp(real_drop_item_num, source_type, source_param, rcd);
				}
				else if (money_type == proto::common::MONEY_TYPE_LIVENESS)
				{
					activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
					is_process_success = p_activity_mgr->add_liveness_count(real_drop_item_num);
				}
				else
				{
					//货币道具
					is_process_success = money_manager_t::add_money(p_role, money_type, real_drop_item_num, source_type, source_param, false, rcd);
				}

				if (rcd && is_process_success)
				{
					proto::common::item_data* p_item_data = rcd->mutable_item();
					if (p_item_data)
					{
						proto::common::item_single *p_item_single = p_item_data->add_item_list();
						if (p_item_single)
						{
							p_item_single->set_tid(p_drop->drop_item_tid());
							p_item_single->set_up_num(real_drop_item_num);
						}
					}
				}
			}
			else if (money_type > proto::common::MONEY_TYPE_END)
			{
				//真正的道具
				if (item_manager_t::check_add_item(p_role, p_drop->drop_item_tid(), real_drop_item_num, proto::common::package_type_main))
				{
					item_manager_t::add_item(p_role, p_drop->drop_item_tid(), real_drop_item_num, source_type, source_param, rcd);
				}
				// 组织邮件内容
				else
				{
					mail_item_info.insert(std::make_pair(p_drop->drop_item_tid(), real_drop_item_num));
					is_need_send_mail = true;
				}
			}
			else
			{
				log_error("item[%d] type[%d] error!", p_drop->drop_item_tid(), money_type);
			}
			break;
		}

		//添加本次掉落的道具列表
		proto::common::item_num_info item_info;
		item_info.set_id( p_drop->drop_item_tid());
		item_info.set_num(real_drop_item_num);
		item_vec.push_back(item_info);

		ITEM_LOG("role[%lu] really drop: drop_bag_id[%d] drop_id[%d]", p_role->get_uid(), drop_bag_id, p_drop->id());

		// 组包(发给center的)
		if (is_need_syn_item_limit)
		{
			p_item_data = ge_msg.add_item_list();
			if (NULL == p_item_data)
			{
				log_error("drop ge_check_drop_items_request add_item_list error");
				return false;
			}
			p_item_data->set_item_tid(p_drop->drop_item_tid());
			p_item_data->set_item_num(real_drop_item_num);
		}
	}

	// 需要发邮件
	if (is_need_send_mail)
	{
		mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, mailcode_enum::sys_mail_collect,
			mail_item_info, proto::common::MAIL_GET_TYPE_DROP);
	}

	// 发给center同步
	if (ge_msg.item_list_size() > 0)
	{
		env::server->send_msg_to_center(op_cmd::ge_check_drop_items_request, p_role->get_uid(), ge_msg);
	}

	p_role->save_self();

	if (rcd)
	{
		p_role->get_personal_info(rcd->mutable_per_info());
	}
	return true;
}

