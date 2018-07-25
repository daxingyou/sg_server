#include "role.hpp"
#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "scene/scene_manager.hpp"
#include "log.hpp"
#include "role_manager.hpp"
#include "role_unify_save.hpp"
#include "item/item_manager.hpp"
#include "hero/hero_manager.hpp"
//#include "equip/equip_manager.hpp"
#include "cache_key.hpp"
#include "utility.hpp"
#include "config/config_manager.hpp"
#include "object/object_manager.hpp"
#include "monster/monster_manager.hpp"
#include "client.pb.h"
#include "tblh/Comprehensive.tbls.h"
#include "script/script_role.h"
#include "tblh/errcode_enum.hpp"
#include "log/log_wrapper.hpp"
#include "fight/fight_manager.hpp"
#include "user_troop/user_troop.hpp"
#include "user_troop/user_troop_mgr.hpp"
#include "trade/trade_manager.hpp"
#include "money_manager.hpp"
#include "mount/mount_manager.hpp"
#include "tblh/LevelupTable.tbls.h"
#include "tblh/FightTypeTable.tbls.h"
#include "tblh/Monster.tbls.h"
#include "sys_notice/sys_notice_manager.hpp"
#include "arena/arena_manager.hpp"
#include "pk/pk_manager.hpp"
#include "center/center_msg_handler.hpp"
#include "king_war/king_war_manager.hpp"
#include "rank/rank_manager.hpp"
#include "tblh/JumpTable.tbls.h"
#include "tblh/rank_common.hpp"
#include "tblh/HeroGrade.tbls.h"
#include "tblh/Hero.tbls.h"
#include "red_point/red_point_manager.hpp"
#include "lifeforce/lifeforce_manager.hpp"
#include "tblh/LifeState.tbls.h"
#include "user_troop/user_troop.hpp"
#include "achieve/active_reward_manager.hpp"
#include "achieve/achieve_common.hpp"
#include "achieve/achieve_common_enum.hpp"
#include "tblh/VIP.tbls.h"
#include <bitset>
#include "tblh/MountTable.tbls.h"
#include "tblh/Patrol.tbls.h"
#include "tblh/PatrolExp.tbls.h"
#include "time_manager.hpp"
#include "tblh/time_type_enum.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "tblh/SectionReward.tbls.h"
#include "page/page_manager.hpp"
#include "item/drop_manager.hpp"
#include "country/country_manager.hpp"
#include "log/log_wrapper.hpp"
#include "tblh/LabelTable.tbls.h"
#include "lifeforce/lifeforce_attr_manager.hpp"
#include "tblh/HeadTable.tbls.h"
#include "general_event/general_info_manager.hpp"
#include "tblh/Career.tbls.h"
#include "tblh/RechargeTotal.tbls.h"
#include "tblh/FrameTable.tbls.h"
#include "tblh/HandBook.tbls.h"

USING_NS_COMMON;
using namespace errcode_enum;


role_t::role_t(uint64_t uid)
	: object_base_t(uid)
	, m_uid(uid)
	, m_script_role(new script_role_t(this))
	, m_package_manager(uid)
	, m_formation_mgr(uid)
	, m_fight_study_mgr(uid)
	, m_role_scene(uid)
	, m_arena(uid)
	, m_role_dungeon_data_t(uid)
	, m_role_random_shop(uid)
	, m_trade(uid)
	, m_role_level_reward(uid)
	, m_role_level_reward_mail(uid)
	, m_business_act_mgr(uid)
	, m_general_event(uid)
	, m_friendliness(uid)
	, m_change_name(0)
	, m_cur_title(0)
{
    if (uid > 0)
    {
        m_key = cache_key_t::create(uid, cache_name::roles);
		m_statistical_key = cache_key_t::create(uid, cache_name::statistical_key);	//不想混用role的存储
    }

	m_task_mgr.reset(new task_manager_t(m_uid));
	m_bounty_mgr.reset(new bounty_manager_t(m_uid));
	m_activity_mgr.reset(new activity_manager_t(m_uid));
	m_money.reset(new money_t(m_uid));
	m_mount_data.reset(new mount_data_t(m_uid));
	m_tower.reset(new tower_t(m_uid));
	m_luckydraw.reset(new luckydraw_t(m_uid));
	m_tactic_mgr.reset(new tactic_manager_t(m_uid));
	m_role_buff_mgr.reset(new role_buff_manager_t(m_uid));
	m_expedition.reset(new expedition_t(m_uid));
	m_liandan_mgr.reset(new liandan_manager_t(m_uid));
	m_family_skill_mgr.reset(new family_skill_manager_t(m_uid));
	m_equip_plan_mgr.reset(new equip_plan_manager_t(m_uid));
	m_patrol_hire.reset(new patrol_hire_t(m_uid));
	m_recharge.reset(new recharge_t(m_uid));
	m_office.reset(new office_t(m_uid));
}

role_t::~role_t()
{
    
}

void role_t::load_role_data(const proto::common::role_data& rd)
{
	m_name = rd.name();
	m_gm = rd.gm();
	m_create_time = rd.create_time();
	m_last_login_time = rd.last_login_time();
    m_last_logout_time = rd.last_logout_time();
    m_continue_login_days = rd.continue_login_days();
    m_cumulative_login_days = rd.cumulative_login_days();
	m_update_day_time = rd.update_day_time();
	m_update_week_time = rd.update_week_time();
	m_role_type = rd.role_type();

	// 设置进入场景
	set_map_tid(rd.map_tid());
	set_current_position(rd.obj_info().current_x(), rd.obj_info().current_y(), rd.obj_info().direction());

	// 设置最后进入场景
	m_role_scene.set_last_scene_info(rd.last_map());

	set_object_id(string_util_t::string_to_uint64(rd.uid()));

	if (rd.has_per_info())
	{
		set_personal_info(rd.per_info());
	}

	m_mode_state = (proto::common::role_mode_state)rd.mode_state();

	if (rd.has_trade_info())
	{
		m_trade.load_data(rd.trade_info());		
	}
	if (rd.has_ttd())
	{
		if (NULL != m_task_mgr)
		{
			m_task_mgr->load_task_type_data(rd.ttd());
		}
	}
	if (rd.has_formation())
	{
		m_formation_mgr.load_all_datas(rd.formation());
	}
    if (rd.has_guide_data())
    {
        const proto::common::new_role_guide_data nrgd = rd.guide_data();
        for (int32_t i = 0; i < nrgd.guide_list_size(); ++i)
        {
            const proto::common::new_role_guide_single nrgs = nrgd.guide_list(i);
			if (m_new_role_guide_list.count(nrgs.tid()) != 0)
			{
				m_new_role_guide_list[nrgs.tid()].status = (NEW_ROLE_GUIDE_STATUS)nrgs.status();
			}
			else 
			{
				new_role_guide_t guide;
				guide.tid = nrgs.tid();
				guide.status = (NEW_ROLE_GUIDE_STATUS)nrgs.status();
				guide.event_type = nrgs.event_type();
				m_new_role_guide_list[guide.tid] = guide;
			}
        }
    }
	m_role_fight_data.CopyFrom(rd.fight());
	m_level = rd.level();
	m_exp = rd.exp();
	m_function_code = common::string_util_t::string_2_to_uint64(rd.function_code());
	m_npc_code = rd.npc_code();
	m_collect_code = rd.collect_code();
	
	if (rd.has_tactic())
	{
		m_tactic_mgr->load_data(rd.tactic());
	}
	if (rd.has_role_buff())
	{
		m_role_buff_mgr->load_data(rd.role_buff());
	}

	if (rd.has_page_list())
	{
		m_page_list.clear();
		m_page_task_map.clear();
		m_page_elite_map.clear();
		m_quick_elite_page_map.clear();
		m_reset_quick_elite_page_map.clear();
	
		const proto::common::page_data& nrgd = rd.page_list();
		///加载已经通关的章节
		for (int32_t i = 0; i < nrgd.pass_page_list_size(); ++i)
		{
			m_page_list.insert(nrgd.pass_page_list(i));
		}
		///加载已经通关的章节任务
		for (int32_t i = 0; i < nrgd.task_info_size(); ++i)
		{
			const proto::common::page_task_single& nrgs = nrgd.task_info(i);
			m_page_task_map.insert(std::make_pair(nrgs.id(), nrgs.status()));
		}

		///加载已经通关的章节精英
		const proto::common::page_elite_data& elite_list = nrgd.elite_data();
		for (int32_t i = 0; i < elite_list.pass_elite_list_size(); ++i)
		{
			const proto::common::page_elite_single& nrgs = elite_list.pass_elite_list(i);
			m_page_elite_map.insert(std::make_pair(nrgs.id(), nrgs.status()));
		}

		//加载扫荡的章节信息 
		for (int32_t i = 0; i < nrgd.quick_data_size(); ++i)
		{
			const proto::common::page_quick_fight_data& nrgs = nrgd.quick_data(i);
			m_quick_elite_page_map.insert(std::make_pair(nrgs.id(), nrgs.num()));
		}

		//加载重置扫荡的章节信息 
		for (int32_t i = 0; i < nrgd.reset_data_size(); ++i)
		{
			const proto::common::page_reset_quick_fight_data& nrgs = nrgd.reset_data(i);
			m_reset_quick_elite_page_map.insert(std::make_pair(nrgs.id(), nrgs.num()));
		}
	}

	if (rd.has_task_list())
	{
		m_daily_task_list.clear();
		const proto::common::active_reward_task_data& active_reward_task = rd.task_list();
		for (int i = 0; i < active_reward_task.task_list_size(); ++i)
		{
			uint32_t task_id = active_reward_task.task_list(i);
			if (0 != task_id)
			{
				m_daily_task_list.insert(task_id);
			}	
		}
	}

	if (rd.has_multi_time())
	{
		m_multi_time_list.clear();
		const proto::common::multi_time_data& multi_time = rd.multi_time();
		for (int i = 0; i < multi_time.time_long_list_size();++i)
		{
			const proto::common::public_struct& public_struct = multi_time.time_long_list(i);
			m_multi_time_list.insert(std::make_pair(public_struct.type(), public_struct.param()));
		}
	}
	m_multi_state = rd.multi_time().state();

	if (rd.has_life_data())
	{
		m_lifeforce_label = rd.life_data().label();
	}

	if (rd.has_head_frame_info())
	{
		const proto::common::head_data& data = rd.head_frame_info();
		for (int i = 0; i < data.head_list_size();i++)
		{
			m_head_list.insert(data.head_list(i));
		}

		for (int i = 0; i < data.frame_list_size(); i++)
		{
			m_frame_list.insert(data.frame_list(i));
		}
	}
}

void role_t::load_cross_data(const proto::common::role_data& rd)
{
	set_object_id(string_util_t::string_to_uint64(rd.uid()));
	m_name = rd.name();
	m_level = rd.level();
	m_function_code = common::string_util_t::string_2_to_uint64(rd.function_code());
	m_mode_state = (proto::common::role_mode_state)rd.mode_state();
	if (rd.has_per_info())
	{
		set_personal_info(rd.per_info());
	}

	if (rd.has_formation())
	{
		m_formation_mgr.load_all_datas(rd.formation());
	}
	m_role_fight_data.CopyFrom(rd.fight());
}

void role_t::load_all_datas(proto::server::dg_role_data_reply const& rdr)
{
    if (m_state != ROLE_STATE_LOADING)
    {
        log_error("m_state[%d] != ROLE_STATE_LOADING", m_state);
        return;
    }

    // role data
    {
        const proto::common::role_data& rd = rdr.role();
		load_role_data(rd);
    }

    // load module data
	{
		// 坐骑要优先于物品加载 坐骑会影响背包大小
		mount_manager_t::load_all_mounts(get_role(), rdr.mount_data());
		achieve_manager_t::load_all_data(get_uid(), rdr.achieve());
		//武将加载要在物品之前 需要把装备数据填充到武将
		hero_manager_t::load_all_datas(get_role(), rdr.hero());
		item_manager_t::load_all_datas(get_role(), rdr.item());
		//家族技能要放在武将和阵型前加载，因为会影响计算属性
		if (NULL != m_family_skill_mgr) m_family_skill_mgr->load_data(rdr.family_skill_data());
		m_fighting = m_formation_mgr.calc_fighting();
//		m_formation_mgr.load_all_datas(rdr.formation());
		m_arena.load_from_db(rdr.arena_role_data());
        m_trade.load_trade_item_data(rdr.trade_item());
        if (NULL != m_task_mgr) m_task_mgr->load_all_task_data(rdr.task());
		if (NULL != m_bounty_mgr) m_bounty_mgr->load_data(rdr.country_bounty());
		if (NULL != m_tower) m_tower->load_data(rdr.tower());
		if (NULL != m_luckydraw) m_luckydraw->load_data(rdr.luckydraw());
		if (NULL != m_expedition) m_expedition->load_data(rdr.expedition_data());
		if (NULL != m_activity_mgr) m_activity_mgr->load_data(rdr.activities_data());
		if (NULL != m_liandan_mgr) m_liandan_mgr->load_data(rdr.liandan_data());
		
		if (NULL != m_equip_plan_mgr) m_equip_plan_mgr->load_data(rdr.equip_plan_data());
		if (NULL != m_patrol_hire) m_patrol_hire->load_data(rdr.patrol_data(), m_last_logout_time);
		if (NULL != m_recharge) m_recharge->load_data(rdr.recharge_data());
		if (NULL != m_office) m_office->load_data(rdr.office_data());
		m_role_dungeon_data_t.load_data(rdr.dungeon_data());
		m_role_dungeon_data_t.load_times_data(rdr.dungeon_times_data());
		m_role_random_shop.load_data(rdr.random_shop_data());
		m_role_level_reward.load_data(rdr.level_rwd_data());
		m_role_level_reward_mail.load_data(rdr.level_rwd_mail_data());
		m_business_act_mgr.load_data(rdr.bussiness_act_data());
		m_general_event.load_data(rdr.general_event_data());

		//没有模块存放，不想混入role数据，挂在buss_data数据下
		const proto::common::bussiness_act_data &business_act_data = rdr.bussiness_act_data();
		load_statistical_data(business_act_data.statistical_data() );
		
		active_reward_manager_t::load_all_data(get_uid(), rdr.active_reward_data());
    }
	///纠正下线前跑商状态 但当前跑商背包负重为0 的情况
	if (m_mode_state == proto::common::role_mode_state_trade && 
		m_trade.get_trade_bag_weight() == 0)
	{
		m_mode_state = proto::common::role_mode_state_normal;
	}

	// 更新速度
	change_real_speed(speed_only_set);
	 
	// add new role init things
	if (m_main_hero_plugin == 0)
	{
		m_is_new_role = true;

		init_new_role_datas(m_role_type);
	}

    // init finish
    m_state = ROLE_STATE_OK;
}


// 新角色初始化
void role_t::init_new_role_datas(uint32_t role_type)
{	

	//成就基础值初始化 需要放在前面
	m_achieve_level = 1;
	m_achieve_recv_reward_level = 0;

	//命力相关初始化
	m_lifestate_level = 1;
	m_lifestar_level = 0;
	m_lifeforce_label = 0;
	///初始化玩家头像和边框 头像采用模型ID 边框从全局表读取默认值
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::default_flame_id);
	uint32_t default_frame = 1;
	if (NULL == p_conf)
	{
		log_error("role[%lu] get Comprehensive default_flame_id[%d] NULL", get_uid(), comprehensive_common::default_flame_id);
	}
	else
	{
		default_frame = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	init_head_frame(role_type, default_frame);
    // 添加主角武将
	hero_manager_t::add_role_hero(get_role(), role_type, false);

	// notify script
	if (NULL != m_script_role)
	{
		m_script_role->init_new_role_data();
	}

	// 初始化精力
	m_vigour = get_vigour_base_limit();

	// add formation
	m_formation_mgr.init_formation();
	m_fighting = m_formation_mgr.calc_fighting();
	//init bounty
	if (NULL != m_bounty_mgr)
	{
		m_bounty_mgr->init();
	}
	//init tower
	if (NULL != m_tower)
	{
		m_tower->init_new_data();
	}
	//init expedition
	if (NULL != m_expedition)
	{
		m_expedition->init_new_data();
	}
	//init tactic
	if (NULL != m_tactic_mgr)
	{
		m_tactic_mgr->add_tactic(0, false);
	}
	//init office
	if (NULL != m_office)
	{
		m_office->init();
	}
    //add first task
    if (NULL != m_task_mgr)
	{
        proto::common::role_change_data rcd;
        
        auto p_first_task_id = GET_CONF(Comprehensive, comprehensive_common::first_task_id);
	    uint32_t first_task_id = GET_COMPREHENSIVE_VALUE_1(p_first_task_id);
        m_task_mgr->active_task_by_id(first_task_id, ACCEPT_TASK_BY_SYSTEM, rcd, false);
		m_task_mgr->active_task_by_cond(rcd, true);
		//自动任务已被取消不用 by王思源
        //auto p_first_branch_task_id = GET_CONF(Comprehensive, comprehensive_common::first_branch_task_id);
		//uint32_t first_branch_task_id = GET_COMPREHENSIVE_VALUE_1(p_first_branch_task_id);
		//m_task_mgr->active_task_by_id(first_branch_task_id, ACCEPT_TASK_BY_SYSTEM, rcd);
    }

	///初始化新玩家活跃奖励任务列表
	active_reward_manager_t::init_task_list(m_uid);

    if (NULL != m_money)
    {
        m_money->set_prestige_level(1);
    }
	m_pk_count = pk_manager_t::get_pk_count();
	m_target_count = pk_manager_t::get_target_count();
	get_trade_info().init_trade();

	m_business_act_mgr.init_newbie();
	if (NULL != m_luckydraw)
	{
		auto p_initial_fate = GET_CONF(Comprehensive, comprehensive_common::initial_fate);
		uint32_t initial_fate = GET_COMPREHENSIVE_VALUE_1(p_initial_fate);
		m_luckydraw->add_lucky(initial_fate);
	}
    //初始化GM权限
    init_gm_level();

	//测试代码 不能提交！！！！！！
    {
        //m_level = 60;
        //open_all_function();
        //item_manager_t::add_item(get_role(), 200008, 1, log_enum::source_type_gm_create_item, 0);
        //item_manager_t::add_item(get_role(), 350001, 1, log_enum::source_type_gm_create_item, 0);
        //money_manager_t::add_money(get_role(), proto::common::MONEY_TYPE_LIFE_STONE, 9999999, log_enum::source_type_gm_add_money, 0);
        //money_manager_t::add_money(get_role(), proto::common::MONEY_TYPE_SILVER, 9999999, log_enum::source_type_gm_add_money, 0);
        //money_manager_t::add_money(get_role(), proto::common::MONEY_TYPE_COPPER, 9999999, log_enum::source_type_gm_add_money, 0);
        // 	m_task_mgr->gm_finish_all_task();
        // 	achieve_common_t::test_add_achieve(m_uid);
        // 
        // 	for (uint32_t i = 0; i < 200; ++i)
        // 	{
        // 		item_manager_t::add_item(get_role(), 210001, 1, log_enum::source_type_gm_create_item, 0);
        // 	}
    }

	save_self();
}

void role_t::peek_all_datas(proto::client::gc_enter_game_reply* egr)
{
    if (NULL == egr)
    {
        log_error("role_uid[%lu] NULL == egr", m_uid);
        return;
    }
    // role data
    {
        peek_data(egr->mutable_role(), true);
    }

    // load module data
    {
        item_manager_t::peek_all_datas(get_role(), egr->mutable_item());
		hero_manager_t::peek_all_datas(get_role(), egr->mutable_hero());
		m_formation_mgr.peek_all_datas(egr->mutable_formation());
		m_arena.peek_data(egr->mutable_arena_data());
        m_trade.peek_trade_item_data(egr->mutable_trade_item());
		if (NULL != m_task_mgr)
		{
			m_task_mgr->peek_all_datas(egr->mutable_task());
			m_task_mgr->notify_task_event_to_client();
		}
        if (NULL != m_mount_data) m_mount_data->peek_data(egr->mutable_mount_data());
		if (NULL != m_activity_mgr) m_activity_mgr->peek_data(egr->mutable_activities_data());
		if (NULL != m_recharge) m_recharge->peek_data(egr->mutable_recharge_data());
	}
}

