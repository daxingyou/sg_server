#ifndef __LOG_LOG_DATA_HPP__
#define __LOG_LOG_DATA_HPP__

#include <boost/asio/deadline_timer.hpp>
#include "logic_log/role_log.hpp"
#include "logic_log/item_log.hpp"
#include "logic_log/money_log.hpp"
#include "logic_log/luck_gift_log.hpp"
#include "logic_log/hero_log.hpp"
#include "logic_log/task_log.hpp"
#include "logic_log/prestige_log.hpp"
#include "logic_log/country_log.hpp"
#include "logic_log/family_log.hpp"
#include "logic_log/friend_log.hpp"
#include "logic_log/mail_log.hpp"
#include "logic_log/pk_log.hpp"
#include "logic_log/shop_log.hpp"
#include "logic_log/equip_log.hpp"
#include "logic_log/lifeforce_log.hpp"
#include "logic_log/achieve_log.hpp"
#include "logic_log/dungeon_log.hpp"
#include "logic_log/role_buff_log.hpp"
#include "logic_log/account_log.hpp"
#include "logic_log/goods_log.hpp"
#include "logic_log/mobile_log.hpp"
#include "logic_log/trade_log.hpp"
#include "logic_log/new_guide_log.hpp"
#include "logic_log/recharge_log.hpp"
#include "logic_log/expedition_log.hpp"
#include "logic_log/random_shop_log.hpp"
#include "logic_log/scene_log.hpp"
#include "logic_log/role_exp_log.hpp"
#include "logic_log/combat_log.hpp"
#include "logic_log/hero_exp_log.hpp"
#include "logic_log/redbag_log.hpp"

class log_data_t
{
public:
    log_data_t(boost::asio::io_service& ios, uint32_t update_to_db_time);
    ~log_data_t();

    void start_timer();
    void close_timer();
    void timer_callback(const boost::system::error_code& e);
    void post_persistency();
    void handle_persistency();

    void incre_log_num() { m_cur_logs_num += 1;}
    void reset_log_num() { m_cur_logs_num = 0; }

    void flush_all();

private:
    boost::asio::deadline_timer m_timer;
    uint32_t m_cur_logs_num = 0;
    uint32_t m_last_save_time = 0;
    uint32_t m_update_to_db_time;

    //单组
public:
    role_log_manager_t      m_role_log_mgr;
	item_log_manager_t      m_item_log_mgr;
	money_log_manager_t     m_money_log_mgr;
	luck_gift_log_manager_t	m_luck_gift_log_mgr;
	hero_log_manager_t		m_hero_log_mgr;
	task_log_manager_t		m_task_log_mgr;
	prestige_log_manager_t	m_prestige_log_mgr;
    country_log_manager_t   m_country_log_mgr;
    family_log_manager_t    m_family_log_mgr;
    friend_log_manager_t    m_friend_log_mgr;
	mail_log_manager_t	m_mail_log_mgr;
	pk_log_manager_t     m_pk_log_mgr;
    shop_log_manager_t m_shop_log_mgr;
	equip_log_manager_t     m_equip_log_mgr;
	achieve_log_manager_t m_achieve_log_mgr;
	lifeforce_log_manager_t m_lifeforce_log_mgr;
	dungeon_log_manager_t m_dungeon_log_mgr;
	role_buff_log_manager_t m_role_buff_log_mgr;
	goods_log_manager_t m_goods_log_mgr;
	new_guide_log_manager_t m_new_guide_log_mgr;
	trade_log_manager_t m_trade_log_mgr;
	recharge_log_manager_t m_recharge_log_mgr;
	expedition_log_manager_t m_expedition_log_mgr;
	random_shop_log_manager_t m_random_shop_log_mgr;
	scene_log_manager_t m_scene_log_mgr;
	role_exp_log_manager_t m_role_exp_log_mgr;
	combat_log_manager_t m_combat_log_mgr;
	hero_exp_log_manager_t m_hero_exp_log_mgr;
	redbag_log_manager_t m_redbag_log_mgr;
    //全局
public:
    account_log_manager_t m_account_log_mgr;
    mobile_log_manager_t m_mobile_log_mgr;
};

#endif
