#include "redis_area.hpp"

redis_area_t g_redis_area;

redis_area_t::redis_area_t()
{

}

redis_area_t::~redis_area_t()
{

}

void redis_area_t::read_db_done()
{
	redis_srv_t::read_db_done();

	// 读取结束之后读取线程没有用了
	m_read_unit_list.clear();
}