void role_t::peek_data(proto::common::role_data* data, bool is_to_client)
{
    if (NULL == data)
    {
        log_error("role_uid[%lu] NULL == data", m_uid);
        return;
    }

    //common
    {
        data->set_uid(string_util_t::uint64_to_string(m_uid));
        data->set_gm(m_gm);
        data->set_name(m_name);
		data->set_map_tid(get_map_tid());
		data->set_channel_id(get_chat_channel());
		data->set_level(get_level());
		data->set_exp(get_exp());
		data->set_function_code(string_util_t::uint64_to_2_string(m_function_code));
		data->set_npc_code(m_npc_code);
		data->set_collect_code(m_collect_code);
		data->set_create_time(m_create_time);
		data->set_charm(m_charm);
		get_object_info(data->mutable_obj_info());
        get_personal_info(data->mutable_per_info());
        m_trade.peek_data(data->mutable_trade_info());
        if (NULL != m_task_mgr) m_task_mgr->peek_task_type_data(data->mutable_ttd());
		if (NULL != m_tactic_mgr) m_tactic_mgr->peek_data(data->mutable_tactic());
		if (NULL != m_role_buff_mgr) m_role_buff_mgr->peek_data(data->mutable_role_buff());
        m_formation_mgr.peek_all_datas(data->mutable_formation());
		data->mutable_fight()->CopyFrom(m_role_fight_data);
		data->set_fighting(get_fighting());
		if (NULL != m_tower)
		{
			data->set_current_tower(m_tower->get_current_tower());
		}
		if (!m_page_list.empty())
		{
			proto::common::page_data* p_page_data = data->mutable_page_list();
			for (auto& it : m_page_list)
			{
				p_page_data->add_pass_page_list(it);
			}
		}

		if (!m_page_task_map.empty())
		{
			proto::common::page_data* p_page_data = data->mutable_page_list();
			for (auto& it : m_page_task_map)
			{
				proto::common::page_task_single* p_page_task_single = p_page_data->add_task_info();
				p_page_task_single->set_id(it.first);
				p_page_task_single->set_status(it.second);
			}
		}

		if (!m_page_elite_map.empty())
		{
			proto::common::page_data* p_page_data = data->mutable_page_list();
			proto::common::page_elite_data* p_page_elite = p_page_data->mutable_elite_data();
			for (auto& it : m_page_elite_map)
			{
				proto::common::page_elite_single* p_page_elite_single = p_page_elite->add_pass_elite_list();
				p_page_elite_single->set_id(it.first);
				p_page_elite_single->set_status(it.second);
			}
		}

		if ( !m_quick_elite_page_map.empty() )
		{
			proto::common::page_data* p_page_data = data->mutable_page_list();
			for (auto& it : m_quick_elite_page_map )
			{
				proto::common::page_quick_fight_data* p_quick_data = p_page_data->add_quick_data();
				p_quick_data->set_id(it.first);
				p_quick_data->set_num(it.second);
			}
		}

		if (!m_reset_quick_elite_page_map.empty())
		{
			proto::common::page_data* p_page_data = data->mutable_page_list();
			for (auto& it : m_reset_quick_elite_page_map)
			{
				proto::common::page_reset_quick_fight_data* p_reset_quick_data = p_page_data->add_reset_data();
				p_reset_quick_data->set_id(it.first);
				p_reset_quick_data->set_num(it.second);
			}
		}

		proto::common::multi_time_data* p_multi_time_data = data->mutable_multi_time();
		if (!m_multi_time_list.empty())
		{
			for (auto& it : m_multi_time_list)
			{
				proto::common::public_struct* p_public = p_multi_time_data->add_time_long_list();
				p_public->set_type(it.first);
				p_public->set_param(it.second);
			}
		}
		p_multi_time_data->set_state(m_multi_state);
		data->set_mode_state(m_mode_state);

		proto::common::life_info* p_life_info = data->mutable_life_data();
		p_life_info->set_label(m_lifeforce_label);

		proto::common::head_data* p_head_frame = data->mutable_head_frame_info();
		if (!m_head_list.empty())
		{
			for (auto& it : m_head_list)
			{
				p_head_frame->add_head_list(it);
			}
		}

		if (!m_frame_list.empty())
		{
			for (auto& it : m_frame_list)
			{
				p_head_frame->add_frame_list(it);
			}
		}
    }
    //game->client
    if (is_to_client)
    {
        data->set_did(user_did_generator::gen_user_did(m_uid));
        data->set_is_new_role(m_is_new_role);
        //将【待完成：NEW_ROLE_GUIDE_STATUS_DOING】的新手引导发给客户端
	   /* if (!m_new_role_guide_list.empty())
		{
			proto::common::new_role_guide_data* p_guide_data = data->mutable_guide_data();
			for (auto guide : m_new_role_guide_list)
			{
				if (NEW_ROLE_GUIDE_STATUS_DOING == guide.second.status)
				{
					NEW_GUIDE_LOG("guide[%d] status[%d] type[%d]", guide.second.tid, guide.second.status, guide.second.event_type);
					proto::common::new_role_guide_single* p_guide_single = p_guide_data->add_guide_list();
					p_guide_single->set_tid(guide.second.tid);
					p_guide_single->set_event_type(guide.second.event_type);
				}
			}
		}*/

		//新手引导
		traverse_guide_id([data](uint32_t guide_id){
			data->add_compeleted_guide_id(guide_id);
		});	
    }
    //game->db
    else
    {
        data->set_last_login_time(m_last_login_time);
        data->set_last_logout_time(m_last_logout_time);
		data->set_update_day_time(m_update_day_time);
		data->set_update_week_time(m_update_week_time);
        //将【准备完成：NEW_ROLE_GUIDE_STATUS_READY_DONE】和【真正完成：NEW_ROLE_GUIDE_STATUS_REALLY_DONE】的新手引导发给db
        if (!m_new_role_guide_list.empty())
        {
            proto::common::new_role_guide_data* p_guide_data = data->mutable_guide_data();
            for (auto guide : m_new_role_guide_list)
            {
                if (NEW_ROLE_GUIDE_STATUS_READY_DONE == guide.second.status || NEW_ROLE_GUIDE_STATUS_REALLY_DONE == guide.second.status)
                {
                    proto::common::new_role_guide_single* p_guide_single = p_guide_data->add_guide_list();
                    p_guide_single->set_tid(guide.second.tid);
                    p_guide_single->set_status(guide.second.status);
					p_guide_single->set_event_type(guide.second.event_type);
                }
            }
        }


		// 设置最后进入场景
		m_role_scene.get_last_scene_info(data->mutable_last_map());

		
		proto::common::active_reward_task_data* p_active_reward_task = data->mutable_task_list();
		for (auto& it : m_daily_task_list)
		{
			p_active_reward_task->add_task_list(it);
		}
    }

	data->mutable_server()->set_server_id(m_server_id);
	data->mutable_server()->set_gate(m_gate_id);
	data->mutable_server()->set_server_name(env::xml->srv_group.group_name);
}

void role_t::peek_global_user_data(proto::server::global_user_data* data)
{
    if (NULL == data)
    {
        log_error("role[%lu] NULL == data", m_uid);
        return;
    }

	data->set_type(m_login_type);
	data->set_level(m_level);
	data->set_fighting(get_fighting());
	data->set_hero_plugin(m_main_hero_plugin);
	data->set_scene_id(m_scene_id);
	data->set_map_tid(m_map_tid);
	data->set_head_frame_id(m_head_frame);
	data->set_charm(m_charm);
	if (m_money)
		data->set_family_contribution(m_money->get_money(proto::common::MONEY_TYPE_CONTRIBUTION));
}

void role_t::peek_cross_data(proto::common::role_cross_data* data, proto::common::role_cross_type type)
{
	if (NULL == data)
	{
		log_error("data is null");
		return;
	}

	proto::common::role_data *p_role_data = data->mutable_rd();
	p_role_data->set_uid(string_util_t::uint64_to_string(m_uid));
	p_role_data->set_name(m_name);
	p_role_data->set_level(get_level());
	get_personal_info(p_role_data->mutable_per_info());
	m_formation_mgr.peek_all_datas(p_role_data->mutable_formation());
	p_role_data->mutable_fight()->CopyFrom(m_role_fight_data);
	p_role_data->mutable_server()->set_server_id(m_server_id);
	p_role_data->mutable_server()->set_server_name(env::xml->srv_group.group_name);
	p_role_data->mutable_server()->set_gate(m_gate_id);
	p_role_data->set_mode_state(m_mode_state);
	p_role_data->set_function_code(string_util_t::uint64_to_2_string( m_function_code));
	p_role_data->mutable_obj_info()->set_fighting_value(m_fighting);
	p_role_data->set_charm(m_charm);
	//m_formation_mgr.peek_fight_data(data->mutable_hd());
	hero_manager_t::peek_all_datas(get_role(), data->mutable_hd());
	item_manager_t::peek_hero_equip_datas(get_role(), data->mutable_item_data());
}

void role_t::peek_fight_data(proto::common::mirror_fight_data* data)
{
	if (NULL == data)
	{
		log_error("data is null");
		return;
	}
	proto::common::obj_single* p_obj = data->mutable_obj();
	p_obj->set_object_uid(string_util_t::uint64_to_string(m_uid));
	p_obj->set_name(get_name());
	p_obj->set_level(get_level());
	p_obj->set_object_type(proto::common::SCENEOBJECT_USER);
	p_obj->set_country(get_country_id());
	p_obj->set_life_level(get_lifestate_level());
	p_obj->set_fighting(get_fighting());
	p_obj->set_head_id(get_head_frame());
	p_obj->set_auto_ignore_lineup( 0 );
	m_formation_mgr.peek_fight_data(data->mutable_heros());
	data->set_formation_id(m_formation_mgr.get_cur_formation_id());
}

void role_t::save_all_datas()
{
    //关闭game|角色跳场景|角色下线：所有数据立刻存
    save_self();
    save_other_datas();
    role_unify_save::save(SAVE_TYPE_LEAVE_CLOSE_GAME);
}
void role_t::save_self(bool is_right_now)
{
    proto::common::role_data data;
	peek_data(&data, false);
    role_unify_save::add_task(get_uid(), get_key(), data, is_right_now);
}
void role_t::save_other_datas()
{
    //注意依赖顺序！！！
    item_manager_t::save_all_datas(get_role());
    hero_manager_t::save_all_datas(get_role());
    if (NULL != m_task_mgr) m_task_mgr->save_all_task_data();
    if (NULL != m_bounty_mgr) m_bounty_mgr->save_self();
	//暂时关闭远征 by王思源
    //if (NULL != m_expedition) m_expedition->save_self();
    if (NULL != m_activity_mgr) m_activity_mgr->save_self();
	if (NULL != m_liandan_mgr) m_liandan_mgr->save_self();
	if (NULL != m_family_skill_mgr) m_family_skill_mgr->save_self();
	if (NULL != m_equip_plan_mgr) m_equip_plan_mgr->save_self();
    achieve_manager_t::save_all_self(m_uid);
    active_reward_manager_t::save_all_self(m_uid);
    get_trade_info().save_trade_item_data(m_uid);
	m_role_dungeon_data_t.save_times_data();
}

//////////////////////////////////////////////////////////////////////////
// 处理中心服下发数据
void role_t::set_center_user_data(const proto::server::eg_reg_user_scene_request& data)
{
	m_family_id = data.family_id();
	m_family_name = data.family_name();
	m_country_id = data.country_id();
	m_family_officer = data.family_officer();
	m_country_officer = data.country_officer();
	m_family_level = data.family_level();
	m_charm = data.charm();

	m_role_scene.set_pre_scene_info(data.scene_data());
}

bool role_t::reg_scene()
{
	// 登陆场景处理
	scene_ptr p_login_scene = NULL;

	switch (m_login_type)
	{
	case proto::server::login_server_scene:	// 上线处理
		{
			p_login_scene = scene_manager_t::find_scene(m_role_scene.get_pre_scene_info().scene_id());
			if (p_login_scene == NULL)
			{
				log_error("role[%lu] login server[%lu] not find, type:%u", m_uid, m_role_scene.get_pre_scene_info().scene_id(), m_role_scene.get_pre_scene_info().type());
				return false;
			}

			uint32_t old_map_tid = m_map_tid;	// 下线地图

			// 中心服要求登陆的场景
			m_scene_id = p_login_scene->get_scene_id();
			m_map_tid = p_login_scene->get_map_tid();

			if (m_map_tid != old_map_tid)
			{
				// 如果是上个场景，去原来的点
				if (m_map_tid == m_role_scene.get_last_map_tid())
				{
					set_current_position(m_role_scene.get_last_pos_x(), m_role_scene.get_last_pos_y(), m_role_scene.get_last_dir());
				}
				else
				{
					sPos born_pos = p_login_scene->get_scene_born_position(get_role());
					if (born_pos.is_zero())
					{
						log_error("role[%lu] scene[%lu:%u] get born position error", m_uid, m_scene_id, m_map_tid);
					}

					set_current_position(born_pos.m_x, born_pos.m_y, born_pos.m_dir);
				}
			}
			//更新在线时长
			update_login_time();
		}
		break;
	case proto::server::user_change_scene:
	case proto::server::user_enter_dungeon:
	case proto::server::forced_change_scene:
	case proto::server::center_change_scene:
		{
			p_login_scene = scene_manager_t::find_scene(m_role_scene.get_pre_scene_info().scene_id());

			if (p_login_scene == NULL)
			{
				log_error("role[%lu] login server[%lu] not find, type:%u", m_uid, m_role_scene.get_pre_scene_info().scene_id(), m_role_scene.get_pre_scene_info().type());
				return false;
			}

			// 中心服要求登陆的场景
			m_scene_id = p_login_scene->get_scene_id();
			m_map_tid = p_login_scene->get_map_tid();

			if (m_role_scene.get_pre_scene_info().des_x() > 0.f && m_role_scene.get_pre_scene_info().des_y() > 0.f)
			{
				set_current_position(m_role_scene.get_pre_scene_info().des_x(), m_role_scene.get_pre_scene_info().des_y(), m_role_scene.get_pre_scene_info().des_dir());
			}
			else
			{
				sPos born_pos = p_login_scene->get_scene_born_position(get_role());
				if (born_pos.is_zero())
				{
					log_error("role[%lu] scene[%lu:%u] get born position error", m_uid, m_scene_id, m_map_tid);
				}

				set_current_position(born_pos.m_x, born_pos.m_y, born_pos.m_dir);
			}

			if (m_role_scene.get_pre_scene_info().troop_id() != 0) { // 跨场景队伍处理
				troop_ptr p_troop = game_troop_mgr_t::get_troop(m_role_scene.get_pre_scene_info().troop_id());
				if (p_troop != NULL) {
					p_troop->cross_server_init(get_role());
				}
			}
		}
		break;
	case proto::server::user_troop_jump:
		{
			troop_ptr p_troop = game_troop_mgr_t::get_troop(m_role_scene.get_pre_scene_info().troop_id());
			if (p_troop != NULL)
			{
				role_ptr p_leader = role_manager_t::find_role(p_troop->get_leader_id());
				if (p_leader != NULL)
				{
					p_login_scene = scene_manager_t::find_scene(p_leader->get_scene_id());

					if (p_login_scene != NULL)
					{
						m_scene_id = p_login_scene->get_scene_id();
						m_map_tid = p_login_scene->get_map_tid();

						set_current_position(p_leader->get_current_x(), p_leader->get_current_y(), p_leader->get_direction());
					}
				}
				else
				{
					log_warn("role[%lu] login server type[user_troop_jump], troop[%lu] leader[%lu] not find", m_uid, m_role_scene.get_pre_scene_info().troop_id(), p_troop->get_leader_id());
				}
			}
			else
			{
				log_warn("role[%lu] login server type[user_troop_jump], troop[%lu] not find", m_uid, m_role_scene.get_pre_scene_info().troop_id());
			}

			// 一个都找不到就用center发来的场景id
			if (p_login_scene == NULL)
			{
				p_login_scene = scene_manager_t::find_scene(m_role_scene.get_pre_scene_info().scene_id());

				if (p_login_scene != NULL)
				{
					m_scene_id = p_login_scene->get_scene_id();
					m_map_tid = p_login_scene->get_map_tid();

					sPos born_pos = p_login_scene->get_scene_born_position(get_role());
					if (born_pos.is_zero())
					{
						log_error("role[%lu] scene[%lu:%u] get born position error", m_uid, m_scene_id, m_map_tid);
					}

					set_current_position(born_pos.m_x, born_pos.m_y, born_pos.m_dir);
				}
			}

			if (p_login_scene == NULL)
			{
				log_error("role[%lu] login server[%lu] not find, type:%u", m_uid, m_role_scene.get_pre_scene_info().scene_id(), m_role_scene.get_pre_scene_info().type());
				return false;
			}

			if (p_troop != NULL) {
				p_troop->cross_server_init(get_role());
			}
		}
		break;
	default:
		{
			log_error("user[%lu] login server type[%u] error", m_uid, m_role_scene.get_pre_scene_info().type());
			return false;
		}
		break;
	}

	// 对于新登陆玩家，初始化状态，其他状态一律不处理，死亡复活在进场景的时候处理
	if (get_object_state() == proto::common::object_state_none)
	{
		set_object_born();
	}

	if (m_update_day_time == 0) { // 跨天(新号)
		m_update_day_time = common::time_util_t::now_time();
	} else if (!time_manager_t::check_same_period(common::time_type_enum::time_one_day, m_update_day_time)) { // 跨天
		do_next_day(common::time_util_t::now_time(), false);
	}
	
	if (m_update_week_time == 0) { // 跨周(新号)
		m_update_week_time = common::time_util_t::now_time();
	} else if (!time_manager_t::check_same_period(common::time_type_enum::time_week, m_update_week_time)) { // 跨周
		do_next_week(common::time_util_t::now_time());
	}

	// 上线后处理下竞技场数据
	get_arena().init();

	// 下发数据登陆场景
	if (!do_login_scene(p_login_scene))
	{
		return false;
	}

	// 通知其他服务器人物场景登陆
	notify_other_server_role_login_scene();

	//上线检测是否要发送等级邮件
	check_send_level_reward_mail();

	// 登陆统一存一下
	save_self();

	log_wrapper_t::send_role_login_log(m_uid, m_gate_id, env::server->get_id());

	return true;
}

bool role_t::do_login_scene(const scene_ptr& p_login_scene)
{
	if (p_login_scene == NULL)
	{
		log_error("role[%lu] login scene can not find", m_uid);
		return false;
	}

	if (p_login_scene->get_scene_state() != SCENE_STATE_NORMAL)
	{
		log_error("role[%lu] login scene[%lu] is not running", m_uid, p_login_scene->get_scene_id());
		return false;
	}

	// 登陆或者重新登陆，下发人物数据
	if (m_login_type == proto::server::login_server_scene || 
		m_login_type == proto::server::relogin_server_scene)
	{
		proto::client::gc_enter_game_reply egr;
		egr.set_reply_code(0);
		egr.set_server_time(common::time_util_t::now_time());
		egr.set_time_zone((int32_t)common::time_util_t::get_timezone());
		peek_all_datas(&egr);
		send_msg_to_client(op_cmd::gc_enter_game_reply, egr);
	}

	notify_new_guide();

	// 通知客户端加载场景，缓存之后的消息
	proto::client::gc_enter_scene_notify notify;
	notify.set_scene_id(common::string_util_t::uint64_to_string(m_scene_id));
	notify.set_map_tid(m_map_tid);
	notify.set_pos_x(m_current_x);
	notify.set_pos_y(m_current_y);
	send_msg_to_client(op_cmd::gc_enter_scene_notify, notify);

	//SCENE_LOG("role[%lu] notify client login scene[%lu]", m_uid, m_scene_id);

	if (!p_login_scene->on_enter_s(get_object_ptr(), m_current_x, m_current_y, m_direction))
	{
		log_error("role[%lu] enter scene[%lu] failed, map is [%u]", m_uid, p_login_scene->get_scene_id(), p_login_scene->get_map_tid());
		return false;
	}

	m_online_state = common::ROLE_ONLINE_STATE_ONLINE;

	if (get_watching_fight_uid())
	{
		set_watching_fight_uid(0);
	}

	//运营活动上线处理
	m_business_act_mgr.on_user_login();

	//天下大势上线
	m_general_event.on_login();

	//请求亲密度
	m_friendliness.seek_data_from_center();
	//SCENE_LOG("role[%lu] enter scene[%lu] success, map is [%u]", m_uid, m_scene_id, m_map_tid);
	return true;
}

