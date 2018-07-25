#include "offline_arena_manager.hpp"
#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "user/global_user_manager.hpp"
#include "redis/global_user_data_mgr.hpp"
#include "client.pb.h"
#include "redis_client.hpp"
#include "config_mgr.h"
#include "utility.hpp"
#include "tblh/errcode_enum.hpp"
#include "log/log_wrapper.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "red_point/red_point_manager.hpp"
#include "family/family_manager.hpp"
#include "tblh/OfflineArenaRobot.tbls.h"
#include "tblh/OfflineRobotInfo.tbls.h"
#include "common/ContainerParse.h"
#include "tblh/time_type_enum.hpp"
#include "time_manager.hpp"
#include "achieve/achieve_common.hpp"

USING_NS_COMMON;

offline_arena_map offline_arena_manager_t::m_offline_arena_map;
offline_arena_map_by_class offline_arena_manager_t::m_offline_arena_map_by_class;
std::map<uint32_t, uint32_t> offline_arena_manager_t::s_buy_info;

void offline_arena_manager_t::load_data()
{
	//读取所有离线竞技数据
	std::vector<uint64_t> role_uids;
	redis_client_t::get_all_keys<uint64_t, std::vector<uint64_t>>("offline_arena", role_uids);
	for (auto role_uid : role_uids)
	{
		uint32_t cur_class = redis_client_t::get_uint32("offline_arena", "cur_class", role_uid);
		uint32_t cur_rank = redis_client_t::get_uint32("offline_arena", "cur_rank", role_uid);
		uint32_t old_record_rank = redis_client_t::get_uint32("offline_arena", "old_record_rank", role_uid);
		uint32_t record_rank = redis_client_t::get_uint32("offline_arena", "record_rank", role_uid);
		uint32_t old_class = redis_client_t::get_uint32("offline_arena", "old_class", role_uid);
		uint32_t fight_count = redis_client_t::get_uint32("offline_arena", "fight_count", role_uid);
		uint32_t buy_count = redis_client_t::get_uint32("offline_arena", "buy_count", role_uid);
		uint32_t score = redis_client_t::get_uint32("offline_arena", "score", role_uid);
		std::string score_award = redis_client_t::get_string("offline_arena", "score_award", role_uid);
		if (0 == cur_rank)
		{
			log_warn("role[%lu] cur_rank[%d] ", role_uid, cur_rank);
			continue;
		}
		offline_arena_ptr p_arena = offline_arena_ptr(new offline_arena_t);
		p_arena->set_uid(role_uid);
		p_arena->set_cur_class(cur_class);
		p_arena->set_cur_rank(cur_rank);
		p_arena->set_old_record_rank(old_record_rank);
		p_arena->set_record_rank(record_rank);
		p_arena->set_old_class(old_class);
		p_arena->set_fight_count(fight_count);
		p_arena->set_buy_count(buy_count);
		p_arena->set_score(score);
		if (!score_award.empty())
		{
			std::vector<std::uint32_t> boxes;
			string_util_t::split<uint32_t>(score_award, boxes, "$");
			for (auto box : boxes)
			{
				p_arena->add_score_award(box);
			}
		}
		add_arena(p_arena);
	}

	//读取静态配置
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::offline_arena_cost_number);
	if (NULL != p_conf)
	{
		Dawn::parseMapContainer<uint32_t, uint32_t>(s_buy_info, p_conf->parameter1().c_str(), ":$");
	}

	//注册时间回调
	time_manager_t::register_func(time_type_enum::time_offline_arena, offline_arena_manager_t::award);
}

void offline_arena_manager_t::one_day()
{
	for (auto it : m_offline_arena_map)
	{
		offline_arena_ptr p_arena = it.second;
		if (NULL == p_arena)
		{
			continue;
		}
		p_arena->one_day();
	}
}

void offline_arena_manager_t::award(uint32_t id, bool is_in_time)
{
	if (!is_in_time)
	{
		return;
	}
	for (auto it : m_offline_arena_map)
	{
		offline_arena_ptr p_arena = it.second;
		if (NULL == p_arena)
		{
			continue;
		}
		p_arena->award();
	}
}

void offline_arena_manager_t::add_arena(offline_arena_ptr p_arena)
{
	uint64_t uid = p_arena->get_uid();
	if (m_offline_arena_map.find(uid) == m_offline_arena_map.end())
	{
		m_offline_arena_map.insert(std::make_pair(uid, p_arena));
	}
	uint32_t cur_class = p_arena->get_cur_class();
	uint32_t cur_rank = p_arena->get_cur_rank();
	auto p_conf = GET_CONF(OfflineArenaClass, cur_class);
	if (NULL == p_conf)
	{
		log_error("OfflineArenaClass NULL == p_conf[%d]", cur_class);
		return;
	}
	if (cur_rank > p_conf->min_rank())
	{
		log_warn("cur_rank[%d] range out of p_conf->min_rank[%d]", cur_rank, p_conf->min_rank());
		return;
	}
	offline_arena_map_by_class::iterator it = m_offline_arena_map_by_class.find(cur_class);
	if (m_offline_arena_map_by_class.find(cur_class) == m_offline_arena_map_by_class.end())
	{
		offline_arena_map_by_rank oambr;
		oambr.insert(std::make_pair(cur_rank, p_arena));
		m_offline_arena_map_by_class.insert(std::make_pair(cur_class, oambr));
	}
	else
	{
		offline_arena_map_by_rank& oambr = it->second;
		if (oambr.find(cur_rank) == oambr.end())
		{
			oambr.insert(std::make_pair(cur_rank, p_arena));
		}
	}
}

offline_arena_ptr offline_arena_manager_t::add_new_arena(uint64_t role_uid)
{
	uint32_t role_level = global_user_data_mgr_t::get_user_level(role_uid);
	OfflineArenaClass* p_class_conf = get_class_conf_by_level(role_level);
	if (NULL == p_class_conf)
	{
		log_error("NULL == p_class_conf role[%lu] level[%d] invalid", role_uid, role_level);
		return NULL;
	}
	offline_arena_ptr p_arena = offline_arena_ptr(new offline_arena_t);
	p_arena->set_uid(role_uid);
	p_arena->set_cur_class(p_class_conf->id());
	p_arena->set_old_class(1);
	p_arena->set_cur_rank(p_class_conf->min_rank() + 1);
	p_arena->set_old_record_rank(p_class_conf->min_rank() + 1);
	p_arena->set_record_rank(p_class_conf->min_rank() + 1);
	p_arena->set_fight_count(get_fight_count());
	add_arena(p_arena);
	p_arena->save_self();
	OFFLINE_ARENA_LOG("new role[%lu] join offline_arena, class[%d]", role_uid, p_class_conf->id());
	log_wrapper_t::send_offline_arena_rank_log(role_uid, rank_op_type_new, 0, 0, p_arena->get_cur_class(), p_arena->get_cur_rank());
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(role_uid);
	if (NULL != p_role)
	{
		proto::server::eg_offline_arena_join_notify ntf;
		p_role->send_msg_to_game(op_cmd::eg_offline_arena_join_notify, ntf);
	}
	return p_arena;
}

