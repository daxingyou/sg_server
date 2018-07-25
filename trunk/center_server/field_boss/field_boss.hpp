#ifndef __CENTER_FIELD_BOSS_H__
#define __CENTER_FIELD_BOSS_H__

#include "macros.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

#include "common.pb.h"
#include "tblh/BossBornTable.tbls.h"

class field_boss_t : public boost::enable_shared_from_this<field_boss_t>
{
public:
	field_boss_t() {};
	~field_boss_t();

	bool init(BossBornTable* conf, proto::common::em_field_boss_state init_state, uint32_t wait_time);
	void clear();

	uint32_t get_monster_id() const { return m_monster_id; }

	uint64_t get_object_id() const { return m_object_id; }

	proto::common::em_field_boss_state get_state() const { return m_state; }

	void set_killer(const std::string& name);
	const std::string& get_killer() const { return m_killer; }

	bool is_killed() const { return m_killed; }

	// 场景 通知召唤成功
	bool summon_result(bool is_success);

protected:
	// 刷新定时器触发
	void on_refresh_timer();
	// 刷新定时器触发
	static void refresh_callback(const boost::weak_ptr<field_boss_t>& p_wboss, const boost::system::error_code& ec);

	// 召唤定时器触发
	void on_summon_timer();
	// 召唤定时器触发
	static void summon_callback(const boost::weak_ptr<field_boss_t>& p_wboss, const boost::system::error_code& ec);

	// 撤离定时器触发
	void on_gone_timer();
	// 撤离定时器触发
	static void gone_callback(const boost::weak_ptr<field_boss_t>& p_wboss, const boost::system::error_code& ec);

public:
	void peek_data(proto::common::filed_boss_one_state* state);

private:
	uint32_t m_id = 0;			// 配置表id
	uint32_t m_monster_id = 0;	// 怪物id
	uint64_t m_object_id = 0;	// object_id
	uint32_t m_map_id = 0;		// 静态场景id
	uint32_t m_last = 0;		// 持续时间
	uint32_t m_start_time = 0;	// 召唤时间
	uint32_t m_pos_x = 0;		// 坐标
	uint32_t m_pos_y = 0;		// 坐标
	uint32_t m_dir = 0;			// 坐标

	std::string m_killer;
	bool m_killed = false;
	proto::common::em_field_boss_state m_state = proto::common::FIELD_BOSS_STATE_NONE;
	boost::shared_ptr<boost::asio::deadline_timer> m_timer;
};

typedef boost::shared_ptr<field_boss_t> filed_boss_ptr;

#endif