#ifndef __CENTER_LOG_UNIFY_SAVE_HPP__
#define __CENTER_LOG_UNIFY_SAVE_HPP__

#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"

struct log_data_block
{
    uint32_t cmd;
    std::string value;
};

class log_unify_save
{
public:

	log_unify_save() { reset();  }
	~log_unify_save() {};
    
    template<typename T_MSG>
    static void add_log(uint32_t cmd, T_MSG msg)
    {
		log_data_block data;
		data.cmd = cmd;
		network::wrap_msg(data.value, msg);
		m_kv_vec.push_back(data);

		if (m_kv_vec.size() >= env::xml->db_log_count_limit)
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

        proto::log::eo_batch_save_logs_notify msg;
        for (uint32_t i = 0;i < m_kv_vec.size(); ++i)
        {
            proto::log::save_to_log* stl = msg.add_logs();
            stl->set_cmd(m_kv_vec[i].cmd);
            stl->set_data(m_kv_vec[i].value);
        }
        env::server->send_msg_to_log(op_cmd::eo_batch_save_logs_notify, msg);
		reset();
    }

protected:

	static void reset()
	{
		m_kv_vec.clear();
	}

private:

    static std::vector<log_data_block> m_kv_vec;
};

#define write_new_log(cmd, msg) log_unify_save::add_log(cmd, msg);

#endif
