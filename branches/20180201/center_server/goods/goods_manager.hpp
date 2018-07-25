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
	// ����һ��ΨһID
	static uint64_t get_new_goods_uid();

	// ����һ��ΨһID
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
	//��ȡ���ۣ���ʾ����Ʒ�嵥
	static void get_selling_list(uint32_t tab, uint32_t type, proto::client::ec_goods_selling_list_reply& reply);
	//����item_tid��ȡ���ۣ���ʾ����Ʒ������Ʒ����Ʒ�б�
	static void get_selling_data(uint64_t user_uid, uint32_t tab, uint32_t rarity, uint32_t item_tid, proto::client::ec_goods_selling_data_reply& reply);
	//��ȡ�Լ���ע����Ʒ�б�
	static void get_care_goods(uint64_t user_uid, uint32_t tab, proto::client::ec_goods_care_reply& reply);
	//��ȡ�Լ����۵���Ʒ�б�
	static void get_self_goods(uint64_t user_uid, proto::client::ec_goods_self_reply& reply);
	//��ȡ��Ʒ���Ƽ��۸�
	static uint32_t get_default_price(uint32_t item_tid);
	//������Ʒ
	static uint32_t buy_goods(uint64_t user_uid, const std::string& user_name, uint64_t goods_uid, uint32_t item_tid, uint32_t buy_count, uint32_t price, proto::common::goods_item* p_item);
	//������Ʒ
	static uint32_t sell_goods(uint64_t user_uid, const std::string& user_name, uint32_t price, const proto::common::goods_item& item_info);
	//ȡ����Ʒ
	static uint32_t return_goods(uint64_t user_uid, const std::string& user_name, uint64_t goods_uid, proto::common::goods_item* p_item);
	//�����ϼ�
	static uint32_t resell_goods(uint64_t user_uid, const std::string& user_name, uint64_t goods_uid, uint32_t sell_count, uint32_t price);
	//��ȡ����
	static uint32_t profit_goods(uint64_t user_uid, const std::string& user_name, uint32_t& profit);
	//��ע��Ʒ
	static uint32_t care_goods(uint64_t user_uid, uint64_t goods_uid, uint32_t operation, uint32_t& heat);
	//һ�������ע
	static void clear_care(uint64_t user_uid, proto::client::ec_goods_clear_care_reply& reply);
	//��Ʒ�¼�
	static uint32_t cancel_goods(uint64_t user_uid, const std::string& user_name, uint64_t goods_uid);
	//��ȡ����ҵ�����
	static uint32_t get_profit(uint64_t user_uid);
private:
	//��ȡ���ۣ���ʾ����Ʒ�嵥
	static void get_selling_goods_list(uint32_t type, proto::client::ec_goods_selling_list_reply& reply, const goods_map_by_tid& gmbt);
	//����item_tid��ȡ���ۣ���ʾ����Ʒ������Ʒ����Ʒ�б�
	static void get_selling_goods_data(uint64_t user_uid, uint32_t item_tid, proto::client::ec_goods_selling_data_reply& reply, const goods_map_by_tid& gmbt);

	static bool is_equip(uint32_t item_tid);
	//���ݼ۸���������
	static bool goods_price_sort_asc(const uint64_t& goods1, const uint64_t& goods2);
	//���ݼ۸�������
	static bool goods_price_sort_desc(const uint64_t& goods1, const uint64_t& goods2);
	//��Ʒ�����ʾ����
	static uint32_t get_display_count();
	//�ѹ�ע����
	static uint32_t get_care_num(uint64_t user_uid);
	//��Ʒ�Ƿ��ڹ�ע�б�
	static uint32_t is_care(uint64_t goods_uid, uint64_t user_uid);
	//���˳�����Ʒ�������
	static uint32_t get_max_sell_count();
	//ƽ���۸���Ч����
	static uint32_t get_deal_count();
	//��������Ʒ�Ƿ�����
	static bool check_sell_full(uint64_t user_uid);
	//����ע�б��Ƿ�����
	static bool check_care_full(uint64_t user_uid);
	//���۸�
	static bool check_price(uint32_t item_tid, uint32_t rarity, uint32_t price);
	
	//��ʾʱ��
	static uint32_t get_public_time();
	//��ͨ��Ʒ����ʱ��
	static uint32_t get_goods_sell_time();
	//��Ʒ����ʱ��
	static uint32_t get_rarity_sell_time();
	//��ע�б��������
	static uint32_t get_care_max_num();
private:
	static uint32_t add_care_goods(uint64_t user_uid, goods_ptr p_goods);

	static uint32_t del_care_goods(uint64_t user_uid, goods_ptr p_goods);

	static void add_deal_price(uint32_t item_tid, uint32_t price);

	static void send_goods_log(uint64_t role_uid, const std::string& role_name, goods_ptr p_goods, common::log_enum::logs_source_type_t type);
private:
	static goods_map m_goods_map;				//���е���Ʒ ��goods_uidΪKEY
	static goods_map_by_tid m_selling_goods_map_by_tid; //���ڳ��۵� ��item_tidΪKEY��������Ʒ
	static goods_map_by_tid m_selling_rarity_map_by_tid; //���ڳ��۵� ��item_tidΪKEY��������Ʒװ��
	static goods_map_by_tid m_public_goods_map_by_tid; //���ڹ�ʾ�� ��item_tidΪKEY��������Ʒ
	static goods_user_map m_user_sell_goods;	//��seller_uidΪkey�����г�����Ʒ
	static goods_user_care_map m_user_care_goods;	//��user_uidΪkey�����й�ע��Ʒ
	static deal_price_map m_deal_price_map;		//��item_tidΪkey�ĳɽ�ƽ���۸�
	static goods_item_by_type m_goods_conf;		//����Ʒ������Ϊ����������item_tid�嵥
	static std::map<uint32_t, Goods*> m_goods_conf_by_tid;	//��item_tidΪ���������ñ�����
};

#endif