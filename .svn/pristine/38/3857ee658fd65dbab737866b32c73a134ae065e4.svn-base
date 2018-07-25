#ifndef __GAME_LIANDAN_MANAGER_H__
#define __GAME_LIANDAN_MANAGER_H__

#include "protos_fwd.hpp"
#include "liandan.hpp"

class liandan_manager_t
{
public:
	liandan_manager_t(uint64_t uid);
	~liandan_manager_t();
public:
	void load_data(const proto::common::liandan_data& ld);
	void peek_data(proto::common::liandan_data* p_data);

	void save_self();
public:
	//解锁槽位
	uint32_t unlock(proto::common::role_change_data* p_data);
	//炼丹
	uint32_t doing(uint32_t liandan_tid, uint32_t count, proto::common::role_change_data* p_data);
	//立即完成
	uint32_t done(uint32_t liandan_index, proto::common::role_change_data* p_data);
	//取回丹药
	uint32_t return_back(uint32_t liandan_index, proto::common::role_change_data* p_data);
private:
	//根据炼丹合成ID（策划表ID）获取槽位
	liandan_ptr get_liandan_by_index(uint32_t liandan_index);
	//获取空闲槽位
	liandan_ptr get_free_liandan();
private:
	std::string m_key = "";
	uint64_t m_owner = 0;
	std::vector<liandan_ptr> m_liandan_vec;
};
typedef boost::shared_ptr<liandan_manager_t> liandan_mgr_ptr;
#endif
