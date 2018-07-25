#include "achieve_method.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "achieve/achieve.hpp"
#include "achieve/achieve_manager.hpp"
#include "role/money_manager.hpp"
#include "item/drop_manager.hpp"
#include "common/log_enum.hpp"
#include "log/log_wrapper.hpp"
#include "achieve_common.hpp"
#include "macros.hpp"
#include "sys_notice/sys_notice_manager.hpp"

USING_NS_CONFIG;
USING_NS_COMMON;
//角色升级
void achieve_method_t::achieve_main_role_level(const uint64_t uid, const proto::common::Achieve_Event_Type type,uint32_t param1, uint32_t param2)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return ;
	}

	uint32_t achieve_id = 0;
	if (!achieve_common_t::get_config_id(uid,achieve_common_achieve,type, achieve_id))
	{
		ACHIEVE_LOG("role_uid[%lu] achieve_id[%d] not legal Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, 0);
		return;
	}
	//role achieve_map没有该成就项就新增一个
	if (!achieve_common_t::add_element( achieve_common_achieve,uid,achieve_id))
	{
		log_error("role[%lu] add achieve[%d] fail!",uid,achieve_id);
		return;
	}

	achieve_ptr p_achieve = achieve_manager_t::get_achieve(uid, achieve_id);
	if (NULL == p_achieve)
	{
		log_error("NULL == p_achieve role_uid[%lu] achieve_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, 0);
		return;
	}

	const uint32_t old_count = p_achieve->get_count();
	if (p_achieve->get_count() < p_role->get_level())
	{
		p_achieve->set_count(p_role->get_level());
	}
		
	const uint32_t new_count = p_achieve->get_count();
	const uint32_t add_count = new_count - old_count;
	p_achieve->save_self(uid, true);
	ACHIEVE_LOG("role[%lu] add achieve[%d] addcount[%d] old count/new count[%d/%d]", uid, achieve_id, add_count,old_count,new_count);

	//查找当前成就进度到了哪一级
	const uint32_t cur_level = p_achieve->get_level();
	uint32_t last_level = cur_level;
	std::deque<uint32_t> drop_id_deque;
	bool result = false;//do{}while()结束 为true 表明可以升级
	do 
	{
		if (!achieve_manager_t::achieve_up_last_level(type, new_count, drop_id_deque, last_level))
		{
			log_error("role[%lu] cannot up achieve[%d:%d] level", uid, achieve_id, cur_level);
			break;
		}

		//判断是否可以升级last_level 到达的等级 cur_level 当前等级
		if (last_level == cur_level)
		{
			const uint32_t count = achieve_common_t::get_target_count(achieve_common_achieve, achieve_id + cur_level + 1);
			const uint32_t cur_target_count = achieve_common_t::get_target_count(achieve_common_achieve,achieve_id + (cur_level == 0 ? cur_level + 1 : cur_level));
			ACHIEVE_LOG("role_uid[%lu] type[%d] current_level[%d] cur_count/target_count[%d/%d]", uid, type, cur_level, 
				new_count, (count == 0 ? cur_target_count : count));
			break;
		}
		if (drop_id_deque.empty())
		{
			log_error("drop_id_deque empty role_uid[%lu] achieve_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, 0);
			break;
		}

		result = true;
	} while (0);

	uint32_t log_type = achieve_manager_t::get_achieve_log_type(type);
	uint32_t  old_achieve_level = p_role->get_achieve_level();
	uint32_t old_achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_ACHIEVE_POINT);
	std::string src;
	if (!result)
	{
		//记录进度
		achieve_manager_t::get_source_type_str(src,achieve_id + (cur_level == 0 ? 1 : cur_level));
		log_wrapper_t::send_achieve_log(uid, achieve_id, cur_level, 0, cur_level, old_count, add_count, new_count,
			old_achieve_point, 0, old_achieve_point, old_achieve_level, 0, old_achieve_level,
			p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, 0,src);
		return;
	}

	//可以升级
	uint32_t loop_num = std::abs(last_level - cur_level);
	ACHIEVE_LOG("role[%lu] achieve[%d] begin up level", uid, achieve_id);
	bool is_start = true;//用于标记记录日志
	while (0 < loop_num)
	{
		const uint32_t old_level = p_achieve->get_level();
		if (!p_achieve->add_level())
		{
			achieve_manager_t::get_source_type_str(src,achieve_id + (old_level == 0 ? 1 : old_level));
			log_error("role[%lu] up achieve[%d:%d] level fail!", uid, achieve_id, old_level);
			log_wrapper_t::send_achieve_log(uid, achieve_id, old_level, 0, old_level, (is_start ? old_count : new_count), (is_start ? add_count : 0), new_count,
				old_achieve_point, 0, old_achieve_point, old_achieve_level, 0, old_achieve_level,
				p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(),log_type, 0, src);
			return;
		}

		proto::client::gc_achieve_finish_notify notify;
		bool b_result = drop_manager_t::drop(p_role, drop_id_deque.front(), log_enum::source_type_achieve_single_finish,
			proto::common::drop_sys_type_achieve, achieve_id, notify.mutable_rcd());

		if (!b_result)
		{
			//此循环需要结束 不能中途中断 
			log_error("b_result false role[%lu] achieve_id[%d] drop_id[%d]", uid, achieve_id, drop_id_deque.front());
		}

		const uint32_t new_level = p_achieve->get_level();
		const uint32_t  new_achieve_level = p_role->get_achieve_level();
		const uint32_t new_achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_ACHIEVE_POINT);
		const uint32_t add_achieve_point = std::abs(new_achieve_point - old_achieve_point);
		const uint32_t up_achieve_level = std::abs(new_achieve_level - old_achieve_level);
		const uint32_t up_level = std::abs(new_level - old_level);

		achieve_manager_t::get_source_type_str(src,achieve_id + (new_level == 0 ? 1 : new_level));
		log_wrapper_t::send_achieve_log(uid, achieve_id, old_level, up_level, new_level, (is_start ? old_count : new_count), (is_start ? add_count : 0), new_count,
			old_achieve_point, add_achieve_point, new_achieve_point, old_achieve_level, up_achieve_level, new_achieve_level,
			p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(),log_type, drop_id_deque.front(),src);

		is_start = false; //add_count 设为0
		old_achieve_level = new_achieve_level;
		old_achieve_point = new_achieve_point;

		drop_id_deque.pop_front();															//从掉落队列删除
		achieve_manager_t::notify_client_achieve_finish(uid, achieve_id, notify);			//通知客户端
		p_role->save_self(true);
		p_achieve->save_self(uid, true);
		ACHIEVE_LOG("finish achieve role_uid[%lu] Achieve_Event_Type[%d] achieve_id[%d]", uid, type, achieve_id);

		--loop_num;
	}
	BROADCAST_SYS_NOTICE_SYS_CHANNEL(uid, proto::common::sys_notify_type_finish_achieve, achieve_id + p_achieve->get_level(), p_role->get_name().c_str());
}
//新增hero
void achieve_method_t::achieve_hero_cultivate(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id,uint32_t num)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}

	uint32_t achieve_id = 0;
	if (!achieve_common_t::get_config_id(uid,achieve_common_achieve,type, achieve_id, event_id))
	{
		ACHIEVE_LOG("role_uid[%lu] achieve_id[%d] not legal Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, event_id);
		return;
	}

	//role achieve_map没有该成就项就新增一个
	if (!achieve_common_t::add_element(achieve_common_achieve,uid, achieve_id))
	{
		log_error("role[%lu] add achieve[%d] fail!", uid, achieve_id);
		return;
	}

	achieve_ptr p_achieve = achieve_manager_t::get_achieve(uid, achieve_id);
	if (NULL == p_achieve)
	{
		log_error("NULL == p_achieve role_uid[%lu] achieve_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, event_id);
		return;
	}

	const uint32_t cur_level = p_achieve->get_level();
	uint32_t cur_count_total = 0;
	if (!achieve_common_t::get_hero_achieve_total(uid,type,event_id,cur_count_total))
	{
		ACHIEVE_LOG("role[%lu] hero achieve[%d:%d] count is zero!",uid,achieve_id,cur_level);
		return;
	}

	ACHIEVE_LOG("cur_count_total[%d]", cur_count_total);

	const uint32_t old_count = p_achieve->get_count();
	if (p_achieve->get_count() < cur_count_total)
	{
		p_achieve->set_count(cur_count_total);
	}

	const uint32_t new_count = p_achieve->get_count();
	const uint32_t add_count = std::abs(new_count - old_count);
	p_achieve->save_self(uid, true);
	ACHIEVE_LOG("role[%lu] add achieve[%d] addcount[%d] old count/new count[%d/%d]", uid, achieve_id, add_count, old_count, new_count);

	//查找当前成就进度到了哪一级

	uint32_t last_level = cur_level;
	std::deque<uint32_t> drop_id_deque;
	bool result = false;//do{}while()结束 为true 表明可以升级
	do
	{
		if (!achieve_manager_t::achieve_up_last_level(type, new_count, drop_id_deque, last_level, event_id))
		{
			log_error("role[%lu] cannot up achieve[%d:%d] level", uid, achieve_id, cur_level);
			break;
		}

		//判断是否可以升级last_level 到达的等级 cur_level 当前等级
		if (last_level == cur_level)
		{
			const uint32_t count = achieve_common_t::get_target_count(achieve_common_achieve,achieve_id + cur_level + 1);
			const uint32_t cur_target_count = achieve_common_t::get_target_count(achieve_common_achieve,achieve_id + (cur_level == 0 ? cur_level + 1 : cur_level));
			ACHIEVE_LOG("role_uid[%lu] type[%d] current_level[%d] cur_count/target_count[%d/%d]", uid, type, cur_level, 
				new_count, (count == 0 ? cur_target_count : count));
			break;
		}
		if (drop_id_deque.empty())
		{
			log_error("drop_id_deque empty role[%lu] achieve_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, event_id);
			break;
		}

		result = true;
	} while (0);

	uint32_t log_type = achieve_manager_t::get_achieve_log_type(type);
	uint32_t  old_achieve_level = p_role->get_achieve_level();
	uint32_t old_achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_ACHIEVE_POINT);
	std::string src;
	if (!result)
	{
		//记录进度
		achieve_manager_t::get_source_type_str(src,achieve_id + (cur_level == 0 ? 1 : cur_level));
		log_wrapper_t::send_achieve_log(uid, achieve_id, cur_level, 0, cur_level, old_count, add_count, new_count,
			old_achieve_point, 0, old_achieve_point, old_achieve_level, 0, old_achieve_level,
			p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, 0,src);
		return;
	}

	//可以升级
	uint32_t loop_num = std::abs(last_level - cur_level);
	ACHIEVE_LOG("role[%lu] achieve[%d] begin up level", uid, achieve_id);
	bool is_start = true;//用于标记记录日志
	while (0 < loop_num)
	{
		const uint32_t old_level = p_achieve->get_level();
		if (!p_achieve->add_level())
		{
			achieve_manager_t::get_source_type_str(src,achieve_id + (old_level == 0 ? 1 : old_level));
			log_error("role[%lu] up achieve[%d:%d] level fail!", uid, achieve_id, old_level);
			log_wrapper_t::send_achieve_log(uid, achieve_id, old_level, 0, old_level, (is_start ? old_count : new_count), (is_start ? add_count : 0), new_count,
				old_achieve_point,0,old_achieve_point,old_achieve_level,0,old_achieve_level,
				p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, 0,src);
			return;
		}

		proto::client::gc_achieve_finish_notify notify;
		bool b_result = drop_manager_t::drop(p_role, drop_id_deque.front(), log_enum::source_type_achieve_single_finish,
			proto::common::drop_sys_type_achieve, achieve_id, notify.mutable_rcd());

		if (!b_result)
		{
			//此循环需要结束 不能中途中断 
			log_error("b_result false role[%lu] achieve_id[%d] drop_id[%d]", uid, achieve_id, drop_id_deque.front());
		}
		
		const uint32_t new_level = p_achieve->get_level();
		const uint32_t  new_achieve_level = p_role->get_achieve_level();
		const uint32_t new_achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_ACHIEVE_POINT);
		const uint32_t add_achieve_point = std::abs(new_achieve_point - old_achieve_point);
		const uint32_t up_achieve_level = std::abs(new_achieve_level - old_achieve_level);
		const uint32_t up_level = std::abs(new_level - old_level);

		achieve_manager_t::get_source_type_str(src,achieve_id + (new_level == 0 ? 1 : new_level));
		log_wrapper_t::send_achieve_log(uid, achieve_id, old_level, up_level, new_level, (is_start ? old_count : new_count), (is_start ? add_count : 0), new_count,
			old_achieve_point, add_achieve_point, new_achieve_point, old_achieve_level, up_achieve_level, new_achieve_level,
			p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(),log_type, drop_id_deque.front(),src);
		
		is_start = false; //add_count 设为0
		old_achieve_level = new_achieve_level;
		old_achieve_point = new_achieve_point;

		drop_id_deque.pop_front();															//从掉落队列删除
		achieve_manager_t::notify_client_achieve_finish(uid, achieve_id, notify);			//通知客户端
		p_role->save_self(true);
		p_achieve->save_self(uid, true);
		ACHIEVE_LOG("finish achieve role_uid[%lu] Achieve_Event_Type[%d] achieve_id[%d]", uid, type, achieve_id);

		--loop_num;
	}
	BROADCAST_SYS_NOTICE_SYS_CHANNEL(uid, proto::common::sys_notify_type_finish_achieve, achieve_id + p_achieve->get_level(), p_role->get_name().c_str());
}

