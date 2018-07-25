#pragma once
#include "protos_fwd.hpp"
#include <map>
#include "collect/collect_point.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

// 采集点
// author: wys
// data:2017.1.23

class collect_manager_t
{
public:
	collect_manager_t();
	~collect_manager_t();
	static uint32_t start_collect(role_ptr p_role, uint64_t collect_point_uid);
	static uint32_t end_collect(role_ptr p_role, uint64_t collect_point_uid, proto::common::role_change_data *p_data);
	static uint32_t cancel_collect(role_ptr p_role, uint64_t collect_point_uid);
	static void close();
public:
	// 查找采集点
	static collect_point_ptr find_collect_point(uint64_t collect_point_id);
	// 删除采集点
	static void delete_collect_point(uint64_t collect_point_id);
	// 添加采集点
	static bool add_collect_point(collect_point_ptr p_collect_point);

	static uint64_t get_collect_point_uid();
public:

	static void refresh_on_time(const boost::system::error_code& ec, uint64_t collect_point_uid);
public:
	static std::map<uint64_t, collect_point_ptr> m_collect_points;

};