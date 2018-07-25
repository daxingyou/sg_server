#ifndef __CHAT_USER_HPP__
#define __CHAT_USER_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "main/chat_server.hpp"
#include "main/chat_server_fwd.hpp"
#include "gate/gate_manager.hpp"
#include "game/game_manager.hpp"

class user_t : public boost::enable_shared_from_this<user_t>
{
public:
    user_t(uint64_t uid, const std::string& name, uint32_t country_id, uint64_t family_id)
        : m_uid(uid)
        , m_nickname(name)
        , m_country_id(country_id)
        , m_family_id(family_id)
        {}
    ~user_t() {}

    template<typename T_MSG>
    void send_msg_to_client(uint16_t cmd, const T_MSG& msg)
    {
        env::server->send_msg_to_gate(gate_manager_t::get_gate_socket_by_id(m_gate_id), cmd, m_uid, msg);
    }

    template<typename T_MSG>
    void send_msg_to_game(uint16_t cmd, const T_MSG& msg)
    {
        env::server->send_msg_to_game(game_manager_t::get_game_socket_by_id(m_game_id), cmd, m_uid, msg);
	}

	template<typename T_MSG>
	void send_msg_to_center(uint16_t cmd, const T_MSG& msg)
	{
		env::server->send_msg_to_center(cmd, m_uid, msg);
	}

	void set_uid(const uint64_t uid) { m_uid = uid; }
    const uint64_t get_uid() const { return m_uid; }

	uint32_t get_country_id() { return m_country_id; }
	void set_country_id(uint32_t country_id) { m_country_id = country_id; }

	uint64_t get_family_id() { return m_family_id; }
	void set_family_id(uint64_t family_id) { m_family_id = family_id; }
	
	std::string get_nickname() { return m_nickname; }

    void set_gate_id(const uint32_t val) { m_gate_id = val; }
    const uint32_t get_gate_id() const { return m_gate_id; }

    void set_game_id(const uint32_t val) { m_game_id = val; }
    const uint32_t get_game_id() const { return m_game_id; }

private:
    uint64_t m_uid = 0;
	uint32_t m_country_id = 0;
	uint64_t m_family_id = 0;
	std::string m_nickname = "";
    uint32_t m_gate_id = 0;
    uint32_t m_game_id = 0;
};
typedef boost::shared_ptr<user_t> user_ptr;
typedef std::set<uint64_t> user_set_type;
#endif
