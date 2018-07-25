#include "fight_study_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "fight/fight_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config_mgr.h"
#include "scene/scene_manager.hpp"

static const uint32_t FIGHT_STUDY_MATCH_TIME = 10000;
static const uint32_t FIGHT_STUDY_WAIT_TIME = 5000;
static const uint32_t FIGHT_STUDY_FREQUENCY = 60;

fight_study_manager_t::fight_study_manager_t(uint64_t owner)
	: m_owner(owner)
{
}

fight_study_manager_t::~fight_study_manager_t()
{
	m_timer.stop();
}

bool fight_study_manager_t::fight_study_request(role_ptr role, uint64_t target_uid)
{
	if (NULL == role)
	{
		log_error("role is null");
		return false;
	}

	uint32_t reply = common::errcode_enum::error_ok;
	role_ptr target = NULL;

	do
	{
		if (!role->can_enter_fight())
		{
			reply = errcode_enum::fight_study_self_in_fight;
			break;
		}

		if (is_in_study(role))
		{
			reply = errcode_enum::fight_study_target_in_match;
			break;
		}

		reply = scene_check(role);
		if (reply != common::errcode_enum::error_ok) {
			break;
		}

		if (!check_frequency(role))
		{
			reply = errcode_enum::fight_study_too_frequency;
			break;
		}

		target = role_manager_t::find_role(target_uid);
		if (NULL == target)
		{
			reply = errcode_enum::notice_role_not_exist;
			break;
		}

		if (!target->can_enter_fight(false))
		{
			reply = errcode_enum::fight_study_target_in_fight;
			break;
		}

		if (is_in_study(target))
		{
			reply = errcode_enum::fight_study_target_in_match;
			break;
		}

		// 判断距离
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::pk_distance);
		uint32_t radius = GET_COMPREHENSIVE_VALUE_1(p_conf);

		if (!role->is_nearby_object(target, radius))
		{
			// log_error("role[%lu] too far away about target [%lu]", role->get_uid(), target->get_uid());
			reply = errcode_enum::notice_fight_target_too_farawary;
			break;
		}

		start_match(role, target);

	} while (0);

	fight_study_reply(role, reply);

	if (reply == 0)
	{
		fight_study_notify(target, role);
	}

	return true;
}

bool fight_study_manager_t::fight_study_cancel(role_ptr role)
{
	if (NULL == role)
	{
		log_error("role is null");
		return false;
	}

	proto::common::role_fight_data& rfd = role->get_role_fight_data();
	proto::common::fight_study_data* fsd = rfd.mutable_study();
	if (fsd->step() != fight_study_step_match)
	{
		log_error("cancel step error");
		return false;
	}

	if (get_remain_time(role) <= 0)
	{
		log_error("cancel time error");
		return false;
	}

	m_timer.stop();
	cancel_all_match(role);

	return true;
}

bool fight_study_manager_t::fight_study_answer_request(role_ptr target, role_ptr role, uint32_t answer)
{
	if (NULL == target)
	{
		log_error("target is null");
		return false;
	}

	if (NULL == role)
	{
		log_error("role is null");
		return false;
	}

	proto::common::role_fight_data& rfd = role->get_role_fight_data();
	proto::common::fight_study_data* fsd = rfd.mutable_study();

	uint32_t reply = 0;
	do 
	{
		if (fsd->step() != fight_study_step_match)
		{
			reply = errcode_enum::fight_study_already_cancel;
			cancel_match(target);
			break;
		}

		if (fsd->target() != target->get_uid())
		{
			reply = errcode_enum::fight_study_already_cancel;
			cancel_match(target);
			break;
		}

		m_timer.stop();

		if (answer > 0)
		{
			start_wait(role, target);
		}
		else
		{
			cancel_match(role);
			cancel_match(target);
		}

	} while (0);
	
	fight_study_answer_notify(role, answer);
	fight_study_answer_reply(target, reply);

	return true;
}

