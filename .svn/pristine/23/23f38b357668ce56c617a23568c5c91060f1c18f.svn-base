#include "friend_manager.hpp"

#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"

uint32_t friend_manager_t::m_friend_cnt1 = 0;
uint32_t friend_manager_t::m_friend_cnt2 = 0;
uint32_t friend_manager_t::m_friend_cnt3 = 0;
uint32_t friend_manager_t::m_friend_cnt4 = 0;
uint32_t friend_manager_t::m_friend_cnt5 = 0;
uint32_t friend_manager_t::m_friend_cnt6 = 0;
uint32_t friend_manager_t::m_friend_cnt7 = 0;
uint32_t friend_manager_t::m_friend_cnt8 = 0;
uint32_t friend_manager_t::m_friendmsg_strlen1 = 0;
uint32_t friend_manager_t::m_friendmsg_log1 = 0;
uint32_t friend_manager_t::m_friendmsg_log2 = 0;
uint32_t friend_manager_t::m_friendoutdel_limit = 0;

uint32_t friend_manager_t::m_max_save_time = 0;
uint32_t friend_manager_t::m_chat_max_content = 0;

bool friend_manager_t::init()
{
	reload();

	return true;
}

void friend_manager_t::reload()
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::friend_cnt1);
	m_friend_cnt1 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friend_cnt2);
	m_friend_cnt2 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friend_cnt3);
	m_friend_cnt3 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friend_cnt4);
	m_friend_cnt4 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friend_cnt5);
	m_friend_cnt5 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friend_cnt6);
	m_friend_cnt6 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friend_cnt7);
	m_friend_cnt7 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friend_cnt8);
	m_friend_cnt8 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friendmsg_strlen1);
	m_friendmsg_strlen1 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friendmsg_log1);
	m_friendmsg_log1 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friendmsg_log2);
	m_friendmsg_log2 = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friendoutdel_limit);
	m_friendoutdel_limit = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friend_chat_max_time);
	m_max_save_time = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::friend_chat_max_content);
	m_chat_max_content = GET_COMPREHENSIVE_VALUE_1(p_conf);
}