void achieve_method_t::achieve_add_family(const uint64_t uid, const proto::common::Achieve_Event_Type type,uint32_t param1, uint32_t param2)
{
	
}

void achieve_method_t::achieve_add_country(const uint64_t uid, const proto::common::Achieve_Event_Type type,uint32_t param1,uint32_t param)
{

}

void achieve_method_t::achieve_exattr(uint64_t uid, proto::common::Achieve_Event_Type type, std::vector<uint32_t>& vec)
{

}

void achieve_method_t::achieve_equip_type_same(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t suit_id, uint32_t param)
{

}

void achieve_method_t::achieve_equip_exattr(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t star_level,uint32_t param)
{
	
}
//n * n 类型的统一处理 （类似 xx星 xx个/xx级 xx 个）
void achieve_method_t::achieve_unify_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id,uint32_t num)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}

	uint32_t achieve_id = 0;
	if (!achieve_common_t::get_config_id(uid,achieve_common_achieve,type, achieve_id, event_id))
	{
		ACHIEVE_LOG("role_uid[%lu] achieve_id[%d] not legal Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, event_id);
		return;
	}

	//role achieve_map没有该成就项就新增一个
	if (!achieve_common_t::add_element(achieve_common_achieve,uid, achieve_id))
	{
		log_error("role[%lu],add achieve[%d] fail!", uid, achieve_id);
		return;
	}

	achieve_ptr p_achieve = achieve_manager_t::get_achieve(uid, achieve_id);
	if (NULL == p_achieve)
	{
		log_error("NULL == p_achieve role_uid[%lu] achieve_id[%d] Achieve_Event_Type[%d] event_id [%d]", uid, achieve_id, type, event_id);
		return;
	}

	uint32_t old_count = p_achieve->get_count();
	p_achieve->set_count(old_count + num);
	const uint32_t new_count = p_achieve->get_count();
	const uint32_t add_count = new_count - old_count;
	p_achieve->save_self(uid, true);
	ACHIEVE_LOG("role[%lu] add achieve[%d] addcount[%d] old count/new count[%d/%d]", uid, achieve_id, add_count, old_count, new_count);
	
	//查找当前成就进度到了哪一级
	const uint32_t cur_level = p_achieve->get_level();
	uint32_t last_level = cur_level;
	std::deque<uint32_t> drop_id_deque;
	bool result = false;//do{}while()结束 为true 表明可以升级
	do
	{
		if (!achieve_manager_t::achieve_up_last_level(type, new_count, drop_id_deque, last_level, event_id))
		{
			log_error("role[%lu] cannot up achieve[%d:%d] level", uid, achieve_id, cur_level);
			break;
		}

		//判断是否可以升级last_level 到达的等级 cur_level 当前等级
		if (last_level == cur_level)
		{
			const uint32_t count = achieve_common_t::get_target_count( achieve_common_achieve,achieve_id + cur_level + 1);
			const uint32_t cur_target_count = achieve_common_t::get_target_count(achieve_common_achieve,achieve_id + (cur_level == 0 ? cur_level + 1 : cur_level));
			ACHIEVE_LOG("role_uid[%lu] type[%d] current_level[%d] cur_count/target_count[%d/%d]", uid, type, cur_level, 
				new_count, (count == 0 ? cur_target_count : count));
			break;
		}
		if (drop_id_deque.empty())
		{
			log_error("drop_id_deque empty role[%lu] achieve_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, event_id);
			break;
		}

		result = true;
	} while (0);

	uint32_t log_type = achieve_manager_t::get_achieve_log_type(type);
	uint32_t  old_achieve_level = p_role->get_achieve_level();
	uint32_t old_achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_ACHIEVE_POINT);
	std::string src;
	if (!result)
	{
		//记录进度
		achieve_manager_t::get_source_type_str(src,achieve_id + (cur_level == 0 ? 1 : cur_level));
		log_wrapper_t::send_achieve_log(uid, achieve_id, cur_level, 0, cur_level, old_count, add_count, new_count,
			old_achieve_point, 0, old_achieve_point, old_achieve_level, 0, old_achieve_level,
			p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, 0,src);
		return;
	}

	//可以升级
	uint32_t loop_num = std::abs(last_level - cur_level);
	ACHIEVE_LOG("role[%lu] achieve[%d] begin up level", uid, achieve_id);
	bool is_start = true;//用于标记记录日志
	while (0 < loop_num)
	{
		const uint32_t old_level = p_achieve->get_level();
		if (!p_achieve->add_level())
		{
			achieve_manager_t::get_source_type_str(src,achieve_id + (old_level == 0 ? 1 : old_level));
			log_error("role[%lu] up achieve[%d:%d] level fail!", uid, achieve_id, old_level);
			log_wrapper_t::send_achieve_log(uid, achieve_id, old_level, 0, old_level, (is_start ? old_count : new_count), (is_start ? add_count : 0), new_count,
				old_achieve_point, 0, old_achieve_point, old_achieve_level, 0, old_achieve_level,
				p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, 0,src);
			return;
		}

		proto::client::gc_achieve_finish_notify notify;
		bool b_result = drop_manager_t::drop(p_role, drop_id_deque.front(), log_enum::source_type_achieve_single_finish,
			proto::common::drop_sys_type_achieve, achieve_id, notify.mutable_rcd());

		if (!b_result)
		{
			//此循环需要结束 不能中途中断 
			log_error("b_result false role[%lu] achieve_id[%d] drop_id[%d]", uid, achieve_id, drop_id_deque.front());
		}

		const uint32_t new_level = p_achieve->get_level();
		const uint32_t  new_achieve_level = p_role->get_achieve_level();
		const uint32_t new_achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_ACHIEVE_POINT);
		const uint32_t add_achieve_point = std::abs(new_achieve_point - old_achieve_point);
		const uint32_t up_achieve_level = std::abs(new_achieve_level - old_achieve_level);
		const uint32_t up_level = std::abs(new_level - old_level);

		achieve_manager_t::get_source_type_str(src,achieve_id + (new_level == 0 ? 1 : new_level));
		log_wrapper_t::send_achieve_log(uid, achieve_id, old_level, up_level, new_level, (is_start ? old_count : new_count), (is_start ? add_count : 0), new_count,
			old_achieve_point, add_achieve_point, new_achieve_point, old_achieve_level, up_achieve_level, new_achieve_level,
			p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(),log_type, drop_id_deque.front(),src);

		is_start = false; //add_count 设为0
		old_achieve_level = new_achieve_level;
		old_achieve_point = new_achieve_point;

		drop_id_deque.pop_front();															//从掉落队列删除
		achieve_manager_t::notify_client_achieve_finish(uid, achieve_id, notify);			//通知客户端
		p_role->save_self(true);
		p_achieve->save_self(uid, true);
		ACHIEVE_LOG("finish achieve role_uid[%lu] Achieve_Event_Type[%d] achieve_id[%d]", uid, type, achieve_id);

		--loop_num;
	}
	BROADCAST_SYS_NOTICE_SYS_CHANNEL(uid, proto::common::sys_notify_type_finish_achieve, achieve_id + p_achieve->get_level(), p_role->get_name().c_str());
}
//累计类型统一处理 类似铜币累加
void achieve_method_t::achieve_total_unify_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type,uint32_t param , uint32_t num )
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}

	uint32_t achieve_id = 0;
	if (!achieve_common_t::get_config_id(uid,achieve_common_achieve,type, achieve_id))
	{
		ACHIEVE_LOG("role_uid[%lu] achieve_id[%d] not legal Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, 0);
		return;
	}
	//role achieve_map没有该成就项就新增一个
	if (!achieve_common_t::add_element(achieve_common_achieve,uid, achieve_id))
	{
		log_error("role[%lu] add achieve[%d] fail!", uid, achieve_id);
		return;
	}

	achieve_ptr p_achieve = achieve_manager_t::get_achieve(uid, achieve_id);
	if (NULL == p_achieve)
	{
		log_error("NULL == p_achieve role_uid[%lu] achieve_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, 0);
		return;
	}

	uint32_t old_count = p_achieve->get_count();
	p_achieve->set_count(old_count + num);
	const uint32_t new_count = p_achieve->get_count();
	const uint32_t add_count = new_count - old_count;
	p_achieve->save_self(uid, true);
	ACHIEVE_LOG("role[%lu] add achieve[%d] addcount[%d] old count/new count[%d/%d]", uid, achieve_id, add_count, old_count, new_count);

	//查找当前成就进度到了哪一级
	const uint32_t cur_level = p_achieve->get_level();
	uint32_t last_level = cur_level;
	std::deque<uint32_t> drop_id_deque;
	bool result = false;//do{}while()结束 为true 表明可以升级
	do
	{
		if (!achieve_manager_t::achieve_up_last_level(type, new_count, drop_id_deque, last_level))
		{
			ACHIEVE_LOG("role[%lu] cannot up achieve[%d:%d] level", uid, achieve_id, cur_level);
			break;
		}

		//判断是否可以升级last_level 到达的等级 cur_level 当前等级
		if (last_level == cur_level)
		{
			const uint32_t count = achieve_common_t::get_target_count(achieve_common_achieve,achieve_id + cur_level + 1);
			const uint32_t cur_target_count = achieve_common_t::get_target_count(achieve_common_achieve,achieve_id + (cur_level == 0 ? cur_level + 1 : cur_level));
			ACHIEVE_LOG("role_uid[%lu] type[%d] current_level[%d] cur_count/target_count[%d/%d]", uid, type, cur_level, 
				new_count, (count == 0 ? cur_target_count : count));
			break;
		}
		if (drop_id_deque.empty())
		{
			log_error("drop_id_deque empty role[%lu] achieve_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, 0);
			break;
		}

		result = true;
	} while (0);

	uint32_t log_type = achieve_manager_t::get_achieve_log_type(type);
	uint32_t  old_achieve_level = p_role->get_achieve_level();
	uint32_t old_achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_ACHIEVE_POINT);
	std::string src;
	if (!result)
	{
		//记录进度
		achieve_manager_t::get_source_type_str(src,achieve_id + (cur_level == 0 ? 1 : cur_level));
		log_wrapper_t::send_achieve_log(uid, achieve_id, cur_level, 0, cur_level, old_count, add_count, new_count,
			old_achieve_point, 0, old_achieve_point, old_achieve_level, 0, old_achieve_level,
			p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, 0,src);
		return;
	}

	//可以升级
	uint32_t loop_num = std::abs(last_level - cur_level);
	ACHIEVE_LOG("role[%lu] achieve[%d] begin up level", uid, achieve_id);
	bool is_start = true;//用于标记记录日志
	while (0 < loop_num)
	{
		const uint32_t old_level = p_achieve->get_level();
		if (!p_achieve->add_level())
		{
			achieve_manager_t::get_source_type_str(src,achieve_id + (old_level == 0 ? 1 : old_level));
			log_error("role[%lu] up achieve[%d:%d] level fail!", uid, achieve_id, old_level);
			log_wrapper_t::send_achieve_log(uid, achieve_id, old_level, 0, old_level, (is_start ? old_count : new_count), (is_start ? add_count : 0), new_count,
				old_achieve_point, 0, old_achieve_point, old_achieve_level, 0, old_achieve_level,
				p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, 0,src);
			return;
		}

		proto::client::gc_achieve_finish_notify notify;
		bool b_result = drop_manager_t::drop(p_role, drop_id_deque.front(), log_enum::source_type_achieve_single_finish,
			proto::common::drop_sys_type_achieve, achieve_id, notify.mutable_rcd());

		if (!b_result)
		{
			//此循环需要结束 不能中途中断 
			log_error("b_result false role[%lu] achieve_id[%d] drop_id[%d]", uid, achieve_id, drop_id_deque.front());
		}

		const uint32_t new_level = p_achieve->get_level();
		const uint32_t  new_achieve_level = p_role->get_achieve_level();
		const uint32_t new_achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_ACHIEVE_POINT);
		const uint32_t add_achieve_point = std::abs(new_achieve_point - old_achieve_point);
		const uint32_t up_achieve_level = std::abs(new_achieve_level - old_achieve_level);
		const uint32_t up_level = std::abs(new_level - old_level);

		achieve_manager_t::get_source_type_str(src, achieve_id + (new_level == 0 ? 1 : new_level));
		log_wrapper_t::send_achieve_log(uid, achieve_id, old_level, up_level, new_level, (is_start ? old_count : new_count), (is_start ? add_count : 0), new_count,
			old_achieve_point, add_achieve_point, new_achieve_point, old_achieve_level, up_achieve_level, new_achieve_level,
			p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(),log_type, drop_id_deque.front(),src);

		is_start = false; //add_count 设为0
		old_achieve_level = new_achieve_level;
		old_achieve_point = new_achieve_point;

		drop_id_deque.pop_front();															//从掉落队列删除
		achieve_manager_t::notify_client_achieve_finish(uid, achieve_id, notify);			//通知客户端
		p_role->save_self(true);
		p_achieve->save_self(uid, true);
		ACHIEVE_LOG("finish achieve role_uid[%lu] Achieve_Event_Type[%d] achieve_id[%d]", uid, type, achieve_id);

		--loop_num;
	}
	BROADCAST_SYS_NOTICE_SYS_CHANNEL(uid, proto::common::sys_notify_type_finish_achieve, achieve_id + p_achieve->get_level(), p_role->get_name().c_str());
}