bool fight_study_manager_t::is_in_study(role_ptr role)
{
	if (NULL == role)
	{
		log_error("role is null");
		return false;
	}

	if (get_remain_time(role) <= 0)
	{
		return false;
	}

	return true;
}

bool fight_study_manager_t::check_frequency(role_ptr role)
{
	if (NULL == role)
	{
		log_error("role is null");
		return false;
	}

	proto::common::role_fight_data& rfd = role->get_role_fight_data();
	const proto::common::fight_study_data& fsd = rfd.study();
	uint32_t now = common::time_util_t::now_time();
	if (fsd.study_time() + FIGHT_STUDY_FREQUENCY > now)
	{
		return false;
	}

	return true;
}

uint32_t fight_study_manager_t::scene_check(const role_ptr& p_role)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (p_role->get_object_state() == proto::common::object_state_arena_match) {
		ROLE_LOG("role[%lu] arena scene can't study", p_role->get_uid());
		return common::errcode_enum::fight_study_arena_error;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_scene) {
		log_error("p_role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
		return common::errcode_enum::notice_unknown;
	}

	if (p_scene->is_family_war_scene()) {
		ROLE_LOG("role[%lu] family_war scene can't study", p_role->get_uid());
		return common::errcode_enum::fight_study_family_error;
	}

	return common::errcode_enum::error_ok;
}

void fight_study_manager_t::start_match(role_ptr role, role_ptr target)
{
	if (NULL == role)
	{
		log_error("role is null");
		return;
	}

	if (NULL == target)
	{
		log_error("target is null");
		return;
	}

	uint32_t now = common::time_util_t::now_time();
	proto::common::fight_study_data data;
	data.set_step(fight_study_step_match);
	data.set_step_time(now);

	proto::common::role_fight_data& role_rfd = role->get_role_fight_data();
	role_rfd.mutable_study()->CopyFrom(data);
	role_rfd.mutable_study()->set_target(target->get_uid());
	role_rfd.mutable_study()->set_study_time(now);

	//role->save_self();		wys 注释 2017.11.24 取消切磋的所有存盘 没必要存盘

	proto::common::role_fight_data& target_rfd = target->get_role_fight_data();
	target_rfd.mutable_study()->CopyFrom(data);
	target_rfd.mutable_study()->set_attacker(role->get_uid());

	//target->save_self();

	m_timer.start(FIGHT_STUDY_MATCH_TIME, this, &fight_study_manager_t::on_start_match_timer, env::server->get_ios());
}

void fight_study_manager_t::on_start_match_timer(const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	role_ptr role = role_manager_t::find_role(m_owner);
	if (NULL != role)
	{
		cancel_all_match(role);

		fight_study_answer_notify(role, 0);
	}
}

void fight_study_manager_t::start_wait(role_ptr role, role_ptr target)
{
	if (NULL == role)
	{
		log_error("role is null");
		return;
	}

	if (NULL == target)
	{
		log_error("target is null");
		return;
	}

	uint32_t now = common::time_util_t::now_time();

	proto::common::role_fight_data& role_rfd = role->get_role_fight_data();
	role_rfd.mutable_study()->set_step(fight_study_step_wait);
	role_rfd.mutable_study()->set_step_time(now);

	//role->save_self();

	proto::common::role_fight_data& target_rfd = target->get_role_fight_data();
	target_rfd.mutable_study()->set_step(fight_study_step_wait);
	target_rfd.mutable_study()->set_step_time(now);

	//target->save_self();

	m_timer.start(FIGHT_STUDY_WAIT_TIME, this, &fight_study_manager_t::on_start_wait_timer, env::server->get_ios());
}

void fight_study_manager_t::on_start_wait_timer(const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	role_ptr role = role_manager_t::find_role(m_owner);
	if (NULL == role)
	{
		log_error("role is null");
		return;
	}

	proto::common::role_fight_data& role_rfd = role->get_role_fight_data();
	role_ptr target = role_manager_t::find_role(role_rfd.study().target());
	if (NULL == target)
	{
		log_error("target is null");
		return;
	}

	proto::common::fight_param param;
	param.set_type(proto::common::fight_type_study);
	fight_manager_t::fight_pvp(role, target, &param);
}

