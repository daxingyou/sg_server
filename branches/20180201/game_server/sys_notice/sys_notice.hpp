#pragma once
#include <boost/shared_ptr.hpp>
#include "protocpp/common.pb.h"
#include "protocpp/server.pb.h"

class sys_notice_t
{
public:
	sys_notice_t() {}
	sys_notice_t(uint64_t uid, uint32_t type, uint32_t param);
public:
	void load_data(const proto::common::sys_notity_single& single);
	bool peek_data(proto::common::sys_notity_single* p_single);

	uint64_t get_uid() { return m_uid; }
	void set_uid(uint64_t val) { m_uid = val; }

	uint32_t get_type() { return m_type; }
	void set_type(uint32_t val) { m_type = val; }

	uint32_t get_tid() { return m_param; }
	void set_tid(uint32_t val) { m_param = val; }

	uint32_t get_count() { return m_count; }
	void set_count(uint32_t val) { m_count = val; }

private:
	uint64_t	m_uid = 0;
	uint32_t	m_type = 0;
	uint32_t	m_param = 0;
	uint32_t	m_count = 0;
};

typedef boost::shared_ptr<sys_notice_t>	sys_notice_ptr;