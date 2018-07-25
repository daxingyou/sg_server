#include "log_data.hpp"
#include "main/log_server.hpp"
#include "main/log_server_fwd.hpp"
#include "zdb_ex.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

log_data_t::log_data_t(boost::asio::io_service& ios, uint32_t update_to_db_time)
    : m_timer(ios)
    , m_update_to_db_time(update_to_db_time)
{

}

log_data_t::~log_data_t()
{

}

void log_data_t::start_timer()
{
    m_timer.expires_from_now(boost::posix_time::seconds(m_update_to_db_time));
    m_timer.async_wait(boost::bind(&log_data_t::timer_callback, this, boost::asio::placeholders::error));
}

void log_data_t::close_timer()
{
    m_timer.cancel();
}

void log_data_t::timer_callback( const boost::system::error_code& e )
{
    if (e == boost::asio::error::operation_aborted)
    {
        
        return;
    }
    else if (e)
    {
        log_error("Timer error: %s", e.message().c_str());
        return;
    }

    handle_persistency();

    start_timer();
}

void log_data_t::handle_persistency()
{
    Connection_T conn = zdb_util_t::get_zdb_conn(env::conn_zdb_pool);
    if (NULL == conn)
    {
        log_error("NULL == conn");
        return;
    }

    //int start_cur = clock();

    TRY
    {
        if (env::server->get_type() == log_server_type_global)
        {
            m_mobile_log_mgr.update_to_db(conn);
            m_account_log_mgr.update_to_db(conn);
        }
        else if (env::server->get_type() == log_server_type_single)
        {
            m_role_log_mgr.update_to_db(conn);
            m_item_log_mgr.update_to_db(conn);
            m_money_log_mgr.update_to_db(conn);
			m_prestige_log_mgr.update_to_db(conn);
            m_luck_gift_log_mgr.update_to_db(conn);
            m_hero_log_mgr.update_to_db(conn);
            m_task_log_mgr.update_to_db(conn);
            m_country_log_mgr.update_to_db(conn);
            m_family_log_mgr.update_to_db(conn);
            m_friend_log_mgr.update_to_db(conn);
            m_mail_log_mgr.update_to_db(conn);
            m_pk_log_mgr.update_to_db(conn);
            m_shop_log_mgr.update_to_db(conn);
			m_equip_log_mgr.update_to_db(conn);
            m_achieve_log_mgr.update_to_db(conn);
            m_lifeforce_log_mgr.update_to_db(conn);
            m_dungeon_log_mgr.update_to_db(conn);
            m_role_buff_log_mgr.update_to_db(conn);
			m_goods_log_mgr.update_to_db(conn);
			m_trade_log_mgr.update_to_db(conn);
			m_new_guide_log_mgr.update_to_db(conn);
			m_recharge_log_mgr.update_to_db(conn);
			m_rmb_log_mgr.update_to_db(conn);
			m_expedition_log_mgr.update_to_db(conn);
			m_random_shop_log_mgr.update_to_db(conn);
			m_scene_log_mgr.update_to_db(conn);
			m_role_exp_log_mgr.update_to_db(conn);
			m_combat_log_mgr.update_to_db(conn);
			m_hero_exp_log_mgr.update_to_db(conn);
			m_redbag_log_mgr.update_to_db(conn);
			m_patrol_hire_log_mgr.update_to_db(conn);
			m_offline_arena_rank_log_mgr.update_to_db(conn);
        }
        else
        {
            log_error("log server type[%d] error!", env::server->get_type());
        }
    }
    CATCH(SQLException)
    {
        log_error("save log data mysql-exception: exception->name: %s | exception_message: %s", Exception_frame.exception->name, Exception_frame.message);
    }
    FINALLY
    {
        Connection_close(conn);

//         int32_t end_cur = clock();
//         int32_t diff= (end_cur - start_cur)/(CLOCKS_PER_SEC/1000);
//         if (diff > 10)
//         {
//             log_info("save logs to db: time[%d]", diff);
//         }
    }
    END_TRY;

}

void log_data_t::flush_all()
{
    handle_persistency();
}