void fight_study_manager_t::cancel_match(role_ptr role)
{
	if (NULL == role)
	{
		log_error("role is null");
		return;
	}

	uint32_t now = common::time_util_t::now_time();

	proto::common::role_fight_data& role_rfd = role->get_role_fight_data();
	role_rfd.mutable_study()->set_step(fight_study_step_none);
	role_rfd.mutable_study()->set_step_time(now);

	//role->save_self();
}

void fight_study_manager_t::cancel_all_match(role_ptr role)
{
	if (NULL == role)
	{
		log_error("role NULL");
		return;
	}
	proto::common::role_fight_data& role_rfd = role->get_role_fight_data();
	uint64_t target_uid = role_rfd.study().target();
	if (target_uid != 0)
	{
		role_ptr p_target = role_manager_t::find_role(target_uid);
		if (p_target != NULL)
		{
			cancel_match(p_target);
		}
	}
	cancel_match(role);
}

void fight_study_manager_t::fight_study_reply(role_ptr role, uint32_t reply_code)
{
	if (NULL == role)
	{
		log_error("role is null");
		return;
	}

	proto::client::gc_fight_study_reply reply;
	reply.set_reply_code(reply_code);

	if (reply_code == 0)
	{
		reply.set_remain_time(get_remain_time(role));
	}

	role->send_msg_to_client(op_cmd::gc_fight_study_reply, reply);
}

uint32_t fight_study_manager_t::get_remain_time(role_ptr role)
{
	if (NULL == role)
	{
		log_error("role is null");
		return 0;
	}

	proto::common::role_fight_data& rfd = role->get_role_fight_data();
	const proto::common::fight_study_data& fsd = rfd.study();
	if (fsd.step() == fight_study_step_none)
	{
		return 0;
	}

	uint32_t now = common::time_util_t::now_time();
	if (fsd.step() == fight_study_step_match)
	{
		if (fsd.step_time() + FIGHT_STUDY_MATCH_TIME > now)
		{
			return fsd.step_time() + FIGHT_STUDY_MATCH_TIME - now;
		}
		else
		{
			return 0;
		}
	}

	if (fsd.step() == fight_study_step_wait)
	{
		if (fsd.step_time() + FIGHT_STUDY_WAIT_TIME / 1000 > now)
		{
			return fsd.step_time() + FIGHT_STUDY_WAIT_TIME / 1000 - now;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

void fight_study_manager_t::fight_study_notify(role_ptr target, role_ptr role)
{
	if (NULL == target)
	{
		log_error("target is null");
		return;
	}

	if (NULL == role)
	{
		log_error("role is null");
		return;
	}

	proto::client::gc_fight_study_notify ntf;
	ntf.set_remain_time(get_remain_time(target));
	ntf.set_attacker_name(role->get_name());
	ntf.set_attacker_uid(string_util_t::uint64_to_string(role->get_uid()));

	target->send_msg_to_client(op_cmd::gc_fight_study_notify, ntf);
}

void fight_study_manager_t::fight_study_answer_notify(role_ptr role, uint32_t answer)
{
	if (NULL == role)
	{
		log_error("role is null");
		return;
	}

	proto::client::gc_fight_study_answer_notify ntf;
	ntf.set_answer(answer);

	if (answer != 0)
	{
		ntf.set_remain_time(get_remain_time(role));
	}

	role->send_msg_to_client(op_cmd::gc_fight_study_answer_notify, ntf);
}

void fight_study_manager_t::fight_study_answer_reply(role_ptr target, uint32_t reply_code)
{
	if (NULL == target)
	{
		log_error("target is null");
		return;
	}

	proto::client::gc_fight_study_answer_reply reply;
	reply.set_reply_code(reply_code);

	if (reply_code == 0)
	{
		reply.set_remain_time(get_remain_time(target));
	}

	target->send_msg_to_client(op_cmd::gc_fight_study_answer_reply, reply);
}

