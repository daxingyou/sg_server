#pragma once
#include "object/object_position.hpp"

class partner_tutor_t;
typedef boost::shared_ptr<partner_tutor_t> partner_tutor_ptr;

class partner_t : public object_position_t
{
public:
	partner_t(uint64_t owner_uid);
	virtual ~partner_t();

	// 是否是伙伴
	virtual bool is_partner() { return true; }

	// 是否是导师
	virtual bool is_tutor() { return false; }

	// 伙伴指针
	virtual partner_ptr get_partner() { return boost::dynamic_pointer_cast<partner_t>(shared_from_this()); }

	// 导师指针
	virtual partner_tutor_ptr get_partner_tutor() { return partner_tutor_ptr(); }

protected:
	uint64_t m_owner_uid = 0;
};