//客户端通知服务器要登陆场景了
//上线需要发送给客户端的消息一律放在这里处理！！！
void role_t::ready_enter_scene()
{
	ROLE_LOG("role[%lu] ready enter scene......", m_uid);

	// 这里处理上线和重新登陆独有的处理
	if (m_login_type == proto::server::login_server_scene ||
		m_login_type == proto::server::relogin_server_scene)
	{
		// 通知一些基础的滚屏信息
		sys_notice_manager_t::on_role_login(get_role());

		// 登陆处理死亡
		if (is_die())
		{
			const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
			if (p_scene != NULL)
			{
				p_scene->on_login_die(get_role());
			}
		}

		get_trade_info().notify_trade_info(true);

		// 发送等级奖励
		if (is_function_open(proto::common::function_code_type_level_reward)){
			proto::client::gc_level_reward_info_notify lv_rwd;
			m_role_level_reward.peek_data(lv_rwd.mutable_list());
			send_msg_to_client(op_cmd::gc_level_reward_info_notify, lv_rwd);
		}

		if (NULL != m_patrol_hire) m_patrol_hire->on_login();

		// 通知其他服务器登陆场景(跨服不需要发)
		notify_other_server_ready_enter_scene();
	}

	//pk_timer
	if (m_login_type != proto::server::relogin_server_scene)
	{
	if (m_pk_value_reduce_time > 0)
	{
		if (NULL == m_pk_reduce_timer)
		{
			m_pk_reduce_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
			} else
		{
			m_pk_reduce_timer->cancel();
		}
		m_pk_reduce_timer->expires_from_now(boost::posix_time::seconds(m_pk_value_reduce_time));
		m_pk_reduce_timer->async_wait(boost::bind(&role_t::on_reduce_pk_timer, boost::weak_ptr<role_t>(get_role()), boost::asio::placeholders::error));
	}

	if (m_pk_punishment_time > 0)
	{
		if (NULL == m_pk_punish_timer)
		{
			m_pk_punish_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
			} else
		{
			m_pk_punish_timer->cancel();
		}
		m_pk_punish_timer->expires_from_now(boost::posix_time::seconds(m_pk_punishment_time));
		m_pk_punish_timer->async_wait(boost::bind(&role_t::on_prison_timeout, boost::weak_ptr<role_t>(get_role()), boost::asio::placeholders::error));
	}
	}

	//是否放在进入场景
	if (m_is_new_role)
	{
		achieve_manager_t::check_send_delay_list(get_uid(),m_is_new_role);
		auto p_first_task_id = GET_CONF(Comprehensive, comprehensive_common::first_task_id);
		uint32_t first_task_id = GET_COMPREHENSIVE_VALUE_1(p_first_task_id);
		if (m_task_mgr->is_activated(first_task_id))
		{
			script_event_task_accept(first_task_id);
		}
		
		script_event_new_role_enter_scene();
		m_is_new_role = false;
	}
	
	red_point_manager_t::init_red_point(get_uid());
	page_manager_t::init(get_uid());

	if (m_troop_id != 0) {
		troop_ptr p_troop = game_troop_mgr_t::get_troop(m_troop_id);
		if (p_troop != NULL) {
			p_troop->cross_server_update(get_role(), m_login_type);
		}
	}

	// 重置状态
	m_login_type = proto::server::scene_change_none;

	// 战斗上线处理
	fight_manager_t::on_role_login(get_role());

	update_role_info_to_center(proto::common::simple_info_type);

	if (get_troop_id() == 0)
	{
		drop_circle_task(TASK_TYPE_ZHUOGUI);
	}
	
	if (is_leader())
	{
		task_ptr p_task = m_task_mgr->get_circle_task(TASK_TYPE_ZHUOGUI);
		if (p_task) 
		{
			role_ptr p_role = role_manager_t::find_role(get_uid());
			if (NULL != p_role)
			{
				game_troop_mgr_t::accept_zhuagui_task(p_role, p_task->get_tid());
			}
		}
	}
}

void role_t::leave_game()
{
	//fight
	fight_manager_t::on_role_logout(get_role());

	leave_scene();
}

void role_t::offline()
{
	set_online_state(common::ROLE_ONLINE_STATE_OFFLINE);

	//pk_timer
	if (m_pk_value_reduce_time > 0)
	{
		m_pk_value_reduce_time = time_util_t::get_remain_time(m_pk_reduce_timer);
	}
	if (m_pk_punishment_time > 0)
	{
		m_pk_punishment_time = time_util_t::get_remain_time(m_pk_punish_timer);
	}
	if (is_patrol())
	{
		set_patrol(false);
	}
	if(get_watching_fight_uid() > 0)
		set_watching_fight_uid(0);

	//离线数据
	proto::server::ge_offline_data_notify ntf;
	if (NULL != m_tower)
	{
		m_tower->peek_self_trigger(ntf.mutable_ord()->mutable_tower_trigger());
	}
	env::server->send_msg_to_center(op_cmd::ge_offline_data_notify, get_uid(), ntf);

	///用于同步玩家的简易信息到center  目前用于查看他人信息
	update_role_info_to_center(proto::common::simple_info_type);
}

