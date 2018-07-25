#ifndef __GAME_FSM_XML_MANAGER_H__
#define __GAME_FSM_XML_MANAGER_H__
#include "macros.hpp"

#include <map>
#include "server.pb.h"
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "fsm_ai_define.hpp"

struct ai_state_conf
{
	ai_state_conf(uint32_t id) : m_id(id) {};

	uint32_t m_id = 0;
	fsm::ai_state m_states;
};

typedef boost::shared_ptr<ai_state_conf> ai_state_conf_ptr;
typedef std::map<uint32_t, ai_state_conf_ptr> ai_state_conf_map;

class fsm_xml_manager_t
{
public:
	static bool init();
	static void clear();
	static bool reload();

	static ai_state_conf_ptr get_fsm_ai_state(uint32_t id);

	static void load_file(uint32_t id, const char* filename);

	static proto::common::object_state_type get_state_type(const std::string& name);

protected:
	static void load_state(const boost::property_tree::ptree& pt, fsm::ai_state& state);
	static ai::ai_event get_event_type(const std::string& name);
	static ai::ai_act get_act_type(const std::string& name);
	static ai::ai_mode get_mode_type(const std::string& name);

protected:
	static ai_state_conf_map m_ai_state_conf_map;

public:
	typedef std::map<std::string, ai::ai_event> name2eventtype;
	static name2eventtype m_name2eventtype;

	typedef std::map<std::string, ai::ai_act> name2acttype;
	static name2acttype m_name2acttype;

	typedef std::map<std::string, proto::common::object_state_type> name2statetype;
	static name2statetype m_name2statetype;

	typedef std::map<std::string, ai::ai_mode> name2modetype;
	static name2modetype m_name2modetype;
};

#endif
