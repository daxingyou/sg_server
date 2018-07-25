#include "role_unify_save.hpp"

std::map<const std::string, data_block> role_unify_save::m_kv_list;
boost::shared_ptr<boost::asio::deadline_timer> role_unify_save::m_save_to_db_timer = NULL;

bool role_unify_save::init()
{
    m_kv_list.clear();
    start_save_to_db_timer();
	return true;
}

void role_unify_save::save(SAVE_TYPE type)
{
    if (m_kv_list.empty())
    {
        return;
    }

    proto::server::gd_batch_save_to_db_notify bstd;
    for (auto it : m_kv_list)
    {
        data_block& db = it.second;
        proto::server::save_to_db* std = bstd.add_datas();
        std->set_uid(db.uid);
        std->set_key(db.key);
        std->set_data(db.value);

        if ((uint32_t)bstd.ByteSize() >= network::MAX_UZLIB_CHAR / 2)
        {
            //超过压缩大小的一半存
            bstd.set_type(SAVE_TYPE_UZLIB_SIZE);
            env::server->send_msg_to_db(op_cmd::gd_batch_save_to_db_notify, bstd);

            proto::server::gd_batch_save_to_db_notify temp;
            bstd.Swap(&temp);
        }
    }
    bstd.set_type(type);
    env::server->send_msg_to_db(op_cmd::gd_batch_save_to_db_notify, bstd);

    m_kv_list.clear();
}

void role_unify_save::start_save_to_db_timer()
{
    uint32_t role_unify_save_interval = env::xml->max_role_data_save_time;
    if (role_unify_save_interval <= 0)
    {
        return;
    }

    if (NULL == m_save_to_db_timer)
    {
        m_save_to_db_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
    }
    m_save_to_db_timer->expires_from_now(boost::posix_time::seconds(role_unify_save_interval));
    m_save_to_db_timer->async_wait(boost::bind(&role_unify_save::save_to_db_timer_callback, boost::asio::placeholders::error));
}

void role_unify_save::save_to_db_timer_callback(const boost::system::error_code& ec)
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

    //定时存
	save(SAVE_TYPE_TIME);
	
    start_save_to_db_timer();
}

void role_unify_save::stop_save_to_db_timer()
{
	if (NULL == m_save_to_db_timer)
	{
		return;
	}
	m_save_to_db_timer->cancel();
}