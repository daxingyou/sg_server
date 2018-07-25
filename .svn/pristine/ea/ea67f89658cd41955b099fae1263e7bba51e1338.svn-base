#ifndef __GAME_ROLE_UNIFY_SAVE_HPP__
#define __GAME_ROLE_UNIFY_SAVE_HPP__

#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "protos_fwd.hpp"

struct data_block
{
    uint64_t uid;
    std::string key;
    std::string value;
};

enum SAVE_TYPE : uint32_t
{
    SAVE_TYPE_ERROR                 = 0,
    SAVE_TYPE_LIST_SIZE             = 1, //达到容器指定数量存
    SAVE_TYPE_IMPORTANT             = 2, //重要数据立刻存
    SAVE_TYPE_UZLIB_SIZE            = 3, //超过压缩大小的一半存
    SAVE_TYPE_TIME                  = 4, //定时存
    SAVE_TYPE_LEAVE_CLOSE_GAME      = 5, //关闭game|角色跳场景|角色下线 所有数据立刻存
};

class role_unify_save
{
public:
	static bool init();
    static void save(SAVE_TYPE type);

    template<typename T>
	static void add_task(uint64_t uid, const std::string& key, T& msg, bool is_right_now=false)
	{
        if (uid <= 0)
        {
            log_error("uid is 0 !");
            return;
        }
        if (key.empty())
        {
            log_error("key is empty !");
            return;
        }

		data_block data;
		data.uid = uid;
		data.key = key;
		network::wrap_msg(data.value, msg);

		if (is_right_now)
		{
            //重要数据立刻存
            proto::server::gd_batch_save_to_db_notify bstd;
            proto::server::save_to_db* std = bstd.add_datas();
            std->set_uid(uid);
            std->set_key(key);
            std->set_data(data.value);
            bstd.set_type(SAVE_TYPE_IMPORTANT);
            env::server->send_msg_to_db(op_cmd::gd_batch_save_to_db_notify, bstd);
		}
        else
        {
            m_kv_list[key] = data;
        }

        //达到容器指定数量存
        if (env::xml->max_role_data_save_size > 0 && m_kv_list.size() >= env::xml->max_role_data_save_size)
        {
            save(SAVE_TYPE_LIST_SIZE);
        }
	}
   
private:
    static std::map<const std::string, data_block> m_kv_list;

public:
	static void start_save_to_db_timer();
	static void save_to_db_timer_callback(const boost::system::error_code& ec);
	static void stop_save_to_db_timer();
private:
	static boost::shared_ptr<boost::asio::deadline_timer> m_save_to_db_timer;
};


#endif
