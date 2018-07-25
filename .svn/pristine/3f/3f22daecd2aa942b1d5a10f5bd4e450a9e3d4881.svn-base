#ifndef __CHAT_USER_UNIFY_SAVE_HPP__
#define __CHAT_USER_UNIFY_SAVE_HPP__

#include "main/chat_server.hpp"
#include "main/chat_server_fwd.hpp"
#include "protos_fwd.hpp"

static const uint32_t user_unify_save_max = 30;

struct data_block
{
    std::string key;
    std::string value;
    bool new_flag;
};

class user_unify_save
{
public:
    static void reset(uint64_t uid)
    {
        m_idx = 0;
        m_uid = uid;
        m_kv_vec.clear();
        m_kv_vec.resize(user_unify_save_max);
        m_ki_map.clear();
    }

    template<typename T>
    static void add_task(const std::string& key, T msg, bool new_flag = false)
    {
        if (m_idx < user_unify_save_max)
        {
            std::pair<std::map<std::string, uint32_t>::iterator, bool> res = m_ki_map.insert(std::make_pair(key, m_idx));
            if (res.second)
            {
                m_kv_vec[m_idx].key = key;
                m_kv_vec[m_idx].new_flag = new_flag;
                network::wrap_msg(m_kv_vec[m_idx].value, msg);
                ++m_idx;

                if (m_idx >= user_unify_save_max)
                {
                    save();
                    uint64_t uid = m_uid;
                    reset(uid);
                }
            }
            else
            {
                uint32_t idx = res.first->second;
                m_kv_vec[idx].value.assign("");
                network::wrap_msg(m_kv_vec[idx].value, msg);
            }
        }
        else
        {
            log_error("add save user key[%s] failed!", key.c_str());
            return;
        }
    }

    static void save()
    {
        if (m_idx <= 0)
        {
            return;
        }

        proto::server::batch_save_to_db_notify bstd;
        for (uint32_t i = 0;i < m_idx; ++i)
        {
            proto::server::save_to_db* std = bstd.add_datas();
            std->set_key(m_kv_vec[i].key);
            std->set_data(m_kv_vec[i].value);
            std->set_new_flag(m_kv_vec[i].new_flag);
        }
        //env::server->send_msg_to_db(op_cmd::hd_batch_save_to_db_notify, m_uid, bstd);
    }

private:
    static uint32_t m_idx;
    static uint64_t m_uid;
    static std::vector<data_block> m_kv_vec;
    static std::map<std::string, uint32_t> m_ki_map;
};


#endif
