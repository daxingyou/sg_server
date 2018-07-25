#ifndef __GAME_ROLE_UNIFY_SAVE_HPP__
#define __GAME_ROLE_UNIFY_SAVE_HPP__

#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "protos_fwd.hpp"

static const uint32_t role_unify_save_max = 30;
static const uint32_t role_unify_import_data_save_max = 10;

struct data_block
{
    uint64_t uid;
    std::string key;
    std::string value;
    bool new_flag;
};

class role_unify_save
{
public:
	role_unify_save() { reset(); }
	~role_unify_save() {};

    template<typename T>
    static void add_task(const std::string& key, T& msg, bool new_flag, bool is_right_now, uint64_t uid)
    {
		data_block data;
		data.uid = uid;
		data.key = key;
		data.new_flag = new_flag;
		network::wrap_msg(data.value, msg);

		m_kv_vec.push_back(data);

		if (new_flag || is_right_now || m_kv_vec.size() >= role_unify_save_max)
		{
			save();
		}
    }

    static void save()
    {
		if (m_kv_vec.empty())
		{
			return;
		}

        proto::server::batch_save_to_db_notify bstd;
        for (uint32_t i = 0;i < m_kv_vec.size(); ++i)
        {
			proto::server::save_to_db* std = bstd.add_datas();
            std->set_uid(m_kv_vec[i].uid);
            std->set_key(m_kv_vec[i].key);
			std->set_data(m_kv_vec[i].value);
			std->set_new_flag(m_kv_vec[i].new_flag);
        }

        env::server->send_msg_to_db(op_cmd::gd_batch_save_to_db_notify, bstd);
		reset();
    }

protected:

	static void reset()
	{
		m_kv_vec.clear();
	}

private:

	static std::vector<data_block> m_kv_vec;
};


#endif
