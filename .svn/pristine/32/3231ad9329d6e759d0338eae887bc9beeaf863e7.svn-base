#pragma once
#include "protos_fwd.hpp"
#include "object/object_position.hpp"
#include <boost/system/error_code.hpp>
// 采集点
// author: wys
// data:2017.1.23

/**
* \brief 定义结构
* 包括怪物id、初始位置、方向等信息
*/

struct t_collect_define
{
	uint32_t m_collect_id;			// id采集
	float m_x;						// 初始位置
	float m_y;						// 初始位置

	t_collect_define() : m_collect_id(0), m_x(0), m_y(0) {};

	t_collect_define(const t_collect_define& def)
	{
		this->m_collect_id = def.m_collect_id;
		this->m_x = def.m_x;
		this->m_y = def.m_y;
	}

	t_collect_define& operator= (const t_collect_define& def)
	{
		this->m_collect_id = def.m_collect_id;
		this->m_x = def.m_x;
		this->m_y = def.m_y;
		return *this;
	}
};

class family_war_platform_t;
typedef boost::shared_ptr<family_war_platform_t> family_war_platform_ptr;

class collect_point_t : public object_base_t
{
public:
	collect_point_t(uint32_t tid);
	virtual ~collect_point_t();

	virtual void del_clear();
	void close();

	// 获取对象指针
	collect_ptr get_collect() { return boost::dynamic_pointer_cast<collect_point_t>(shared_from_this()); }
	virtual proto::common::EM_SCENEOBJECT_TYPE get_object_type() { return proto::common::SCENEOBJECT_COLLECT_POINT; }

	virtual family_war_platform_ptr get_family_war_platform() { return family_war_platform_ptr(); }

	virtual void get_object_info(proto::common::object_common_info* object_info);
	
	bool init_collect_point(float x, float y, uint64_t object_uid = 0);
	uint32_t get_tid() { return m_tid; }

	virtual uint32_t can_collect(role_ptr p_role);
	virtual uint32_t start_collect(role_ptr p_role);
	virtual uint32_t end_collect(role_ptr p_role, proto::common::role_change_data *rcd);
	virtual void cancel_collect(const role_ptr& p_role);
	void relive();

	virtual uint32_t get_level() { return 1; }
public:
	void		start_refresh_timer(uint32_t refresh_time);
	void		cancel_refresh_timer();
//	void		refresh_on_time(const boost::system::error_code& ec);

protected:
	boost::shared_ptr<boost::asio::deadline_timer> m_refresh_timer;
	uint32_t m_tid = 0;
	uint32_t m_collect_count = 0;			// 采集次数
	uint32_t m_collect_time = 0;			// 采集需要时间
	uint32_t m_last_collect_time = 0;		// 上次采集时间
	bool	 m_collecting = false;			// 是否正在采集
	proto::common::collect_type m_collect_type = proto::common::collect_type_normal;
};

typedef boost::shared_ptr<collect_point_t> collect_point_ptr;