void role_t::set_last_logout_time(uint32_t logout_time)
{
    m_last_logout_time = logout_time;
	if (m_last_logout_time >= m_last_login_time)
	{
		m_online_time = m_last_logout_time - m_last_login_time;
		ROLE_LOG("role[%lu] online time: %d seconds", m_uid, m_online_time);
	}
    else
    {
        m_last_logout_time = m_last_login_time + 1;
        m_online_time = 1;
        log_error("role[%lu] logout_time[%d] < login_time[%d]", m_uid, m_last_logout_time, m_last_login_time);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void role_t::on_move_event(scene_ptr p_scene)
{
	if (NULL == p_scene)
	{
		log_error("p_scene null error");
		return;
	}

	do_something_for_area(p_scene);
}

// 区域处理
void role_t::do_something_for_area(scene_ptr p_scene)
{
	if (NULL == p_scene)
	{
		log_error("p_scene null error");
		return;
	}

	m_role_scene.update_area(get_role(), p_scene);

	// 组队队员并且未暂离不碰怪
	if (is_troop_member_not_leave()) 
		return;

	// 暗雷触发计算,战斗中不触发
	if (get_fight_uid() == 0 && m_role_scene.get_scene_trigger_status())
	{
		if (m_role_scene.trigger_time_pass() == 0)
		{
			uint32_t monster_id = m_role_scene.get_trigger_monster_id();
			Monster* p_monster = GET_CONF(Monster, monster_id);
			if (NULL != p_monster)
			{
				proto::common::fight_param fp;
				fp.set_type(proto::common::fight_type_dark_monster);
				fp.mutable_dark()->set_monster_id(monster_id);
				if (m_role_scene.get_task_id() > 0)
				{
					fp.mutable_dark()->set_task_id(m_role_scene.get_task_id());
				}
				fight_manager_t::fight_pve(get_role(), p_monster->get_fight_id(), &fp);
			}
			else
			{
				log_error("monster[%u] null error", monster_id);
			}

			m_role_scene.reset_trigger_time();
		}
	}
}

// 更新任务暗雷
void role_t::update_area_trigger()
{
	m_role_scene.update_area_trigger(get_role());
}

// 通知周围玩家自己的场景信息
void role_t::notify_object_common_info_for_nearbies()
{
	proto::client::gc_scene_object_info_notify ntf;
	get_object_info(ntf.mutable_common_info());
	send_msg_to_nine(op_cmd::gc_scene_object_info_notify, ntf);
}

void role_t::notify_object_common_info_for_me()
{
	proto::client::gc_scene_object_info_notify ntf;
	get_object_info(ntf.mutable_common_info());
	send_msg_to_client(op_cmd::gc_scene_object_info_notify, ntf);
}

void role_t::role_enter_scene(uint64_t scene_id, proto::client::gc_enter_scene_reply& reply)
{
	if (m_scene_id != scene_id)
	{
		log_error("role[%lu] scene[%lu] != jump_scene[%lu] error", m_uid, m_scene_id, scene_id);
		reply.set_reply_code(errcode_enum::notice_jump_error);
		return;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
	if (p_scene == NULL)
	{
		log_error("role[%lu] enter scene[%lu] can not find", m_uid, m_scene_id);
		reply.set_reply_code(errcode_enum::notice_jump_error);
		return;
	}

	if (!p_scene->send_nine_to_me(m_nine_key, reply.mutable_vision()))
	{
		reply.set_reply_code(errcode_enum::notice_jump_error);
		return;
	}

	reply.set_reply_code(errcode_enum::error_ok);
}

void role_t::get_object_info(proto::common::object_common_info* object_info)
{
	if (NULL == object_info)
	{
		log_error("NULL == object_info");
		return;
	}

	object_base_t::get_object_info(object_info);
	//object_info->set_speed(get_real_speed());
	object_info->set_mode_id(m_mode_id);
	object_info->set_object_name(m_name);
	object_info->set_level(get_level());
	object_info->set_fighting_value(get_fighting());
	object_info->set_mount_tid(get_mount_data()->get_turn_mount_tid());
	object_info->set_plugin(get_sync_plugin());
	object_info->set_head(get_head_frame());
	proto::common::object_state_info* object_state = object_info->mutable_object_state();
	if (object_state != NULL)
	{
		//object_state->set_obj_state(m_object_state);
		object_state->set_mode_state(m_mode_state);
		object_state->set_obj_state_param(common::string_util_t::uint64_to_string(m_obj_state_param));
	}

	object_info->set_pk_value(m_pk_value);

	if (0 != m_family_id) {
		object_info->set_family_id(string_util_t::uint64_to_string(m_family_id));
		object_info->set_family_name(m_family_name);
		object_info->set_family_officer(m_family_officer);
	}

	if (m_country_id != 0) {
		country_manager_t::peek_country_common(m_country_id, object_info->mutable_country());
		object_info->set_country_officer(m_country_officer);
	}

	object_info->set_is_leader(is_leader());
	object_info->set_troop_id(string_util_t::uint64_to_string(m_troop_id));
	if (get_lifeforce_label() > 0 )
	{
		object_info->set_state_level(get_lifeforce_label());
	}
	

	if (NULL != m_role_buff_mgr) m_role_buff_mgr->peek_obj_data(object_info);

	object_info->set_pk_mode(m_pk_mode);
	object_info->set_pk_ex(m_pk_ex);

	if (m_show_temporary_title) {
		object_info->set_cur_title(m_temporary_title);
	} else {
		object_info->set_cur_title(m_cur_title);
	}
}

void role_t::get_personal_info(proto::common::personal_info* personal)
{
	if (NULL == personal)
	{
		log_error("NULL == personal");
		return;
	}
	personal->Clear();
	personal->set_pk_value_reduce_time(m_pk_value_reduce_time);
	personal->set_pk_value(m_pk_value);
	personal->set_pk_count(m_pk_count);
	personal->set_target_count(m_target_count);
	personal->set_errantry(m_errantry);
	personal->set_pk_punishment(m_pk_punishment_time);
	// 家族id区 object_common_info
	//personal->set_family_id(string_util_t::uint64_to_string(m_family_id));
    personal->set_city_id(m_city_id);
	personal->set_luck_gift_gold(m_luck_gift_gold);
	personal->set_hero_uid(string_util_t::uint64_to_string(m_main_hero_uid));
	personal->set_cur_form_id(m_formation_mgr.get_cur_formation_id());
	personal->set_equip_smelt_value(m_equip_smelt_value);
	personal->set_achieve_level(m_achieve_level);
	personal->set_achieve_recv_reward_level(m_achieve_recv_reward_level);
	for (auto box_id : m_equip_smelt_box_vec)
	{
		personal->add_equip_smelt_box(box_id);
	}
	m_money->peek_data(personal);
	personal->set_lifestar_level(m_lifestar_level);
	personal->set_lifestate_level(m_lifestate_level);
	personal->set_plugin(m_main_hero_plugin);
	personal->set_patrol_count(m_patrol_count);
	personal->set_equip_lucky(m_equip_lucky);

	// 精力值会先计算后赋值
	personal->set_vigour(get_vigour());
	personal->set_vigour_buy(m_vigour_buy);
	personal->set_vigour_time(m_vigour_time);
	///头像和边框
	personal->set_head_frame(m_head_frame);

	// 忽略布阵
	personal->set_ignore_lineup_flag(get_ignore_lineup_flag() ? 1 : 0);

	personal->set_month_card_id(m_business_act_mgr.get_month_card_id());
	m_business_act_mgr.get_activity_shop().peek_data(personal);
 
	for (auto career_id : m_career_id_vec)
	{
		personal->add_career_id(career_id);
	}
	personal->set_change_name(m_change_name);
	for (auto title_id : m_title_id_vec)
	{
		personal->add_title_id(title_id);
	}
	personal->set_cur_title(m_cur_title);
	for (auto handbook : m_handbook_vec)
	{
		personal->add_handbook(handbook);
	}
}

// 设置角色私有信息
void role_t::set_personal_info(const proto::common::personal_info& personal)
{
	m_pk_value_reduce_time = personal.pk_value_reduce_time();
	m_pk_value = personal.pk_value();
	m_pk_count = personal.pk_count();
	m_target_count = personal.target_count();
	m_errantry = personal.errantry();
    m_pk_punishment_time = personal.pk_punishment();
    m_city_id = personal.city_id();
	m_main_hero_uid = string_util_t::string_to_uint64(personal.hero_uid());
	m_equip_smelt_value = personal.equip_smelt_value();
	m_achieve_level = personal.achieve_level();
	m_achieve_recv_reward_level = personal.achieve_recv_reward_level();
	for (int32_t i = 0; i < personal.equip_smelt_box_size(); ++i)
	{
		m_equip_smelt_box_vec.push_back(personal.equip_smelt_box(i));
	}
	m_money->load_data(personal);
	m_formation_mgr.set_cur_formation_id(personal.cur_form_id());

	m_lifestar_level = personal.lifestar_level();
	m_lifestate_level = personal.lifestate_level();
	m_main_hero_plugin = personal.plugin();
	m_patrol_count = personal.patrol_count();
	m_equip_lucky = personal.equip_lucky();

	m_vigour = personal.vigour();
	m_vigour_buy = personal.vigour_buy();
	m_vigour_time = personal.vigour_time();

	if (m_vigour_time == 0 && m_vigour < get_vigour_base_limit()) {
		m_vigour_time = common::time_util_t::now_time();
	}

	m_head_frame = personal.head_frame();
	m_business_act_mgr.get_activity_shop().load_data(personal);
	for (auto career_id : personal.career_id())
	{
		m_career_id_vec.push_back(career_id);
	}
	m_change_name = personal.change_name();
	for (auto title_id : personal.title_id())
	{
		m_title_id_vec.push_back(title_id);
	}
	m_cur_title = personal.cur_title();
	for (auto handbook : personal.handbook())
	{
		m_handbook_vec.push_back(handbook);
	}
}

// get set function end

// 改变玩家状态
bool role_t::change_pk_mode(proto::common::EM_PK_MODE mode, uint32_t ex /*= 0*/)
{
	if (mode != m_pk_mode) {
		m_pk_mode = mode;
		m_pk_ex = ex;

		// 切换成和平模式清理临时称号
		if (mode == proto::common::PK_PEACE && m_show_temporary_title) {
			m_show_temporary_title = false;
			m_temporary_title = 0;
		}

		notify_object_common_info_for_nearbies();
	}

	return true;
}

void role_t::notify_other_server_ready_enter_scene()
{
    //to center
    proto::server::ge_ready_enter_scene_notify ntf_center;
    env::server->send_msg_to_center(op_cmd::ge_ready_enter_scene_notify, get_uid(), ntf_center);
}

void role_t::notify_other_server_role_login_scene()
{
	//to chat:场景登陆
	proto::server::gh_syn_online_role_notify ntf_chat;
	proto::common::chat_syn_user_info* p_sys_infos = ntf_chat.mutable_chat_info();
	p_sys_infos->set_gate_id(get_gate_id());
	//p_sys_infos->set_uid(m_uid);
	p_sys_infos->set_country_id(get_country_id());
	p_sys_infos->set_family_id(get_family_id());
	p_sys_infos->set_nickname(get_name());
	//p_sys_infos->set_channel_id(get_chat_channel());
	env::server->send_msg_to_chat(op_cmd::gh_syn_online_role_notify, m_uid, ntf_chat);
}

void role_t::notify_the_other_servers_del_role()
{
	// game --> chat: 让chat彻底删除role
	proto::server::gh_kick_role_notify gh_kick;
	env::server->send_msg_to_chat(op_cmd::gh_kick_role_notify, get_uid(), gh_kick);
}

void role_t::notify_new_guide()
{
	for (auto& guide : m_new_role_guide_list)
	{
		///NEW_GUIDE_LOG("role[%lu] guide[%d] status[%d] type[%d]",m_uid, guide.second.tid, guide.second.status, guide.second.event_type);
		if (NEW_ROLE_GUIDE_STATUS_READY_DONE != guide.second.status)
			continue;

		proto::client::gc_touch_new_role_guide_notify msg;
		msg.set_tid(guide.first);
		msg.set_type(guide.second.event_type);
		send_msg_to_client(op_cmd::gc_touch_new_role_guide_notify, msg);
		NEW_GUIDE_LOG("role[%lu] guide[%d] status[%d] type[%d]", m_uid,guide.second.tid, guide.second.status, guide.second.event_type);
		break;
	}
}
void role_t::calc_fighting()
{
	uint32_t new_fighting = m_formation_mgr.calc_fighting();
	//log_warn("new_fighting[%d]", new_fighting);
	uint32_t label_fight = lifeforce_attr_manager_t::add_fighting(get_uid());
	new_fighting += label_fight;

	if (m_fighting != new_fighting) {
		m_fighting = new_fighting;

		if (m_is_in_scene) { // 不在场景中不发送给客户端
			proto::client::gc_update_fighting_notify ntf;
			ntf.set_fighting(m_fighting);
			send_msg_to_client(op_cmd::gc_update_fighting_notify, ntf);

			// 同步给center
			center_msg_handler_t::sync_data_to_center(m_uid, proto::server::data_sync_type_fighting_value, m_fighting, true);
		}
	}
}

bool role_t::add_exp(uint32_t add_val, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data)
{
    if (0 == add_val)
    {
        ROLE_LOG("role[%lu] add exp[%d] error!", m_uid, add_val);
        return false;
    }
	//经验增益BUFF
	if (m_buff_addexp > 0)
	{
		add_val += add_val * m_buff_addexp / 10000;
	}
	
    // 获取下一级所需经验
    uint32_t next_level = m_level + 1;
    LevelupTable *p_config = GET_CONF(LevelupTable, next_level);
    if (NULL == p_config)
    {
        log_error("level_up config not find level[%d]", next_level);
        return false;
    }
	uint32_t old_level = m_level;
	uint32_t old_exp = m_exp;

    uint32_t new_exp = m_exp + add_val;
    uint32_t level_up_need_exp = p_config->level_up_exp();
    bool level_up = false;

	if( is_limit_level() == true )
	{
		if (get_limit_exp() > new_exp)
		{//卡级后的经验
			set_exp(new_exp, source_type, source_param, p_data);
			sys_notice_manager_t::broadcast_system_msg_for_money(m_uid, proto::common::MONEY_TYPE_EXP, add_val);
		}
		else
		{
			uint32_t money_value = add_val;
			if (get_limit_exp() > old_exp && get_limit_exp() < new_exp ) 
			{
				uint32_t exp_value = get_limit_exp();
				money_value = new_exp - get_limit_exp();
				set_exp(exp_value, source_type, source_param, p_data);
				sys_notice_manager_t::broadcast_system_msg_for_money(m_uid, proto::common::MONEY_TYPE_EXP, exp_value);
			}

			uint32_t value = general_info_manager_t::get_add_feats(m_level, money_value);
			money_manager_t::add_money(get_role(), proto::common::MONEY_TYPE_FEATS, value, log_enum::source_type_role_add_exp_limit, money_value, true, p_data);
			//sys_notice_manager_t::broadcast_system_msg_for_money(m_uid, proto::common::MONEY_TYPE_EXP, exp_value);
		}
		log_wrapper_t::send_role_exp_log(m_uid, old_level, old_exp, add_val, m_level, m_exp, source_type, source_param);
		return true;
	}

    if (new_exp >= level_up_need_exp)
    {
		new_exp -= level_up_need_exp;
		level_up = true;
    }
    
    if (level_up)
    {
		uint32_t new_level = m_level + 1;
		while (true)
		{
			LevelupTable *p_config = GET_CONF(LevelupTable, new_level + 1);
			if (NULL == p_config)
			{
				log_error("level_up config not find level[%d]", new_level + 1);
				break;
			}
			if (new_exp < p_config->level_up_exp())
			{
				break;
			}

			++new_level;

			new_exp -= p_config->level_up_exp();
		}
        set_level(new_level, source_type, source_param, p_data);

		//升级后,检测发送等级奖励邮件
		check_send_level_reward_mail();

		//升级check功能开放
		m_business_act_mgr.check_all_yyhd();
    }
	set_exp(new_exp, source_type, source_param, p_data);
	log_wrapper_t::send_role_exp_log(m_uid, old_level, old_exp, add_val, m_level, m_exp, source_type, source_param);
	sys_notice_manager_t::broadcast_system_msg_for_money(m_uid, proto::common::MONEY_TYPE_EXP, add_val);

	return true;
}

void role_t::set_exp(uint32_t set_val, uint32_t source_typ, uint32_t source_param, proto::common::role_change_data* p_data)
{
    if (m_exp == set_val)
    {
        return;
    }

    uint32_t old_exp = m_exp;
    uint32_t new_exp = set_val;
    uint32_t up_exp = abs(new_exp - old_exp);
    m_exp = new_exp;
    save_self();
    if (NULL != p_data)
    {
        p_data->set_exp(new_exp);
    }
    ROLE_LOG("role[%lu] set exp: old[%d] up[%d] new[%d]", m_uid, old_exp, up_exp, new_exp);
}

void role_t::set_level(uint32_t set_val, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data)
{
    if (m_level == set_val)
    {
        return;
    }

	uint32_t old_level = m_level;
    uint32_t new_level = set_val;
    uint32_t up_level = abs(new_level - old_level);
    m_level = new_level;
    save_self(true);
    if (NULL != p_data)
    {
        p_data->set_level(m_level);
    }
    ROLE_LOG("role[%lu] set level: old[%d] up[%d] new[%d]", m_uid, old_level, up_level, new_level);

    // 更新任务
	if (m_task_mgr != NULL)
	{
		if (p_data) {
			m_task_mgr->try_change_task_state(TASK_TARGET_TYPE_LEVEL, *p_data, 1, m_level, false);	
		} else {
			proto::common::role_change_data rcd;
			m_task_mgr->try_change_task_state(TASK_TARGET_TYPE_LEVEL, rcd, 1, m_level, false);	
		}
	}

    // 完成升级等级成就
    if (m_level > old_level)
    {
        achieve_manager_t::notify_achieve_state(get_uid(), proto::common::ACHIEVE_MAIN_ROLE_LEVEL, 0, m_level );
        red_point_manager_t::check_activity_join(get_uid());
        script_event_level_up(m_level);
		update_role_info_to_center(proto::common::simple_info_type);
		page_manager_t::check_page_elite(get_uid());
		m_general_event.on_level_up();
		if (NULL != m_office)
		{
			m_office->check_levelup();
		}
    }

    // 同步等级给center
    center_msg_handler_t::sync_data_to_center(m_uid, proto::server::data_sync_type_level, m_level, true);
}

//////////////////////////////////////////////////////////////////////////
void role_t::move_to(float x, float y, uint64_t time_stamp, float direction, uint32_t high/* = 0*/)
{
	const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
	if (NULL == p_scene)
	{
		log_error("role[%lu] nullptr == pScene[%lu]", m_uid, m_scene_id);
		return;
	}

	// 有队伍不是暂离不能移动
	if (is_troop_member_not_leave())
	{
		log_warn("following member[%lu] cannot move", m_uid);
		return;
	}

	if (p_scene->on_move_s(get_object_ptr(), x, y, direction, time_stamp, high))
	{
		// 是队长让队员同步移动
		if (is_team_leader())
		{
			troop_ptr p_troop = game_troop_mgr_t::get_troop(m_troop_id);
			if (p_troop != NULL)
			{
				p_troop->leader_move(p_scene, m_real_speed, x, y, direction, high);
			}
		}
	}
}


void role_t::on_enter_scene(scene_ptr p_scene)
{
	if (NULL == p_scene)
	{
		log_error("p_scene null error");
		return;
	}

	object_base_t::on_enter_scene(p_scene);

	switch (m_login_type)
	{
	case proto::server::scene_change_none:	// 同场景跳转处理
		{
			set_object_state(proto::common::object_state_normal);

			// 同步场景信息给center
			proto::server::ge_role_scene_info_notify notify;
			notify.set_scene_id(m_scene_id);
			notify.set_map_tid(m_map_tid);
			env::server->send_msg_to_center(op_cmd::ge_role_scene_info_notify, m_uid, notify);

			save_self();
		}
		break;
	case proto::server::login_server_scene:		// 登陆处理
	case proto::server::relogin_server_scene:	// 重新登陆场景的处理
		// 不处理状态
		break;
	default:	// 其他就是跨服跳场景
		{
			set_object_state(proto::common::object_state_normal);
		}
		break;
	}

	if (m_login_type == proto::server::relogin_server_scene) { // 重新登陆通知区域
		m_role_scene.role_reenter_scene(get_role(), p_scene);
	} else { // 刷新区域
		m_role_scene.role_enter_scene(get_role(), p_scene);
	}

	m_city_id = p_scene->get_city_id();

	// 队长的话重置跟随点
	if (m_troop_id != 0 && is_team_leader())
	{
		troop_ptr p_troop = game_troop_mgr_t::get_troop(m_troop_id);
		if (NULL != p_troop)
		{
			p_troop->reset_move(m_current_x, m_current_y, m_direction);
		}
	}
}

void role_t::on_leave_scene(scene_ptr p_scene)
{
	if (NULL == p_scene)
	{
		log_error("p_scene null error");
		return;
	}

	object_base_t::on_leave_scene(p_scene);

	m_role_scene.role_leave_scene();

	// 记录最后静态场景id
	if (p_scene->get_map_type() == proto::server::map_type_static)
	{
		m_role_scene.set_last_scene_info(m_map_tid, m_current_x, m_current_y, m_direction);
	}

	// 打断读条
	if (m_transmission_type != proto::common::transmission_type_none) {
		on_break_transmission(false);
	}
}

// void role_t::set_fight_data(const proto::common::fight_data& data) wsy 注释 旧的战斗
// {
// 	m_fight_data.CopyFrom(data);
// }
// 
// proto::common::fight_data& role_t::get_fight_data() wsy 注释 旧的战斗
// {
// 	return m_fight_data;
// }

void role_t::on_next_day(uint32_t curr_time, bool pass_week)
{
	do_next_day(curr_time, true);

	if (pass_week) do_next_week(curr_time);
}

void role_t::do_next_day(uint32_t curr_time, bool is_online)
{
	// 累计登陆天数
	m_cumulative_login_days += 1;
	achieve_common_t::notify_progress_state(m_uid, proto::common::ACHIEVE_TOTAL_LOGIN, 1, 1);
	ROLE_LOG("role[%lu] cumulative login days: %d", m_uid, m_cumulative_login_days);

	// 连续登录天数
	if (0 == m_update_day_time || curr_time - m_update_day_time <= DAY_SECOND) {
		m_continue_login_days += 1;
		achieve_common_t::notify_progress_state(m_uid, proto::common::ACHIEVE_SUCCESSION_LOGIN, 1, m_continue_login_days);
	} else {
		m_continue_login_days = 0;
	}
	ROLE_LOG("role[%lu] continue login days: %d", m_uid, m_continue_login_days);

	m_trade.do_next_day(is_online);
	
	if (NULL != m_money) m_money->set_prestige_today(0);

	m_pk_count = pk_manager_t::get_pk_count();

	m_target_count = pk_manager_t::get_target_count();

	if (NULL != m_task_mgr) m_task_mgr->one_day();
	
	if (NULL != m_bounty_mgr) m_bounty_mgr->one_day();
	
	if (NULL != m_tower) m_tower->one_day();

	if (NULL != m_recharge) m_recharge->one_day();
	
	if (NULL != m_office) m_office->one_day();

	if (NULL != m_expedition) m_expedition->one_day();
	
	if (NULL != m_activity_mgr) m_activity_mgr->one_day();

	if (NULL != m_patrol_hire) m_patrol_hire->one_day();

	if (NULL != m_luckydraw) m_luckydraw->one_day();
	
	hero_manager_t::do_next_day(get_role());
	arena_manager_t::do_next_day(get_role());

	active_reward_manager_t::init_task_list(m_uid);				///每日活跃奖励任务列表初始化

	// 清理精力购买次数
	clear_vigour_buy_times(is_online);

	m_update_day_time = curr_time;

	m_business_act_mgr.oneday();

	m_quick_elite_page_map.clear();
	m_reset_quick_elite_page_map.clear();
	m_role_dungeon_data_t.one_day();
	m_general_event.one_day();

	// 跨天巡逻场次清0
	m_patrol_count = 0;

	save_self();

	if (is_online)
	{
		proto::client::gc_next_day_notify ntf;
		ntf.set_trade_point(get_trade_info().get_trade_point());
		send_msg_to_client(op_cmd::gc_next_day_notify, ntf);
	}
}

void role_t::do_next_week(uint32_t curr_time)
{
	if (NULL != m_task_mgr) m_task_mgr->one_week();
	
	m_equip_smelt_value = 0;
	m_equip_smelt_box_vec.clear();

	if (NULL != m_activity_mgr) m_activity_mgr->one_week();

	m_update_week_time = curr_time;

	save_self();
}

void role_t::join_family(uint64_t family_id, const std::string& family_name, uint32_t officer, uint32_t country_id, uint32_t family_lv)
{
	m_family_id = family_id;
	m_family_name = family_name;
	m_family_officer = officer;
	m_country_id = country_id;
	m_country_officer = 0;
	set_family_level(family_lv);

	if (NULL != m_task_mgr)
	{
		proto::common::role_change_data rcd;
		m_task_mgr->try_change_task_state(TASK_TARGET_TYPE_JOIN_FAMILY, rcd, 1, 1);
	}

	// 家族已经加入了国家
	if (m_country_id != 0) on_join_country();
	
	notify_object_common_info_for_nearbies();
	update_role_info_to_center(proto::common::simple_info_type);

	if (NULL != m_family_skill_mgr)
	{
		proto::client::gc_role_change_data_notify ntf;
		hero_manager_t::all_heros_update_attr(get_role(), ntf.mutable_change_data());
		send_msg_to_client(op_cmd::gc_role_change_data_notify, ntf);
	}
}

void role_t::leave_family()
{
	m_family_id = 0;
	m_family_name.clear();
	m_family_officer = 0;
	m_country_id = 0;
	m_country_officer = 0;

	if (NULL != m_expedition)
	{
		m_expedition->clear_glory();
	}

	notify_object_common_info_for_nearbies();
	update_role_info_to_center(proto::common::simple_info_type);

	if (NULL != m_family_skill_mgr)
	{
		proto::client::gc_role_change_data_notify ntf;
		hero_manager_t::all_heros_update_attr(get_role(), ntf.mutable_change_data());
		send_msg_to_client(op_cmd::gc_role_change_data_notify, ntf);
	}
}

void role_t::set_country_id(uint32_t country_id)
{
	m_country_id = country_id;
	m_country_officer = 0;

	if (m_country_id != 0) on_join_country();

	notify_object_common_info_for_nearbies();
}

void role_t::on_join_country()
{
	if (NULL != m_task_mgr)
	{
		proto::common::role_change_data rcd;
		m_task_mgr->try_change_task_state(TASK_TARGET_TYPE_JOIN_COUNTRY, rcd, 1, 1);
	}
}

void role_t::set_country_officer(uint32_t val)
{
	m_country_officer = val;

	notify_object_common_info_for_nearbies();
}

void role_t::set_fight_uid(uint64_t uid)
{
	m_role_fight_data.set_fight_uid(uid);
	// cross服不处理
	if (env::server->get_type() == 1)
		return;

	if (uid != 0)
	{
		change_object_state(proto::common::object_state_fighting);
	}
	else
	{
		m_fight_end_time = common::time_util_t::now_time();
	}

	save_self();
}

uint32_t role_t::set_watching_fight_uid(uint64_t uid, bool need_delete)
{
	uint64_t old_watching_fight_uid = get_watching_fight_uid();
	m_role_fight_data.set_watching_fight_uid(uid);
	// cross服不处理
	if (env::server->get_type() == 1)
		return common::errcode_enum::error_ok;

	if (uid != 0)
	{
		return change_object_state(proto::common::object_state_watch_fighting);
	}
	else
	{
		if (old_watching_fight_uid > 0)
		{
			combat_ptr p_combat = fight_manager_t::find_combat(old_watching_fight_uid);
			// 战斗还在
			if (NULL != p_combat && need_delete)
			{
                p_combat->remove_watching_role(get_uid());
			}
		}
		return change_object_state(proto::common::object_state_normal);
	}
	return common::errcode_enum::error_ok;
}

/**
 * \param self:是不是自己检测
 */
bool role_t::can_enter_fight(bool self /*= true*/)
{
	if (is_die())
	{
		//log_warn("role[%lu] is die", m_uid);
		return false;
	}

	if (get_fight_uid() != 0)
	{
		if (self) {
			if (fight_manager_t::check_and_close_pre_combat(get_role()) == false) { // 尝试关闭上一场战斗
				return false;
			}
		} else {
			return false;
		}
	}

	// 匹配状态无法进入战斗 (这么做的目的是防止玩家在场景中匹配时被拉进战斗 但无需担心竞技场战斗是否会无法进入 cross没有玩家状态)
	if (get_object_state() == proto::common::object_state_arena_match)
	{
		log_error("can_enter_fight role[%lu] object_state_arena_match can't enter fight", get_uid());
		return false;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::FIGHT_FINISH_COUNT_DOWN);
	uint32_t fight_finish_count_down = GET_COMPREHENSIVE_VALUE_1(conf);
	uint32_t now_time = common::time_util_t::now_time();

	// 战斗结束还没退出结算界面，都是战斗状态，但是超过一段时间后还是能进行战斗
	if (get_object_state() == proto::common::object_state_fighting)
	{
		if (now_time - m_fight_end_time >= fight_finish_count_down)
		{
			change_object_state(proto::common::object_state_normal);
		}
	}
	
    if (get_object_state() == proto::common::object_state_fighting)
	{
		//log_warn("role[%lu] in fight_state", m_uid);
        return false;
    }

	return true;
}


void role_t::syn_cross_id(uint32_t cross_id)
{
	proto::server::ga_syn_cross_id_notify ntf;
	ntf.set_user_id(get_uid());
	ntf.set_cross_id(cross_id);

	send_msg_to_gate(op_cmd::ga_syn_cross_id_notify, ntf);
}


uint32_t role_t::change_object_state(proto::common::object_state_type new_state, uint64_t state_param/* = 0*/)
{
	// 死亡不能改成任何状态，只能复活
	if (is_die()) {
		ROLE_LOG("role[%lu] is die, can not change state to [%u]", m_uid, new_state);
		return common::errcode_enum::sys_notice_you_dead;
	}

	if (new_state == m_object_state) {
		return common::errcode_enum::notice_change_state_error;
	}

	switch (new_state) {
		case proto::common::object_state_normal:
		case proto::common::object_state_fighting:
			break;
		case proto::common::object_state_watch_fighting: {
			if (m_object_state == proto::common::object_state_fighting) {
				log_error("role[%lu] is fight state, can not change state to [%u]", m_uid, new_state);
				return common::errcode_enum::notice_change_state_error;
			}
			break;
		}
		case proto::common::object_state_busy:
		case proto::common::object_state_attack:
		case proto::common::object_state_arena_match:
		case proto::common::object_state_transmission: {
			if (m_object_state == proto::common::object_state_fighting || m_object_state == proto::common::object_state_watch_fighting) {
				log_error("role[%lu] is fight state, can not change state to [%u]", m_uid, new_state);
				return common::errcode_enum::notice_change_state_error;
			}
			break;
		}
		default: {
			log_error("role[%lu] new_state[%u] not define", m_uid, new_state);
			return common::errcode_enum::notice_change_state_error;
		}
	}
	// 状态离开
	on_leave_object_state(m_object_state);
	// 设置新状态
	set_object_state(new_state);
	m_obj_state_param = state_param;

	// 广播消息
	proto::client::gc_change_sys_state_notify msg;
	msg.set_role_uid(string_util_t::uint64_to_string(m_uid));
	msg.set_reply_code(errcode_enum::error_ok);
	proto::common::object_state_info* object_state = msg.mutable_object_state();
	if (object_state != NULL) {
		object_state->set_obj_state(m_object_state);
		object_state->set_obj_state_param(common::string_util_t::uint64_to_string(m_obj_state_param));
	}
	send_msg_to_nine(op_cmd::gc_change_sys_state_notify, msg);

	return common::errcode_enum::error_ok;
}

void role_t::on_leave_object_state(proto::common::object_state_type old_state)
{
	switch (old_state) {
		case proto::common::object_state_transmission:
			if (m_transmission_type != proto::common::transmission_type_none) { // 还处于传送状态就打断，并通知
				on_break_transmission();
			}
			break;
		default:
			break;
	}
}

uint32_t role_t::toggle_ride_mount(proto::common::role_mode_state new_state)
{
	if (is_die()) { // 死亡
		ROLE_LOG("role[%lu] is die now can not change mode state", m_uid);
		return common::errcode_enum::sys_notice_you_dead;
	}

	if (new_state == proto::common::role_mode_state_trade) { // 跑商状态服务器更改
		ROLE_LOG("role[%lu] client can't set state == trade", m_uid);
		return common::errcode_enum::notice_unknown;
	}

	if (is_troop_member_not_leave()) { // 未暂离队员不能上下马
		ROLE_LOG("role[%lu] troop member can't ride", m_uid);
		return common::errcode_enum::notice_unknown;
	}

	if (new_state == m_mode_state) { // 算设置成功
		ROLE_LOG("role[%lu] is already state[%u]", m_uid, new_state);
		return common::errcode_enum::error_ok;
	}

	if (get_trade_info().get_trade_bag_weight() > 0) { // 跑商不能上下马
		ROLE_LOG("role[%lu] is trade can't set state[%u]", m_uid, new_state);
		return common::errcode_enum::notice_unknown;
	}

	switch (new_state) {
		case proto::common::role_mode_state_normal:
			break;
		case proto::common::role_mode_state_mount: {
			if (get_mount_data() == NULL || get_mount_data()->get_use_mount() == NULL) {
				log_error("role[%lu] mount data error", m_uid);
				return common::errcode_enum::notice_unknown;
			}
			break;
		}
		default: {
			log_error("role[%lu] new_state[%u] not define", m_uid, new_state);
			return common::errcode_enum::notice_unknown;
		}
	}

	uint32_t real_speed = 0;
	m_mode_state = new_state;

	if (m_troop_id != 0 && is_team_leader()) {
		const troop_ptr& p_troop = game_troop_mgr_t::get_troop(m_troop_id);
		if (p_troop != NULL) { // 找到队伍，更新
			real_speed = get_speed_by_type(p_troop->is_member_trade() ? proto::common::role_mode_state_trade : m_mode_state);
			p_troop->update_mount_state(m_mode_state, real_speed);
		}
	}

	if (real_speed == 0) {
		real_speed = get_speed_by_type(m_mode_state);
	}

	set_real_speed(real_speed);
	notify_object_common_info_for_nearbies();

	save_self(); // 保存坐骑状态

	ROLE_LOG("role[%lu] change mode_state[%u] speed[%d]", m_uid, m_mode_state, real_speed);

	return common::errcode_enum::error_ok;
}

uint32_t role_t::toggle_trade_state(proto::common::role_mode_state new_state)
{
	if (new_state == proto::common::role_mode_state_mount) { // 坐骑状态这里不能设置
		log_error("role[%lu] toggle_trade can't set state mount", m_uid);
		return common::errcode_enum::notice_unknown;
	}

	if (new_state == m_mode_state) { // 算设置成功
		ROLE_LOG("role[%lu] is already state[%u]", m_uid, new_state);
		return common::errcode_enum::error_ok;
	}

	switch (new_state) {
		case proto::common::role_mode_state_normal: {
			if (get_trade_info().get_trade_bag_weight() > 0) {
				log_error("role[%lu] trade bag_weight > 0", m_uid);
				return common::errcode_enum::notice_unknown;
			}
			break;
		}
		case proto::common::role_mode_state_trade: {
			if (get_trade_info().get_trade_bag_weight() == 0) {
				log_error("role[%lu] trade bag_weight == 0", m_uid);
				return common::errcode_enum::notice_unknown;
			}
			break;
		}
		default: {
			log_error("role[%lu] new_state[%u] not define", m_uid, new_state);
			return common::errcode_enum::notice_unknown;
		}
	}

	uint32_t real_speed = 0;
	m_mode_state = new_state;

	if (m_troop_id != 0 && m_troop_state == proto::common::troop_state_normal) { // 为暂离影响全队速度
		const troop_ptr& p_troop = game_troop_mgr_t::get_troop(m_troop_id);
		if (p_troop != NULL && p_troop->update_trade_state(get_role(), m_mode_state)) { // 找到队伍，更新
			real_speed = p_troop->get_troop_speed();
		}
	}

	if (real_speed == 0) {
		real_speed = get_speed_by_type(m_mode_state);
	}

	set_real_speed(real_speed);
	notify_object_common_info_for_nearbies();

	save_self(); // 保存坐骑状态

	ROLE_LOG("role[%lu] change mode_state[%u] speed[%d]", m_uid, m_mode_state, real_speed);

	return common::errcode_enum::error_ok;
}

void role_t::move_to_prison()
{
	//传送
	//uint32_t prison_id = pk_manager_t::get_prison_id();
	uint32_t prison_point = pk_manager_t::get_prison_point();
	scene_manager_t::role_jump_point(get_role(), prison_point);

	uint32_t reduce_pk_value = pk_manager_t::get_pk_value_reduce();
	uint32_t count = m_pk_value / reduce_pk_value;
	if (m_pk_value % reduce_pk_value > 0)
	{
		count += 1;
	}
	m_pk_punishment_time = count * pk_manager_t::get_pk_value_reduce_time();
	
	m_pk_value = 0;
	if (NULL == m_pk_punish_timer)
	{
		m_pk_punish_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	else
	{
		m_pk_punish_timer->cancel();
	}
	m_pk_punish_timer->expires_from_now(boost::posix_time::seconds(m_pk_punishment_time));
	m_pk_punish_timer->async_wait(boost::bind(&role_t::on_prison_timeout, boost::weak_ptr<role_t>(get_role()), boost::asio::placeholders::error));
	
	proto::client::gc_role_change_data_notify ntf;
	get_personal_info(ntf.mutable_change_data()->mutable_per_info());
	send_msg_to_client(op_cmd::gc_role_change_data_notify, ntf);
}

void role_t::add_pk_value(uint32_t val, uint32_t source_type, uint32_t source_param)
{
	uint32_t old_pk_value = m_pk_value;
	m_pk_value += val;
	uint32_t max_pk_value = pk_manager_t::get_max_pk_value();
	if (m_pk_value >= max_pk_value)
	{
		m_pk_value = max_pk_value;
	}
	if (old_pk_value == 0 && m_pk_value > 0)
	{
		m_pk_value_reduce_time = pk_manager_t::get_pk_value_reduce_time();
		if (NULL == m_pk_reduce_timer)
		{
			m_pk_reduce_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
		}
		m_pk_reduce_timer->expires_from_now(boost::posix_time::seconds(m_pk_value_reduce_time));
		m_pk_reduce_timer->async_wait(boost::bind(&role_t::on_reduce_pk_timer, boost::weak_ptr<role_t>(get_role()), boost::asio::placeholders::error));
	}

	rank_manager_t::update_criminal(m_uid, m_pk_value);
	
	log_wrapper_t::send_pk_log(get_uid(), get_level(), old_pk_value, val, m_pk_value, 1, log_enum::opt_type_add, source_type, source_param);
	notify_object_common_info_for_nearbies();

	//成就通知
	achieve_manager_t::notify_achieve_state(get_uid(), proto::common::ACHIEVE_PK_VALUE,0, val);
}

void role_t::del_pk_value(uint32_t val, uint32_t source_type, uint32_t source_param)
{
	uint32_t old_pk_value = m_pk_value;
	if (m_pk_value > val)
	{
		m_pk_value -= val;
	}
	else
	{
		m_pk_value = 0;
	}
	if (old_pk_value > 0 && m_pk_value == 0)
	{
		m_pk_value_reduce_time = 0;
		if (NULL != m_pk_reduce_timer)
		{
			m_pk_reduce_timer->cancel();
		}
	}

	rank_manager_t::update_criminal(m_uid, m_pk_value);
	
	log_wrapper_t::send_pk_log(get_uid(), get_level(), old_pk_value, val, m_pk_value, 1, log_enum::opt_type_del, source_type, source_param);
	notify_object_common_info_for_nearbies();
}

void role_t::on_reduce_pk_value_timer()
{
	m_pk_value_reduce_time = pk_manager_t::get_pk_value_reduce_time();
	if (NULL == m_pk_reduce_timer)
	{
		m_pk_reduce_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_pk_reduce_timer->expires_from_now(boost::posix_time::seconds(m_pk_value_reduce_time));
	m_pk_reduce_timer->async_wait(boost::bind(&role_t::on_reduce_pk_timer, boost::weak_ptr<role_t>(get_role()), boost::asio::placeholders::error));
	uint32_t reduce_pk_value = pk_manager_t::get_pk_value_reduce();
	del_pk_value(reduce_pk_value, log_enum::source_type_role_auto_reduce_pk_value, 0);
	notify_object_common_info_for_nearbies();
}

void role_t::on_reduce_pk_timer(const boost::weak_ptr<role_t>& p_wrole, const boost::system::error_code& ec)
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

	role_ptr p_role(p_wrole.lock());
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	p_role->on_reduce_pk_value_timer();
}

void role_t::on_prison_timeout(const boost::weak_ptr<role_t>& p_wrole, const boost::system::error_code& ec)
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

	role_ptr p_role(p_wrole.lock());
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	p_role->on_prison_timer();
}

void role_t::set_in_family_war(bool val)
{
	m_is_in_family_war = val;

	if (!m_is_in_family_war)
	{
		m_family_war_hero.clear();

		formation_ptr p_form = m_formation_mgr.get_formation(proto::common::formation_type_family);
		if (NULL != p_form)
		{
			p_form->reset_heroes();
		}
	}
}

void role_t::set_family_war_heros(std::set<uint64_t>& heros)
{
	m_family_war_hero = heros;
}

void role_t::add_family_war_hero(uint64_t hero_uid)
{
	m_family_war_hero.insert(hero_uid);
}

void role_t::del_family_war_hero(uint64_t hero_uid)
{
	m_family_war_hero.erase(hero_uid);

	formation_ptr p_form = m_formation_mgr.get_formation(proto::common::formation_type_family);
	if (NULL == p_form)
	{
		log_error("user[%lu] p_from [%u] is NULL", get_uid(), proto::common::formation_type_family);
		return;
	}

	p_form->remove(hero_uid);
	proto::client::gc_update_formation_reply reply;
	p_form->peek_data(reply.mutable_formation());
	send_msg_to_client(op_cmd::gc_update_formation_reply, reply);
}

bool role_t::is_hero_active(uint64_t hero_uid)
{
	if (hero_uid == get_main_hero_uid())
	{
		return true;
	}

	std::set<uint64_t>::const_iterator citr = m_family_war_hero.find(hero_uid);
	if (citr != m_family_war_hero.end())
	{
		return true;
	}

	return false;
}

void role_t::on_prison_timer()
{
	m_pk_punishment_time = 0;
	if (NULL != m_pk_punish_timer)
	{
		m_pk_punish_timer->cancel();
	}

	proto::client::gc_role_change_data_notify ntf;
	get_personal_info(ntf.mutable_change_data()->mutable_per_info());
	send_msg_to_client(op_cmd::gc_role_change_data_notify, ntf);

	uint32_t prison_point = pk_manager_t::get_prison_out_point();
	//传送
	log_warn("role[%lu] move_out_prison[%d]", m_uid, prison_point);
	scene_manager_t::role_jump_point(get_role(), prison_point);
}

void role_t::add_errantry(uint32_t val, uint32_t source_type, uint32_t source_param)
{
	uint32_t old_errantry = m_errantry;
	m_errantry += val;
	log_wrapper_t::send_pk_log(get_uid(), get_level(), old_errantry, val, m_errantry, 2, log_enum::opt_type_add, source_type, source_param);
}

bool role_t::is_in_king_city()
{
	auto king_city = GET_CONF(Comprehensive, comprehensive_common::imperial_city_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(king_city);

	return m_map_tid == val;
}

void role_t::on_object_die()
{
	// 基类调用设置状态
	object_base_t::on_object_die();

	m_die_time = common::time_util_t::now_time();

	m_revive_time = 0;

	// 死亡9屏幕通知
	notify_object_common_info_for_nearbies();
}

void role_t::on_object_revive()
{
	object_base_t::on_object_revive();

	m_die_time = 0;

	m_revive_time = 0;

	// 复活通知9屏幕
	notify_object_common_info_for_nearbies();

	// 先复活, 再拉人，
	const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
	if (NULL != p_scene)
	{
		p_scene->on_revive_role(get_role());
	}
}

uint32_t role_t::start_transmission(proto::common::transmission_type type /*= proto::common::transmission_type_user*/)
{
	// 已经在传送状态了，直接更新时间
	if (get_object_state() == proto::common::object_state_transmission) {
		m_obj_state_param = common::time_util_t::now_time();
	} else {
		uint32_t back_code = change_object_state(proto::common::object_state_transmission, common::time_util_t::now_time());
		if (back_code != common::errcode_enum::error_ok) {
			ROLE_LOG("role[%lu] set transmission state error", m_uid);
			return back_code;
		}
	}

	uint32_t reading_time = scene_manager_t::get_pk_jump_time(get_pk_mode());
	if (reading_time == 0) {
		log_error("role[%lu] pk_mode[%u] transmission reading_time == 0", m_uid, get_pk_mode());
		return common::errcode_enum::notice_unknown;
	}

	if (NULL == m_transmission_timer) {
		m_transmission_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}

	if (NULL == m_transmission_timer) {
		log_error("m_transmission_timer null error");
		return common::errcode_enum::notice_unknown;
	}

	m_transmission_timer->expires_from_now(boost::posix_time::seconds(reading_time + 2)); // 加一个延迟时间
	m_transmission_timer->async_wait(boost::bind(&role_t::transmission_callback, boost::weak_ptr<role_t>(get_role()), boost::asio::placeholders::error));

	m_transmission_type = type;

	ROLE_LOG("role[%lu] start transmission, type:%u", m_uid, type);
	return common::errcode_enum::error_ok;
}

void role_t::cancel_transmission()
{
	// 主动打断，先设置传送状态
	if (m_transmission_type != proto::common::transmission_type_none) {
		m_transmission_type = proto::common::transmission_type_none;

		if (NULL != m_transmission_timer) {
			m_transmission_timer->cancel();
			m_transmission_timer.reset();
		}
	}

	// 人物状态修改
	if (get_object_state() == proto::common::object_state_transmission)
		change_object_state(proto::common::object_state_normal);
}

void role_t::do_break_transmission()
{
	// 打断传送状态
	if (get_object_state() == proto::common::object_state_transmission) {
		change_object_state(proto::common::object_state_normal);
	}
}

void role_t::on_break_transmission(bool is_notify /*= true*/)
{
	m_transmission_type = proto::common::transmission_type_none;

	if (NULL != m_transmission_timer) {
		m_transmission_timer->cancel();
		m_transmission_timer.reset();
	}

	if (is_notify) {
		proto::client::gc_jump_reading_break_notify ntf;
		send_msg_to_client(op_cmd::gc_jump_reading_break_notify, ntf);
	}
}

void role_t::on_transmission_timer()
{
	if (m_transmission_type != proto::common::transmission_type_none) {
		proto::client::gc_jump_reading_break_notify ntf;
		send_msg_to_client(op_cmd::gc_jump_reading_break_notify, ntf);

		m_transmission_type = proto::common::transmission_type_none;
	}

	// 传送定时器结束
	if (NULL != m_transmission_timer) {
		// m_transmission_timer->cancel();
		m_transmission_timer.reset();
	}

	// 人物状态修改
	if (get_object_state() == proto::common::object_state_transmission)
		change_object_state(proto::common::object_state_normal);
}

void role_t::transmission_callback(const boost::weak_ptr<role_t>& p_wrole, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted) {
		return;
	} else if (ec) {
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	role_ptr p_role(p_wrole.lock());
	if (NULL == p_role) {
		log_error("NULL == p_role");
		return;
	}

	p_role->on_transmission_timer();
}

void role_t::del_clear()
{
	if (NULL != m_script_role)
	{
		m_script_role->dispose();
		m_script_role = NULL;
	}
	if (NULL != m_pk_punish_timer)
	{
		m_pk_punish_timer->cancel();
	}
	if (NULL != m_pk_reduce_timer)
	{
		m_pk_reduce_timer->cancel();
	}

	if (NULL != m_transmission_timer) {
		m_transmission_timer->cancel();
	}

	if (NULL != m_tower)
	{
		m_tower->del_auto_fight_timer();
	}

	if (NULL != m_role_buff_mgr)
	{
		m_role_buff_mgr->del_timer();
	}

	if (m_patrol_hire) {
		m_patrol_hire->del_timer();
	}
}

void role_t::set_main_hero_tid(uint32_t tid)
{
	m_main_hero_tid = tid;
	auto p_hero_conf = GET_CONF(Hero, tid);
	if (NULL == p_hero_conf)
	{
		log_error("NULL == p_hero_conf[%d]", tid);
		return;
	}
	m_job = p_hero_conf->profession();
}

void role_t::update_login_time()
{
	ROLE_LOG("role[%lu] last login time: %s[%d]", m_uid, time_util_t::FormatTime_to_string(m_last_login_time, true).c_str(), m_last_login_time);

    //重置在线时间
    m_online_time = 0;

	// 更新登陆时间
	m_last_login_time = common::time_util_t::now_time();

	// 累计登陆天数初始化为1
	if (!m_cumulative_login_days) m_cumulative_login_days = 1;
	achieve_common_t::notify_progress_state(m_uid, proto::common::ACHIEVE_TOTAL_LOGIN,1,1);
}

void role_t::check_update_fighting(uint64_t hero_uid)
{
	if (m_formation_mgr.is_in_cur_form(hero_uid))
	{
		calc_fighting();
	}
}

bool role_t::get_ignore_lineup_flag()
{
	return m_role_fight_data.ignore_lineup_flag();
}

void role_t::save_ignore_lineup_flag(uint32_t ignore_lineup_flag)
{
	m_role_fight_data.set_ignore_lineup_flag(ignore_lineup_flag);
	save_self();
}

bool role_t::get_auto_fight_flag()
{
	return m_role_fight_data.auto_fight_flag();
}

void role_t::save_auto_fight_flag(uint32_t auto_fight_flag)
{
	m_role_fight_data.set_auto_fight_flag(auto_fight_flag);
}

void role_t::combat_notify_round(uint64_t against_id, uint32_t round)
{
	// 脚本事件
	script_event_enter_fight(against_id, round);
}

void role_t::combat_notify_prepare(uint64_t against_id, uint32_t round)
{
	// 脚本事件
	script_event_ready_fight(against_id, round);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//新手引导
//初始化新手引导
void role_t::init_new_role_guide(uint32_t guide_tid, bool is_not_waiting,uint32_t event_type)
{
	auto conf = GET_CONF(Comprehensive, comprehensive_common::first_guide_id);
	uint32_t first_guide_id = GET_COMPREHENSIVE_VALUE_1(conf);
    std::map<uint32_t, new_role_guide_t>::iterator it = m_new_role_guide_list.find(guide_tid);
    if (it != m_new_role_guide_list.end())
    {
        new_role_guide_t& guide = it->second;
        /*if (is_not_waiting && NEW_ROLE_GUIDE_STATUS_READY_DONE != guide.status)
        {
            log_error("role[%lu] init new_role_guide[%d] failed: is_not_waiting is true, but status is not NEW_ROLE_GUIDE_STATUS_READY_DONE[%d]!", m_uid, guide.tid, guide.status);
            guide.status = is_not_waiting;
            return;
        }*/
        guide.is_not_waiting = is_not_waiting;
        //NEW_GUIDE_LOG(" role[%lu] set new_role_guide tid[%d] is_not_waiting[%d] event_type[%d] status[%d]", m_uid, guide.tid, guide.is_not_waiting,guide.event_type, guide.status);
    }
    else
    {
        new_role_guide_t guide;
        guide.tid = guide_tid;
        guide.is_not_waiting = is_not_waiting;
		guide.event_type = event_type;
        guide.status = (guide_tid == first_guide_id ? NEW_ROLE_GUIDE_STATUS_READY_DONE : NEW_ROLE_GUIDE_STATUS_DOING);
        m_new_role_guide_list[guide.tid] = guide;
        NEW_GUIDE_LOG(" role[%lu] add new_role_guide tid[%d] is_not_waiting[%d] event_type[%d] status[%d]", m_uid, guide.tid, guide.is_not_waiting,event_type, guide.status);
    }
}

//完成新手引导 来自脚本
void role_t::finish_new_role_guide_from_script(uint32_t guide_tid)
{
	if ( 0 == guide_tid)
	{
		return;
	}
    std::map<uint32_t, new_role_guide_t>::iterator itr = m_new_role_guide_list.find(guide_tid);
    if (itr == m_new_role_guide_list.end())
    {
        log_error("role[%lu] not find new_role_guide[%d]", m_uid, guide_tid);
        return;
    }

    new_role_guide_t& guide = itr->second;
    if (NEW_ROLE_GUIDE_STATUS_REALLY_DONE == guide.status)
    {
		NEW_GUIDE_LOG("role[%lu] has finished new_role_guide[%d]", m_uid, guide.tid);
		return;
    }

    if (guide.is_not_waiting)
    {
        guide.status = NEW_ROLE_GUIDE_STATUS_REALLY_DONE;
		NEW_GUIDE_LOG("role[%lu] finish new_role_guide[%d] status[%d] from script", m_uid, guide.tid, guide.status);
		auto conf = GET_CONF(Comprehensive, comprehensive_common::second_guide_id);
		uint32_t second_guide_id = GET_COMPREHENSIVE_VALUE_1(conf);
		if (guide_tid == second_guide_id)
		{
			open_function((uint32_t)proto::common::function_code_type_second_new_guide);
		}
    }
    else
    {
        guide.status = NEW_ROLE_GUIDE_STATUS_READY_DONE;
		NEW_GUIDE_LOG("role[%lu] need to waiting to client then finish new_role_guide[%d]", m_uid, guide.tid);
    }

	save_self();

    proto::client::gc_touch_new_role_guide_notify msg;
    msg.set_tid(guide_tid);
	msg.set_type(guide.event_type);
    send_msg_to_client(op_cmd::gc_touch_new_role_guide_notify, msg);
	NEW_GUIDE_LOG("role[%lu] new_guild id[%d] type[%d] status[%d]",m_uid,guide_tid,guide.event_type,guide.status);
	log_wrapper_t::send_new_guide_log(m_uid, guide_tid, log_enum::source_type_finish_new_guide, guide.status);
}

void role_t::traverse_guide_id(std::function<void(uint32_t)> func)
{
	for(auto& pair : m_new_role_guide_list){
		if(NEW_ROLE_GUIDE_STATUS_REALLY_DONE == pair.second.status)
			func(pair.first);
	}
}

//完成新手引导 来自客户端
void role_t::finish_new_role_guide_from_client(uint32_t guide_tid)
{
    std::map<uint32_t, new_role_guide_t>::iterator itr = m_new_role_guide_list.find(guide_tid);
    if (itr == m_new_role_guide_list.end())
    {
        log_error("role[%lu] not find new_role_guide[%d]", m_uid, guide_tid);
        return;
    }

    new_role_guide_t& guide = itr->second;
    if (NEW_ROLE_GUIDE_STATUS_REALLY_DONE == guide.status)
    {
		NEW_GUIDE_LOG("role[%lu] has finished new_role_guide[%d]", m_uid, guide.tid);
        return;
    }
	///有些新手引导不是由服务器发往客户端进行触发,而是客户端主动触发然后通知服务器完成
	/* if (NEW_ROLE_GUIDE_STATUS_READY_DONE != guide.status)
	 {
		 log_error("role[%lu] finish new_role_guide[%d] failed: status[%d] error!", guide.status);
		 return;
	 }*/
    if (guide.is_not_waiting)
    {
        log_error("role[%lu] finish new_role_guide[%d] failed: the guide is not waiting to client finish!", m_uid, guide.tid);
        return;
    }
    
    guide.status = NEW_ROLE_GUIDE_STATUS_REALLY_DONE;

	NEW_GUIDE_LOG(" role[%lu] finish new_role_guide[%d] status[%d] from client", m_uid, guide.tid, guide.status);
	auto conf = GET_CONF(Comprehensive, comprehensive_common::second_guide_id);
	uint32_t second_guide_id = GET_COMPREHENSIVE_VALUE_1(conf);
	if (guide_tid == second_guide_id)
	{
		open_function((uint32_t)proto::common::function_code_type_second_new_guide);
	}

	save_self();
	script_event_finish_new_guide(guide_tid);
	log_wrapper_t::send_new_guide_log(m_uid, guide_tid, log_enum::source_type_finish_new_guide, guide.status);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//脚本事件的处理函数
void role_t::script_event_level_up(uint32_t level)
{
    if (NULL != m_script_role)
    {
        m_script_role->on_level_up(level);
    }
}

void role_t::script_event_create_item(uint32_t item_itd)
{
    if (NULL != m_script_role)
    {
        m_script_role->on_create_item(item_itd);
    }
}

void role_t::script_event_enter_area(uint32_t area_itd)
{
    if (NULL != m_script_role)
    {
        m_script_role->on_enter_area(area_itd);
    }
}

uint32_t role_t::get_sync_plugin()
{
	if (m_task_plugin > 0)
	{
		return m_task_plugin;
	}
	if (m_change_plugin > 0)
	{
		return m_change_plugin;
	}
	return m_main_hero_plugin;
}

void role_t::set_main_hero_plugin(uint32_t plugin)
{
	m_main_hero_plugin = plugin;
	//save_self();

	// 同步等级给center
	center_msg_handler_t::sync_data_to_center(get_uid(), proto::server::data_sync_type_main_plugin, m_main_hero_plugin, true);
}

void role_t::set_change_plugin(uint32_t val)
{
	m_change_plugin = val;
}

void role_t::set_task_plugin(uint32_t val)
{
	m_task_plugin = val;
}

uint32_t role_t::get_all_hero_star()
{
	uint32_t star = 0;
	for (hero_map::const_iterator citr = m_hero_list.begin();
		citr != m_hero_list.end(); ++citr) {
		if (citr->second != NULL) {
			switch (citr->second->get_hero_state()) {
				case hero_type::hero_type_normal: {
					star += citr->second->get_star();
					break;
				}
				case hero_type::hero_type_main: {
					if (get_main_hero_uid() == citr->first) {
						star += citr->second->get_star();
					}
					break;
				}
				default: {
					break;
				}
			}
		}
	}

	return star;
}

void role_t::script_event_enter_fight(uint32_t fight_tid, uint32_t round_tid)
{
	if (NULL != m_script_role)
	{
		m_script_role->on_enter_fight(fight_tid, round_tid);
	}
}

void role_t::script_event_ready_fight(uint32_t fight_tid, uint32_t round_tid)
{
	if (NULL != m_script_role)
	{
		m_script_role->on_ready_fight(fight_tid, round_tid);
	}
}

void role_t::script_event_combat_end(uint32_t fight_tid)
{
	if (NULL != m_script_role)
	{
		m_script_role->on_end_fight(fight_tid);
	}
}

void role_t::script_event_use_item(uint32_t item_tid)
{
	if (NULL != m_script_role)
	{
		m_script_role->on_use_item(item_tid);
	}
}

void role_t::script_event_new_role_enter_scene()
{
	if (NULL != m_script_role)
	{
		m_script_role->on_first_enter_scene();
	}
}

void role_t::script_event_finish_new_guide(uint32_t guide_tid)
{
	if (NULL != m_script_role)
	{
		m_script_role->on_finish_new_guide_touch(guide_tid);
	}
}

void role_t::update_troop_info(uint64_t troop_id, uint32_t state /*= 0*/, bool is_leader /*= false*/)
{
	bool is_update = false;
	if (troop_id != m_troop_id) {
		m_troop_id = troop_id;
		is_update = true;
	}

	if (state != m_troop_state) {
		m_troop_state = state;
	}

	if (is_leader != m_is_leader) {
		m_is_leader = is_leader;
		is_update = true;
	}

	if (m_online_state != common::ROLE_ONLINE_STATE_LOGOUT && is_update) notify_object_common_info_for_nearbies();
}

uint32_t role_t::can_create_troop()
{
	if (m_object_state == proto::common::object_state_death ||
		m_object_state == proto::common::object_state_fighting ||
		m_object_state == proto::common::object_state_watch_fighting) {
		return common::errcode_enum::user_troop_err_state;
	}

	return common::errcode_enum::error_ok;
}

uint32_t role_t::can_join_troop()
{
	if (m_object_state == proto::common::object_state_death ||
		m_object_state == proto::common::object_state_fighting ||
		m_object_state == proto::common::object_state_watch_fighting) {
		return common::errcode_enum::user_troop_err_state;
	}

	if (m_mode_state == proto::common::role_mode_state_trade) {
		return common::errcode_enum::user_troop_trade_join_error;
	}

	if (m_role_scene.is_patrol_state()) {
		return common::errcode_enum::user_troop_patrol_join_error;
	}

	return common::errcode_enum::error_ok;
}

void role_t::set_family_officer(uint32_t val)
{
	m_family_officer = val;

	notify_object_common_info_for_nearbies();
}




//////////////////////////////////////////////////////////////////////////
//命力相关

bool role_t::level_up_lifestar()
{
	LifeState* p_config = GET_CONF(LifeState, m_lifestate_level);
	if (NULL == p_config)
	{
		log_error("NULL == p_config role[%lu] lifestate level[%d]", m_uid, m_lifestate_level);
		return false;
	}
	//是否已经升到最高星级
	if (m_lifestar_level >= p_config->piece_count())
	{
		log_error("role[%lu] lifestar level already [%d]level unable up level", m_uid, m_lifestar_level);
		return false;
	}

	++m_lifestar_level;
	if (1 == m_lifestate_level && 1 == m_lifestar_level)
	{
		notify_object_common_info_for_nearbies();
	}
	
	return true;
}

bool role_t::level_up_lifestate(uint32_t& recode)
{
	//境界是获取当前等级表数据
	LifeState* p_config = GET_CONF(LifeState, m_lifestate_level);
	do 
	{
		if (NULL == p_config)
		{
			log_error("NULL == p_config role[%lu] lifestate level[%d]", m_uid, m_lifestate_level);
			recode = errcode_enum::notice_lifeforce_config_error;
			break;
		}

		//是否升到命星等级最大 
		if (m_lifestar_level < p_config->piece_count())
		{
			LIFEFORCE_LOG("role[%lu] lifestar level not enough lifestar level[%d]", m_uid, m_lifestar_level);
			recode = errcode_enum::notice_lifeforce_level_not_enough;
			break;
		}

		//是否完成突破任务
		uint32_t break_task_id = p_config->finish_task_id();
		if (!lifeforce_manager_t::break_task_is_finish(m_uid, break_task_id))
		{
			LIFEFORCE_LOG("role[%lu] unable up lifestate level because not finish break task[%d]", m_uid, break_task_id);
			recode = errcode_enum::notice_lifeforce_break_task_not_finish;
			break;
		}

		LifeState* p_next_config = GET_CONF(LifeState, m_lifestate_level + 1);
		if (NULL == p_next_config)
		{
			LIFEFORCE_LOG("role[%lu] unable up lifestate level[%d] already break max level",m_uid,m_lifestate_level);
			recode = errcode_enum::notice_lifeforce_state_level_already_max;
			break;
		}

	} while (0);

	if (recode == errcode_enum::error_ok)
	{
		++m_lifestate_level;
		reset_lifestar_level();
		notify_object_common_info_for_nearbies();
		LIFEFORCE_LOG("role[%lu] up lifestate level[%d] success", m_uid, m_lifestate_level);
		return true;
	}
	LIFEFORCE_LOG("role[%lu] up lifestate level[%d] fail error code[%d]",m_uid, m_lifestate_level, recode);
	return false;
}

bool role_t::level_up_lifelabel()
{
	const uint32_t cur_label_level = get_lifeforce_label();
	LabelTable* p_config = GET_CONF(LabelTable, cur_label_level + 1);
	if (NULL == p_config)
	{
		log_error("NULL == p_config id[%d]",cur_label_level + 1);
		return false;
	}
	++m_lifeforce_label;
	notify_object_common_info_for_nearbies();
	ROLE_LOG("role[%lu] life label level from[%d] up to[%d]",get_uid(),cur_label_level,m_lifeforce_label);
	return true;
}

void role_t::script_event_task_accept(uint32_t task_id)
{
	if (NULL != m_script_role)
	{
		m_script_role->on_task_accept(task_id);
	}
}

void role_t::script_event_task_finish(uint32_t task_id)
{
	if (NULL != m_script_role)
	{
		m_script_role->on_task_finish(task_id);
	}
}

void role_t::open_function(uint32_t func)
{
	if (func > 64)
	{
		return;
	}
	if (CT_BIT_CHECK_64(m_function_code, func))
	{
		log_error("func[%d] is opened", func);
		return;
	}
	CT_BIT_SET_64(m_function_code, func);

	// 没有上线不发
	if (m_online_state != ROLE_ONLINE_STATE_ONLINE)
	{
		return;
	}

	proto::client::gc_update_function_code_ntf ntf;
	ntf.set_function_code(common::string_util_t::uint64_to_2_string(m_function_code));
	send_msg_to_client(op_cmd::gc_update_function_code_ntf, ntf);
}

void role_t::close_function(uint32_t func)
{
	if (func > 64)
	{
		return;
	}
	if (!CT_BIT_CHECK_64(m_function_code, func))
	{
		log_error("func[%d] is closed", func);
		return;
	}
	CT_BIT_CLR_64(m_function_code, func);

	// 没有上线不发
	if (m_online_state != ROLE_ONLINE_STATE_ONLINE)
	{
		return;
	}

	proto::client::gc_update_function_code_ntf ntf;
	ntf.set_function_code(common::string_util_t::uint64_to_2_string(m_function_code));
	send_msg_to_client(op_cmd::gc_update_function_code_ntf, ntf);
}

bool role_t::is_function_open(uint32_t func)
{
	if (func > 64)
	{
		return false;
	}
	return CT_BIT_CHECK_64(m_function_code, func);
}

void role_t::set_npc_code(uint32_t func)
{
	if (func != m_npc_code)
	{
		m_npc_code = func;

		// 没有上线不发
		if (m_online_state != ROLE_ONLINE_STATE_ONLINE)
		{
			return;
		}

		// 通知客户端
		proto::client::gc_update_npc_code_ntf ntf;
		ntf.set_npc_code(m_npc_code);
		send_msg_to_client(op_cmd::gc_update_npc_code_ntf, ntf);
	}
}

void role_t::set_collect_code(uint32_t func)
{
	if (func != m_collect_code)
	{
		m_collect_code = func;
		// 通知客户端
		proto::client::gc_update_collect_code_ntf ntf;
		ntf.set_collect_code(m_collect_code);
		send_msg_to_client(op_cmd::gc_update_collect_code_ntf, ntf);
	}
}


void role_t::open_all_function()
{
	m_function_code = 0x7FFFFFFFFFFFFFFF;

	proto::client::gc_update_function_code_ntf ntf;
	ntf.set_function_code(common::string_util_t::uint64_to_2_string(m_function_code));
	send_msg_to_client(op_cmd::gc_update_function_code_ntf, ntf);
}

void role_t::change_real_speed(speed_update_type type /*= speed_change_update*/)
{
	// 对于组队中的队员直接return
	if (is_troop_member_not_leave()) return;

	uint32_t real_speed = 0;
	if (m_troop_id != 0 && is_team_leader()) {
		const troop_ptr& p_troop = game_troop_mgr_t::get_troop(m_troop_id);
		if (p_troop != NULL) { // 找到队伍，更新
			real_speed = get_speed_by_type(p_troop->is_member_trade() ? proto::common::role_mode_state_trade : m_mode_state);
			p_troop->update_mount_state(m_mode_state, real_speed); // 更新队员新的速度
		}
	}

	if (real_speed == 0) {
		real_speed = get_speed_by_type(m_mode_state);
	}

	//bool is_change = false;
	if (real_speed != get_real_speed()) {
		set_real_speed(real_speed);
		//is_change = true;

		ROLE_LOG("role[%lu] change real speed[%d]", m_uid, real_speed);
	}

	//// 还未上线不更新
	//if (m_online_state == ROLE_ONLINE_STATE_LOGOUT) return;

	//switch (type) {
	//	case speed_change_update:
	//		if (is_change)
	//			notify_object_common_info_for_nearbies();
	//		break;
	//	case speed_only_set:
	//		break;
	//	case speed_forced_update:
	//		notify_object_common_info_for_nearbies();
	//		break;
	//}
}

void role_t::update_troop_mount(proto::common::role_mode_state team_state, uint32_t speed)
{
	bool is_change = false;

	if (m_mode_state != proto::common::role_mode_state_trade && m_mode_state != team_state) { // 跑商情况乘骑不改变模型
		if (team_state != proto::common::role_mode_state_mount || (get_mount_data() != NULL && get_mount_data()->get_use_mount() != NULL)) { // 没有坐骑，不改变模型
			m_mode_state = team_state;
			save_self(); // 保存坐骑状态
			is_change = true;
		}
	}

	if (speed != get_real_speed()) {
		set_real_speed(speed);
		is_change = true;
	}

	if (m_online_state != common::ROLE_ONLINE_STATE_LOGOUT && is_change) notify_object_common_info_for_nearbies();
}

void role_t::update_troop_speed(uint32_t speed)
{
	if (speed != get_real_speed())
	{
		// 组队改变队员速度
		ROLE_LOG("role[%lu] change speed[%u] by team", m_uid, speed);

		set_real_speed(speed);

		if (m_online_state != common::ROLE_ONLINE_STATE_LOGOUT) notify_object_common_info_for_nearbies();
	}
}

uint32_t role_t::get_speed_by_type(proto::common::role_mode_state state)
{
	uint32_t base_speed = 0;
	switch (state) {
		case proto::common::role_mode_state_normal: {
			auto p_role_base_speed = GET_CONF(Comprehensive, comprehensive_common::role_base_speed);
			base_speed = GET_COMPREHENSIVE_VALUE_1(p_role_base_speed);
			break;
		}
		case proto::common::role_mode_state_mount: {
			do {
				mount_ptr p_mount = get_cur_use_mount();
				if (p_mount != NULL) {
					MountTable *p_config = GET_CONF(MountTable, p_mount->get_tid());
					if (p_config) {
						base_speed = p_config->mount_speed();
						break; // find
					} else {
						log_error("role[%lu] mount config[%d] NULL", m_uid, p_mount->get_tid());
					}
				} else {
					log_error("role[%lu] can't find cur_use_mount", m_uid);
				}

				auto p_role_base_speed = GET_CONF(Comprehensive, comprehensive_common::role_base_speed);
				base_speed = GET_COMPREHENSIVE_VALUE_1(p_role_base_speed);
			} while (0);

			break;
		}
		case proto::common::role_mode_state_trade: {
			do {
				mount_ptr p_mount = get_cur_use_mount();
				if (p_mount != NULL) {
					MountTable *p_config = GET_CONF(MountTable, p_mount->get_tid());
					if (p_config) {
						base_speed = p_config->trade_speed();
						break; // find
					} else {
						log_error("role[%lu] mount config[%d] NULL", m_uid, p_mount->get_tid());
					}
				} else {
					log_error("role[%lu] can't find cur_use_mount", m_uid);
				}

				auto p_role_base_speed = GET_CONF(Comprehensive, comprehensive_common::role_base_speed);
				base_speed = GET_COMPREHENSIVE_VALUE_1(p_role_base_speed);
			} while (0);

			break;
		}
		default: {
			log_error("role[%lu] mode state[%u] error", m_uid, state);
			auto p_role_base_speed = GET_CONF(Comprehensive, comprehensive_common::role_base_speed);
			base_speed = GET_COMPREHENSIVE_VALUE_1(p_role_base_speed);
		}
	}

	// 速度
	return (base_speed + m_buff_speed) * (1 + (float)m_buff_prec_speed / 10000);
}

bool role_t::init_new_guide()
{
	// lua 上线处理
	if (NULL != m_script_role)
	{
		m_script_role->on_login();
		return true;
	}
	log_error("role[%lu] init new guide fail !!!!",m_uid);
	return false;
}

void role_t::send_error_code(uint32_t code)
{
	proto::client::gc_common_error_notify ntf;
	ntf.set_reply_code(code);
	send_msg_to_client(op_cmd::gc_common_error_notify, ntf);
}

uint32_t role_t::patrol_request(uint32_t map_id, uint32_t patrol)
{
	if (is_troop_member_not_leave()) {
		log_warn("following member[%lu] can't change patrol state", m_uid);
		return common::errcode_enum::patrol_error_3;
	}

	if (0 == patrol)
	{
		//退出巡逻
		if (!m_role_scene.is_patrol_state())
		{
			log_error("role[%lu] is not patrol", get_uid());
			return errcode_enum::notice_unknown;
		}
		set_patrol(false);
	}
	else
	{
		//进入巡逻
		if (0 == map_id)
		{
			//当前地图进入巡逻
			auto p_conf = GET_CONF(Patrol, get_map_tid());
			if (NULL == p_conf)
			{
				log_error("NULL == p_conf  Patrol[%d]", get_map_tid());
				return errcode_enum::patrol_error_2;
			}
			if (get_level() < p_conf->level().first)
			{
				log_error("role[%lu] level[%d] not enough need[%d]", get_uid(), get_level(), p_conf->level().first);
				return errcode_enum::patrol_error_2;
			}
			// 判断是否被通缉
			if (pk_manager_t::is_criminal(get_pk_value()))
			{
				log_error("role[%lu] pk_value[%d] is_criminal", get_uid(), get_pk_value());
				return errcode_enum::patrol_error_1;
			}
			// 判断是否有跑商的
			if (get_trade_info().get_trade_bag_weight() > 0)
			{
				SCENE_LOG("role[%lu] trade", get_uid());
				return errcode_enum::patrol_error_1;
			}

			if (!set_patrol(true))
			{
				SCENE_LOG("role[%lu] set patrol error!", get_uid());
				return common::errcode_enum::patrol_error_2;
			}
		}
		else
		{
			//进入目标地图巡逻
			auto p_conf = GET_CONF(Patrol, map_id);
			if (NULL == p_conf)
			{
				log_error("NULL == p_conf  Patrol[%d]", map_id);
				return errcode_enum::patrol_error_2;
			}
			if (get_level() < p_conf->level().first)
			{
				log_error("role[%lu] level[%d] not enough need[%d]", get_uid(), get_level(), p_conf->level().first);
				return errcode_enum::patrol_error_2;
			}
			if (pk_manager_t::is_criminal(get_pk_value()))
			{
				log_error("role[%lu] pk_value[%d] is_criminal", get_uid(), get_pk_value());
				return errcode_enum::patrol_error_1;
			}
			uint32_t reply_code = scene_manager_t::role_jump_scene(get_role(), proto::server::user_change_scene, map_id);
			if (reply_code != errcode_enum::error_ok)
			{
				log_error("role[%lu] jump_scene[%d] failed", get_uid(), map_id);
				return errcode_enum::notice_jump_state_error;
			}
			
			if (!set_patrol(true))
			{
				SCENE_LOG("role[%lu] set patrol error!", get_uid());
				return common::errcode_enum::patrol_error_2;
			}
		}
	}

	return errcode_enum::error_ok;
}

bool role_t::set_patrol(bool val)
{
	if (!m_role_scene.set_patrol_state(val))
	{
		return false;
	}

	if (m_troop_id != 0 && is_leader()) { // 队长更改队员巡逻状态
		troop_ptr p_troop = game_troop_mgr_t::get_troop(m_troop_id);
		if (p_troop != NULL) {
			p_troop->update_member_patrol(m_uid, val);
		}
	}

	proto::client::gc_patrol_notify ntf;
	ntf.set_patrol(m_role_scene.is_patrol_state() ? 1 : 0);
	send_msg_to_client(op_cmd::gc_patrol_notify, ntf);

	return true;
}

void role_t::troop_patrol(bool val)
{
	if (!m_role_scene.set_patrol_state(val))
	{
		return;
	}

	proto::client::gc_patrol_notify ntf;
	ntf.set_patrol(m_role_scene.is_patrol_state() ? 1 : 0);
	send_msg_to_client(op_cmd::gc_patrol_notify, ntf);
}

uint32_t role_t::get_patrol_exp()
{
	std::map<uint32_t, PatrolExp*> tmp_map;
	GET_ALL_CONF(PatrolExp, tmp_map);
	for (auto it : tmp_map)
	{
		auto p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		if (m_patrol_count >= p_conf->count().first && m_patrol_count <= p_conf->count().second)
		{
			return p_conf->exp();
		}
	}
	return 0;
}

void role_t::update_offline_data(const proto::common::offline_role_data& ord)
{
	if (NULL != m_tower)
	{
		const proto::common::tower_trigger_data& ttd = ord.tower_trigger();
		for (int32_t i = 0; i < ttd.other_trigger_size(); ++i)
		{
			m_tower->add_other_trigger(ttd.other_trigger(i));
		}
	}
}

uint32_t role_t::get_level_reward(uint32_t id, proto::common::role_change_data* p_data)
{
	if (!p_data) {
		log_error("role[%lu] p_data == null", m_uid);
		return common::errcode_enum::notice_unknown;
	}

	if (!is_function_open(proto::common::function_code_type_level_reward)) {
		log_error("role[%lu] level reward not open!", m_uid);
		return common::errcode_enum::notice_level_reward_no_open;
	}

	uint32_t back_code = m_role_level_reward.get_level_reward(id, get_role(), p_data);
	if (back_code == common::errcode_enum::error_ok && m_role_level_reward.is_recived_all_reward()) {
		// 所有奖励领取完关闭这个功能
		close_function(proto::common::function_code_type_level_reward);
	}

	return back_code;
}

void role_t::draw_notify(uint32_t money)
{
	money_manager_t::add_money(get_role(), get_redbag_money_type(), money, log_enum::source_type_role_luck_gift_grab, 0);
	proto::client::gc_redbag_draw_notify ntf;
	ntf.set_money(money);
	get_personal_info(ntf.mutable_rcd()->mutable_per_info());
	send_msg_to_client(op_cmd::gc_redbag_draw_notify, ntf);
}

uint32_t role_t::send_redbag(uint32_t count, uint32_t money, proto::common::role_change_data* p_data)
{
	if (0 == get_family_id())
	{
		log_error("role[%lu] no in family", get_uid());
		return errcode_enum::family_err_code_not_join_family;
	}
	if (money < count * get_draw_min())
	{
		log_error("role[%lu] send redbag count[%d] money[%d] min", get_uid(), count, money);
		return errcode_enum::red_envelope_min;
	}
	if (money > count * get_draw_max())
	{
		log_error("role[%lu] send redbag count[%d] money[%d] max", get_uid(), count, money);
		return errcode_enum::red_envelope_max;
	}
	if (!money_manager_t::use_money(get_role(), get_redbag_money_type(), money, log_enum::source_type_role_luck_gift_send, 0, true, p_data))
	{
		log_error("role[%lu] send redbag count[%d] money[%d] money not enough", get_uid(), count, money);
		return errcode_enum::notice_gold_money_not_enough;
	}
	if (NULL != p_data)
	{
		get_personal_info(p_data->mutable_per_info());
	}
	proto::server::ge_redbag_send_notify ntf;
	ntf.set_money(money);
	ntf.set_count(count);
	env::server->send_msg_to_center(op_cmd::ge_redbag_send_notify, get_uid(), ntf);
	return errcode_enum::error_ok;
}

uint32_t role_t::get_draw_min()
{
	auto draw_min = GET_CONF(Comprehensive, comprehensive_common::red_envelope_min);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(draw_min);
	return val;
}

uint32_t role_t::get_draw_max()
{
	auto draw_max = GET_CONF(Comprehensive, comprehensive_common::red_envelope_max);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(draw_max);
	return val;
}

proto::common::MONEY_TYPE role_t::get_redbag_money_type()
{
	auto redbag_money_type = GET_CONF(Comprehensive, comprehensive_common::red_envelope_currency);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(redbag_money_type);
	return (proto::common::MONEY_TYPE)val;
}

void role_t::update_role_info_to_center(proto::common::role_data_type type,bool is_save)
{
	formation_ptr p_formatimon = m_formation_mgr.get_cur_formation();
	if (NULL == p_formatimon)
	{
		log_error("NULL == p_formatimon role[%lu]", m_uid);
		return;
	}
	proto::server::ge_role_simple_data_notify notify;
	notify.set_type(type);
	proto::common::role_blob_data* p_blob_data = notify.mutable_simple_data();
	bool is_send = false;
	switch (type)
	{
	case proto::common::simple_info_type:
	{
		proto::common::role_simple_data* p_data = p_blob_data->mutable_simple();
		proto::common::hero_data* p_hero_data = p_data->mutable_hero();
		proto::common::item_data* p_item_data = p_data->mutable_item();
		const std::map<uint32_t, PosHeroData>&  hero_list = p_formatimon->get_pos_hero();
		std::vector<uint64_t> list; ///已装备在武将身上的的装备
		do
		{
			for (auto& it : hero_list)
			{
				hero_map::iterator iter = m_hero_list.find(it.second.uid);
				if (iter == m_hero_list.end())
				{
					//log_error("role[%lu] find hero[%lu] null", m_uid, it.second.uid);
					continue;
				}
				hero_ptr p_hero = iter->second;
				if (NULL == p_hero)
				{
					//log_error("role[%lu] hero[%lu] null", m_uid, it.second.uid);
					continue;
				}

				proto::common::hero_single* p_hero_single = p_hero_data->add_hero_list();
				p_hero->peek_data(p_hero_single);
				p_hero_single->clear_talent_data();
				p_hero_single->clear_save_hp();
				const proto::common::base_attr& base_attr = p_hero_single->base_attr();
				for (int i = 0 ; i < base_attr.equips_size();++i)
				{
					const proto::common::hero_equip& equip = base_attr.equips(i);
					const uint64_t equip_uid = string_util_t::string_to_uint64(equip.equip_uid());
					list.push_back(equip_uid);
				}	
			}

			for (auto& it : list)
			{
				item_ptr p_item = item_manager_t::get_item_by_uid(get_role(), it);
				if (NULL == p_item)
				{
					log_error("role [%lu] item[%lu] is not exist!", m_uid, it);
					continue;
				}
				proto::common::item_single* p_item_single = p_item_data->add_item_list();
				p_item->peek_data(p_item_single);
				p_item_single->clear_expired_time();
				p_item_single->clear_num();
				p_item_single->clear_price();
				p_item_single->clear_package();
				p_item_single->clear_up_num();
				p_item_single->clear_pos();
				p_item_single->clear_param();
				p_item_single->clear_param2();
				p_item_single->clear_find_role();
				p_item_single->clear_state();
			}
			is_send = true;
		} while (false);

	}break;
	default:
		break;
	}
	if (is_send)
	{
		env::server->send_msg_to_center(op_cmd::ge_role_simple_data_notify, get_uid(), notify);
	}	
}




void role_t::check_send_level_reward_mail()
{
	m_role_level_reward_mail.check_send_level_reward_mail(get_role());
}

void role_t::accept_sync_task(const proto::common::task_state &task_info, proto::common::role_change_data &rcd)
{
	if (!task_info.has_type())
	{
		log_error("owner[%lu],sync task without task_type", get_uid() );
		return;
	}

	if (!m_task_mgr->is_circle_task(task_info.type()))
	{
		log_error("owner[%lu],sync task is not circle type", get_uid());
		return;
	}

	//清理任务
	if (!task_info.has_id() || task_info.id() == 0)
	{
		m_task_mgr->del_circle_task(task_info.type());
		log_error("owner[%lu], drop task, type[%u] by sync", get_uid(), task_info.type() );
		return;
	}

	//如果已经有任务了先清理
	if ( m_task_mgr->get_circle_task(task_info.type()) != NULL )
	{
		drop_circle_task(task_info.type());
	}
	role_ptr p_role = role_manager_t::find_role( get_uid() );	//TODO role_t 转p_role方法
	if (NULL == p_role)
	{
		log_error("[%lu] p_role is NULL ", get_uid());
		return;
	}

	//环任务特殊处理部分
	task_ptr p_circle_task = m_task_mgr->get_circle_task(task_info.type());
	if (NULL == p_circle_task)
	{
		uint64_t uid = task_manager_t::gen_task_uid();
		p_circle_task.reset(new task_t(uid, 0));
		p_circle_task->set_type(task_info.type());
		m_task_mgr->add_task(p_circle_task);
		p_circle_task->set_circle_count(0);
		p_circle_task->set_max_circle_count(task_info.max_circle_count() );
		p_circle_task->set_circle_id(task_info.circle_id());
		p_circle_task->save_self( p_role );
	}

	//自动接取任务
	if ( m_task_mgr->active_task_by_id(task_info.id(), ACCEPT_TASK_BY_SYSTEM, rcd, false) == false )
	{
		return;
	}

	//修改任务数据
	task_ptr p_task =  m_task_mgr->get_circle_task(task_info.type());
	if (NULL == p_task)
	{
		log_error("owner[%lu],accept task id[%d] failed ", get_uid(), task_info.id());
		return;
	}

	p_task->set_circle_count(task_info.circle_count());
	p_task->set_max_circle_count(task_info.max_circle_count());
	p_task->set_circle_id(task_info.circle_id());
	p_task->save_self(p_role);

	//通知客户端
	p_role->m_task_mgr->notify_client_taskstate<proto::client::gc_accept_task_reply>( p_role, p_task, op_cmd::gc_accept_task_reply, rcd, errcode_enum::error_ok);
}

void role_t::drop_circle_task(uint32_t task_type)
{
	m_task_mgr->del_circle_task(task_type);
}

//添加家族资金 
void role_t::add_family_money(uint32_t money, uint32_t source_type, uint32_t source_param)
{
	if (get_family_id() == 0) {
		MONEY_LOG("role[%lu] family == 0, can't add family money", m_uid);
		return;
	}

	proto::server::ge_modify_family_money_request msg;
	msg.set_family_id(get_family_id());
	msg.set_type(0);
	msg.set_num(money);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);
	env::server->send_msg_to_center(op_cmd::ge_modify_family_money_request, m_uid, msg);
}
//添加国家资金
void role_t::add_country_money(uint32_t money, uint32_t source_type, uint32_t source_param)
{
	if (get_country_id() == 0) {
		MONEY_LOG("role[%lu] country == 0, can't add country money", m_uid);
		return;
	}

	proto::server::ge_modify_country_money_request msg;
	msg.set_country_id(get_country_id());
	msg.set_type(0);
	msg.set_num(money);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);
	env::server->send_msg_to_center(op_cmd::ge_modify_country_money_request, m_uid, msg);
}

void role_t::init_gm_level()
{
	set_gm(env::xml->gm_level);
}

//////////////////////////////////////////////////////////////////////////

uint32_t role_t::get_vigour()
{
	if (m_vigour_time == 0 || common::time_util_t::now_time() < m_next_add_time ) {
		return m_vigour;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::energy_recover_interval);
	uint32_t interval = GET_COMPREHENSIVE_VALUE_1(p_conf);

	uint32_t add_val = (common::time_util_t::now_time() - m_vigour_time) / interval;
	if (add_val == 0) { // 更新下下次增加的时间
		m_next_add_time = m_vigour_time + interval;
	} else {
		uint32_t base_limit = get_vigour_base_limit();
		if (m_vigour + add_val >= base_limit) { // 增加到最大基础值，并且清理时间
			add_vigour(base_limit - m_vigour, log_enum::source_type_vigour_time_add);
		} else { // 增加并更新下次什么时候增加时间
			m_vigour_time += add_val * interval;
			m_next_add_time = m_vigour_time + interval;
			add_vigour(add_val, log_enum::source_type_vigour_time_add);
		}
	}

	return m_vigour;
}

uint32_t role_t::get_vigour_base_limit()
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::energy_limit1);
	return GET_COMPREHENSIVE_VALUE_1(p_conf);
}

uint32_t role_t::get_vigour_max_limit()
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::energy_limit2);
	return GET_COMPREHENSIVE_VALUE_1(p_conf);
}

