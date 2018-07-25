#include "collect_point.hpp"
#include "config/config_manager.hpp"
#include "tblh/CollectTable.tbls.h"
#include "item/drop_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "scene/scene_manager.hpp"
#include "collect_manager.hpp"
#include "king_war/king_war_scene.hpp"

collect_point_t::collect_point_t(uint32_t tid)
	: m_tid(tid)
{
}


collect_point_t::~collect_point_t()
{
}

void collect_point_t::del_clear()
{
	cancel_refresh_timer();
}

void collect_point_t::close()
{
	cancel_refresh_timer();
}

void collect_point_t::get_object_info(proto::common::object_common_info* object_info)
{
	if (NULL == object_info)
	{
		log_error("object_info null error");
		return;
	}

	object_base_t::get_object_info(object_info);

	object_info->set_collect_tid(m_tid);
}

bool collect_point_t::init_collect_point(float x, float y, uint64_t object_uid)
{
	CollectTable *p_config = GET_CONF(CollectTable, m_tid);
	if (NULL == p_config)
	{
		log_error("collect p_config NULL tid[%d]", m_tid);
		return false;
	}

	if(object_uid == 0)
		set_object_id(collect_manager_t::get_collect_point_uid());

	set_current_position(x, y);
	m_collect_count = 0;

	m_collect_type = (proto::common::collect_type)p_config->type();

	// 0为无限次采集 不需要时间控制
	if(p_config->collect_times_max() > 0)
		m_refresh_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));

	m_collect_time = p_config->collect_time();

	return true;
}

uint32_t collect_point_t::can_collect(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("collect p_role NULL");
		return errcode_enum::notice_unknown;
	}

	// 死亡不能采集
	if (p_role->is_die())
	{
		log_error("role[%lu] is die", p_role->get_object_id());
		return errcode_enum::sys_notice_you_dead;
	}

	if (p_role->is_troop_member_not_leave()) {
		log_error("role[%lu] troop member can't collect", p_role->get_uid());
		return errcode_enum::notice_unknown;
	}

	CollectTable *p_config = GET_CONF(CollectTable, m_tid);
	if (NULL == p_config)
	{
		log_error("collect p_config NULL tid[%d]", m_tid);
		return errcode_enum::notice_unknown;
	}

	// 判断状态
	if (is_die())
	{
		return errcode_enum::notice_collect_max_times;
	}

	// 判断次数限制 0为无限制
	if (p_config->collect_times_max() != 0 && m_collect_count >= p_config->collect_times_max())
		return errcode_enum::notice_collect_max_times;
	

	switch (p_config->collect_cond())
	{
	case proto::common::collect_cond_type_task:
		{	// 检查任务状态 未接取和已完成两种状态认为不满足条件
			task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
			if (NULL == p_task_mgr)
			{
				log_error("drop task_mgr NULL role_uid[%lu]", p_role->get_uid());
				return errcode_enum::notice_unknown;
			}
			task_ptr p_task = p_task_mgr->get_task(p_config->collect_cond_param(0));
			if (NULL == p_task || (p_task->get_state() != TASK_STATE_DOING && p_task->get_state() != TASK_STATE_CAN_FINISH))
				return errcode_enum::notice_task_id_not_exist;
		}
		break;
	case proto::common::collect_cond_type_level:
		{
			if (p_role->get_level() < p_config->collect_cond_param(0))
			{
				return errcode_enum::error_level_not_enough;
			}
		}
		break;
	case proto::common::collect_cond_type_king_war_family:
		{
			const scene_ptr& p_country_scene = scene_manager_t::find_scene(m_scene_id);
			if (NULL == p_country_scene)
			{
				log_error("scene[%lu] can not find", m_scene_id);
				return errcode_enum::notice_unknown;
			}

			if (!p_country_scene->is_king_war_city())
			{
				log_error("scene[%lu] is not king scene", m_scene_id);
				return errcode_enum::notice_unknown;
			}

			king_war_scene_ptr p_king_scene = p_country_scene->get_king_war_scene_ptr();
			if (p_king_scene == NULL)
			{
				log_error("scene[%lu] is not king secne", m_scene_id);
				return errcode_enum::notice_unknown;
			}

			uint32_t back_code = p_king_scene->is_can_collect_artifacts(p_role);
			if (back_code != common::errcode_enum::error_ok)
			{
				return back_code;
			}
		}
		break;
	default:
		break;
	}

	if (!p_config->is_share())
	{
		// 有可能采集中的人掉线 收不到结束采集消息 判断一下上次采集时间
		uint32_t now_time = common::time_util_t::now_time();
		if (now_time - m_last_collect_time >= m_collect_time)
			m_collecting = false;

		if (m_collecting)
			return errcode_enum::notice_is_collecting_by_other;
	}

	// 普通类型检测掉落所需格子
	if (m_collect_type == proto::common::collect_type_normal)
	{
		if (!drop_manager_t::check_bag_num(p_role, p_config->drop_bag_id()))
		{
			return errcode_enum::notice_main_bag_max_size;
		}
	}

	return errcode_enum::error_ok;
}

