#pragma once

#include <boost/shared_ptr.hpp>

#include "family_common.hpp"
#include "common.pb.h"

class family_build
{
	friend class family_data_mgr_t;

public:
	family_build(proto::common::family_build_type type, uint64_t family_id);
	~family_build() {};

public:
	// �����ȼ�
	uint32_t get_level() const { return m_base.level(); }
	// ��ȴʱ��
	uint32_t get_cd_time() const { return m_base.cd_time(); }

public:
	// ��ʼ��
	void init();

	void peek_data(proto::common::family_build_data* data);

	void upgrade(uint32_t time_now);

	void clear_cd();

protected:
	std::string m_key = "";							// key = m_family_id + m_type
	proto::common::family_build_type m_type;		// ���ñ�id
	uint64_t m_family_id = 0;						// ����uid

	proto::common::family_build_data m_base;	// ������Ϣ�����ڷ����ͻ���
};

typedef boost::shared_ptr<family_build> family_build_ptr;