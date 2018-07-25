#ifndef __CENTER_SHOP_GOODS_DATA_H__
#define __CENTER_SHOP_GOODS_DATA_H__

#include <boost/shared_ptr.hpp>
#include <map>

#include "shop_common.hpp"

#include "common.pb.h"
#include "client.pb.h"
#include "server.pb.h"

#include "tblh/ShopTable.tbls.h"

class shop_user_data_t
{
public:
	// �û��޹�����
	typedef boost::shared_ptr<shop_user_t> shop_user_ptr;
	typedef std::map<uint32_t, shop_user_ptr> shop_user_map;

public:
	shop_user_data_t(uint64_t role_uid);

	// ���ߴ���
	void on_login_load();

	// ���
	void clear();

public:
	// �õ�����޹��б�
	bool get_shop_list_by_type(uint32_t type, proto::client::ec_shop_goods_reply& reply);

	shop_user_ptr get_presonal_limit_info(uint32_t id);

	// �����޹�����
	uint32_t add_shop_goods_num(uint32_t goods_id, uint32_t goods_num, ShopTable* p_shop_conf, proto::server::eg_give_user_goods_notify& notify);

private:
	uint64_t m_role_uid = 0;
	shop_user_map m_shop_user_map;
};

#endif
