#ifndef __TASK_H__
#define __TASK_H__

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "common.pb.h"
#include "client.pb.h"
#include "macros.hpp"
#include "tblh/TaskCircle.tbls.h"
/*
* =====================================================================================
*
*       Filename:  task.hpp
*
*    Description:  任务逻辑
*
* =====================================================================================
*/
enum task_accept_cond
{
	task_accept_cond_lifestar = 1,	//命力境界
	task_accept_cond_family = 2,	//家族
	task_accept_cond_office = 3,	//逆命师等级
};
/**
* @brief 任务触发条件类型
*/
enum task_active_type
{
	ACTIVE_EVERY_DAY_TIME = 1,//每天定时触发
	ACTIVE_APPOINT_TIME = 2,//指定时间触发
	ACTIVE_GENERAL_LEVEL = 3,//根据主将等级触发
	ACTIVE_HERO_LEVEL = 4,//根据英雄等级触发
	ACTIVE_PREVIOUS_TASK_FINISH = 5,//完成前置任务触发
	ACTIVE_GENERAL_CREATED_DAY = 6,//主将创建后第几天触发
	ACTIVE_FIRST_CHARGE_REWARD = 7,//领取首充礼包后触发
	ACTIVE_AUTO = 8,//模块上线后自动解锁
	ACTIVE_NPC = 9,//从npc处接取
	ACTIVE_GENERAL_LEVEL_GREATER = 10, //主将等级大于指定等级触发
};

enum accept_task_type
{
	ACCEPT_TASK_BY_SYSTEM,
	ACCEPT_TASK_BY_MANUAL,
	ACCEPT_TASK_BY_GMCMD,
};

enum
{
	CREATE_ROLE_INIT_TASK_ID = 340001,
};

/**
* @brief 任务类型
*/
enum task_type
{

	TASK_TYPE_TRUNK = 1,//主线任务
	TASK_TYPE_BRANCH = 2,//支线任务
	TASK_TYPE_DAILY = 3,//每日任务
	TASK_TYPE_WEEKLY = 4,//周常任务
	TASK_TYPE_ARMY_DAILY = 5, //军团日常
	TASK_TYPE_ARMY_WEEKLY = 6, //军团周常
	TASK_TYPE_COUNTRY_DAILY = 7, //国家日常
	TASK_TYPE_COUNTRY_WEEKLY = 8, //国家周常
	TASK_TYPE_ACTIVITY = 9, //活动任务
	TASK_TYPE_LOOKUP = 10, //寻访任务
	TASK_TYPE_FAMILY = 11, //家族任务
	TASK_TYPE_MASTER = 12, //经验环任务
	TASK_TYPE_BOUNTY = 13, //悬赏任务
	TASK_TYPE_TREASURE = 15, //藏宝图任务
	TASK_TYPE_BREAK = 16, //突破任务
	TASK_TYPE_SHILIAN = 17,//试炼任务
	TASK_TYPE_ZHUOGUI = 18,//捉鬼任务
	TASK_TYPE_JIAZU	  = 19,//家族任务
	TASK_TYPE_GUOJIA  = 20,//国家任务
	TASK_TYPE_OFFICE = 21,//逆命师任务
	TASK_TYPE_XIULIAN = 22,//修炼任务
};

//enum special_task
//{
//	FIRST_TASK_ID = 1,
//	FIRST_BRANCH_TASK_ID = 1000001,
//};

/**
* @brief 任务目标类型
*/
enum task_target_type
{
	TASK_TARGET_TYPE_KILL_MONSTER = 1,
	TASK_TARGET_TYPE_DIALOGUE = 2,
	TASK_TARGET_TYPE_COLLECT = 3,
	TASK_TARGET_TYPE_RUN = 4,
	TASK_TARGET_TYPE_USE_PROP = 5,
	TASK_TARGET_TYPE_LEVEL = 6,
	TASK_TARGET_TYPE_KILL_MONSTER_COLLECT = 7,
	
	TASK_TARGET_TYPE_KILL_USER = 9,
	TASK_TARGET_TYPE_CHOISE         = 10,
	TASK_TARGET_TYPE_KILL_MONSTER2 = 11,
	TASK_TARGET_TYPE_KILL_MONSTER3 = 12,
	TASK_TARGET_TYPE_JOIN_COUNTRY = 13,
	TASK_TARGET_TYPE_TREASURE = 15,
	TASK_TARGET_TYPE_RECRUIT = 16,
	TASK_TARGET_TYPE_KILL_VISIBLE_MONSTER = 17,
	TASK_TARGET_TYPE_DUNGEON = 18,
	TASK_TARGET_TYPE_JOIN_FAMILY = 19,
	TASK_TARGET_TYPE_PRACTISE = 20,
	TASK_TARGET_TYPE_HERO_GRADE = 21,
	TASK_TARGET_TYPE_HERO_FORM = 22,
	TASK_TARGET_TYPE_HERO_EQUIP = 23,
	TASK_TARGET_TYPE_EQUIP_STRENGTHEN_5 = 24,
	TASK_TARGET_TYPE_EQUIP_STRENGTHEN_10 = 25,
	TASK_TARGET_TYPE_COMMIT_PROP = 26,
	TASK_TARGET_TYPE_MAP_DARK_MONSTER = 27,
	TASK_TARGET_TYPE_SPECIAL_MONSTER = 28,
	TASK_TARGET_TYPE_OFFLIEN_ARENA = 29,
	TASK_TARGET_TYPE_GENERAL_EVENT = 30,
};