uint32_t role_t::get_max_buy_times()
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::energy_buy_cost);
	return GET_COMPREHENSIVE_VALUE_1(p_conf);
}

void role_t::add_vigour(uint32_t val, uint32_t source_type, uint32_t source_param /*= 0*/)
{
	uint32_t old_vigour = m_vigour;
	m_vigour += val;

	uint32_t base_limit = get_vigour_base_limit();
	if (m_vigour >= base_limit && m_vigour_time != 0) {
		m_vigour_time = 0;
		m_next_add_time = 0;
		save_self();
	}

	log_wrapper_t::send_money_log(get_uid(), get_level(), old_vigour, val, m_vigour, proto::common::MONEY_TYPE_VIGOUR, log_enum::opt_type_add, source_type, source_param);
}

bool role_t::user_vigour(uint32_t val, uint32_t source_type, uint32_t source_param /*= 0*/)
{
	if (m_vigour < val) {
		return false;
	}

	uint32_t old_vigour = m_vigour;
	m_vigour -= val;

	uint32_t base_limit = get_vigour_base_limit();
	if (m_vigour < base_limit && m_vigour_time == 0) {
		m_vigour_time = common::time_util_t::now_time();

		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::energy_recover_interval);
		uint32_t interval = GET_COMPREHENSIVE_VALUE_1(p_conf);
		m_next_add_time = m_vigour_time + interval;
		save_self();
	}

	log_wrapper_t::send_money_log(get_uid(), get_level(), old_vigour, val, m_vigour, proto::common::MONEY_TYPE_VIGOUR, log_enum::opt_type_del, source_type, source_param);

	return true;
}