void achieve_method_t::achieve_replace_model_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id, uint32_t new_value)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}

	uint32_t achieve_id = 0;
	if (!achieve_common_t::get_config_id(uid, achieve_common_achieve, type, achieve_id))
	{
		ACHIEVE_LOG("role_uid[%lu] achieve_id[%d] not legal Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, event_id);
		return;
	}
	//role achieve_map没有该成就项就新增一个
	if (!achieve_common_t::add_element(achieve_common_achieve, uid, achieve_id))
	{
		log_error("role[%lu] add achieve[%d] fail!", uid, achieve_id);
		return;
	}

	achieve_ptr p_achieve = achieve_manager_t::get_achieve(uid, achieve_id);
	if (NULL == p_achieve)
	{
		log_error("NULL == p_achieve role_uid[%lu] achieve_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, event_id);
		return;
	}

	const uint32_t old_count = p_achieve->get_count();
	if (p_achieve->get_count() < new_value)
	{
		p_achieve->set_count(new_value);
	}

	const uint32_t new_count = p_achieve->get_count();
	const uint32_t add_count = new_count - old_count;
	p_achieve->save_self(uid, true);
	ACHIEVE_LOG("role[%lu] add achieve[%d] addcount[%d] old count/new count[%d/%d]", uid, achieve_id, add_count, old_count, new_count);

	//查找当前成就进度到了哪一级
	const uint32_t cur_level = p_achieve->get_level();
	uint32_t last_level = cur_level;
	std::deque<uint32_t> drop_id_deque;
	bool result = false;//do{}while()结束 为true 表明可以升级
	do
	{
		if (!achieve_manager_t::achieve_up_last_level(type, new_count, drop_id_deque, last_level))
		{
			log_error("role[%lu] cannot up achieve[%d:%d] level", uid, achieve_id, cur_level);
			break;
		}

		//判断是否可以升级last_level 到达的等级 cur_level 当前等级
		if (last_level == cur_level)
		{
			const uint32_t count = achieve_common_t::get_target_count(achieve_common_achieve, achieve_id + cur_level + 1);
			const uint32_t cur_target_count = achieve_common_t::get_target_count(achieve_common_achieve, achieve_id + (cur_level == 0 ? cur_level + 1 : cur_level));
			ACHIEVE_LOG("role_uid[%lu] type[%d] current_level[%d] cur_count/target_count[%d/%d]", uid, type, cur_level,
				new_count, (count == 0 ? cur_target_count : count));
			break;
		}
		if (drop_id_deque.empty())
		{
			log_error("drop_id_deque empty role_uid[%lu] achieve_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, event_id);
			break;
		}

		result = true;
	} while (0);

	uint32_t log_type = achieve_manager_t::get_achieve_log_type(type);
	uint32_t  old_achieve_level = p_role->get_achieve_level();
	uint32_t old_achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_ACHIEVE_POINT);
	std::string src;
	if (!result)
	{
		//记录进度
		achieve_manager_t::get_source_type_str(src, achieve_id + (cur_level == 0 ? 1 : cur_level));
		log_wrapper_t::send_achieve_log(uid, achieve_id, cur_level, 0, cur_level, old_count, add_count, new_count,
			old_achieve_point, 0, old_achieve_point, old_achieve_level, 0, old_achieve_level,
			p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, 0, src);
		return;
	}

	//可以升级
	uint32_t loop_num = std::abs(last_level - cur_level);
	ACHIEVE_LOG("role[%lu] achieve[%d] begin up level", uid, achieve_id);
	bool is_start = true;//用于标记记录日志
	while (0 < loop_num)
	{
		const uint32_t old_level = p_achieve->get_level();
		if (!p_achieve->add_level())
		{
			achieve_manager_t::get_source_type_str(src, achieve_id + (old_level == 0 ? 1 : old_level));
			log_error("role[%lu] up achieve[%d:%d] level fail!", uid, achieve_id, old_level);
			log_wrapper_t::send_achieve_log(uid, achieve_id, old_level, 0, old_level, (is_start ? old_count : new_count), (is_start ? add_count : 0), new_count,
				old_achieve_point, 0, old_achieve_point, old_achieve_level, 0, old_achieve_level,
				p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, 0, src);
			return;
		}

		proto::client::gc_achieve_finish_notify notify;
		bool b_result = drop_manager_t::drop(p_role, drop_id_deque.front(), log_enum::source_type_achieve_single_finish,
			proto::common::drop_sys_type_achieve, achieve_id, notify.mutable_rcd());

		if (!b_result)
		{
			//此循环需要结束 不能中途中断 
			log_error("b_result false role[%lu] achieve_id[%d] drop_id[%d]", uid, achieve_id, drop_id_deque.front());
		}

		const uint32_t new_level = p_achieve->get_level();
		const uint32_t  new_achieve_level = p_role->get_achieve_level();
		const uint32_t new_achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_ACHIEVE_POINT);
		const uint32_t add_achieve_point = std::abs(new_achieve_point - old_achieve_point);
		const uint32_t up_achieve_level = std::abs(new_achieve_level - old_achieve_level);
		const uint32_t up_level = std::abs(new_level - old_level);

		achieve_manager_t::get_source_type_str(src, achieve_id + (new_level == 0 ? 1 : new_level));
		log_wrapper_t::send_achieve_log(uid, achieve_id, old_level, up_level, new_level, (is_start ? old_count : new_count), (is_start ? add_count : 0), new_count,
			old_achieve_point, add_achieve_point, new_achieve_point, old_achieve_level, up_achieve_level, new_achieve_level,
			p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, drop_id_deque.front(), src);

		is_start = false; //add_count 设为0
		old_achieve_level = new_achieve_level;
		old_achieve_point = new_achieve_point;

		drop_id_deque.pop_front();															//从掉落队列删除
		achieve_manager_t::notify_client_achieve_finish(uid, achieve_id, notify);			//通知客户端
		p_role->save_self(true);
		p_achieve->save_self(uid, true);
		ACHIEVE_LOG("finish achieve role_uid[%lu] Achieve_Event_Type[%d] achieve_id[%d]", uid, type, achieve_id);

		--loop_num;
	}
	BROADCAST_SYS_NOTICE_SYS_CHANNEL(uid, proto::common::sys_notify_type_finish_achieve, achieve_id + p_achieve->get_level(), p_role->get_name().c_str());
}

