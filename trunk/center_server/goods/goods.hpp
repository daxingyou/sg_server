#ifndef __CENTER_GOODS_H__
#define __CENTER_GOODS_H__

#include "macros.hpp"

#include "server.pb.h"
#include "common.pb.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <map>
#include <vector>

class goods_t : public boost::enable_shared_from_this<goods_t>
{
public:
	goods_t(){}
public:
	void set_uid(uint64_t val) { m_uid = val; }
	uint64_t get_uid() { return m_uid; }
	
	void set_seller_uid(uint64_t val) { m_seller_uid = val; }
	uint64_t get_seller_uid() { return m_seller_uid; }

	void set_item_tid(uint32_t val) { m_item_tid = val; }
	uint32_t get_item_tid() { return m_item_tid; }

	void set_item_num(uint32_t val) { m_item_num = val; }
	uint32_t get_item_num() { return m_item_num; }

	void set_attr(const std::map<uint32_t, uint32_t>& attrs) { m_attrs = attrs; }
	const std::map<uint32_t, uint32_t>& get_attr() { return m_attrs; }

	void set_exattr(const std::vector<uint32_t>& exattrs) { m_exattrs = exattrs; }
	const std::vector<uint32_t>& get_exattr() { return m_exattrs; }
	
	void set_name(uint32_t val) { m_name = val; }
	uint32_t get_name() { return m_name; }

	void set_special(uint32_t val) { m_special = val; }
	uint32_t get_special() { return m_special; }

	void set_score(uint32_t val) { m_score = val; }
	uint32_t get_score() { return m_score; }

	void set_rarity(uint32_t val) { m_rarity = val; }
	uint32_t get_rarity() { return m_rarity; }

	void set_item_price(uint32_t val) { m_item_price = val; }
	uint32_t get_item_price() { return m_item_price; }

	void set_selled_num(uint32_t val) { m_selled_num = val; }
	uint32_t get_selled_num() { return m_selled_num; }

	void set_profit(uint32_t val) { m_profit = val; }
	uint32_t get_profit() { return m_profit; }

	void set_state(uint32_t val) { m_state = val; }
	uint32_t get_state() { return m_state; }

	void set_expired_time(uint32_t val) { m_expired_time = val; }
	uint32_t get_expired_time() { return m_expired_time; }

	void set_destine_uid(uint64_t uid) { m_destine_uid = uid;  }
	uint64_t get_destine_uid() { return m_destine_uid; }
	
	void set_heat(uint32_t val) { m_heat = val; }
	uint32_t get_heat() { return m_heat; }

	void peek_data(proto::common::goods_single* p_info);

	void peek_equip_data(proto::common::equip_single* p_equip);

	void load_equip_data(const proto::common::equip_single& es);

	void save_self();
public:
	void start_timer();
	void stop_timer();
private:
	uint64_t m_uid = 0;
	uint64_t m_seller_uid = 0;
	uint32_t m_item_tid = 0;
	uint32_t m_item_num = 0;
	std::map<uint32_t, uint32_t> m_attrs;
	std::vector<uint32_t> m_exattrs;
	uint32_t m_name = 0;
	uint32_t m_special = 0;
	uint32_t m_strengthen_level = 0;
	uint32_t m_strmaster_level = 0;
	uint32_t m_remake_material = 0;
	uint32_t m_lock_material = 0;
	uint32_t m_strengthen_material = 0;
	uint32_t m_strengthen_material_ex = 0;
	uint32_t m_strengthen_money = 0;
	uint32_t m_score = 0;
	uint32_t m_rarity = 0;
	uint32_t m_item_price = 0;
	uint32_t m_selled_num = 0;
	uint32_t m_profit = 0;
	uint32_t m_state = 0;
	uint32_t m_expired_time = 0;
	uint32_t m_heat = 0;
	uint64_t m_destine_uid = 0;		//Ô¤¶¨ÕßID

	boost::shared_ptr<boost::asio::deadline_timer> m_timer;
	
};
typedef boost::shared_ptr<goods_t> goods_ptr;

#endif