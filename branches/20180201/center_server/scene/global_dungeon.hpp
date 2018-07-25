#ifndef __GLOBAL_DUNGEON_H__
#define __GLOBAL_DUNGEON_H__

#include "global_scene.hpp"
#include "server.pb.h"

class global_dungeon_t : public global_scene_t
{
public:
	global_dungeon_t(uint64_t scene_id, uint32_t map_tid, const proto::server::dungeon_create_data& data);
	virtual ~global_dungeon_t() {};

public:
	// 是否是副本
	virtual bool is_dungeon_scene() const { return true; }
	// 获取副本场景指针
	virtual global_dungeon_ptr get_global_dungeon_ptr() { return boost::dynamic_pointer_cast<global_dungeon_t>(shared_from_this()); }

	const proto::server::dungeon_create_data& get_create_data() const { return m_create_data; }

	//virtual uint32_t check_troop_join();

protected:
	proto::server::dungeon_create_data m_create_data;
};

#endif