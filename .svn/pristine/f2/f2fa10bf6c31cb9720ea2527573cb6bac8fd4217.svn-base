#pragma once

#include "redis_srv.hpp"

class redis_area_t : public redis_srv_t
{
public:
	redis_area_t();
	virtual ~redis_area_t();

protected:
	// 读取结束的回调
	virtual void read_db_done();
};

extern redis_area_t g_redis_area;