#ifndef __TACTIC_MANAGER_H__
#define __TACTIC_MANAGER_H__

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "common.pb.h"
#include "client.pb.h"
#include "macros.hpp"
#include "tactic.hpp"

class tactic_manager_t : public boost::enable_shared_from_this<tactic_manager_t>
{
public:
	tactic_manager_t(uint64_t role_uid);
    ~tactic_manager_t() {}

	void init();
public:
	void load_data(const proto::common::tactic_data& tactic_data);
	void peek_data(proto::common::tactic_data* p_tactic_data);
public:
	//开放阵法
	uint32_t add_tactic(uint32_t tactic_id, bool notify=true);
	//阵法是否开放
	bool has_tactic(uint32_t tactic_id);
private:
	uint64_t m_role_uid = 0;
	std::map<uint32_t, tactic_ptr>	m_tactic_map;
};
typedef boost::shared_ptr<tactic_manager_t> tactic_mgr_ptr;
#endif // !__TACTIC_MANAGER_H__