enum task_state
{
	TASK_STATE_DOING = 0, //进行中
	TASK_STATE_CAN_FINISH = 1,//可交付
	TASK_STATE_FINISH = 2,//已完成
	TASK_STATE_NOT_ACCEPT = 3,//未接取
};
class role_t;
typedef boost::shared_ptr<role_t> role_ptr;
class scene_t;
class task_t
{
public:
	task_t(){}
	task_t(uint64_t uid, uint32_t task_id);

	void trans_map_progress_2_string();
	void trans_str_progress_2_map();
	bool have_target(task_target_type type, uint32_t target_id);
	bool has_target_progress(task_target_type type, uint32_t target_id);
	uint32_t get_target_progress(task_target_type type, uint32_t target_id);
	void set_target_progress(task_target_type type, uint32_t target_id, uint32_t cnt);
	bool can_finished();
	void advance(task_target_type target_type, uint32_t target_id, uint32_t cnt, role_ptr p_role, bool is_add=true);
	bool is_target_before_done(task_target_type  type, uint32_t target_id);
	bool can_advance(task_target_type target_type, uint32_t target_id);
	// 改变任务状态
	void change_state(task_state now_state,uint64_t role_uid);
	void seek_help();
	uint32_t get_commit_item_tid();
	uint32_t get_commit_item_count();

    void load_data(const proto::common::task_state& task);
    void peek_data(proto::common::task_state* task);
    //void save_self(uint64_t role_uid);
    void save_self(role_ptr p_role);	//兼容历史代码

	uint32_t get_tid() { return m_tid; }
	void set_tid(uint32_t val) { m_tid = val; }

	uint64_t get_uid() { return m_uid; }
	void set_uid(uint64_t val) { m_uid = val; }

	uint32_t get_type() { return m_type; }
	void set_type(uint32_t val) { m_type = val; }

	std::string get_progress() { return m_progress; }
	void set_progress(const std::string& val) { m_progress = val; }
	void clear_progress() { m_progress_map.clear(); }

	uint32_t get_state() { return m_state; }
	void set_state(uint32_t val) { m_state = val; }

	uint32_t get_accept_time() { return m_accept_time; }
	void set_accept_time(uint32_t val) { m_accept_time = val; }

	uint32_t get_circle_id() { return m_circle_id; }
	void set_circle_id(uint32_t val) { m_circle_id = val; }

	uint32_t get_circle_count() { return m_circle_count; }
	void set_circle_count(uint32_t val) { m_circle_count = val; }
	void add_circle_count() { ++m_circle_count; }

	uint32_t get_max_circle_count() { return m_max_circle_count; }
	void set_max_circle_count(uint32_t val) { m_max_circle_count = val; }

	uint32_t get_accept_level() { return m_accept_level; }
	void set_accept_level(uint32_t val) { m_accept_level = val; }

	uint32_t get_star() { return m_star; }
	void set_star(uint32_t val) { m_star = val; }
 private:
	bool m_on_seek_help = false;

	/*任务id*/
	uint32_t		m_tid = 0;
	uint64_t		m_uid = 0;
	uint32_t		m_type = 0;
	/*任务进度格式为：任务目标类型:任务目标id:任务目标数量*/
	std::string m_progress = "";
	std::vector<std::tuple<uint32_t, uint32_t, uint32_t> > m_progress_map;
	/*任务状态（0：进行中；1：可交付；2：已完成；3：未接）*/
	uint32_t m_state = 0;
	/*任务接取时间*/
	uint32_t		m_accept_time = 0;
	uint32_t		m_circle_id = 0;
	uint32_t		m_circle_count = 0;
	uint32_t		m_max_circle_count = 0;
	uint32_t		m_accept_level = 0;
	uint32_t		m_star = 0;
};
typedef boost::shared_ptr<task_t> task_ptr;

//环任务每一环的信息
struct task_circle_single_t
{
	uint32_t					circle_count;	//环数
	uint32_t					type;			//任务类型
	std::vector<uint32_t>		task_id_range;	//任务库
	float						exp_coeff;		//经验系数
	uint32_t					award;			//分段奖励
};
typedef boost::shared_ptr<task_circle_single_t> task_circle_single_ptr;
typedef std::vector<task_circle_single_ptr> task_circle_single_vec;
//环任务整个环的信息
struct task_circle_t
{
	uint32_t					circle_id;			//环任务ID（师门任务为0，悬赏任务为策划表id）
	uint32_t					max_circle_count;	//最大环数
	task_circle_single_vec		circle_single_vec;	//每环的环信息
};

typedef boost::shared_ptr<task_circle_t> task_circle_ptr;
typedef std::map<uint32_t, task_circle_ptr> task_circle_map_by_level;//make_circle_key作为key
typedef std::map<uint32_t, task_circle_map_by_level> task_circle_map;//玩家等级作为key
#endif // !__TASK_H__

