#include "field_boss.hpp"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "config/config_manager.hpp"
#include "global_id.hpp"
#include "tblh/BossBornTable.tbls.h"
#include "scene/global_scene_manager.hpp"
#include "tblh/sys_notice_enum.hpp"

field_boss_t::~field_boss_t()
{

}

bool field_boss_t::init(BossBornTable* conf, proto::common::em_field_boss_state init_state, uint32_t wait_time)
{
	if (!conf) {
		log_error("BossBornTable null error");
		return false;
	}

	m_id = conf->id();
	m_monster_id = conf->monster_tid();
	m_map_id = conf->map_id();
	m_state = init_state;
	m_last = conf->last_time();

	if (init_state == proto::common::FIELD_BOSS_STATE_NONE && wait_time != 0) {
		if (NULL == m_timer) {
			m_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
		}

		if (NULL == m_timer) {
			log_error("field_boss_t m_timer null error");
			return false;
		}

		m_timer->expires_from_now(boost::posix_time::seconds(wait_time));
		m_timer->async_wait(boost::bind(&field_boss_t::refresh_callback, boost::weak_ptr<field_boss_t>(shared_from_this()), boost::asio::placeholders::error));
	}

	return true;
}

void field_boss_t::clear()
{
	if (NULL != m_timer) {
		m_timer->cancel();
	}
}

void field_boss_t::set_killer(const std::string& name)
{
	m_killed = true;
	m_killer = name;
	m_state = proto::common::FIELD_BOSS_STATE_KILL; // 设置被击杀，以场景服的状态为准

	if (NULL != m_timer) {
		m_timer->cancel();
		m_timer.reset();
	}
}

void field_boss_t::on_refresh_timer()
{
	// 先去场景召唤怪物
	bool is_init_success = false;
	do {
		global_scene_ptr p_scene = global_scene_manager_t::get_scene_by_id(m_map_id);
		if (p_scene == NULL) {
			log_error("field_boss_t[%u] scene[%u] not find or not reg", m_id, m_map_id);
			break;
		}

		BossBornTable* p_bossinfo = GET_CONF(BossBornTable, m_id);
		if (NULL == p_bossinfo) {
			log_error("BossBornTable not found boss [%u]", m_id);
			break;
		}

		boss_born_pos born_pos;
		if (!p_bossinfo->get_one_boss_born_pos(born_pos)) {
			log_error("field_boss_t[%u] get one born pos error", m_id);
			break;
		}

		int32_t pos_x = (int32_t)(std::get<0>(born_pos));
		int32_t pos_y = (int32_t)(std::get<1>(born_pos));

		int32_t radius = (int32_t)p_bossinfo->map_radius();

		pos_x = random_util_t::randBetween(pos_x - radius, pos_x + radius);
		pos_y = random_util_t::randBetween(pos_y - radius, pos_y + radius);

		if (pos_x == -1 || pos_y == -1) {
			log_error("rand boss[%u] born posX[%d] posY[%d] radius[%d] error", m_id, pos_x, pos_y, radius);
			break;
		}

		m_pos_x = pos_x;
		m_pos_y = pos_y;
		m_dir = std::get<2>(born_pos);

		m_object_id = common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_MONSTER);
		if (m_object_id == 0)
		{
			log_error("g_id_allocator make field_boss object_id error");
			break;
		}

		m_start_time = common::time_util_t::now_time();

		// 通知场景召唤boss
		proto::server::eg_summon_field_boss_request msg;
		msg.set_id(m_id);
		msg.set_scene_id(p_scene->get_scene_id());
		msg.set_object_id(m_object_id);
		msg.set_monster_id(m_monster_id);
		msg.set_pos_x(m_pos_x);
		msg.set_pos_y(m_pos_y);
		msg.set_dir(m_dir);
		p_scene->send_msg_to_game(op_cmd::eg_summon_field_boss_request, msg);

		is_init_success = true;
	} while (0);

	if (is_init_success && NULL != m_timer) { // 成功，启动场景召唤定时器
		BOSS_LOG("field_boss[%u] start summon in scene[%u]", m_id, m_map_id);

		m_timer->expires_from_now(boost::posix_time::seconds(5));	// 5秒召唤等待时间，时间到了没召唤说明失败了
		m_timer->async_wait(boost::bind(&field_boss_t::summon_callback, boost::weak_ptr<field_boss_t>(shared_from_this()), boost::asio::placeholders::error));
		return;
	}

	// 失败清理
	log_error("field_boss[%u] start summon failed", m_id);

	if (NULL != m_timer) { // 取消定时器
		m_timer->cancel();
		m_timer.reset();
	}

	m_state = proto::common::FIELD_BOSS_STATE_GONE; // 失败状态设置
}

