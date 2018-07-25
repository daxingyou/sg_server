#ifndef __CENTER_REDBAG_MANAGER_H__
#define __CENTER_REDBAG_MANAGER_H__

#include "redbag.hpp"
#include "log_enum.hpp"

class global_user_t;
typedef boost::shared_ptr<global_user_t> global_user_ptr;
typedef std::map<uint64_t, redbag_ptr> redbag_map;

class redbag_manager_t
{
public:
	redbag_manager_t(){}
	~redbag_manager_t(){}
	// 生成一个唯一ID
	static uint64_t get_new_redbag_uid();

	static void load_data();

	static void redbag_timeout(uint64_t redbag_uid, const boost::system::error_code& ec);

	static void add_redbag(redbag_ptr p_redbag);

	static void del_redbag(redbag_ptr p_redbag);

	static redbag_ptr get_redbag_by_uid(uint64_t redbag_uid);

	static void stop_timer();
public:
	//抢红包
	static uint32_t draw(uint64_t redbag_uid, global_user_ptr p_role, proto::common::redbag_data* p_data);
	//发红包
	static void send(global_user_ptr p_role, uint32_t count, uint32_t money);
private:
	static redbag_map m_redbag_map;				//所有的红包 以redbag_uid为KEY
};

#endif