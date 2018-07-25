#ifndef __CENTER_GOODS_MANAGER_H__
#define __CENTER_GOODS_MANAGER_H__

#include "goods.hpp"
#include "tblh/Goods.tbls.h"
#include "log_enum.hpp"
#define equip_prefix 21
class global_user_t;
typedef boost::shared_ptr<global_user_t> global_user_ptr;

typedef std::map<uint64_t, goods_ptr> goods_map;
typedef std::vector<uint64_t> goods_vec;
typedef std::map<uint64_t, goods_vec> goods_user_map;
typedef std::map<uint32_t, goods_vec> goods_map_by_tid;
typedef std::map<uint32_t, std::vector<uint32_t>> goods_item_by_type;

typedef std::map<uint32_t, std::vector<uint32_t>> deal_price_map;

typedef std::map<uint64_t, uint64_t> goods_care_map;
typedef std::map<uint64_t, goods_care_map> goods_user_care_map;
class goods_manager_t
{
public:
	goods_manager_t(){}
	~goods_manager_t(){}
	// 生成一个唯一ID
	static uint64_t get_new_goods_uid();

	// 生成一个唯一ID
	static uint64_t get_new_care_uid();

	static void make_conf();

	static void clear_conf();

	static void load_data();

	static void goods_timeout(uint64_t goods_uid, const boost::system::error_code& ec);

	static void add_goods(goods_ptr p_goods);

	static void del_selling_goods(goods_ptr p_goods);

	static void del_public_goods(goods_ptr p_goods);

	static void clear_user_sell_goods(goods_ptr p_goods);

	static goods_ptr get_goods_by_uid(uint64_t goods_uid);

	static void stop_timer();
public:
	//获取出售（公示）商品清单
	static void get_selling_list(uint32_t tab, uint32_t type, proto::client::ec_goods_selling_list_reply& reply);
	//根据item_tid获取出售（公示）珍品（非珍品）商品列表
	static void get_selling_data(uint64_t user_uid, uint32_t tab, uint32_t rarity, uint32_t item_tid, proto::client::ec_goods_selling_data_reply& reply);
	//获取自己关注的商品列表
	static void get_care_goods(uint64_t user_uid, uint32_t tab, proto::client::ec_goods_care_reply& reply);
	//获取自己出售的商品列表
	static void get_self_goods(uint64_t user_uid, proto::client::ec_goods_self_reply& reply);
	//获取珍品的推荐价格
	static uint32_t get_default_price(uint32_t item_tid);
	//购买商品
	static uint32_t buy_goods(uint64_t user_uid, const std::string& user_name, uint64_t goods_uid, uint32_t item_tid, uint32_t buy_count, uint32_t price, proto::common::goods_item* p_item);
	//寄售商品
	static uint32_t sell_goods(uint64_t user_uid, const std::string& user_name, uint32_t price, const proto::common::goods_item& item_info);
	//取回商品
	static uint32_t return_goods(uint64_t user_uid, const std::string& user_name, uint64_t goods_uid, proto::common::goods_item* p_item);
	//重新上架
	static uint32_t resell_goods(uint64_t user_uid, const std::string& user_name, uint64_t goods_uid, uint32_t sell_count, uint32_t price);
	//领取收益
	static uint32_t profit_goods(uint64_t user_uid, const std::string& user_name, uint32_t& profit);
	//关注商品
	static uint32_t care_goods(uint64_t user_uid, uint64_t goods_uid, uint32_t operation, uint32_t& heat);
	//一键清理关注
	static void clear_care(uint64_t user_uid, proto::client::ec_goods_clear_care_reply& reply);
	//商品下架
	static uint32_t cancel_goods(uint64_t user_uid, const std::string& user_name, uint64_t goods_uid);
	//获取该玩家的收益
	static uint32_t get_profit(uint64_t user_uid);
private:
	//获取出售（公示）商品清单
	static void get_selling_goods_list(uint32_t type, proto::client::ec_goods_selling_list_reply& reply, const goods_map_by_tid& gmbt);
	//根据item_tid获取出售（公示）珍品（非珍品）商品列表
	static void get_selling_goods_data(uint64_t user_uid, uint32_t item_tid, proto::client::ec_goods_selling_data_reply& reply, const goods_map_by_tid& gmbt);

	static bool is_equip(uint32_t item_tid);
	//根据价格升序排列
	static bool goods_price_sort_asc(const uint64_t& goods1, const uint64_t& goods2);
	//根据价格降序排列
	static bool goods_price_sort_desc(const uint64_t& goods1, const uint64_t& goods2);
	//商品最大显示数量
	static uint32_t get_display_count();
	//已关注数量
	static uint32_t get_care_num(uint64_t user_uid);
	//商品是否在关注列表
	static uint32_t is_care(uint64_t goods_uid, uint64_t user_uid);
	//个人出售商品最大数量
	static uint32_t get_max_sell_count();
	//平均价格生效次数
	static uint32_t get_deal_count();
	//检查出售商品是否已满
	static bool check_sell_full(uint64_t user_uid);
	//检查关注列表是否已满
	static bool check_care_full(uint64_t user_uid);
	//检查价格
	static bool check_price(uint32_t item_tid, uint32_t rarity, uint32_t price);
	
	//公示时间
	static uint32_t get_public_time();
	//普通物品出售时间
	static uint32_t get_goods_sell_time();
	//珍品出售时间
	static uint32_t get_rarity_sell_time();
	//关注列表最大数量
	static uint32_t get_care_max_num();
private:
	static uint32_t add_care_goods(uint64_t user_uid, goods_ptr p_goods);

	static uint32_t del_care_goods(uint64_t user_uid, goods_ptr p_goods);

	static void add_deal_price(uint32_t item_tid, uint32_t price);

	static void send_goods_log(uint64_t role_uid, const std::string& role_name, goods_ptr p_goods, common::log_enum::logs_source_type_t type);
private:
	static goods_map m_goods_map;				//所有的商品 以goods_uid为KEY
	static goods_map_by_tid m_selling_goods_map_by_tid; //正在出售的 以item_tid为KEY的所有商品
	static goods_map_by_tid m_selling_rarity_map_by_tid; //正在出售的 以item_tid为KEY的所有珍品装备
	static goods_map_by_tid m_public_goods_map_by_tid; //正在公示的 以item_tid为KEY的所有商品
	static goods_user_map m_user_sell_goods;	//以seller_uid为key的所有出售商品
	static goods_user_care_map m_user_care_goods;	//以user_uid为key的所有关注商品
	static deal_price_map m_deal_price_map;		//以item_tid为key的成交平均价格
	static goods_item_by_type m_goods_conf;		//以物品分类作为索引的所有item_tid清单
	static std::map<uint32_t, Goods*> m_goods_conf_by_tid;	//以item_tid为索引的配置表数据
};

#endif