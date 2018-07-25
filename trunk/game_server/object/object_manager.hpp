#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__
#include "protos_fwd.hpp"
#include "object_position.hpp"

// ǰ������
class object_manager_t
{
public:
	// ��������
	static object_base_ptr find_object(const object_id_type& id_type);
	// ɾ������
	static void delete_creature(const object_id_type& id_type);
};

#endif // !__GAME_OBJECT_MANAGER_H__