#ifndef __RED_POINT_H__
#define __RED_POINT_H__

#include <boost/smart_ptr/shared_ptr.hpp>
#include "protocpp/common.pb.h"
#include <set>

//#define MAX_BIT 32

class red_point_t
{
public:
	red_point_t() {}
	red_point_t(proto::common::RED_POINT_TYPE type,uint64_t role_uid);

	proto::common::RED_POINT_TYPE get_type() { return m_red_point_type; }
	void set_type(proto::common::RED_POINT_TYPE type) { m_red_point_type = type; }

	std::string& get_list() { return m_list; }

	bool get_state() { return m_b_state; }
	void set_state() { m_b_state = true; }
	void reset_state() { m_b_state = false; }

	bool is_send() { return m_b_state; }

	void append_to_list(uint32_t val);
	void append_to_list(std::string& dec);


	void remove_from_list(uint32_t val);
	void slipt_to_set(std::set<uint32_t>& set_list);
private:
	//告知客户端那些需要显示 中间以 : 隔开
	std::string m_list = "";

	proto::common::RED_POINT_TYPE m_red_point_type = proto::common::RED_POINT_TYPE_BEGIN;
	bool		m_b_state = true;		//红点是否亮

	//role_id
	uint64_t m_role_uid = 0;
};
typedef boost::shared_ptr<red_point_t>	red_point_ptr;

#endif // !__RED_POINT_H__