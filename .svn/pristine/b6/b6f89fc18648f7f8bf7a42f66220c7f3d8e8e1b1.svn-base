#include "mount_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "config/config_manager.hpp"
#include "tblh/MountTable.tbls.h"
#include "trade/trade.hpp"
#include "item/package.hpp"
#include "item/item_manager.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "red_point/red_point_manager.hpp"
#include "achieve/achieve_common.hpp"

#define ROLE_MOUNT_MAX_NUM	100

mount_manager_t::mount_manager_t()
{
}


mount_manager_t::~mount_manager_t()
{
}

void mount_manager_t::load_all_mounts(role_ptr role, const proto::common::mount_data &mount_data)
{
	if (NULL == role)
	{
		log_error("mount role NULL");
		return;
	}
	if (NULL == role->get_mount_data())
	{
		log_error("mount role->get_mount_data NULL");
		return;
	}

	role->get_mount_data()->load_data(mount_data);

	if(role->get_mount_data()->get_use_mount_tid() != 0)
		mount_manager_t::use_mount(role, role->get_mount_data()->get_use_mount_tid(), false);
}

uint32_t mount_manager_t::add_mount(role_ptr role, uint32_t mount_tid, proto::common::role_change_data *rcd)
{
	if (NULL == role)
	{
		log_error("mount role NULL");
		return errcode_enum::notice_role_not_exist;
	}
	if (NULL == role->get_mount_data())
	{
		log_error("mount role->get_mount_data NULL");
		return errcode_enum::notice_unknown;
	}
	if (role->get_mount_data()->get_mount(mount_tid) != NULL)
		return errcode_enum::notice_mount_has_one;

	auto conf = GET_CONF(Comprehensive, comprehensive_common::mount_limit);
	uint32_t mount_limit = GET_COMPREHENSIVE_VALUE_1(conf);
	if (role->get_mount_data()->get_mount_size() >= mount_limit)
	{
		log_error("mount mount_size[%d] > mount_limit[%d]", role->get_mount_data()->get_mount_size(), mount_limit);
		return errcode_enum::notice_mount_max_size;
	}

	MountTable *p_mount_config = GET_CONF(MountTable, mount_tid);
	if (NULL == p_mount_config)
	{
		log_error("mount p_mount_config NULL mount_tid[%d]", mount_tid);
		return errcode_enum::notice_mount_not_find;
	}

	mount_ptr p_mount = mount_ptr(new mount_t(mount_tid, proto::common::mount_state_type_normal));
	if (NULL == p_mount)
	{
		log_error("[wys] mount p_mount NULL");
		return errcode_enum::notice_unknown;
	}
	role->get_mount_data()->add_mount(p_mount);

	// 没有默认坐骑 就把第一个设为默认
	if (role->get_mount_data()->get_use_mount_tid() == 0)
	{
		mount_manager_t::use_mount(role, mount_tid);
	}
	if (rcd)
	{
		proto::common::mount_data_single *p_mount_data = rcd->mutable_mount()->add_mount_data_list();
		p_mount_data->set_mount_tid(mount_tid);
		p_mount_data->set_mount_state((proto::common::mount_state_type)p_mount->get_state());
	}
	red_point_manager_t::red_point_notify(role->get_uid(), proto::common::RED_POINT_TYPE::RED_POINT_ADD_MOUNT,mount_tid);

	achieve_common_t::notify_progress_state(role->get_uid(),proto::common::Achieve_Event_Type::ACHIEVE_MOUNT_NUMBER_TYPE, p_mount_config->quality());
	return errcode_enum::error_ok;
}

uint32_t mount_manager_t::use_mount(role_ptr role, uint32_t mount_tid, bool notify)
{
	if (NULL == role)
	{
		log_error("[wys] mount role NULL");
		return errcode_enum::notice_role_not_exist;
	}
	if (NULL == role->get_mount_data())
	{
		log_error("wys mount role->get_mount_data NULL");
		return errcode_enum::notice_unknown;
	}

	MountTable *p_config = GET_CONF(MountTable, mount_tid);
	if (NULL == p_config)
	{
		log_error("[wys] mount config[%d] NULL", mount_tid);
		return errcode_enum::notice_mount_not_find;
	}

	package_ptr p_packeage = item_manager_t::get_package(role, proto::common::package_type_trade);
	if (NULL == p_packeage)
	{
		log_error("[wys] mount package[%d] not find", proto::common::package_type_trade);
		return errcode_enum::notice_not_find_package;
	}
	if (p_packeage->get_bag_use_num() > p_config->space())
	{
		log_error("use_mount trade_package role[%lu] bag_use_num[%d] > mount_space[%d]", role->get_uid(), p_packeage->get_bag_use_num(), p_config->space());
		return errcode_enum::notice_mount_bag_bum_not_enough;
	}
	if (role->get_trade_info().get_trade_bag_weight() > p_config->weight())
	{
		log_error("use_mount trade_package role[%lu] bag_weight[%d] > mount_weight[%d]", role->get_uid(), role->get_trade_info().get_trade_bag_weight(), p_config->weight());
		return errcode_enum::notice_mount_bag_weight_not_enough;
	}

	if (!role->get_mount_data()->use_mount(mount_tid))
		return errcode_enum::notice_mount_not_find;
	role->change_real_speed(notify ? role_t::speed_forced_update : role_t::speed_only_set);
	p_packeage->set_max_size(p_config->space());
	role->get_trade_info().set_trade_bag_max_weight(p_config->weight());
	role->save_self();
	if (notify)
	{
		role->notify_object_common_info_for_nearbies();
	}
	return errcode_enum::error_ok;
}

uint32_t mount_manager_t::turn_mount(role_ptr role, uint32_t mount_tid)
{
	if (NULL == role)
	{
		log_error("[wys] mount role NULL");
		return errcode_enum::notice_role_not_exist;
	}
	if (NULL == role->get_mount_data())
	{
		log_error("[wys] mount role->get_mount_data NULL");
		return errcode_enum::notice_unknown;
	}

	MountTable *p_config = GET_CONF(MountTable, mount_tid);
	if (NULL == p_config)
	{
		log_error("[wys] mount config[%d] NULL", mount_tid);
		return errcode_enum::notice_mount_not_find;
	}

	if (!role->get_mount_data()->turn_mount(mount_tid))
		return errcode_enum::notice_mount_not_find;

	return errcode_enum::error_ok;
}

uint32_t mount_manager_t::cancel_turn_mount(role_ptr role, uint32_t mount_tid)
{
	if (NULL == role)
	{
		log_error("[wys] mount role NULL");
		return errcode_enum::notice_role_not_exist;
	}
	if (NULL == role->get_mount_data())
	{
		log_error("[wys] mount role->get_mount_data NULL");
		return errcode_enum::notice_unknown;
	}

	if (!role->get_mount_data()->cancel_turn_mount(mount_tid))
		return errcode_enum::notice_mount_not_find;
	return errcode_enum::error_ok;
}
