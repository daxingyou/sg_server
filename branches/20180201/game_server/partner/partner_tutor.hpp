#pragma once

#include "partner.hpp"

class partner_tutor_t : public partner_t
{
public:
	partner_tutor_t(uint64_t owner_uid);
	virtual ~partner_tutor_t();

	virtual void del_clear();

	// 是否是导师
	virtual bool is_tutor() { return true; }

	// 获取对象类型
	proto::common::EM_SCENEOBJECT_TYPE get_object_type() { return proto::common::SCENEOBJECT_TUTOR; }

	// 导师指针
	virtual partner_tutor_ptr get_partner_tutor() { return boost::dynamic_pointer_cast<partner_tutor_t>(shared_from_this()); }

public:
	bool init();

};