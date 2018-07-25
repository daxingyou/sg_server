#pragma once

#include "macros.hpp"
#include "protos_fwd.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <map>
#include "tblh/RandomShopTable.tbls.h"

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

/**
 * \brief 人物随机商店数据
 */

class role_random_shop_t
{
public:
	role_random_shop_t(uint64_t uid);
	~role_random_shop_t();

public:
	typedef std::map<uint32_t, proto::common::random_shop_data> random_shop_map;

public:
	void load_data(const proto::common::role_random_shop_data& data);
	void save_data(const proto::common::random_shop_data& data);

public:
	void get_random_shop_info(const role_ptr& p_role, uint32_t shop_id, proto::client::gc_random_shop_info_reply& reply);
	void buy_random_shop(const role_ptr& p_role, uint32_t shop_id, uint32_t index, proto::client::gc_random_shop_buy_reply& reply);
	void role_refresh_random_shop(const role_ptr& p_role, uint32_t shop_id, proto::client::gc_random_shop_refresh_reply& reply);

private:
	uint32_t refresh_random_shop(const role_ptr& p_role, RandomShopTable* p_conf, proto::common::random_shop_data* p_data, bool is_auto = true);
	
protected:
	uint64_t m_role_uid = 0;
	random_shop_map m_random_shop_map;
};