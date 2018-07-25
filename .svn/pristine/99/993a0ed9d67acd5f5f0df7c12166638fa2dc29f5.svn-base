#pragma once

#include <boost/shared_ptr.hpp>
#include <vector>

#include "main/log_server_fwd.hpp"
#include "main/source_type_converter.hpp"
#include "log.hpp"
#include "log_enum.hpp"
#include "common_struct.hpp"
#include "common/zdb_ex.hpp"
#include "protos_fwd.hpp"

class combat_log_t
{
public:
	combat_log_t(uint64_t role_uid,
		uint32_t role_level,
		uint32_t fight_type,
		uint32_t combat_type,
		const std::string &team_members,
		const std::string &targets,
		uint64_t fight_uid,
		uint32_t is_win,
		uint32_t start_time,
		uint32_t fight_time,
		uint32_t finish_time,
		uint32_t log_time)
		: m_role_uid(role_uid)
		, m_role_level(role_level)
		, m_fight_type(fight_type)
		, m_combat_type(combat_type)
		, m_team_members(team_members)
		, m_targets(targets)
		, m_fight_uid(fight_uid)
		, m_is_win(is_win)
		, m_start_time(start_time)
		, m_fight_time(fight_time)
		, m_finish_time(finish_time)
		, m_log_time(log_time)
	{
	}

	bool find_fight_type_desc(uint32_t fight_type, std::string &fight_type_desc)
	{
		switch (fight_type)
		{
		case proto::common::fight_type_study:
			fight_type_desc = "切磋";
			break;// 切磋
		case proto::common::fight_type_field_boss:
			fight_type_desc = "野外boss";
			break;// 野外boss
		case proto::common::fight_type_task:
			fight_type_desc = "任务战斗";
			break;// 任务战斗
		case proto::common::fight_type_dark_monster:
			fight_type_desc = "暗雷战斗";
			break; // 暗雷战斗
 		case proto::common::fight_type_against:
 			fight_type_desc = "剧情战斗";
 			break;// 对阵战斗
		case proto::common::fight_type_cross_arena_rank:
			fight_type_desc = "跨服竞技场排位";
			break;// 跨服战斗(竞技场排位)
		case proto::common::fight_type_cross_arena_train:
			fight_type_desc = "跨服竞技场练习";
			break;// 跨服战斗(竞技场练习)
		case proto::common::fight_type_pk:
			fight_type_desc = "强行PK";
			break;// 强行PK
		case proto::common::fight_type_light_monster:
			fight_type_desc = "明雷战斗";
			break;// 明雷战斗
		case proto::common::fight_type_challenge:
			fight_type_desc = "皇城约战";
			break;//皇城约战
		case proto::common::fight_type_world_cup:
			fight_type_desc = "演武大赛";
			break;//演武大赛
		case proto::common::fight_type_tower:
			fight_type_desc = "爬塔";
			break;//爬塔
		case proto::common::fight_type_expedition:
			fight_type_desc = "远征";
			break;//远征
		case proto::common::fight_type_family_war:
			fight_type_desc = "家族战龙柱";
			break;// 家族战龙柱
		case proto::common::fight_type_family_pk:
			fight_type_desc = "家族战pk";
			break;// 家族战pk
		case proto::common::fight_type_page_pass_pk:
			fight_type_desc = "章节通过战斗";
			break;// 章节通过战斗
		case proto::common::fight_type_offline_arena:
			fight_type_desc = "离线竞技";
			break;// 离线竞技
		default:
			return false;
		}
		return true;
	}

// 	// 战斗方式
// 	enum combat_type
// 	{
// 		combat_type_single_role = 1,		// 单人战斗
// 		combat_type_troop = 2,				// 组队战斗
//	}
	bool find_combat_type_desc(uint32_t combat_type, std::string &combat_type_desc)
	{
		switch (combat_type)
		{
		case 1:
			combat_type_desc = "单人战斗";
			break;
		case 2:
			combat_type_desc = "组队战斗";
			break;
		default:
			return false;
		}
		return true;
	}

	void update_to_db(const Connection_T& conn)
	{
		TRY
		{
		std::string fight_type_str;
		if (!find_fight_type_desc(m_fight_type, fight_type_str))
		{
			log_error("can not find a description of the fight_type[%d]!", m_fight_type);
			return;
		}
		std::string combat_type_str;
		if (!find_combat_type_desc(m_combat_type, combat_type_str))
		{
			log_error("can not find a description of the combat_type[%d]!", m_combat_type);
			return;
		}

		PreparedStatement_T pre = Connection_prepareStatement(conn,
			"insert into `combat_log` set \
                `role_uid`=?, \
				`role_level`=?, \
				`fight_type`=?, \
				`fight_type_desc`=?, \
				`combat_type`=?, \
				`combat_type_desc`=?, \
				`team_member`=?, \
				`targets`=?, \
				`fight_uid`=?, \
				`is_win`=?, \
                `start_time`=from_unixtime(?), \
                `fight_time`=?, \
                `finish_time`=from_unixtime(?), \
                `create_time`=from_unixtime(?);");
		PreparedStatement_setLLong(pre, 1, m_role_uid);
		PreparedStatement_setInt(pre, 2, m_role_level);
		PreparedStatement_setInt(pre, 3, m_fight_type);
		PreparedStatement_setString(pre, 4, fight_type_str.c_str());
		PreparedStatement_setInt(pre, 5, m_combat_type);
		PreparedStatement_setString(pre, 6, combat_type_str.c_str());
		PreparedStatement_setString(pre, 7, m_team_members.c_str());
		PreparedStatement_setString(pre, 8, m_targets.c_str());
		PreparedStatement_setLLong(pre, 9, m_fight_uid);
		PreparedStatement_setInt(pre, 10, m_is_win);
		PreparedStatement_setInt(pre, 11, m_start_time);
		PreparedStatement_setInt(pre, 12, m_fight_time);
		PreparedStatement_setInt(pre, 13, m_finish_time);
		PreparedStatement_setInt(pre, 14, m_log_time);
		PreparedStatement_execute(pre);
		}
			CATCH(SQLException)
		{
			log_error("role[%lu] insert combat log to db error: %s", m_role_uid, Exception_frame.message);
		}
		FINALLY
		{
		}
		END_TRY;
	}

private:
	uint64_t m_role_uid;
	uint32_t m_role_level;
	uint32_t m_fight_type;
	uint32_t m_combat_type;
	std::string m_team_members;
	std::string m_targets;
	uint64_t m_fight_uid;
	uint32_t m_is_win;
	uint32_t m_start_time;
	uint32_t m_fight_time;
	uint32_t m_finish_time;
	uint32_t m_log_time;
};
typedef boost::shared_ptr<combat_log_t> combat_log_ptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////
class combat_log_manager_t
{
public:
	void add_log(combat_log_ptr log_ptr)
	{
		m_logs_list.push_back(log_ptr);
	}

	void update_to_db(const Connection_T& conn)
	{
		int32_t vec_size = m_logs_list.size();
		for (int32_t i = 0; i < vec_size; ++i)
		{
			if (m_logs_list[i])
			{
				m_logs_list[i]->update_to_db(conn);
			}
		}
		m_logs_list.clear();
	}

private:
	typedef std::vector<combat_log_ptr> combat_log_vec_type;
	combat_log_vec_type m_logs_list;
};