void role_t::add_vigour_buy_times(uint32_t val /*= 1*/)
{
	m_vigour_buy += val;

	save_self();
}

void role_t::clear_vigour_buy_times(bool is_notify /*= true*/)
{
	m_vigour_buy = 0;
	save_self();

	if (is_notify) { // 在线通知
		proto::client::gc_vigour_info_notify ntf;
		proto::common::role_change_data* p_rcd = ntf.mutable_rcd();
		if (p_rcd) {
			get_personal_info(p_rcd->mutable_per_info());
		}
		send_msg_to_client(op_cmd::gc_vigour_info_notify, ntf);
	}
}


void role_t::get_replace_head_frame(uint32_t type, uint32_t id)
{
	uint32_t reply_code = errcode_enum::error_ok;
	do 
	{
		if (1 == type)
		{
			if (0 == m_head_list.count(id))
			{
				log_error("role[%lu] change head[%d] error, not active yet", get_uid(), id);
				reply_code = errcode_enum::notice_unknown;
				break;
			}

			set_head_frame(id, 0);
			ROLE_LOG("role[%lu] change head[%d] ok", get_uid(), id);
		}

		if (2 == type)
		{
			if (0 == m_frame_list.count(id))
			{
				log_error("role[%lu] change frame[%d] error, not active yet", get_uid(), id);
				reply_code = errcode_enum::notice_unknown;
				break;
			}

			set_head_frame(0, id);
			ROLE_LOG("role[%lu] change frame[%d] ok", get_uid(), id);
		}
		
	} while (false);
	
	if (reply_code == errcode_enum::error_ok)
	{
		save_self();
		notify_object_common_info_for_nearbies();
	}
	proto::client::gc_replace_head_reply reply;
	reply.set_reply_code(reply_code);
	reply.set_head_info(get_head_frame());
	send_msg_to_client(op_cmd::gc_replace_head_reply, reply);
}

