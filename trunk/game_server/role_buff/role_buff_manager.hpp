#ifndef __ROLE_BUFF_MANAGER_H__
#define __ROLE_BUFF_MANAGER_H__

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "common.pb.h"
#include "client.pb.h"
#include "macros.hpp"
#include "role_buff.hpp"

class role_buff_manager_t : public boost::enable_shared_from_this<role_buff_manager_t>
{
public:
	role_buff_manager_t(uint64_t role_uid);
    ~role_buff_manager_t() {}

	void init();
	void del_timer();
public:
	void load_data(const proto::common::role_buff_data& buff_data);
	void peek_data(proto::common::role_buff_data* p_buff_data);
	void peek_obj_data(proto::common::object_common_info* p_obj_info);

	void notify_to_client();
public:
	bool add_role_buff(uint32_t buff_id, uint32_t source_type, uint32_t source_param);
	void del_role_buff(uint32_t buff_id, uint32_t source_type, uint32_t source_param, bool notify = false);
	void del_role_buff_by_type(uint32_t buff_type, uint32_t source_type, uint32_t source_param, bool notify = false);
private:
	uint64_t m_role_uid = 0;
	std::map<uint32_t, role_buff_ptr>	m_buff_map;
};
typedef boost::shared_ptr<role_buff_manager_t> role_buff_mgr_ptr;
#endif // !__TACTIC_MANAGER_H__