void offline_arena_manager_t::change_rank(offline_arena_ptr p_arena, uint32_t cur_class, uint32_t rank)
{
	if (NULL != p_arena)
	{
		uint32_t old_rank = p_arena->get_cur_rank();
		p_arena->set_cur_rank(rank);
		if (rank < p_arena->get_record_rank())
		{
			p_arena->set_record_rank(rank);
		}
		p_arena->save_self();
		OFFLINE_ARENA_LOG("role[%lu] occupy class[%d] rank from[%d] to[%d]", p_arena->get_uid(), cur_class, old_rank, rank);
		if (old_rank < rank)
		{
			//下降
			log_wrapper_t::send_offline_arena_rank_log(p_arena->get_uid(), rank_op_type_down, p_arena->get_cur_class(), old_rank, p_arena->get_cur_class(), p_arena->get_cur_rank());
		}
		else
		{
			//上升
			log_wrapper_t::send_offline_arena_rank_log(p_arena->get_uid(), rank_op_type_up, p_arena->get_cur_class(), old_rank, p_arena->get_cur_class(), p_arena->get_cur_rank());
		}
	}
	else
	{
		OFFLINE_ARENA_LOG("robot occupy class[%d] rank[%d]", cur_class, rank);
	}
	auto p_conf = GET_CONF(OfflineArenaClass, cur_class);
	if (NULL == p_conf)
	{
		log_error("OfflineArenaClass NULL == p_conf[%d]", cur_class);
		return;
	}
	if (rank > p_conf->min_rank())
	{
		log_warn("cur_rank[%d] range out of p_conf->min_rank[%d]", rank, p_conf->min_rank());
		return;
	}
	offline_arena_map_by_class::iterator it = m_offline_arena_map_by_class.find(cur_class);
	if (m_offline_arena_map_by_class.find(cur_class) == m_offline_arena_map_by_class.end())
	{
		offline_arena_map_by_rank oambr;
		oambr.insert(std::make_pair(rank, p_arena));
		m_offline_arena_map_by_class.insert(std::make_pair(cur_class, oambr));
	}
	else
	{
		offline_arena_map_by_rank& oambr = it->second;
		offline_arena_map_by_rank::iterator it_rank = oambr.find(rank);
		if (it_rank == oambr.end())
		{
			oambr.insert(std::make_pair(rank, p_arena));
		}
		else
		{
			if (NULL == p_arena)
			{
				oambr.erase(it_rank);
			}
			else
				it_rank->second = p_arena;
		}
	}
}

void offline_arena_manager_t::peek_data(uint64_t role_uid, proto::client::ec_offline_arena_data_reply& reply)
{
	offline_arena_ptr p_self;
	offline_arena_map::iterator it = m_offline_arena_map.find(role_uid);
	if (it == m_offline_arena_map.end())
	{
		p_self = add_new_arena(role_uid);
	}
	else
	{
		p_self = it->second;
	}
	if (NULL == p_self)
	{
		log_error("NULL == p_self[%lu]", role_uid);
		return;
	}
	p_self->peek_data(reply.mutable_self_info());
	uint32_t old_class = p_self->get_old_class();
	uint32_t cur_class = p_self->get_cur_class();
	if (old_class < cur_class)
	{
		proto::server::eg_offline_arena_class_award_notify ntf;
		for (uint32_t i = cur_class; i > old_class; --i)
		{
			auto p_class_conf = GET_CONF(OfflineArenaClass, i);
			if (NULL == p_class_conf)
			{
				log_error("NULL == p_class_conf[%d] OfflineArenaClass", i);
				continue;
			}
			ntf.add_drop_id(p_class_conf->class_up_award());
		}
		global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(role_uid);
		if (NULL != p_role)
		{
			ntf.set_cur_class(cur_class);
			p_role->send_msg_to_game(op_cmd::eg_offline_arena_class_award_notify, ntf);
			p_self->set_old_class(cur_class);
			p_self->save_self();
		}
	}

	//自己的信息获取完毕 开始match
	OfflineArenaMatch* p_match_conf = get_match_conf_by_rank(p_self->get_cur_rank());
	if (NULL == p_match_conf)
	{
		log_error("NULL == p_match_conf role[%lu] rank[%d] invalid", role_uid, p_self->get_cur_rank());
		return;
	}
	for (uint32_t i = 1; i <= 10; ++i)
	{
		proto::common::offline_arena_general* p_single = reply.add_enemy_info();
		peek_general_data(p_self->get_cur_class(), i, p_single);
	}
	for (auto sub_vec : p_match_conf->enemy())
	{
		if (sub_vec.size() < 2)
		{
            log_error("sub_vec.size()[%d] error!", (int32_t)sub_vec.size());
			continue;
		}
		int32_t rank = 0;
		uint32_t type = sub_vec.at(0);
		if (0 == type)
		{
			//type为0表示直接取排名
			if (2 == sub_vec.size())
			{
				rank = sub_vec.at(1);
			}
			else if (3 == sub_vec.size())
			{
				uint32_t min_rank = sub_vec.at(1);
				uint32_t max_rank = sub_vec.at(2);
				rank = random_util_t::randBetween(min_rank, max_rank);
                if (rank < 0)
                {
                    log_error("min_rank[%d] max_rank[%d] error!", min_rank, max_rank);
                    continue;
                }
			}
			else
			{
				log_error("sub_vec.size()[%d] error!", (int32_t)sub_vec.size());
				continue;
			}
		}
		else
		{
			if (3 != sub_vec.size())
			{
				log_error("3 != sub_vec.size()[%d]", (int32_t)sub_vec.size());
				continue;
			}
			if (1 == type)
			{
				//type为1表示向上取区间
				uint32_t min_rank = p_self->get_cur_rank() * (100 - sub_vec.at(2)) / 100;
				uint32_t max_rank = p_self->get_cur_rank() * (100 - sub_vec.at(1)) / 100;
				rank = random_util_t::randBetween(min_rank, max_rank);
                if (rank < 0)
                {
                    log_error("min_rank[%d] max_rank[%d] error!", min_rank, max_rank);
                    continue;
                }
			}
			else if (2 == type)
			{
				//type为2表示向下取区间
				uint32_t min_rank = p_self->get_cur_rank() * (100 + sub_vec.at(1)) / 100;
				uint32_t max_rank = p_self->get_cur_rank() * (100 + sub_vec.at(2)) / 100;
				rank = random_util_t::randBetween(min_rank, max_rank);
                if (rank < 0)
                {
                    log_error("min_rank[%d] max_rank[%d] error!", min_rank, max_rank);
                    continue;
                }
			}
			else
			{
				log_error("p_match_conf[%d] enemy invalid", type);
				continue;
			}
		}
        
		proto::common::offline_arena_general* p_single = reply.add_enemy_info();
		peek_general_data(p_self->get_cur_class(), rank, p_single);
	}
}

