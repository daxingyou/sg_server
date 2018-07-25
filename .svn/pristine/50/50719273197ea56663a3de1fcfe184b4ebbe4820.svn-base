#include "chat_msg_manager.hpp"
#include "item/item_manager.hpp"
#include "hero/hero_manager.hpp"

bool chat_msg_manager_t::chat_get_role_info(const uint64_t role_uid, proto::common::chat_common_role_info* p_data, std::vector<proto::common::chat_param>& param_vec)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]",role_uid);
		return false;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	if (param_vec.empty())
	{
		log_error("param_vec.empty()");
		return false;
	}

	for (auto& it : param_vec)
	{
		switch (it.type())
		{
		case proto::common::chat_item:
		{
			uint64_t item_uid = string_util_t::string_to_uint64(it.value());
			item_ptr p_item = item_manager_t::get_item_by_uid(p_role, item_uid);
			if (NULL == p_item)
			{
				log_error("role [%lu] item [%u] is not exist!", role_uid, item_uid);
				continue;
			}
			else
			{
				proto::common::item_single* p_item_single = p_data->add_item_single();
				p_item->peek_data(p_item_single);
			}
		}break;
		case proto::common::chat_hero:
		{
			uint64_t hero_uid = string_util_t::string_to_uint64(it.value());
			hero_ptr p_hero = hero_manager_t::get_hero_by_unique_id(p_role, hero_uid);
			if (NULL == p_hero)
			{
				log_error("role [%lu] hero [%lu] is not exist!", role_uid, hero_uid);
				continue;
			}
			else
			{
				proto::common::hero_single* p_hero_single = p_data->add_hero_single();
				p_hero->peek_data(p_hero_single);
			}
		}break;
		default:
			return false;
		}
	}
	return true;
}


