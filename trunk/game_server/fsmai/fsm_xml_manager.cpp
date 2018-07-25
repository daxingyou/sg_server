#include "fsm_xml_manager.hpp"

#include <dirent.h>
#include <boost/lexical_cast.hpp>
#include <boost/assign/std/map.hpp>
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "log.hpp"

ai_state_conf_map fsm_xml_manager_t::m_ai_state_conf_map;
fsm_xml_manager_t::name2eventtype fsm_xml_manager_t::m_name2eventtype;
fsm_xml_manager_t::name2acttype fsm_xml_manager_t::m_name2acttype;
fsm_xml_manager_t::name2statetype fsm_xml_manager_t::m_name2statetype;
fsm_xml_manager_t::name2modetype fsm_xml_manager_t::m_name2modetype;

bool fsm_xml_manager_t::init()
{
	boost::assign::insert(m_name2eventtype)
		("killed", ai::ai_event_killed)
		("enter_fight", ai::ai_event_enter_fight)
		("win_fight", ai::ai_event_win_fight);

	boost::assign::insert(m_name2acttype)
		("death", ai::ai_act_death)
		("disappear", ai::ai_act_disappear)
		("delete", ai::ai_act_delete)
		("revive", ai::ai_act_revive)
		("start_alert", ai::ai_act_start_alert)
		("start_fight", ai::ai_act_start_fight)
		("change_state", ai::ai_act_change_state);

	boost::assign::insert(m_name2statetype)
		("normal", proto::common::object_state_normal)
		("death", proto::common::object_state_death)
		("fight", proto::common::object_state_fighting)
		("alert", proto::common::object_state_alert);

	boost::assign::insert(m_name2modetype)
		("timer_disappear", ai::ai_mode_time_disappear)
		("timer_revive", ai::ai_mode_time_revive)
		("trigger_alert", ai::ai_mode_trigger_alert)
		("timer_alert", ai::ai_mode_timer_alert);

	return reload();
}

void fsm_xml_manager_t::clear()
{
	m_ai_state_conf_map.clear();
}

bool fsm_xml_manager_t::reload()
{
	clear();

	if (env::xml == NULL)
	{
		log_error("env::xml null error");
		return false;
	}

	std::string dir(env::xml->ai_path);
	dir.append("/");

	DIR * pDir = opendir(dir.c_str());
	if (NULL == pDir)
	{
		log_error("can not open dir[%s]", dir.c_str());
		return false;
	}

	struct dirent *pEntry;

	while (NULL != (pEntry = readdir(pDir)))
	{
		if (pEntry->d_type != DT_REG) continue;

		uint32_t ai_id = atoi(pEntry->d_name);
		std::string ai_name = boost::lexical_cast<std::string>(ai_id) + ".xml";

		if (ai_name != pEntry->d_name)
			continue;

		ai_name = dir + ai_name;
		load_file(ai_id, ai_name.c_str());
	}

	closedir(pDir);

	return true;
}

ai_state_conf_ptr fsm_xml_manager_t::get_fsm_ai_state(uint32_t id)
{
	ai_state_conf_map::iterator itr = m_ai_state_conf_map.find(id);
	if (itr != m_ai_state_conf_map.end())
	{
		return itr->second;
	}

	return NULL;
}

void fsm_xml_manager_t::load_file(uint32_t id, const char* filename)
{
	if (NULL == filename)
	{
		log_error("filename[%d.xml] null error", id);
		return;
	}

	using boost::property_tree::ptree;
	ptree pt;

	FUNC_CHECK_START
		read_xml(filename, pt);
		if (pt.size() == 0)
			return;

		const ptree& root = pt.get_child("root");
		if (root.size() == 0)
			return;

		ai_state_conf_ptr p_conf = ai_state_conf_ptr(new ai_state_conf(id));
		if (p_conf == NULL)
		{
			log_error("p_conf null error");
			return;
		}

		load_state(root, p_conf->m_states);

		m_ai_state_conf_map.insert(std::make_pair(id, p_conf));
	
	FUNC_CHECK_END
}

void fsm_xml_manager_t::load_state(const boost::property_tree::ptree& pt, fsm::ai_state& state)
{
	FUNC_CHECK_START
		if (pt.size() == 0)
			return;

		for (const auto& child : pt)
		{
			if (child.first == "<xmlattr>")
			{
				state.state_type = get_state_type(child.second.get<std::string>("name", ""));

				//printf("\033[31m state_type:%d \033[0m\n", state_type);
			}

			if (child.first == "event")
			{
				fsm::ai_event event;

				for (const auto& events : child.second)
				{
					if (events.first == "<xmlattr>")
					{
						event.event_type = get_event_type(events.second.get<std::string>("name", ""));

						//printf("\033[31m event_type:%d \033[0m\n", event_type);
					}

					if (events.first == "act")
					{
						for (const auto& act : events.second)
						{
							fsm::ai_act t;
							t.act_type = get_act_type(act.second.get<std::string>("name", ""));
							t.act_param = act.second.get<std::string>("param", "");
							event.acts.push_back(t);
						}
					}
				}

				state.events.push_back(event);
			}

			if (child.first == "mode")
			{
				fsm::ai_mode mode;

				for (const auto& modes : child.second)
				{
					if (modes.first == "<xmlattr>")
					{
						mode.mode_type = get_mode_type(modes.second.get<std::string>("name", ""));

						//printf("\033[31m mode_type:%d \033[0m\n", mode_type);
					}

					if (modes.first == "act")
					{
						for (const auto& act : modes.second)
						{
							fsm::ai_act t;
							t.act_type = get_act_type(act.second.get<std::string>("name", ""));
							mode.acts.push_back(t);
						}
					}
				}

				state.modes.push_back(mode);
			}

			if (child.first == "state")
			{
				fsm::ai_state child_state;
				load_state(child.second, child_state);
				state.children.push_back(child_state);
			}
		}
	FUNC_CHECK_END
}

ai::ai_event fsm_xml_manager_t::get_event_type(const std::string& name)
{
	name2eventtype::const_iterator citr = m_name2eventtype.find(name);
	if (citr != m_name2eventtype.end())
	{
		return citr->second;
	}

	return ai::ai_event_none;
}

ai::ai_act fsm_xml_manager_t::get_act_type(const std::string& name)
{
	name2acttype::const_iterator citr = m_name2acttype.find(name);
	if (citr != m_name2acttype.end())
	{
		return citr->second;
	}

	return ai::ai_act_none;
}

proto::common::object_state_type fsm_xml_manager_t::get_state_type(const std::string& name)
{
	name2statetype::const_iterator citr = m_name2statetype.find(name);
	if (citr != m_name2statetype.end())
	{
		return citr->second;
	}

	return proto::common::object_state_none;
}

ai::ai_mode fsm_xml_manager_t::get_mode_type(const std::string& name)
{
	name2modetype::const_iterator citr = m_name2modetype.find(name);
	if (citr != m_name2modetype.end())
	{
		return citr->second;
	}

	return ai::ai_mode_none;
}