void offline_arena_manager_t::peek_enemy_data(uint64_t role_uid, proto::common::mirror_fight_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	if (m_offline_arena_map.find(role_uid) == m_offline_arena_map.end())
	{
		log_error("role[%lu] not in offline arena", role_uid);
		return;
	}
	redis_client_t::get_protobuf("offline_arena", "fight_data", role_uid, *p_data);
}

void offline_arena_manager_t::peek_rank_data(uint64_t role_uid, uint32_t cur_class, proto::client::ec_offline_arena_rank_reply& reply)
{
	for (uint32_t i = 1; i <= 100; ++i)
	{
		proto::common::offline_arena_general* p_single = reply.add_role_list();
		peek_general_data(cur_class, i, p_single);
	}
	reply.set_self_class(0);
	reply.set_self_rank(0);
	offline_arena_map::iterator it = m_offline_arena_map.find(role_uid);
	if (it != m_offline_arena_map.end())
	{
		offline_arena_ptr p_arena = it->second;
		if (NULL != p_arena)
		{
			reply.set_self_class(p_arena->get_cur_class());
			reply.set_self_rank(p_arena->get_cur_rank());
		}
	}
}

void offline_arena_manager_t::peek_record_data(uint64_t role_uid, proto::common::offline_arena_record_data* p_data)
{
	offline_arena_map::iterator it = m_offline_arena_map.find(role_uid);
	if (it == m_offline_arena_map.end())
	{
		log_error("role[%lu] not in offline arena", role_uid);
		return;
	}
	offline_arena_ptr p_arena = it->second;
	if (NULL == p_arena)
	{
		log_error("role[%lu] not in offline arena", role_uid);
		return;
	}
	p_arena->peek_record_data(p_data);
}

uint32_t offline_arena_manager_t::sweep(uint64_t self_uid, uint32_t self_rank, uint32_t enemy_type, uint64_t enemy_id, uint32_t enemy_rank, uint32_t count)
{
	//先判断自己是否存在
	global_user_ptr p_self = global_user_manager_t::get_global_user_by_uid(self_uid);
	if (NULL == p_self)
	{
		log_error("role[%lu] not exist", self_uid);
		return errcode_enum::notice_role_null;
	}
	if (p_self->get_level() < get_sweep_level())
	{
		log_error("role[%lu] level[%d] not enough, need[%d]", self_uid, p_self->get_level(), get_sweep_level());
		return errcode_enum::error_level_not_enough;
	}
	offline_arena_map::iterator it = m_offline_arena_map.find(self_uid);
	if (it == m_offline_arena_map.end())
	{
		log_error("role[%lu] not in offline arena", self_uid);
		return errcode_enum::notice_role_null;
	}
	offline_arena_ptr p_arena = it->second;
	if (NULL == p_arena)
	{
		log_error("role[%lu] not in offline arena", self_uid);
		return errcode_enum::notice_role_null;
	}
	if (0 == count || p_arena->get_fight_count() < count)
	{
		//战斗次数耗尽
		log_error("role[%lu] fight_count not enough", self_uid);
		return errcode_enum::offline_arena_lack_times;
	}
	if (p_arena->get_enemy_id() > 0)
	{
		//正在攻打目标
		log_error("role[%lu] fighting", self_uid);
		return errcode_enum::notice_is_fighting;
	}
	if (self_rank != p_arena->get_cur_rank())
	{
		//自己的排名发生了变化
		log_error("role[%lu] fight failed, self_rank changed", self_uid);
		return errcode_enum::offline_arena_myrank_change;
	}
	if (enemy_rank < self_rank)
	{
		log_error("role[%lu] fight failed, self_rank low than enemy_rank", self_uid);
		return errcode_enum::offline_arena_no_challenge;
	}
	//再判断对手状态
	if (enemy_type == proto::common::SCENEOBJECT_USER)
	{
		//如果是玩家先判断玩家是否存在
		offline_arena_map::iterator it_enemy = m_offline_arena_map.find(enemy_id);
		if (it_enemy == m_offline_arena_map.end())
		{
			log_error("role[%lu] fight user failed, enemy[%lu] not in offline arena", self_uid, enemy_id);
			return errcode_enum::notice_unknown;
		}
		offline_arena_ptr p_enemy = it_enemy->second;
		if (NULL == p_enemy)
		{
			log_error("role[%lu] fight user failed, enemy[%lu] not in offline arena", self_uid, enemy_id);
			return errcode_enum::notice_unknown;
		}
		if (enemy_rank != p_enemy->get_cur_rank())
		{
			//对手的排名发生了变化
			log_error("role[%lu] fight user failed, enemy[%lu] rank changed", self_uid, enemy_id);
			return errcode_enum::offline_arena_targetrank_change;
		}
		OFFLINE_ARENA_LOG("role[%lu] sweep enemy[%lu] count[%d]", p_arena->get_uid(), enemy_id, count);
	}
	else if (enemy_type == proto::common::SCENEOBJECT_MONSTER)
	{
		uint32_t cur_class = p_arena->get_cur_class();
		uint32_t robot_id = enemy_id;
		//如果是机器人先判断对应排名和机器人ID是否匹配
		if (robot_id != get_robot_by_rank(cur_class, enemy_rank))
		{
			log_error("role[%lu] fight robot failed, robot[%d] class[%d] rank[%d] not match", self_uid, robot_id, cur_class, enemy_rank);
			return errcode_enum::notice_unknown;
		}
		//再查找对应排名是否已经存在玩家
		offline_arena_map_by_class::iterator it_class = m_offline_arena_map_by_class.find(cur_class);
		if (it_class != m_offline_arena_map_by_class.end())
		{
			offline_arena_map_by_rank& oambr = it_class->second;
			if (oambr.find(enemy_rank) != oambr.end())
			{
				//当前排名已被玩家占有
				log_error("role[%lu] fight robot failed, rank[%d] belong to user", self_uid, enemy_rank);
				return errcode_enum::offline_arena_targetrank_change;
			}
		}
		OFFLINE_ARENA_LOG("role[%lu] sweep robot[%d] count[%d]", p_arena->get_uid(), robot_id, count);
	}
	else
	{
		log_error("role[%lu] sweep failed, enemy_type[%d] invalid", self_uid, enemy_type);
		return errcode_enum::notice_unknown;
	}
	p_arena->set_fight_count(p_arena->get_fight_count() - count);
	p_arena->set_score(p_arena->get_score() + count * get_win_score());
	p_arena->save_self();
	achieve_common_t::notify_progress_state(self_uid, proto::common::ACHIEVE_OFFLINE_BATTLE_TOTAL_POINT,0, count * get_win_score());
	uint32_t cur_class = p_arena->get_cur_class();
	auto p_class_conf = GET_CONF(OfflineArenaClass, cur_class);
	if (NULL == p_class_conf)
	{
		log_error("NULL == p_class_conf OfflineArenaClass[%d]", cur_class);
		return errcode_enum::notice_unknown;
	}
	//战斗结算
	proto::server::eg_offline_arena_sweep_award_notify ntf;
	ntf.set_drop_id(p_class_conf->win_award());
	ntf.set_count(count);
	p_self->send_msg_to_game(op_cmd::eg_offline_arena_sweep_award_notify, ntf);
	return errcode_enum::error_ok;
}

