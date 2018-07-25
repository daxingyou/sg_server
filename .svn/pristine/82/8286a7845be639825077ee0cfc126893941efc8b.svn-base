#include "object_manager.hpp"
#include "role/role_manager.hpp"
#include "monster/monster_manager.hpp"
#include "collect/collect_manager.hpp"

// 查找生物
object_position_ptr object_manager_t::find_object(const object_id_type& id_type)
{
	switch (id_type.second)
	{
	case proto::common::SCENEOBJECT_USER:
		return role_manager_t::find_role(id_type.first);

	case proto::common::SCENEOBJECT_MONSTER:
		return monster_manager_t::find_monster(id_type.first);

	case proto::common::SCENEOBJECT_COLLECT_POINT:
		return collect_manager_t::find_collect_point(id_type.first);

	default:
		break;
	}
	return NULL;
}

// 删除生物
void object_manager_t::delete_creature(const object_id_type& id_type)
{
	switch (id_type.second)
	{
	case proto::common::SCENEOBJECT_USER:
		//role_manager_t::del_role(object_id);
		break;

	case proto::common::SCENEOBJECT_MONSTER:
		monster_manager_t::delete_monster(id_type.first);
		break;

	case proto::common::SCENEOBJECT_COLLECT_POINT:
		collect_manager_t::delete_collect_point(id_type.first);
		break;

	default:
		break;
	}
}