uint32_t collect_point_t::start_collect(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("collect p_role NULL");
		return errcode_enum::notice_unknown;
	}
	m_collecting = true;
	m_last_collect_time = common::time_util_t::now_time();
	return errcode_enum::error_ok;
}

uint32_t collect_point_t::end_collect(role_ptr p_role, proto::common::role_change_data *rcd)
{
	if (NULL == p_role)
	{
		log_error("collect p_role NULL");
		return errcode_enum::notice_unknown;
	}

	m_collecting = false;
	++m_collect_count;
	
	CollectTable *p_config = GET_CONF(CollectTable, m_tid);
	if (NULL == p_config)
	{
		log_error("collect p_config NULL tid[%d]", m_tid);
		return errcode_enum::notice_unknown;
	}

	if (p_config->collect_times_max() != 0 && m_collect_count > p_config->collect_times_max())
		return errcode_enum::notice_collect_max_times;

	switch (m_collect_type)
	{
	case proto::common::collect_type_normal:
		{
			// 采集奖励
			drop_manager_t::drop(p_role, p_config->drop_bag_id(), log_enum::source_type_role_collect,
				proto::common::drop_sys_type_collect, m_tid, rcd);
		}
		break;
	case proto::common::collect_type_artifacts:
		{
			const scene_ptr& p_country_scene = scene_manager_t::find_scene(m_scene_id);
			if (NULL == p_country_scene)
			{
				log_error("scene[%lu] can not find", m_scene_id);
				return errcode_enum::notice_unknown;
			}

			if (!p_country_scene->is_king_war_city())
			{
				log_error("scene[%lu] is not king scene", m_scene_id);
				return errcode_enum::notice_unknown;
			}

			king_war_scene_ptr p_king_scene = p_country_scene->get_king_war_scene_ptr();
			if (p_king_scene == NULL)
			{
				log_error("scene[%lu] is not king secne", m_scene_id);
				return errcode_enum::notice_unknown;
			}

			p_king_scene->role_get_artifacts(p_role, m_object_id, p_config->drop_bag_id());
		}
		break;
	default:
		{
			log_error("collect_point_t[%u] type[%u] end_collect error ", m_tid, m_collect_type);
			return errcode_enum::notice_unknown;
		}
		break;
	}

	// 判断次数限制
	if (p_config->collect_times_max() != 0 && m_collect_count == p_config->collect_times_max())
	{
		set_object_die();
		// 消失
		leave_scene();

		if (p_config->refresh_time() != 0) {
			start_refresh_timer(p_config->refresh_time());
		} else { // 刷新时间为0 说明不刷新
				 // 由于是智能指针所以没事
			collect_manager_t::delete_collect_point(m_object_id);
		}
	}
	
	return errcode_enum::error_ok;
}

void collect_point_t::cancel_collect(const role_ptr& p_role)
{
	m_collecting = false;
}

void collect_point_t::start_refresh_timer(uint32_t refresh_time)
{
	if (NULL == m_refresh_timer)
		return;

	if (0 == refresh_time)
	{
		log_error("city_manager refresh_time = 0");
		return;
	}
	m_refresh_timer->expires_from_now(boost::posix_time::seconds(refresh_time));
	m_refresh_timer->async_wait(boost::bind(&collect_manager_t::refresh_on_time, boost::asio::placeholders::error, get_object_id()));
}

void collect_point_t::cancel_refresh_timer()
{
	if (m_refresh_timer != NULL)
		m_refresh_timer->cancel();
}

void collect_point_t::relive()
{
	const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
	if (p_scene == NULL)
	{
		log_error("collect_point[%lu:%d] scene[%lu] NULL", get_object_id(), m_tid, m_scene_id);
		return;
	}
	 
	 
	// todo 坐标生成
	if (!init_collect_point(get_current_x(), get_current_y(), get_object_id()))
	{
		log_error("collect_point[%d] init error", m_tid);
		return;
	}
	 
	if ( !p_scene->init_collect_point(get_collect(), false) )
	{
		log_error("collect_point[%d] scene init error", m_tid);
		return;
	}
}
 
// void collect_point_t::refresh_on_time(const boost::system::error_code& ec)
// {
//     if (ec == boost::asio::error::operation_aborted)
//     {
//         
//         return;
//     }
//     else if (ec)
//     {
//         log_error("Timer error: %s", ec.message().c_str());
//         return;
//     }
// 
// 	const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
// 	if (p_scene == NULL)
// 	{
// 		log_error("collect_point[%lu:%d] scene[%lu] NULL", get_object_id(), m_tid, m_scene_id);
// 		return;
// 	}
// 
// 
// 	// todo 坐标生成
// 	if (!init_collect_point(get_current_x(), get_current_y(), get_object_id()))
// 	{
// 		log_error("collect_point[%d] init error", m_tid);
// 		return;
// 	}
// 
// 	if ( !p_scene->init_collect_point(shared_from_this(), false) )
// 	{
// 		log_error("collect_point[%d] scene init error", m_tid);
// 		return;
// 	}
// }