uint32_t offline_arena_manager_t::fight(uint64_t self_uid, uint32_t self_rank, uint32_t enemy_type, uint64_t enemy_id, uint32_t enemy_rank)
{
	//先判断自己是否存在
	global_user_ptr p_self = global_user_manager_t::get_global_user_by_uid(self_uid);
	if (NULL == p_self)
	{
		log_error("role[%lu] not exist", self_uid);
		return errcode_enum::notice_role_null;
	}
	offline_arena_map::iterator it = m_offline_arena_map.find(self_uid);
	if (it == m_offline_arena_map.end())
	{
		log_error("role[%lu] not in offline arena", self_uid);
		return errcode_enum::notice_role_null;
	}
	offline_arena_ptr p_arena = it->second;
	if (NULL == p_arena)
	{
		log_error("role[%lu] not in offline arena", self_uid);
		return errcode_enum::notice_role_null;
	}
	if (0 == p_arena->get_fight_count())
	{
		//战斗次数耗尽
		log_error("role[%lu] fight_count zero", self_uid);
		return errcode_enum::offline_arena_lack_times;
	}
	if (p_arena->get_enemy_id() > 0)
	{
		//正在攻打目标
		log_error("role[%lu] fighting", self_uid);
		return errcode_enum::notice_is_fighting;
	}
	if (self_rank != p_arena->get_cur_rank())
	{
		//自己的排名发生了变化
		log_error("role[%lu] fight failed, self_rank changed", self_uid);
		return errcode_enum::offline_arena_myrank_change;
	}
	if (enemy_rank <= 12 && self_rank > get_fight_top_ten_rank())
	{
		//自己的排名不满足挑战前12的条件
		log_error("role[%lu] fight failed, self_rank not in fight_top_ten_rank", self_uid);
		return errcode_enum::offline_arena_no_challenge;
	}
	proto::server::eg_offline_arena_fight_request msg;
	//再判断对手状态
	if (enemy_type == proto::common::SCENEOBJECT_USER)
	{
		//如果是玩家先判断玩家是否存在
		offline_arena_map::iterator it_enemy = m_offline_arena_map.find(enemy_id);
		if (it_enemy == m_offline_arena_map.end())
		{
			log_error("role[%lu] fight user failed, enemy[%lu] not in offline arena", self_uid, enemy_id);
			return errcode_enum::notice_unknown;
		}
		offline_arena_ptr p_enemy = it_enemy->second;
		if (NULL == p_enemy)
		{
			log_error("role[%lu] fight user failed, enemy[%lu] not in offline arena", self_uid, enemy_id);
			return errcode_enum::notice_unknown;
		}
		if (enemy_rank != p_enemy->get_cur_rank())
		{
			//对手的排名发生了变化
			log_error("role[%lu] fight user failed, enemy[%lu] rank changed", self_uid, enemy_id);
			return errcode_enum::offline_arena_targetrank_change;
		}
		redis_client_t::get_protobuf("offline_arena", "fight_data", enemy_id, *msg.mutable_enemy_data());
		OFFLINE_ARENA_LOG("role[%lu] fight enemy[%lu] ", p_arena->get_uid(), enemy_id);
	}
	else if (enemy_type == proto::common::SCENEOBJECT_MONSTER)
	{
		uint32_t cur_class = p_arena->get_cur_class();
		uint32_t robot_id = enemy_id;
		//如果是机器人先判断对应排名和机器人ID是否匹配
		if (robot_id != get_robot_by_rank(cur_class, enemy_rank))
		{
			log_error("role[%lu] fight robot failed, robot[%d] class[%d] rank[%d] not match", self_uid, robot_id, cur_class, enemy_rank);
			return errcode_enum::notice_unknown;
		}
		//再查找对应排名是否已经存在玩家
		offline_arena_map_by_class::iterator it_class = m_offline_arena_map_by_class.find(cur_class);
		if (it_class != m_offline_arena_map_by_class.end())
		{
			offline_arena_map_by_rank& oambr = it_class->second;
			if (oambr.find(enemy_rank) != oambr.end())
			{
				//当前排名已被玩家占有
				log_error("role[%lu] fight robot failed, rank[%d] belong to user", self_uid, enemy_rank);
				return errcode_enum::offline_arena_targetrank_change;
			}
		}
		proto::common::obj_single* p_obj = msg.mutable_enemy_data()->mutable_obj();
		p_obj->set_monster_id(enemy_id);
		p_obj->set_object_uid(string_util_t::uint64_to_string(enemy_id));
		p_obj->set_object_type(proto::common::SCENEOBJECT_MONSTER);
		OFFLINE_ARENA_LOG("role[%lu] fight robot[%d] ", p_arena->get_uid(), robot_id);
	}
	else
	{
		log_error("role[%lu] fight failed, enemy_type[%d] invalid", self_uid, enemy_type);
		return errcode_enum::notice_unknown;
	}
	//设置回调参数
	p_arena->set_fight_count(p_arena->get_fight_count() - 1);
	p_arena->set_enemy_type(enemy_type);
	p_arena->set_enemy_id(enemy_id);
	p_arena->set_enemy_rank(enemy_rank);
	//发送战斗请求到自己所在的game
	msg.set_offline_arena_class(p_arena->get_cur_class());
	p_self->send_msg_to_game(op_cmd::eg_offline_arena_fight_request, msg);
	return errcode_enum::error_ok;
}