void achieve_method_t::achieve_direct_replace_model_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id /*= 0*/, uint32_t new_value /*= 0*/)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}

	uint32_t achieve_id = 0;
	if (!achieve_common_t::get_config_id(uid, achieve_common_achieve, type, achieve_id))
	{
		ACHIEVE_LOG("role_uid[%lu] achieve_id[%d] not legal Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, event_id);
		return;
	}
	//role achieve_map没有该成就项就新增一个
	if (!achieve_common_t::add_element(achieve_common_achieve, uid, achieve_id))
	{
		log_error("role[%lu] add achieve[%d] fail!", uid, achieve_id);
		return;
	}

	achieve_ptr p_achieve = achieve_manager_t::get_achieve(uid, achieve_id);
	if (NULL == p_achieve)
	{
		log_error("NULL == p_achieve role_uid[%lu] achieve_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, event_id);
		return;
	}

	const uint32_t old_count = p_achieve->get_count();
	p_achieve->set_count(new_value);

	const uint32_t new_count = p_achieve->get_count();
	const uint32_t add_count = new_count - old_count;
	p_achieve->save_self(uid, true);
	ACHIEVE_LOG("role[%lu] add achieve[%d] addcount[%d] old count/new count[%d/%d]", uid, achieve_id, add_count, old_count, new_count);

	//查找当前成就进度到了哪一级
	const uint32_t cur_level = p_achieve->get_level();
	uint32_t last_level = cur_level;
	std::deque<uint32_t> drop_id_deque;
	bool result = false;//do{}while()结束 为true 表明可以升级
	do
	{
		if (!achieve_manager_t::achieve_up_last_level(type, new_count, drop_id_deque, last_level))
		{
			log_error("role[%lu] cannot up achieve[%d:%d] level", uid, achieve_id, cur_level);
			break;
		}

		//判断是否可以升级last_level 到达的等级 cur_level 当前等级
		if (last_level == cur_level)
		{
			const uint32_t count = achieve_common_t::get_target_count(achieve_common_achieve, achieve_id + cur_level + 1);
			const uint32_t cur_target_count = achieve_common_t::get_target_count(achieve_common_achieve, achieve_id + (cur_level == 0 ? cur_level + 1 : cur_level));
			ACHIEVE_LOG("role_uid[%lu] type[%d] current_level[%d] cur_count/target_count[%d/%d]", uid, type, cur_level,
				new_count, (count == 0 ? cur_target_count : count));
			break;
		}
		if (drop_id_deque.empty())
		{
			log_error("drop_id_deque empty role_uid[%lu] achieve_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, event_id);
			break;
		}

		result = true;
	} while (0);

	uint32_t log_type = achieve_manager_t::get_achieve_log_type(type);
	uint32_t  old_achieve_level = p_role->get_achieve_level();
	uint32_t old_achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_ACHIEVE_POINT);
	std::string src;
	if (!result)
	{
		//记录进度
		achieve_manager_t::get_source_type_str(src, achieve_id + (cur_level == 0 ? 1 : cur_level));
		log_wrapper_t::send_achieve_log(uid, achieve_id, cur_level, 0, cur_level, old_count, add_count, new_count,
			old_achieve_point, 0, old_achieve_point, old_achieve_level, 0, old_achieve_level,
			p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, 0, src);
		return;
	}

	//可以升级
	uint32_t loop_num = std::abs(last_level - cur_level);
	ACHIEVE_LOG("role[%lu] achieve[%d] begin up level", uid, achieve_id);
	bool is_start = true;//用于标记记录日志
	while (0 < loop_num)
	{
		const uint32_t old_level = p_achieve->get_level();
		if (!p_achieve->add_level())
		{
			achieve_manager_t::get_source_type_str(src, achieve_id + (old_level == 0 ? 1 : old_level));
			log_error("role[%lu] up achieve[%d:%d] level fail!", uid, achieve_id, old_level);
			log_wrapper_t::send_achieve_log(uid, achieve_id, old_level, 0, old_level, (is_start ? old_count : new_count), (is_start ? add_count : 0), new_count,
				old_achieve_point, 0, old_achieve_point, old_achieve_level, 0, old_achieve_level,
				p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, 0, src);
			return;
		}

		proto::client::gc_achieve_finish_notify notify;
		bool b_result = drop_manager_t::drop(p_role, drop_id_deque.front(), log_enum::source_type_achieve_single_finish,
			proto::common::drop_sys_type_achieve, achieve_id, notify.mutable_rcd());

		if (!b_result)
		{
			//此循环需要结束 不能中途中断 
			log_error("b_result false role[%lu] achieve_id[%d] drop_id[%d]", uid, achieve_id, drop_id_deque.front());
		}

		const uint32_t new_level = p_achieve->get_level();
		const uint32_t  new_achieve_level = p_role->get_achieve_level();
		const uint32_t new_achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE::MONEY_TYPE_ACHIEVE_POINT);
		const uint32_t add_achieve_point = std::abs(new_achieve_point - old_achieve_point);
		const uint32_t up_achieve_level = std::abs(new_achieve_level - old_achieve_level);
		const uint32_t up_level = std::abs(new_level - old_level);

		achieve_manager_t::get_source_type_str(src, achieve_id + (new_level == 0 ? 1 : new_level));
		log_wrapper_t::send_achieve_log(uid, achieve_id, old_level, up_level, new_level, (is_start ? old_count : new_count), (is_start ? add_count : 0), new_count,
			old_achieve_point, add_achieve_point, new_achieve_point, old_achieve_level, up_achieve_level, new_achieve_level,
			p_role->get_achieve_recv_reward_level(), p_role->get_achieve_recv_reward_level(), log_type, drop_id_deque.front(), src);

		is_start = false; //add_count 设为0
		old_achieve_level = new_achieve_level;
		old_achieve_point = new_achieve_point;

		drop_id_deque.pop_front();															//从掉落队列删除
		achieve_manager_t::notify_client_achieve_finish(uid, achieve_id, notify);			//通知客户端
		p_role->save_self(true);
		p_achieve->save_self(uid, true);
		ACHIEVE_LOG("finish achieve role_uid[%lu] Achieve_Event_Type[%d] achieve_id[%d]", uid, type, achieve_id);

		--loop_num;
	}
	BROADCAST_SYS_NOTICE_SYS_CHANNEL(uid, proto::common::sys_notify_type_finish_achieve, achieve_id + p_achieve->get_level(), p_role->get_name().c_str());
}