void role_t::get_head_frame_list()
{
	ROLE_LOG("role[%lu] gets head frame list", get_uid());
	proto::client::gc_head_frame_list_reply reply;
	std::set<uint32_t>& head_list = get_head_list();
	std::set<uint32_t>& frame_list = get_frame_list();
	proto::common::head_data* p_data = reply.mutable_head_info();
	for (auto& it : head_list)
	{
		p_data->add_head_list(it);
	}

	for (auto& it : frame_list)
	{
		p_data->add_frame_list(it);
	}
	reply.set_head_frame_id(get_head_frame());
	send_msg_to_client(op_cmd::gc_head_frame_list_reply, reply);
}


void role_t::add_head_frame(proto::common::HEAD_FRAME_ADD_TYPE type, uint32_t param)
{
	//添加新解锁的头像
	std::map<uint32_t, HeadTable*> head_confs;
	GET_ALL_CONF(HeadTable, head_confs);
	if (head_confs.empty())
	{
		ROLE_LOG("HeadTable empty，role[%lu] get failed", get_uid());
		return;
	}

	for (auto& it : head_confs)
	{
		HeadTable* p_config = it.second;
		if (NULL == p_config)
			continue;
		const std::map<uint32_t, uint32_t>& cond_list = p_config->cond();
		if (cond_list.empty())
			continue;
		for (auto& ix : cond_list)
		{
			if ((uint32_t)type != ix.first)
			{
				break;
			}
			if (param != ix.second)
			{
				break;
			}
			add_head(p_config->id());
			break;
		}
	}

	//添加新解锁的头像框
	std::map<uint32_t, FrameTable*> frame_confs;
	GET_ALL_CONF(FrameTable, frame_confs);
	if (frame_confs.empty())
	{
		ROLE_LOG("role[%lu] get FrameTable conf failed, empty", get_uid());
		return;
	}

	for (auto& it : frame_confs)
	{
		FrameTable* p_config = it.second;
		if (NULL == p_config)
			continue;
		const std::map<uint32_t, uint32_t>& cond_list = p_config->cond();
		if (cond_list.empty())
			continue;
		for (auto& ix : cond_list)
		{
			if ((uint32_t)type != ix.first)
			{
				break;
			}
			if (param != ix.second)
			{
				break;
			}
			add_frame(p_config->id());
			break;
		}
	}
}

void role_t::init_head_frame(uint32_t head, uint32_t frame /*= 0*/)
{
	if (0 == head && 0 == frame)
	{
		log_error("both head and frame are 0 of role[%lu], no need to init", m_uid);
		return;
	}

	if (0 != head)
	{
		HeadTable* p_config = GET_CONF(HeadTable, head);
		if (NULL == p_config)
		{
			log_error("role[%lu] get HeadTable head[%d] conf NULL", get_uid(), head);
			return;
		}
		add_head(head);
	}

	if (0 != frame)
	{
		FrameTable* p_config = GET_CONF(FrameTable, frame);
		if (NULL == p_config)
		{
			log_error("role[%lu] get FrameTable frame[%d] conf NULL", get_uid(), frame);
			return;
		}
		add_frame(frame);
	}

	set_head_frame(head, frame);
}

void role_t::set_head_frame(uint32_t head, uint32_t frame /*= 0*/)
{
	if (0 == head && 0 == frame)
	{
		log_error("role[%lu] set_head_frame error, head and frame id are both 0", get_uid());
		return;
	}

	if (0 != head)
	{
		set_head(head);
	}

	if (0 != frame)
	{
		set_frame(frame);
	}

	// 同步等级给center
	center_msg_handler_t::sync_data_to_center(get_uid(), proto::server::data_sync_type_head_frame, get_head_frame(), true);
}

void role_t::set_head(uint32_t val)
{
	if (0 == val)
		return;
	uint32_t frame = get_frame();
	m_head_frame = (uint32_t)((val << 16) | frame);
}

void role_t::set_frame(uint32_t val)
{
	if (0 == val)
	{
		log_error("role[%lu] set frame[0]", get_uid());
		return;
	}

	ROLE_LOG("role[%lu] set frame[%d] ok", get_uid(), val);

	uint32_t head = get_head();
	m_head_frame = (uint32_t)((head << 16) | val);
}

void role_t::add_head(uint32_t val)
{
	if (0 == m_head_list.count(val))
	{
		m_head_list.insert(val);
	}
}

void role_t::add_frame(uint32_t val)
{
	if (0 == m_frame_list.count(val))
	{
		m_frame_list.insert(val);
		ROLE_LOG("role[%lu] add frame[%d] ok", get_uid(), val);
	}
	else
	{
		log_error("role[%lu] add frame[%d] failed, already owned", get_uid(), val);
	}
}

uint32_t role_t::get_frame()
{
	return (uint32_t)(m_head_frame & 0xFFFF);
}

uint32_t role_t::get_head()
{
	return (uint32_t)((m_head_frame >> 16) & 0xFFFF);
}

uint32_t role_t::career_award(uint32_t career_id, proto::client::gc_career_award_reply& reply)
{
	auto p_conf = GET_CONF(Career, career_id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf Career[%d]", career_id);
		return errcode_enum::notice_unknown;
	}
	if (m_level < career_id)
	{
		log_error("role[%lu] Career[%d] level[%d] not enough", m_uid, career_id, m_level);
		return errcode_enum::error_level_not_enough;
	}
	auto it = std::find(m_career_id_vec.begin(), m_career_id_vec.end(), career_id);
	if (it != m_career_id_vec.end())
	{
		log_error("role[%lu] Career[%d] already award", m_uid, career_id);
		return errcode_enum::notice_unknown;
	}
	m_career_id_vec.push_back(career_id);
	save_self();
	drop_manager_t::drop(get_role(), p_conf->drop_id(), log_enum::source_type_role_career_award, proto::common::drop_sys_type_career, career_id, reply.mutable_rcd());
	get_personal_info(reply.mutable_rcd()->mutable_per_info());
	ROLE_LOG("role[%lu] award career[%d] success", m_uid, career_id);
	return errcode_enum::error_ok;
}