void offline_arena_manager_t::fight_result(uint64_t role_uid, uint32_t is_win)
{
	offline_arena_ptr p_arena;
	proto::client::ec_offline_arena_fight_reply reply;
	do 
	{
		offline_arena_map::iterator it = m_offline_arena_map.find(role_uid);
		if (it == m_offline_arena_map.end())
		{
			log_error("role[%lu] not in offline arena", role_uid);
			reply.set_reply_code(errcode_enum::notice_role_null);
			break;
		}
		p_arena = it->second;
		if (NULL == p_arena)
		{
			log_error("role[%lu] not in offline arena", role_uid);
			reply.set_reply_code(errcode_enum::notice_role_null);
			break;
		}
		uint32_t enemy_type = p_arena->get_enemy_type();
		uint64_t enemy_id = p_arena->get_enemy_id();
		uint32_t self_rank = p_arena->get_cur_rank();
		if (p_arena->get_enemy_id() == 0)
		{
			//没有正在攻打的目标
			log_error("role[%lu] not fighting", role_uid);
			reply.set_reply_code(errcode_enum::notice_role_null);
			break;
		}
		//战报
		uint32_t now_time = time_util_t::now_time();
		offline_arena_record_ptr p_self_record = offline_arena_record_ptr(new offline_arena_record_t);
		p_self_record->set_is_win(is_win);
		p_self_record->set_record_time(now_time);
		p_self_record->set_old_rank(self_rank);
		p_self_record->set_enemy_type(enemy_type);
		p_self_record->set_enemy_uid(string_util_t::uint64_to_string(enemy_id));
		p_self_record->set_side(0);
		
		uint32_t cur_class = p_arena->get_cur_class();
		//判断对手类型
		if (enemy_type == proto::common::SCENEOBJECT_USER)
		{
			//如果是玩家先判断玩家是否存在
			offline_arena_map::iterator it_enemy = m_offline_arena_map.find(enemy_id);
			if (it_enemy == m_offline_arena_map.end())
			{
				log_error("role[%lu] fight_user_result error, enemy[%lu] not in offline arena", role_uid, enemy_id);
				reply.set_reply_code(errcode_enum::notice_unknown);
				break;
			}
			offline_arena_ptr p_enemy = it_enemy->second;
			if (NULL == p_enemy)
			{
				log_error("role[%lu] fight_user_result error, enemy[%lu] not in offline arena", role_uid, enemy_id);
				reply.set_reply_code(errcode_enum::notice_unknown);
				break;
			}
			uint32_t enemy_rank = p_enemy->get_cur_rank();
			if (cur_class != p_enemy->get_cur_class())
			{
				//对手去了更高的组别
				log_warn("role[%lu] fight_user_result error, enemy[%lu] not in same class", role_uid, enemy_id);
				reply.set_reply_code(errcode_enum::offline_arena_targetrank_change);
			}
			else
			{
				if (is_win)
				{
					if (enemy_rank < self_rank)
					{
						//对手的排名更高才替换
						change_rank(p_arena, cur_class, enemy_rank);
						change_rank(p_enemy, cur_class, self_rank);
						reply.set_reply_code(errcode_enum::error_ok);
					}
					else
					{
						reply.set_reply_code(errcode_enum::offline_arena_no_change);
					}
					p_arena->set_score(p_arena->get_score() + get_win_score());
					OFFLINE_ARENA_LOG("role[%lu] win enemy[%lu] ", p_arena->get_uid(), enemy_id);
					achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_OFFLINE_BATTLE_TOTAL_POINT, 0, get_win_score());
				}
				else
				{
					p_arena->set_score(p_arena->get_score() + get_lose_score());
					reply.set_reply_code(errcode_enum::error_ok);
					OFFLINE_ARENA_LOG("role[%lu] lose enemy[%lu] ", p_arena->get_uid(), enemy_id);
					achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_OFFLINE_BATTLE_TOTAL_POINT, 0, get_lose_score());
				}
			}
			p_self_record->set_enemy_name(global_user_data_mgr_t::get_name(enemy_id));
			p_self_record->set_enemy_fighting(global_user_data_mgr_t::get_user_fighting_value(enemy_id));
			p_self_record->set_enemy_plugin(global_user_data_mgr_t::get_user_plugin(enemy_id));
			p_self_record->set_enemy_headpic(global_user_data_mgr_t::get_user_hero_plugin(enemy_id));
			p_self_record->set_enemy_level(global_user_data_mgr_t::get_user_level(enemy_id));
			p_self_record->set_new_rank(p_arena->get_cur_rank());
			p_arena->add_record(p_self_record);
			p_arena->save_record_data();

			//敌人的战报
			offline_arena_record_ptr p_enemy_record = offline_arena_record_ptr(new offline_arena_record_t);
			p_enemy_record->set_is_win(is_win > 0 ? 0 : 1);
			p_enemy_record->set_record_time(now_time);
			p_enemy_record->set_old_rank(enemy_rank);
			p_enemy_record->set_enemy_type(proto::common::SCENEOBJECT_USER);
			p_enemy_record->set_enemy_uid(string_util_t::uint64_to_string(role_uid));
			p_enemy_record->set_enemy_name(global_user_data_mgr_t::get_name(role_uid));
			p_enemy_record->set_enemy_fighting(global_user_data_mgr_t::get_user_fighting_value(role_uid));
			p_enemy_record->set_enemy_plugin(global_user_data_mgr_t::get_user_plugin(role_uid));
			p_enemy_record->set_enemy_headpic(global_user_data_mgr_t::get_user_hero_plugin(role_uid));
			p_enemy_record->set_enemy_level(global_user_data_mgr_t::get_user_level(role_uid));
			p_enemy_record->set_new_rank(p_enemy->get_cur_rank());
			p_enemy_record->set_side(1);
			p_enemy->add_record(p_enemy_record);
			p_enemy->save_record_data();
			red_point_manager_t::red_point_notify(enemy_id, proto::common::RED_POINT_OFFLINE_ARENA);
		}
		else if (enemy_type == proto::common::SCENEOBJECT_MONSTER)
		{
			uint32_t cur_class = p_arena->get_cur_class();
			uint32_t enemy_rank = p_arena->get_enemy_rank();
			bool b_occupy = false;
			//查找对应排名是否已经存在玩家
			offline_arena_map_by_class::iterator it_class = m_offline_arena_map_by_class.find(cur_class);
			if (it_class != m_offline_arena_map_by_class.end())
			{
				offline_arena_map_by_rank& oambr = it_class->second;
				if (oambr.find(enemy_rank) != oambr.end())
				{
					//当前排名已被玩家占有
					log_warn("role[%lu] fight_robot_result error, rank[%d] belong to user", role_uid, enemy_rank);
					b_occupy = true;
				}
			}
			if (!b_occupy)
			{
				if (is_win)
				{
					if (enemy_rank < self_rank)
					{
						//对手的排名更高才替换
						change_rank(p_arena, cur_class, enemy_rank);
						change_rank(NULL, cur_class, self_rank);
						reply.set_reply_code(errcode_enum::error_ok);
					}
					else
					{
						reply.set_reply_code(errcode_enum::offline_arena_no_change);
					}
					p_arena->set_score(p_arena->get_score() + get_win_score());
					OFFLINE_ARENA_LOG("role[%lu] win robot[%lu] ", p_arena->get_uid(), enemy_id);
					achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_OFFLINE_BATTLE_TOTAL_POINT, 0, get_win_score());
				}
				else
				{
					p_arena->set_score(p_arena->get_score() + get_lose_score());
					reply.set_reply_code(errcode_enum::error_ok);
					OFFLINE_ARENA_LOG("role[%lu] lose robot[%lu] ", p_arena->get_uid(), enemy_id);
					achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_OFFLINE_BATTLE_TOTAL_POINT, 0, get_lose_score());
				}
			}
			else
			{
				reply.set_reply_code(errcode_enum::offline_arena_occupy);
			}

			OfflineRobotInfo *p_robot_info = GET_CONF(OfflineRobotInfo, hash_util_t::hash_to_uint32(cur_class, enemy_rank));
			if (NULL == p_robot_info)
			{
				log_error("p_robot_info NULL cur_class[%d] rank[%d]", cur_class, enemy_rank);
				reply.set_reply_code(errcode_enum::notice_unknown);
				break;
			}
			p_self_record->set_enemy_name(common::string_util_t::convert_to_utf8(p_robot_info->name()));
			p_self_record->set_enemy_fighting(p_robot_info->fight_value());
			p_self_record->set_enemy_plugin(p_robot_info->plugin());
			p_self_record->set_enemy_headpic(p_robot_info->headpic());
			p_self_record->set_enemy_level(p_robot_info->lv());
			p_self_record->set_new_rank(p_arena->get_cur_rank());
			p_arena->add_record(p_self_record);
			p_arena->save_record_data();
		}
		else
		{
			log_error("role[%lu] fight failed, enemy_type[%d] invalid", role_uid, enemy_type);
			reply.set_reply_code(errcode_enum::notice_unknown);
		}
		break;
	} while (true);
	if (NULL != p_arena)
	{
		p_arena->set_enemy_type(0);
		p_arena->set_enemy_id(0);
		p_arena->set_enemy_rank(0);
		p_arena->save_self();
	}
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(role_uid);
	if (NULL != p_role)
	{
		//玩家在线发送结果给客户端
		p_role->send_msg_to_client(op_cmd::ec_offline_arena_fight_reply, reply);
	}
}

