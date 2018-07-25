#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__
#include "protos_fwd.hpp"
#include "object_position.hpp"

// 前置声明
class object_manager_t
{
public:
	// 查找生物
	static object_base_ptr find_object(const object_id_type& id_type);
	// 删除生物
	static void delete_creature(const object_id_type& id_type);
};

#endif // !__GAME_OBJECT_MANAGER_H__