uint32_t role_t::handbook_award(uint32_t handbook_id, proto::client::gc_handbook_award_reply& reply)
{
	auto p_conf = GET_CONF(HandBook, handbook_id);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf HandBook[%d]", handbook_id);
		return errcode_enum::notice_unknown;
	}
	
	auto it = std::find(m_handbook_vec.begin(), m_handbook_vec.end(), handbook_id);
	if (it != m_handbook_vec.end())
	{
		log_error("role[%lu] handbook[%d] already award", m_uid, handbook_id);
		return errcode_enum::notice_unknown;
	}
	for (auto hero_id : p_conf->heros())
	{
		if (NULL == hero_manager_t::get_hero(get_role(), hero_id))
		{
			log_error("role[%lu] handbook[%d] hero[%d] not exist", m_uid, handbook_id, hero_id);
			return errcode_enum::notice_unknown;
		}
	}
	m_handbook_vec.push_back(handbook_id);
	if (p_conf->drop() > 0)
	{
		drop_manager_t::drop(get_role(), p_conf->drop(), log_enum::source_type_role_handbook_award, proto::common::drop_sys_type_handbook, handbook_id, reply.mutable_rcd());
	}
	if (!p_conf->attr_add().empty())
	{
		hero_manager_t::all_heros_update_attr(get_role(), reply.mutable_rcd());
	}
	get_personal_info(reply.mutable_rcd()->mutable_per_info());
	save_self();
	ROLE_LOG("role[%lu] award handbook[%d] success", m_uid, handbook_id);
	return errcode_enum::error_ok;
}

uint32_t role_t::change_name_request(const std::string& new_name)
{
	auto change_name_level = GET_CONF(Comprehensive, comprehensive_common::role_player_change_name_level);
	uint32_t need_level = GET_COMPREHENSIVE_VALUE_1(change_name_level);
	if (m_level < need_level)
	{
		log_error("role[%lu] level[%d] not enough, need[%d]", m_uid, m_level, need_level);
		return errcode_enum::error_level_not_enough;
	}
	uint32_t cost = 0;
	if (m_change_name > 0)
	{
		auto change_name_cost = GET_CONF(Comprehensive, comprehensive_common::role_change_name);
		cost = GET_COMPREHENSIVE_VALUE_1(change_name_cost);

		if (!money_manager_t::use_money(get_role(), proto::common::MONEY_TYPE_YUANBAO, cost, log_enum::source_type_role_change_name, 0))
		{
			return errcode_enum::notice_gold_money_not_enough;
		}
	}
	proto::server::gd_change_name_request req;
	req.set_new_name(new_name);
	req.set_cost(cost);
	env::server->send_msg_to_db(op_cmd::gd_change_name_request, m_uid, req);
	return errcode_enum::error_ok;
}

void role_t::add_title(uint32_t title_id)
{
	std::vector<uint32_t>::iterator it = std::find(m_title_id_vec.begin(), m_title_id_vec.end(), title_id);
	if (it == m_title_id_vec.end())
	{
		m_title_id_vec.push_back(title_id);
		m_cur_title = title_id;
		notify_object_common_info_for_nearbies();
		save_self();
	}
}

uint32_t role_t::change_title(uint32_t title_id)
{
	if (title_id > 0)
	{
		std::vector<uint32_t>::iterator it = std::find(m_title_id_vec.begin(), m_title_id_vec.end(), title_id);
		if (it == m_title_id_vec.end())
		{
			log_error("role[%lu] title[%d] not exist", m_uid, title_id);
			return errcode_enum::notice_unknown;
		}
	}
	
	m_cur_title = title_id;
	save_self();
	notify_object_common_info_for_nearbies();
	return errcode_enum::error_ok;
}

void role_t::change_temporary_title(uint32_t title_id, bool is_broadcast/* = true*/)
{
	// 有变化才广播
	bool is_change = false;
	do {
		if (!m_show_temporary_title) {
			m_show_temporary_title = true;
			is_change = true;
		}

		if (m_temporary_title != title_id) {
			m_temporary_title = title_id;
			is_change = true;
		}
	} while (0);

	if (is_broadcast && is_change) {
		notify_object_common_info_for_nearbies();
	}
}

bool role_t::is_team_leader()
{
	// 判断是否有队伍
	if (get_troop_id() == 0)
		return false;
	troop_ptr p_troop = game_troop_mgr_t::get_troop(get_troop_id());
	if (NULL == p_troop)
	{
		log_error("p_troop NULL troop_id[%lu]", get_troop_id());
		return false;
	}
	return p_troop->get_leader_id() == get_uid();
}

uint32_t role_t::get_team_member_size() {
	// 判断是否有队伍
	if (get_troop_id() == 0)
		return 0;

	troop_ptr p_troop = game_troop_mgr_t::get_troop(get_troop_id());
	if (NULL == p_troop)
	{
		log_error("p_troop NULL troop_id[%lu]", get_troop_id());
		return 0;
	}
	return p_troop->get_member_size();
}

bool role_t::get_team_member_list(std::vector<object_base_ptr> &team_member_list)
{
	// 判断是否有队伍
	if (get_troop_id() == 0)
		return false;
	troop_ptr p_troop = game_troop_mgr_t::get_troop(get_troop_id());
	if (NULL == p_troop)
	{
		log_error("p_troop NULL troop_id[%lu]", get_troop_id());
		return false;
	}
	troop_t::troop_member_vec member_list;
	p_troop->get_normal_members(member_list);
	object_base_ptr p_member = object_base_ptr();

	for (uint32_t i = 0; i < member_list.size(); ++i)
	{
		troop_t::troop_member_ptr p_troop_member = member_list[i];
		if (NULL == p_troop_member)
		{
			log_error("p_troop_member NULL");
			continue;
		}
		if (p_troop_member->get_state() != 0)
			continue;

		if (p_troop_member->get_uid() == get_uid())
			p_member = get_role();
		else
		{
			p_member = object_manager_t::find_object(p_troop_member->get_object_id_type());
		}
			

		if (NULL == p_member)
		{
			log_error("p_member_role NULL uid[%lu]", p_troop_member->get_uid());
			continue;
		}

		team_member_list.push_back(p_member);
	}

	// 生成队伍等级
	if (team_member_list.empty()) {
		log_error("check_fill_team_member member_list_size = 0");
		return false;
	}
	if (team_member_list.size() == 1)
		team_member_list.clear();
	return team_member_list.size() > 1;
}

void role_t::fill_hero_to_formation(formation_ptr& form, uint32_t team_member_size, bool needleader)
{
	uint32_t team_heroes_max_num = fight_manager_t::get_hero_count_by_member_size(team_member_size);

	std::vector<hero_ptr> result;
	hero_manager_t::get_all_heroes_by_fight_value(get_role(), result);

	std::map<uint32_t, PosHeroData> hero_pos;
	hero_ptr p_hero = hero_ptr();
	uint32_t hero_fight_pos = 0;
	// 队长必须上阵主公
	if (needleader)
	{
		// 加入主公
		hero_pos.insert(std::make_pair(++hero_fight_pos, PosHeroData( get_uid(), proto::common::SCENEOBJECT_USER, get_main_hero_uid() )));
		for (uint32_t i = 0; i < result.size(); ++i)
		{
			p_hero = result[i];
			if (NULL == p_hero)
			{
				log_error("user_troop fill_member_hero_to_formation p_hero NULL pos[%d]", i);
				continue;
			}
			// 排除主公 避免重复插入
			if (p_hero->get_unique_id() == get_main_hero_uid())
				continue;
			hero_pos.insert(std::make_pair(++hero_fight_pos, PosHeroData( get_uid(), proto::common::SCENEOBJECT_USER, p_hero->get_unique_id())));
			// 达到上阵上限
			if (hero_pos.size() >= team_heroes_max_num)
				break;
		}
	}
	else {
		uint32_t size = team_heroes_max_num > result.size() ? result.size() : team_heroes_max_num;
		// 非队长就默认上阵最强的
		for (uint32_t i = 0; i < size; ++i)
		{
			p_hero = result[i];
			if (NULL == p_hero) {
				log_error("user_troop fill_member_hero_to_formation p_hero NULL pos[%d]", i);
				continue;
			}

			//如果是主角只允许上阵当前主角
			if (p_hero->is_main_hero() && get_main_hero_uid() != p_hero->get_unique_id() ) {
				continue;
			}
			
			hero_pos.insert(std::make_pair(++hero_fight_pos, PosHeroData(get_uid(), proto::common::SCENEOBJECT_USER, p_hero->get_unique_id() )));
		}
	}
	m_formation_mgr.update_formation(get_role(), form->get_index(), proto::common::fight_type_none, hero_pos, 0);
}

bool role_t::get_hero_list(uint32_t fight_type, uint32_t team_member_size, std::vector<uint64_t>& list)
{
	formation_ptr form = get_form_data(fight_type, team_member_size);
	if (NULL == form)
		return false;
	std::map<uint32_t, PosHeroData>::const_iterator it;
	for (it = form->get_pos_hero().begin(); it != form->get_pos_hero().end(); ++it)
	{
		list.push_back(it->second.uid);
	}
	return true;
}

formation_ptr role_t::get_form_data(uint32_t fight_type, uint32_t team_member_size)
{
	formation_ptr form = formation_ptr();
	//uint32_t team_heroes_max_num = fight_manager_t::get_hero_count_by_member_size(team_member_size);
	if (team_member_size == 2)
	{
		if (is_team_leader())
			form = m_formation_mgr.get_formation(proto::common::formation_type_troop_leader_2);
		else
			form = m_formation_mgr.get_formation(proto::common::formation_type_troop_2);

		if (form->get_pos_hero().size() < 1)
		{
			fill_hero_to_formation(form, team_member_size, is_team_leader());
		}
	}
	else if (team_member_size == 3)
	{
		if (is_team_leader())
			form = m_formation_mgr.get_formation(proto::common::formation_type_troop_leader_3);
		else
			form = m_formation_mgr.get_formation(proto::common::formation_type_troop_3);
		if (form->get_pos_hero().size() < 1)
		{
			fill_hero_to_formation(form, team_member_size, is_team_leader());
			//p_troop->fill_member_hero_to_formation()
		}
	}
	else
	{
		switch (fight_type)
		{
		case proto::common::fight_type_expedition:
			return m_formation_mgr.get_formation(proto::common::formation_type_expedition);
		case proto::common::fight_type_family_war:
		case proto::common::fight_type_family_pk:
			return m_formation_mgr.get_formation(proto::common::formation_type_family);
		default:
			return m_formation_mgr.get_cur_formation();
		}
	}
	return form;
}

bool role_t::fill_fight_camp(
	uint32_t camp_index,
	uint64_t unique_offset,
	uint32_t team_member_size,
	proto::common::fight_camp* camp,
	proto::common::fight_type fight_type,
	proto::common::hero_state_data* state,
	int32_t team_member_pos)
{
	if (NULL == camp)
	{
		log_error("_fill_fight_camp role camp is null");
		return false;
	}
	formation_ptr form = get_form_data(fight_type, team_member_size);
	if (NULL == form)
	{
		log_error("_fill_fight_camp form is null");
		return false;
	}

	bool ret = true;
	camp->set_camp(camp_index);
	

	camp->set_tactic(form->get_tactic_id());
	proto::common::obj_data* od = camp->mutable_objs();
	ret &= fill_obj_single(
		camp_index, 
		od->add_obj(), 
		fight_type,
		team_member_size,
		team_member_pos);
	ret &= fill_hero_data(
		camp_index, 
		unique_offset, 
		team_member_size, 
		fight_type, 
		camp->mutable_heroes(), 
		state, 
		team_member_pos);
	if (ret == true && team_member_size > 1)
	{
		troop_ptr p_troop = game_troop_mgr_t::get_troop(get_troop_id());
		if (NULL != p_troop)
		{
			p_troop->notify_formation();
		}
	}
	return ret;
}

uint32_t role_t::get_camp_unique_offset(uint32_t camp)
{
	if (camp == proto::common::combat_camp_1)
		return CAMP_SELF_UNIQUE_OFFSET;
	else
		return CAMP_ENEMY_UNIQUE_OFFSET;
}

// 填充武将列表
bool role_t::fill_hero_data(
	uint32_t camp_index,
	uint64_t unique_offset,
	uint32_t team_member_size,
	proto::common::fight_type fight_type,
	proto::common::hero_data* fhd,
	proto::common::hero_state_data* state,
	int32_t team_member_pos)
{
	formation_ptr form = get_form_data(fight_type, team_member_size);
	if (NULL == form)
	{
		log_error("_fill_fight_camp form is null");
		return false;
	}
	if (form == NULL)
	{
		log_error("form null error");
		return false;
	}

	if (fhd == NULL)
	{
		log_error("fhd null error");
		return false;
	}

	uint32_t count = fight_manager_t::get_hero_count_by_member_size(team_member_size);

	bool ret = true;
	const std::map<uint32_t, PosHeroData>& pos_hero = form->get_pos_hero();
	uint32_t index = 0;
	fight_manager_t::clear_hero_single_by_master_id(fhd, get_uid());

	for (auto pos_data : pos_hero)
	{
		uint32_t pos = (pos_data.first);
		if (team_member_pos >= 0)
			pos = team_member_pos + index;
		uint64_t hero_uid = pos_data.second.uid;

		hero_ptr hero = get_hero_by_unique_id(hero_uid);
		if (hero == NULL)
		{
			// 如果是多人战斗 阵上会有队友的武将
			//log_error("formation [%u] pos [%u] hero [%lu] not exists", form->get_index(), pos, hero_uid);
			continue;
		}

		//uint64_t fight_uid = unique_offset + pos;
		
		// 判断
		proto::common::hero_single* single = fhd->add_hero_list();
		if (single == NULL)
		{
			continue;
		}

		proto::common::hero_state* hs = fight_manager_t::get_hero_state( state, pos );

		ret &= fill_hero_single(
			camp_index, 
			unique_offset, 
			hero, 
			single, 
			pos, 
			fight_type, 
			hs);
		++index;
		if (index >= count)
			break;
	}

	return ret;
}

hero_ptr role_t::get_hero_by_unique_id(uint64_t unique_id)
{
	auto it = m_hero_list.find(unique_id);
	if (it != m_hero_list.end())
	{
		return it->second;
	}

	return hero_ptr(NULL);
}

bool role_t::fill_hero_single(
	uint32_t camp_index,
	uint64_t unique_offset,
	hero_ptr p_hero,
	proto::common::hero_single* single,
	uint32_t pos,
	proto::common::fight_type fight_type,
	proto::common::hero_state* state)
{
	if (p_hero == NULL)
	{
		log_error("p_hero null error");
		return false;
	}

	if (single == NULL)
	{
		log_error("single null error");
		return false;
	}

	bool ret = true;
	p_hero->peek_data(single, true);
	fill_hero_skill(fight_type, p_hero, single);

	proto::common::fight_attr* fight = single->mutable_fight_attr();
	fight->set_master(string_util_t::uint64_to_string(get_uid()));
	fight->set_pos(pos);
	fight->set_camp(camp_index);
	if (NULL != state)
	{
		fight->mutable_start()->CopyFrom(*state);
	}

	uint64_t fight_uid = unique_offset + pos;
	proto::common::base_attr* base = single->mutable_base_attr();
	base->set_fight_hero_uid(string_util_t::uint64_to_string(fight_uid)); // 战斗时的唯一ID为起始ID+pos

	if (fight_type == proto::common::fight_type_expedition)
	{
		expedition_ptr p_expedition = get_expedition();
		if (NULL != p_expedition)
		{
			single->set_save_hp(p_expedition->get_hp(p_hero->get_unique_id()));
		}
	}
	return ret;
}

bool role_t::fill_hero_skill(
	proto::common::fight_type fight_type, 
	hero_ptr p_hero, 
	proto::common::hero_single* single)
{
	if (p_hero == NULL)
	{
		log_error("p_hero is null");
		return false;
	}

	if (single == NULL)
	{
		log_error("single is null");
		return false;
	}
	single->clear_skill();
	proto::common::skill_data* sd = single->mutable_skill();
	proto::common::skill_single* skill = NULL;

	std::vector<hero_skill> hero_skills;
	p_hero->get_fight_skill_list(hero_skills);

	for (uint32_t i = 0; i < hero_skills.size(); ++i)
	{
		if (hero_skills[i].skill_level == 0)
			continue;
		
		skill = sd->add_skills();
		fill_hero_skill_data(
			skill, 
			fight_type, 
			p_hero->get_unique_id(), 
			hero_skills[i].skill_id,
			hero_skills[i].skill_level, 
			hero_skills[i].skill_index);
	}
	return true;
}

bool role_t::fill_hero_skill_data(
	proto::common::skill_single* skill,
	proto::common::fight_type fight_type, 
	uint64_t hero_unique_id, 
	uint32_t skill_id,
	uint32_t skill_level, 
	uint32_t skill_type)
{
	if (NULL == skill)
	{
		log_error("fill_hero_skill_data skill NULL");
		return false;
	}

	SkillEffectTable *p_conf = NULL;

	skill->set_skill_id(skill_id);
	skill->set_skill_level(skill_level);
	//	skill->set_skill_type(skill_type);
	p_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill_id, skill_level));
	if (p_conf == NULL)
	{
		log_error("fill_hero_skill_data p_conf is null role[%lu] skill_id[%d] skill_level[%d]", m_uid, skill_id, skill_level);
		return false;
	}
	else
	{
		skill->set_skill_cd(p_conf->start_cd());
	}

	if (fight_type == proto::common::fight_type_expedition)
	{
		expedition_ptr p_expedition = get_expedition();
		if (NULL != p_expedition)
		{
			int32_t skill_cd = p_expedition->get_skill_cd(hero_unique_id, skill_id);
			if (skill_cd != -1)
				skill->set_skill_cd(skill_cd);
		}
	}

	return true;
}

item_ptr role_t::get_item_by_uid(uint64_t uid, proto::common::package_type package)
{
	return m_package_manager.get_item_by_uid(uid, package);
}

hero_map* role_t::get_hero_list()
{
	return &m_hero_list;
}

bool role_t::fill_obj_single(
	uint32_t camp_index, 
	proto::common::obj_single* single, 
	proto::common::fight_type fight_type,
	uint32_t team_member_size,
	int32_t team_member_pos)
{
	if (single == NULL)
	{
		log_error("single is null");
		return false;
	}

	single->set_object_uid(string_util_t::uint64_to_string(get_uid()));
	single->set_object_type(get_object_type());
	single->set_camp(camp_index);
	single->set_name(get_name());
	single->set_level(get_level());
	single->set_head_id(get_head_frame());

	single->set_team_member_size(team_member_size);
	single->set_team_member_pos(team_member_pos);
	single->set_auto_ignore_lineup(is_auto_ignore_lineup(fight_type) ? 1 : 0);
	// PVE 保持上次战斗的自动战斗选择
	if (combat_t::auto_fight_inherit(fight_type))
	{
		single->set_auto_fight(get_auto_fight_flag());
	}

	return true;
}

void role_t::add_statistical_count(uint32_t id, uint32_t count, uint32_t option )
{
	switch (option)
	{
		case proto::common::STATISTICAL_OPTION::STATISTICAL_OPTION_ADD:
		{
			m_statistical_map[id] += count;
		}
		break;
		case proto::common::STATISTICAL_OPTION::STATISTICAL_OPTION_REPLACE:
		{
			m_statistical_map[id] = count;
		}
		break;
		case proto::common::STATISTICAL_OPTION::STATISTICAL_OPTION_CMP_REPLACE:
		{
			if (m_statistical_map[id] >= count)
				return;
			m_statistical_map[id] = count;
		}
		break;
	}

	//存储
	save_statistical_data();
}

uint32_t role_t::get_statistical_count(uint32_t id)
{
	auto iter_f = m_statistical_map.find(id);
	if ( iter_f == m_statistical_map.end() )
		return 0;
	return iter_f->second;
}

void role_t::save_statistical_data()
{
	proto::common::statistical_save_data data;

	for (auto it : m_statistical_map)
	{
		proto::common::statistical_save_info *p_info = data.add_info();

		p_info->set_id(it.first);
		p_info->set_num(it.second);
	}
	role_unify_save::add_task(get_uid(), m_statistical_key, data);
}

void role_t::load_statistical_data(const proto::common::statistical_save_data &data)
{
	m_statistical_map.clear();
	for (int i = 0; i < data.info_size(); ++i)
	{
		const proto::common::statistical_save_info &p_info = data.info(i);
		m_statistical_map[p_info.id()] = p_info.num();
	}
}

bool role_t::is_limit_level()
{
	uint32_t task_id = general_info_manager_t::get_limit_task( get_level() );
	if (task_id == 0)
		return false;
	
	return m_task_mgr->is_done(task_id) == false;
}

uint32_t role_t::get_limit_exp()
{
	return general_info_manager_t::get_limit_exp(get_level());
}


bool role_t::is_auto_ignore_lineup(uint32_t fight_type)
{
	// 判断这次战斗是否可以跳过布阵
	if (!combat_t::ignore_lineup(fight_type))
		return false;

	// 判断这个角色是否设置了  跳过布阵
	if (!get_ignore_lineup_flag())
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////