void offline_arena_manager_t::record_award(uint64_t role_uid)
{
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(role_uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] not exist", role_uid);
		return;
	}
	offline_arena_map::iterator it = m_offline_arena_map.find(role_uid);
	if (it == m_offline_arena_map.end())
	{
		log_error("role[%lu] not in offline arena", role_uid);
		return;
	}
	offline_arena_ptr p_arena = it->second;
	if (NULL == p_arena)
	{
		log_error("role[%lu] not in offline arena", role_uid);
		return;
	}
	uint32_t old_record = p_arena->get_old_record_rank();
	uint32_t record = p_arena->get_record_rank();
	if (old_record <= record)
	{
		log_error("role[%lu] no record award", role_uid);
		return;
	}
	p_arena->set_old_record_rank(record);
	p_arena->save_self();
	OFFLINE_ARENA_LOG("role[%lu] award record_rank[%d] ", role_uid, record);
	std::map<uint32_t, uint32_t> item_id_count;
	proto::server::eg_offline_arena_record_award_notify ntf;
	for (uint32_t i = record; i < old_record; ++i)
	{
		OfflineArenaAward* p_conf = get_award_conf_by_rank(p_arena->get_cur_class(), i);
		if (NULL == p_conf)
		{
			log_error("NULL == p_conf OfflineArenaAward class[%d] rank[%d]", p_arena->get_cur_class(), i);
			continue;
		}
		for (auto it : p_conf->up_award())
		{
			item_id_count[it.first] += it.second;
		}
	}
	for (auto it : item_id_count)
	{
		proto::common::mail_item_obj* p_single = ntf.add_item_list();
		p_single->set_item_id(it.first);
		p_single->set_item_num(it.second);
	}
	p_role->send_msg_to_game(op_cmd::eg_offline_arena_record_award_notify, ntf);
}

void offline_arena_manager_t::score_award(uint64_t role_uid, uint32_t score)
{
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(role_uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] not exist", role_uid);
		return;
	}
	offline_arena_map::iterator it = m_offline_arena_map.find(role_uid);
	if (it == m_offline_arena_map.end())
	{
		log_error("role[%lu] not in offline arena", role_uid);
		return;
	}
	offline_arena_ptr p_arena = it->second;
	if (NULL == p_arena)
	{
		log_error("role[%lu] not in offline arena", role_uid);
		return;
	}
	p_arena->score_award(score);
}

