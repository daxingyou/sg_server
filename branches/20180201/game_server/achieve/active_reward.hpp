#pragma once

#include <boost//smart_ptr/shared_ptr.hpp>
#include "protocpp/protos_fwd.hpp"


class active_reward_t
{
public:
	active_reward_t() {}
	active_reward_t(uint64_t uid,uint32_t tid,uint64_t role_uid,uint32_t type);
	~active_reward_t(){}

	void				load_data(uint64_t role_uid,const proto::common::active_reward_data_single& single);
	void				peek_data(proto::common::active_reward_data_single* p_single, bool is_client = true);
	void				save_self(uint64_t role_uid,bool is_new = false);

public:
	std::string			get_key()const { return m_key; }
	
	uint64_t			get_uid() { return m_uid; }
	void				set_uid(const uint64_t val) { m_uid = val; }

	uint32_t			get_type() { return m_type; }
	void				set_type(const uint32_t val) { m_type = val; }

	uint32_t			get_tid() { return m_tid; }
	void				set_tid(const uint32_t val) { m_tid = val; }

	uint32_t			get_count() { return m_count; }
	void				set_count(const uint32_t val);
	void				reset_count() { m_count = 0; }

	uint32_t			get_event_id() { return m_event_id; }
	void				set_event_id(const uint32_t val) { m_event_id = val; }

	bool				get_is_finish() { return m_is_finish; }
	void				set_is_finish() { m_is_finish = true; }
	void				reset_is_finish() { m_is_finish = false; }

	bool				get_is_grant() { return m_is_grant; }
	void				set_is_grant() { m_is_grant = true; }
	void				reset_is_grant() { m_is_grant = false; }


	bool				compare_is_same(const uint32_t id, const uint32_t type, const uint32_t event_id);
private:
	std::string			m_key = "";			
	uint64_t			m_uid = 0;				///唯一ID
	uint32_t			m_tid = 0;				///tid
	uint32_t			m_count = 0;			///进度
	uint32_t			m_type = 0;				///类型
	uint32_t			m_event_id = 0;			///事件ID
	bool				m_is_finish = false;	///是否已经完成
	uint64_t			m_role_uid = 0;			///role_uid
	bool				m_is_grant = false;		///是否已经发放奖励
};

typedef	boost::shared_ptr<active_reward_t>		active_reward_ptr;
