#ifndef __GAME_TOWER_TRIGGER_H__
#define __GAME_TOWER_TRIGGER_H__
#include <boost/shared_ptr.hpp>
#include "protos_fwd.hpp"

class tower_trigger_t
{
public:
	tower_trigger_t(){}

	~tower_trigger_t(){}

	// ¥Ê¥¢”Î∂¡»°
public:
	void load_data(const proto::common::tower_trigger& single);
	void peek_data(proto::common::tower_trigger* p_single);
public:
	void set_uid(uint64_t val) { m_uid = val; }
	void set_tid(uint32_t val) { m_tid = val; }
	void set_expired_time(uint32_t val) { m_expired_time = val; }
	void set_finder_name(const std::string& val) { m_finder_name = val; }

	uint64_t get_uid() { return m_uid; }
	uint32_t get_tid() { return m_tid; }
	uint32_t get_expired_time() { return m_expired_time; }
	const std::string& get_finder_name() { return m_finder_name; }
private:
	uint64_t m_uid = 0;
	uint32_t m_tid = 0;
	uint32_t m_expired_time = 0;
	std::string m_finder_name = "";
};

typedef boost::shared_ptr<tower_trigger_t> tower_trigger_ptr;
typedef std::map<uint64_t, tower_trigger_ptr> tower_trigger_map;
#endif // !__GAME_TOWER_TRIGGER_H__
