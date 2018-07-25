#include "friend_manager.hpp"
#include "role/role_manager.hpp"
#include "role/money_manager.hpp"
#include "log_enum.hpp"
#include "item/item_manager.hpp"
#include "common.pb.h"
#include "tblh/errcode_enum.hpp"
#include "tblh/Item.tbls.h"
#include "tblh/errcode_enum.hpp"
#include "item/drop_manager.hpp"

bool friend_manager_t::m_close = false;

int32_t friend_manager_t::handle_friend_point_operation(uint64_t role_id, proto::server::friend_point_operation op, int32_t parameter1, int32_t parameter2, proto::common::role_change_data* p_change_data, int32_t& info1, int32_t& info2, std::string* p_info3, int32_t& info4)
{
	switch (op)
	{
	case proto::server::player_get_point_from_friend:
	{
		role_ptr p_role = role_manager_t::find_role(role_id);
		if (NULL == p_role){
			log_error("p_role[%lu] is null ", role_id);
			return errcode_enum::notice_unknown;
		}
		if (!money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_FRIENDSHIP_POINT, (uint32_t)parameter1, log_enum::source_type_friend_get_fp, 0, true, p_change_data)) {
			log_error("user[%lu] add friend point[%u] failed", role_id, parameter1);
			return errcode_enum::notice_unknown;
		}
	}
	break;
	case proto::server::player_get_relation_award:
	{
		role_ptr p_role = role_manager_t::find_role(role_id);
		if (NULL == p_role)
		{
			log_error("p_role[%lu] is null ", role_id);
			return errcode_enum::notice_unknown;
		}

		//if (!item_manager_t::check_add_item(p_role, (uint32_t)parameter1, 1)) {
		//	return errcode_enum::notice_unknown;
		//}
		bool ret = drop_manager_t::drop(p_role, parameter1, log_enum::source_type_friend_get_relation_award, (proto::common::drop_sys_type)32, 0, p_change_data);
		if (!ret)
		{
			log_error("user[%lu] relationaward drop faild", role_id);
			return errcode_enum::notice_unknown;
		}
	}
	break;
	case proto::server::player_give_present:
	{
		role_ptr p_role = role_manager_t::find_role(role_id);
		if (NULL == p_role)
		{
			log_error("p_role[%lu] is null ", role_id);
			return errcode_enum::notice_unknown;
		}

		uint32_t need_item_id = (uint32_t)parameter1;
		uint32_t need_item_quantity = (uint32_t)parameter2;

		uint32_t possessed_quantity = item_manager_t::get_item_count(p_role, need_item_id);
		if (possessed_quantity < need_item_quantity)
		{
			log_debug("user[%lu] item[%u] quantity[%u] not enough([%u]need)", role_id, need_item_id, possessed_quantity, need_item_quantity);
			return errcode_enum::gift_lack;
		}
		
		Item* p_config = GET_CONF(Item, need_item_id);
		if (p_config == NULL)
		{
			log_error("item config[%u] is not exist", need_item_id);
			return errcode_enum::notice_unknown;
		}

		if (p_config->param_size() < 3)
		{
			log_error("gift config error! param size less than 3!");
			return errcode_enum::notice_unknown;
		}

		info1 = p_config->param(0);
		info2 = p_config->param(1);
		info4 = p_config->param(2);

		if (p_info3)
		{
			*p_info3 = common::string_util_t::convert_to_utf8(p_config->name_desc());
		}
		if (errcode_enum::error_ok != item_manager_t::remove_item(p_role, need_item_id, need_item_quantity, log_enum::source_type_friend_send_gift, 0, p_change_data))
		{
			log_error("p_role[%lu] remove item failed when send gift", role_id);
			return errcode_enum::notice_unknown;
		}
	}
	break;
	}
	return 0;
}

//	add_money(role_ptr p_role, proto::common::MONEY_TYPE type, uint32_t count, uint32_t source_type, uint32_t source_param, 