uint32_t offline_arena_manager_t::buy(uint64_t role_uid, uint32_t money)
{
	offline_arena_map::iterator it = m_offline_arena_map.find(role_uid);
	if (it == m_offline_arena_map.end())
	{
		log_error("role[%lu] not in offline arena", role_uid);
		return errcode_enum::notice_role_null;
	}
	offline_arena_ptr p_arena = it->second;
	if (NULL == p_arena)
	{
		log_error("role[%lu] not in offline arena", role_uid);
		return errcode_enum::notice_role_null;
	}
	if (p_arena->get_fight_count() >= get_fight_count())
	{
		log_error("role[%lu] fight_count full", role_uid);
		return errcode_enum::offline_arena_times_toomuch;
	}
	uint32_t buy_count = p_arena->get_buy_count();
	if (s_buy_info.find(buy_count+1) == s_buy_info.end())
	{
		log_error("role[%lu] buy_count over", role_uid);
		return errcode_enum::notice_unknown;
	}
	uint32_t cost_money = s_buy_info[buy_count + 1];
	if (cost_money != money)
	{
		log_error("role[%lu] money[%d] param error", role_uid, money);
		return errcode_enum::notice_unknown;
	}
	p_arena->set_fight_count(p_arena->get_fight_count() + get_buy_fight_count());
	p_arena->set_buy_count(buy_count + 1);
	p_arena->save_self();
	OFFLINE_ARENA_LOG("role[%lu] buy fight_count ", role_uid);
	return errcode_enum::error_ok;
}

void offline_arena_manager_t::form_notify(uint64_t role_uid, const proto::common::mirror_fight_data& form_data)
{
	if (m_offline_arena_map.find(role_uid) == m_offline_arena_map.end())
	{
		OFFLINE_ARENA_LOG("form_notify role[%lu] not in offline_arena", role_uid);
		return;
	}
	redis_client_t::set_protobuf("offline_arena", "fight_data", role_uid, form_data);
	OFFLINE_ARENA_LOG("role[%lu] form_notify success", role_uid);
}


void offline_arena_manager_t::update_role_level(uint64_t role_uid, uint32_t level)
{
	offline_arena_map::iterator it = m_offline_arena_map.find(role_uid);
	if (it == m_offline_arena_map.end())
	{
		log_warn("role[%lu] not in offline arena", role_uid);
		return;
	}
	offline_arena_ptr p_arena = it->second;
	if (NULL == p_arena)
	{
		log_error("role[%lu] not in offline arena", role_uid);
		return;
	}
	uint32_t old_class = p_arena->get_cur_class();
	auto p_class_conf = get_class_conf_by_level(level);
	if (NULL == p_class_conf)
	{
		log_error("NULL == p_class_conf[%d] OfflineArenaClass", level);
		return;
	}
	uint32_t new_class = p_class_conf->id();
	if (new_class == old_class)
	{
		return;
	}
	uint32_t old_rank = p_arena->get_cur_rank();
	//从旧分段map里面删掉，如果在最低排名之上的话
	offline_arena_map_by_class::iterator it_class = m_offline_arena_map_by_class.find(old_class);
	if (it_class != m_offline_arena_map_by_class.end())
	{
		offline_arena_map_by_rank& oambr = it_class->second;
		offline_arena_map_by_rank::iterator it_rank = oambr.find(p_arena->get_cur_rank());
		if (it_rank != oambr.end())
		{
			oambr.erase(it_rank);
		}
	}

	p_arena->set_cur_class(new_class);
	p_arena->set_cur_rank(p_class_conf->min_rank() + 1);
	p_arena->set_old_record_rank(p_class_conf->min_rank() + 1);
	p_arena->set_record_rank(p_class_conf->min_rank() + 1);
	p_arena->save_self();
	OFFLINE_ARENA_LOG("role[%lu] class up from[%d] to[%d] ", role_uid, old_class, new_class);
	log_wrapper_t::send_offline_arena_rank_log(role_uid, rank_op_type_class, old_class, old_rank, p_arena->get_cur_class(), p_arena->get_cur_rank());
}

void offline_arena_manager_t::gm_rank(uint64_t role_uid, uint32_t rank)
{
	offline_arena_ptr p_self;
	offline_arena_map::iterator it = m_offline_arena_map.find(role_uid);
	if (it == m_offline_arena_map.end())
	{
		p_self = add_new_arena(role_uid);
	}
	else
	{
		p_self = it->second;
	}
	if (NULL == p_self)
	{
		log_error("NULL == p_self[%lu]", role_uid);
		return;
	}
	uint32_t cur_class = p_self->get_cur_class();
	offline_arena_map_by_class::iterator it_class = m_offline_arena_map_by_class.find(cur_class);
	if (it_class == m_offline_arena_map_by_class.end())
	{
		//目标rank是机器人
		change_rank(NULL, cur_class, p_self->get_cur_rank());
		change_rank(p_self, cur_class, rank);
		return;
	}
	offline_arena_map_by_rank& oambr = it_class->second;
	offline_arena_map_by_rank::iterator it_rank = oambr.find(rank);
	if (it_rank == oambr.end())
	{
		//目标rank是机器人
		change_rank(NULL, cur_class, p_self->get_cur_rank());
		change_rank(p_self, cur_class, rank);
	}
	else
	{
		//目标rank是玩家
		offline_arena_ptr p_enemy = it_rank->second;
		if (NULL != p_enemy)
		{
			change_rank(p_enemy, cur_class, p_self->get_cur_rank());
			change_rank(p_self, cur_class, rank);
		}
	}
}

OfflineArenaClass* offline_arena_manager_t::get_class_conf_by_level(uint32_t level)
{
	OfflineArenaClass* p_ret = NULL;
	for (uint32_t i = 1; i <= MAX_ARENA_CLASS; ++i)
	{
		auto p_conf = GET_CONF(OfflineArenaClass, i);
		if (NULL == p_conf)
		{
			log_error("OfflineArenaClass NULL == p_conf[%d]", i);
			return NULL;
		}
		if (level >= p_conf->level())
		{
			p_ret = p_conf;
		}
		else
		{
			break;
		}
	}
	return p_ret;
}

OfflineArenaMatch* offline_arena_manager_t::get_match_conf_by_rank(uint32_t rank)
{
	OfflineArenaMatch* p_ret = NULL;
	std::map<uint32_t, OfflineArenaMatch*> confs;
	GET_ALL_CONF(OfflineArenaMatch, confs);
	for (auto it : confs)
	{
		OfflineArenaMatch* p_conf = it.second;
		if (NULL == p_conf)
		{
			log_error("NULL == p_config OfflineArenaMatch table");
			return NULL;
		}
		if (1 == p_conf->rank().size())
		{
			if (rank == p_conf->rank().at(0))
			{
				p_ret = p_conf;
				break;
			}
		}
		else if (2 == p_conf->rank().size())
		{
			if (rank >= p_conf->rank().at(0) && rank <= p_conf->rank().at(1))
			{
				p_ret = p_conf;
				break;
			}
		}
	}
	return p_ret;
}

