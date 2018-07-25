#ifndef __GAME_TREASURE_MANAGER_H__
#define __GAME_TREASURE_MANAGER_H__

#include "role/role.hpp"
#include "item/item.hpp"
#include "config/config_manager.hpp"
class Item;
enum treasure_type
{
	treasure_type_troop		= 1,	//队伍
	treasure_type_single	= 2,	//单人
};

enum treasure_event
{
	treasure_event_copper	= 1,	//铜币
	treasure_event_item		= 2,	//物品
	treasure_event_thief	= 3,	//盗墓贼
	treasure_event_note		= 4,	//盗墓笔记
};

class treasure_manager_t
{
public:
	//组队放入寻宝道具
	static uint32_t use_item_treasure(role_ptr p_role, item_ptr use_item, proto::common::role_change_data * p_data);
	//开启盗墓笔记
	static uint32_t use_item_note_closed(role_ptr p_role, item_ptr use_item, Item * config, proto::common::role_change_data * p_data);
	//使用已开启的盗墓笔记挖宝
	static uint32_t use_item_note_opened(role_ptr p_role, item_ptr use_item, proto::common::role_change_data * p_data);
	//获取宝藏
	static void gather_treasure(role_ptr p_role, uint32_t event_id, uint32_t item_id = 0);
};

#endif
