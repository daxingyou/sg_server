#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "family_common.hpp"
#include "common.pb.h"
#include "user/global_user.hpp"
#include <map>

class family_member;
typedef boost::shared_ptr<family_member> family_member_ptr;

class family_member : public boost::enable_shared_from_this<family_member>
{
	friend class family_data_mgr_t;

public:
	family_member(uint64_t uid, uint64_t family_id);
	~family_member() {};

public:
	bool init(const global_user_ptr& p_user, uint32_t officer);

	bool init(uint32_t officer);

	bool update_officer(uint32_t new_officer);

	// ��������
	void set_online();
	// ��������
	void set_offline(uint32_t last_time);

	// ������Ϥ�仯
	void update_attr(const global_user_ptr& p_user);

	void peek_data(proto::common::family_member* p_data);
	void peek_prayer_data(proto::common::family_prayer_data* p_data);
	void peek_prayer_times_data(proto::client::ec_family_prayer_list_reply& reply);

public:
	uint64_t get_uid() const { return m_uid; }

	// �Ƿ�����
	bool is_online() const;

	// �ȼ�
	uint32_t get_level() const { return m_base.level(); }

	// ��ʷ����
	uint32_t get_total_contribution() const { return m_base.total_family_contribution(); }
	// ������ʷ����
	void add_total_contribution(uint32_t val);

	// �õ�����ʱ��
	uint32_t get_join_time() const { return m_join_time; }

	// ְλ
	uint32_t get_officer() const { return m_base.officer(); }

	// ����
	const std::string& get_name() const { return m_base.role_name(); }

	// ͷ��
	uint32_t get_plugin() const { return m_base.plugin(); }

	// ��ҫֵ
	void set_glory(uint32_t glory) { m_base.set_glory(glory); }
	uint32_t get_glory() { return m_base.glory(); }

	// �Ƿ�����Ը��Ϣ
	bool is_have_prayer_hero();

	// �õ��������
	uint32_t get_prayer_gift_times();

	// �õ����ͺͻ�ü�¼
	void get_prayer_record_list(proto::common::family_prayer_record_list* p_list);

	// �õ���Ը����
	uint32_t get_prayer_times();

	// �õ�������ȡ����
	uint32_t get_prayer_rwd_times();

	// ������Ը
	void ask_prayer_hero(uint32_t hero_tid, uint32_t num);

	// ������Ƭ
	uint32_t ask_gift_hero(const family_member_ptr& p_family_member, uint32_t hero_tid, uint32_t hero_material_id, uint32_t rarity, uint32_t hero_num);

	// ������˳ɹ��Ĵ���(�������轱������id)
	void do_gift_reward(const family_member_ptr& p_family_member, uint32_t hero_tid, uint32_t hero_material_id, uint32_t rarity, uint32_t hero_num, uint32_t& gift_rwd_item);

	// �����̵��޹���Ϣ
public:
	// �õ��޹���Ϣ
	const proto::common::family_shop_personal_data& get_shop_personal_data() const { return m_shop_personal_data; }

	// �õ������޹�����
	void peek_family_shop_personal_data(proto::common::family_shop_personal_data* p_data);

	// ���ص�ǰ��������
	uint32_t get_personal_buy_count(uint32_t index);

	// �����޹�����
	void add_personal_buy_count(uint32_t refresh_time, uint32_t index, uint32_t num);

	// ʧ�ܻ���
	void personal_failed_back(uint32_t refresh_time, uint32_t index, uint32_t num);

private:
	// ����
	void prayer_day_update(bool is_save = true);
	void prayer_week_update(bool is_save = true);

	// �������
	uint32_t get_gift_time(uint64_t uid);
	// ���ӱ����������
	void add_gift_time(uint64_t uid);

	// ������Ը����
	void add_prayer_contribution(uint32_t val);

	// ������������������轱��
	void add_prayer_gift_times();

	// ����޹��Ƿ����
	void check_shop_time_out();

protected:
	uint64_t m_uid = 0;						// ���uid
	uint64_t m_family_id = 0;				// ����id
	proto::common::family_member m_base;	// ������Ϣ�����ڷ����ͻ���
	uint32_t m_join_time = 0;				// �������ʱ��
	family_impeach_type m_is_impeach_ack = family_impeach_type_none;	 // ������Ӧ

protected:
	// ��Ը���������ݣ���Щ�������뿪�����ɾ����
	uint32_t m_last_prayer_day_time = 0;
	uint32_t m_prayer_times = 0;
	uint32_t m_prayer_rwd_times = 0;
	uint32_t m_last_prayer_week_time = 0;
	uint32_t m_prayer_gift_times = 0;

	uint32_t m_prayer_hero_tid = 0;			// ��Ը�佫tid
	uint32_t m_prayer_hero_num = 0;			// ��Ը�佫��ǰ����
	uint32_t m_prayer_hero_max = 0;			// ��Ը�佫�������
	uint32_t m_prayer_start_time = 0;		// ��Ը��ʼʱ��
	uint32_t m_prayer_complete_time = 0;	// ��Ը��ɺ�ʲôʱ��������¼ɾ��
	proto::common::family_gift_data_list m_gift_list;		// ��¼���͵��˵Ĵ���
	std::map<uint64_t, uint32_t> m_gift_list_map;
	proto::common::family_prayer_record_list m_record_list; // ���ͺͻ�ü�¼

protected:
	proto::common::family_shop_personal_data m_shop_personal_data;
};