void offline_arena_manager_t::peek_general_data(uint32_t cur_class, uint32_t rank, proto::common::offline_arena_general* p_single)
{
	if (NULL == p_single)
	{
		log_error("NULL == p_single");
		return;
	}
	offline_arena_map_by_class::iterator it_class = m_offline_arena_map_by_class.find(cur_class);
	if (it_class == m_offline_arena_map_by_class.end())
	{
		peek_robot_general_data(cur_class, rank, p_single);
		return;
	}
	offline_arena_map_by_rank& oambr = it_class->second;
	offline_arena_map_by_rank::iterator it_rank = oambr.find(rank);
	if (it_rank == oambr.end())
	{
		peek_robot_general_data(cur_class, rank, p_single);
		return;
	}
	offline_arena_ptr p_arena = it_rank->second;
	if (NULL == p_arena)
	{
		peek_robot_general_data(cur_class, rank, p_single);
		return;
	}
	uint64_t role_uid = p_arena->get_uid();
	p_single->set_role_uid(string_util_t::uint64_to_string(role_uid));
	p_single->set_role_name(global_user_data_mgr_t::get_name(role_uid));
	p_single->set_fighting(get_offline_arena_fighting(role_uid));
	p_single->set_plugin(global_user_data_mgr_t::get_user_plugin(role_uid));
	p_single->set_level(global_user_data_mgr_t::get_user_level(role_uid));
	uint64_t family_id = global_user_data_mgr_t::get_user_family_id(role_uid);
	family_ptr p_family = family_manager_t::get_family(family_id);
	if (NULL != p_family)
	{
		p_single->set_family_name(p_family->get_name());
	}
	p_single->set_cur_rank(rank);
	p_single->set_object_type(proto::common::SCENEOBJECT_USER);
	p_single->set_headpic(global_user_data_mgr_t::get_user_hero_plugin(role_uid));
}

void offline_arena_manager_t::peek_robot_general_data(uint32_t cur_class, uint32_t rank, proto::common::offline_arena_general* p_single)
{
	if (NULL == p_single)
	{
		log_error("NULL == p_single");
		return;
	}
	p_single->set_object_type(proto::common::SCENEOBJECT_MONSTER);
	//uint32_t monster_id = get_robot_by_rank(cur_class, rank);
	OfflineRobotInfo *p_robot_info = GET_CONF(OfflineRobotInfo, hash_util_t::hash_to_uint32(cur_class, rank));
	if (NULL == p_robot_info)
	{
		log_error("p_robot_info NULL cur_class[%d] rank[%d]", cur_class, rank);
		return;
	}
	
	p_single->set_role_name(common::string_util_t::convert_to_utf8(p_robot_info->name()));
	p_single->set_fighting(p_robot_info->fight_value());
	p_single->set_cur_rank(rank);
	p_single->set_plugin(p_robot_info->plugin());
	p_single->set_headpic(p_robot_info->headpic());
	p_single->set_level(p_robot_info->lv());
	p_single->set_fight_against_id(p_robot_info->against_ids());
}

uint32_t offline_arena_manager_t::get_robot_by_rank(uint32_t cur_class, uint32_t rank)
{
	std::map<uint32_t, OfflineArenaRobot*> confs;
	GET_ALL_CONF(OfflineArenaRobot, confs);
	for (auto it : confs)
	{
		OfflineArenaRobot* p_conf = it.second;
		if (NULL == p_conf)
		{
			log_error("NULL == p_config OfflineArenaRobot table");
			return 0;
		}
		if (p_conf->sublevel() != cur_class)
		{
			continue;
		}
		if (2 != p_conf->rank().size())
		{
			log_error("p_config[%d] OfflineArenaRobot rank invalid", p_conf->id());
			return 0;
		}
		if (rank >= p_conf->rank().at(0) && rank <= p_conf->rank().at(1))
		{
			return p_conf->against();
		}
	}
	return 0;
}

uint32_t offline_arena_manager_t::get_offline_arena_fighting(uint64_t role_uid)
{
	if (m_offline_arena_map.find(role_uid) == m_offline_arena_map.end())
	{
		log_error("role[%lu] not in offline arena", role_uid);
		return 0;
	}
	proto::common::mirror_fight_data mfd;
	redis_client_t::get_protobuf("offline_arena", "fight_data", role_uid, mfd);
	return mfd.obj().fighting();
}

OfflineArenaAward* offline_arena_manager_t::get_award_conf_by_rank(uint32_t cur_class, uint32_t rank)
{
	OfflineArenaAward* p_ret = NULL;
	std::map<uint32_t, OfflineArenaAward*> confs;
	GET_ALL_CONF(OfflineArenaAward, confs);
	for (auto it : confs)
	{
		OfflineArenaAward* p_conf = it.second;
		if (NULL == p_conf)
		{
			log_error("NULL == p_config OfflineArenaAward table");
			return NULL;
		}
		if (p_conf->sublevel() != cur_class)
		{
			continue;
		}
		if (1 == p_conf->rank().size())
		{
			if (rank == p_conf->rank().at(0))
			{
				p_ret = p_conf;
				break;
			}
		}
		else if (2 == p_conf->rank().size())
		{
			if (rank >= p_conf->rank().at(0) && rank <= p_conf->rank().at(1))
			{
				p_ret = p_conf;
				break;
			}
		}
	}
	return p_ret;
}

uint32_t offline_arena_manager_t::get_fight_count()
{
	auto fight_count = GET_CONF(Comprehensive, comprehensive_common::offline_arena_refresh_number);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(fight_count);
	return val;
}

uint32_t offline_arena_manager_t::get_buy_fight_count()
{
	auto buy_fight_count = GET_CONF(Comprehensive, comprehensive_common::offline_arena_cost_number);
	uint32_t val = GET_COMPREHENSIVE_VALUE_2(buy_fight_count);
	return val;
}

uint32_t offline_arena_manager_t::get_fight_top_ten_rank()
{
	auto fight_top_ten_rank = GET_CONF(Comprehensive, comprehensive_common::offline_arena_challenge_top);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(fight_top_ten_rank);
	return val;
}

uint32_t offline_arena_manager_t::get_sweep_level()
{
	auto sweep_level = GET_CONF(Comprehensive, comprehensive_common::offline_arena_saodang_level);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(sweep_level);
	return val;
}

uint32_t offline_arena_manager_t::get_win_score()
{
	auto win_score = GET_CONF(Comprehensive, comprehensive_common::offline_arena_jifen_win);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(win_score);
	return val;
}

uint32_t offline_arena_manager_t::get_lose_score()
{
	auto lose_score = GET_CONF(Comprehensive, comprehensive_common::offline_arena_jifen_loss);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(lose_score);
	return val;
}
