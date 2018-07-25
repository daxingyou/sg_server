#include "partner_manager.hpp"

partner_manager_t::partner_map partner_manager_t::m_partner_map;

void partner_manager_t::close_clear()
{
	m_partner_map.clear();
}

partner_ptr partner_manager_t::find_partner(uint64_t uid)
{
	partner_map::iterator itr = m_partner_map.find(uid);
	if (itr != m_partner_map.end()) {
		return itr->second;
	}

	return NULL;
}

void partner_manager_t::delete_partner(uint64_t uid)
{
	partner_map::iterator itr = m_partner_map.find(uid);
	if (itr != m_partner_map.end()) {
		if (NULL != itr->second) {
			if (itr->second->is_in_scene()) {
				itr->second->leave_scene();
			}
			itr->second->del_clear();
		}
		m_partner_map.erase(itr);
	}
}

bool partner_manager_t::add_partner(partner_ptr p_partner)
{
	if (p_partner == NULL) {
		log_error("partner_ptr null error");
		return false;
	}

	std::pair<partner_map::iterator, bool> ret = m_partner_map.insert(std::make_pair(p_partner->get_object_id(), p_partner));
	if (!ret.second) {
		log_error("partner[%lu] insert error, it is exist", p_partner->get_object_id());
		return false;
	}

	return true;
}