void field_boss_t::refresh_callback(const boost::weak_ptr<field_boss_t>& p_wboss, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted) {
		return;
	} else if (ec) {
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	filed_boss_ptr p_boss(p_wboss.lock());
	if (NULL == p_boss) {
		log_error("NULL == p_boss");
		return;
	}

	p_boss->on_refresh_timer();
}

void field_boss_t::on_gone_timer()
{
	// 取消定时器
	if (NULL != m_timer) {
		m_timer->cancel();
		m_timer.reset();
	}

	if (m_state == proto::common::FIELD_BOSS_STATE_START) { // 存活，通知删除
		m_state = proto::common::FIELD_BOSS_STATE_GONE;

		global_scene_ptr p_scene = global_scene_manager_t::get_scene_by_id(m_map_id);
		if (p_scene != NULL) {
			proto::server::eg_field_boss_die_notify notify;
			notify.set_object_id(m_object_id);
			notify.set_mode(proto::server::FILED_BOSS_END); // 活动时间到了结束
			p_scene->send_msg_to_game(op_cmd::eg_field_boss_die_notify, notify);
		}
	}
}

void field_boss_t::gone_callback(const boost::weak_ptr<field_boss_t>& p_wboss, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted) {
		return;
	} else if (ec) {
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	filed_boss_ptr p_boss(p_wboss.lock());
	if (NULL == p_boss) {
		log_error("NULL == p_boss");
		return;
	}

	p_boss->on_gone_timer();
}

void field_boss_t::peek_data(proto::common::filed_boss_one_state* state)
{
	if (state) {
		state->set_id(m_id);
		state->set_monster_tid(m_monster_id);
		state->set_state(m_state);
		state->set_map_id(m_map_id);
		state->set_pos_x(m_pos_x);
		state->set_pos_y(m_pos_y);
		if (is_killed()) {
			state->set_name(m_killer);
		}
	}
}

void field_boss_t::summon_callback(const boost::weak_ptr<field_boss_t>& p_wboss, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted) {
		return;
	} else if (ec) {
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	filed_boss_ptr p_boss(p_wboss.lock());
	if (NULL == p_boss) {
		log_error("NULL == p_boss");
		return;
	}

	p_boss->on_summon_timer();
}

void field_boss_t::on_summon_timer()
{
	// scene 召唤超时
	log_error("field_boss[%u] start summon time out failed", m_id);
	if (NULL != m_timer) { // 取消定时器
		m_timer->cancel();
		m_timer.reset();
	}

	m_state = proto::common::FIELD_BOSS_STATE_GONE; // 失败状态设置
}

bool field_boss_t::summon_result(bool is_success)
{
	do {
		if (!is_success) {
			log_error("field boss[%u] scene summon failed", m_id);
			break;
		}

		if (m_state != proto::common::FIELD_BOSS_STATE_NONE) {
			log_error("field boss[%u] state[%u] != proto::common::FIELD_BOSS_STATE_NONE", m_id, m_state);
			break;
		}

		if (NULL == m_timer) {
			log_error("field boss[%u] m_timer null error", m_id);
			break;
		}

		uint32_t curr_time = common::time_util_t::now_time();
		if (m_start_time + 5 < curr_time) {
			log_error("field_boss[%u:%u] summon too slow", m_id, m_monster_id);
			break;
		}

		m_state = proto::common::FIELD_BOSS_STATE_START;
		m_timer->expires_from_now(boost::posix_time::seconds(m_start_time + m_last - curr_time));	// 设置删除时间
		m_timer->async_wait(boost::bind(&field_boss_t::gone_callback, boost::weak_ptr<field_boss_t>(shared_from_this()), boost::asio::placeholders::error));

		proto::client::gc_sys_notice_notify ntf;
		proto::common::sys_notice_data* data = ntf.add_notices();
		if (data) {
			data->set_type(proto::common::SYS_NOTICE_DEFAULT);
			data->set_table_type_id(proto::common::sys_notify_type_field_boss);
			data->add_param_str_list(common::string_util_t::uint32_to_string(m_monster_id));
			data->add_param_str_list(common::string_util_t::uint32_to_string(m_map_id));
			data->add_param_str_list(common::string_util_t::uint32_to_string(m_pos_x));
			data->add_param_str_list(common::string_util_t::uint32_to_string(m_pos_y));
		}

		std::string cmd_data;
		network::wrap_msg(cmd_data, ntf);
		proto::server::ea_broadcast_notify msg_to_gate;
		msg_to_gate.set_cmd(op_cmd::gc_sys_notice_notify);
		msg_to_gate.set_data(cmd_data);
		env::server->send_msg_to_all_gate(op_cmd::ea_broadcast_notify, 0, msg_to_gate);

		BOSS_LOG("field boss[%u:%u] summon success", m_id, m_monster_id);
		return true;
	} while (0);

	if (NULL != m_timer) { // 取消定时器
		m_timer->cancel();
		m_timer.reset();
	}

	m_state = proto::common::FIELD_BOSS_STATE_GONE; // 失败状态设置
	return false;
}
