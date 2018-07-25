#ifndef __SHAKE_MONEY_TREE_H__
#define __SHAKE_MONEY_TREE_H__

#include "macros.hpp"
#include "protos_fwd.hpp"
class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class shake_money_tree_t 
{
public:
	shake_money_tree_t(uint64_t uid);

	void load_data(const proto::common::money_tree_data& data);
	void peek_data(proto::common::money_tree_data &data);
	void save_data();
	void oneday();
	void init_newbie();
public:
	uint32_t get_money_tree_info(proto::client::gc_get_money_tree_reply &msg);

	uint32_t shake_money_tree( proto::client::gc_shake_money_tree_reply &msg);
	
	uint32_t level_up_money_tree( proto::client::gc_levelup_money_tree_reply &msg);

	static uint32_t  get_crit_type( uint32_t tree_level);

	static bool rand_weighting(std::map<uint32_t, uint32_t> &weighting_map, uint32_t &key);
private:
	std::string m_key = "";
	uint64_t m_role_uid = 0;
	uint32_t m_shake_num  = 0;
	uint32_t m_tree_level = 1;
};




#endif