#pragma once

#include "family.hpp"
#include "user/global_user.hpp"
#include "server.pb.h"
#include "client.pb.h"

#include <deque>

class family_manager_t
{
public:
	typedef std::map<uint64_t, family_ptr> family_map;
	static family_map m_family_map;

	struct family_sort_t
	{
		uint64_t m_family_id = 0;
		uint32_t m_family_lv = 0;
		uint32_t m_prestige = 0;
		std::string m_family_name = "";

		family_sort_t(uint64_t family_id, uint32_t family_lv, const std::string& family_name, uint32_t prestige) 
			: m_family_id(family_id), m_family_lv(family_lv), m_family_name(family_name), m_prestige(prestige) 
		{
		}

		family_sort_t(const family_sort_t& ref)
		{
			this->m_family_id = ref.m_family_id;
			this->m_family_lv = ref.m_family_lv;
			this->m_prestige = ref.m_prestige;
			this->m_family_name = ref.m_family_name;
		}

		family_sort_t& operator =(const family_sort_t& ref)
		{
			this->m_family_id = ref.m_family_id;
			this->m_family_lv = ref.m_family_lv;
			this->m_prestige = ref.m_prestige;
			this->m_family_name = ref.m_family_name;

			return *this;
		}

		friend bool operator< (const family_sort_t& lhs, const family_sort_t& rhs)
		{
			if (lhs.m_prestige != rhs.m_prestige)
			{
				return lhs.m_prestige > rhs.m_prestige;
			}
			
			if (lhs.m_family_lv != rhs.m_family_lv)
			{
				return lhs.m_family_lv > rhs.m_family_lv;
			}

			return lhs.m_family_id < rhs.m_family_id;
		}
	};

	typedef std::vector<family_sort_t> family_sort_vec;
	static family_sort_vec m_family_sort_vec;

public:
	// ���������������
	static bool init();
	// ����
	static bool close();

	// ʱ��ص�
public:
	// ÿ�ո��»ص�
	static void daily_update_callback(uint32_t id, bool is_in_time);

	// ��ս��ص�
	static void war_init_callback(uint32_t id, bool is_in_time);
	// ��ս�ص�
	static void war_prepare_callback(uint32_t id, bool is_in_time);
	// ��ս�ص�
	static void war_start_callback(uint32_t id, bool is_in_time);

public:
	// ���������������
	static uint32_t get_max_apply_size();

	// �õ�����Ĭ�ϵȼ�
	static uint32_t get_build_define_level(proto::common::family_build_type type);

	// ����һ������ΨһID
	static uint64_t get_new_family_uid();

public:
	// ��������
	static void create_family_from_game(const global_user_ptr& p_user, 
		const std::string& name, const std::string& declaration, 
		proto::server::eg_create_family_reply& reply);

	// ɾ������
	static bool del_family_when_no_member(uint64_t family_id);

	// ��������
	static void process_family_sort();

	// ����������
	static uint32_t invite_family_member(const global_user_ptr& p_user, uint64_t invite_uid);

	// ����������
	static void join_family_request(const global_user_ptr& p_user, uint64_t family_id, proto::client::ec_join_family_reply& reply);

	// �ܾ��������
	static void decline_join_family(const global_user_ptr& p_user, uint64_t uid, proto::client::ec_decline_join_family_reply& reply);

	// һ���ܾ��������
	static void one_key_decline_join_family(const global_user_ptr& p_user, proto::client::ec_one_key_decline_join_family_reply& reply);

	// ͬ��������
	static void approve_join_family(const global_user_ptr& p_user, uint64_t uid, proto::client::ec_approve_join_family_reply& reply);

	// ��������ظ�
	static uint32_t reply_family_invite(const global_user_ptr& p_user, const proto::common::family_invite_result& result);

	// һ��ͬ��������
	static void one_key_approve_join_family(const global_user_ptr& p_user, proto::client::ec_one_key_approve_join_family_reply& reply);

	// �˼���
	static void leave_family(const global_user_ptr& p_user, proto::client::ec_leave_family_reply& reply);

	// ������ְ
	static uint32_t assign_member(const global_user_ptr& p_user, uint64_t uid, uint32_t officer);

	// �����Ա
	static uint32_t fire_member(const global_user_ptr& p_user, uint64_t uid);

	// ������Ա
	static uint32_t kick_member(const global_user_ptr& p_user, uint64_t uid);

	// �õ������б�
	static void get_family_list(uint32_t page, proto::client::ec_get_family_list_reply& reply);

	// ģ����ѯ
	static void get_family_fuzzy_query(const std::string& name, proto::client::ec_family_fuzzy_query_reply& reply);

	// �õ�������Ϣ
	static void get_family_info(const global_user_ptr& p_user, proto::client::ec_get_family_info_reply& reply);

	// �õ������Ա��Ϣ
	static void get_family_member_list(const global_user_ptr& p_user, proto::client::ec_get_family_member_list_reply& reply);

	// �����Զ���Ϊѧͽ
	static void toggle_auto_newbie(const global_user_ptr& p_user, proto::client::ec_toggle_auto_newbie_reply& reply);

	// ��������
	static void modify_family_declaration(const global_user_ptr& p_user, const std::string& declaration, proto::client::ec_modify_family_declaration_reply& reply);

	// ���ù���
	static void modify_family_notification(const global_user_ptr& p_user, const std::string& notification, proto::client::ec_modify_family_notification_reply& reply);

	// �õ����������б�
	static void get_family_apply_list(const global_user_ptr& p_user, proto::client::ec_family_apply_list_reply& reply);

	// �õ������б�
	static void get_family_build_list(const global_user_ptr& p_user, proto::client::ec_get_family_build_info_reply& reply);

	// ��������
	static void upgrade_family_build(const global_user_ptr& p_user, proto::common::family_build_type type, proto::client::ec_upgrade_family_build_reply& reply);

	// ������cd
	static void clear_family_build_cd(const global_user_ptr& p_user, proto::common::family_build_type type, proto::client::ec_clear_family_build_upgrade_cd_reply& reply);

	// ������ҫֵ
	static void family_update_glory(uint64_t role_uid, uint64_t family_id, uint32_t glory);
	
public:
	// ������ֺϷ���
	static uint32_t is_family_name_vaild(const std::string& name);

	// ֪ͨchat�����Ա�仯
	static void notify_chat_member_change(uint64_t family_id, uint64_t role_uid, uint32_t change_type);

	// Ȩ�޼��
	static bool has_family_privilige(uint32_t officer, family_privilige_type type);

public:
	static family_ptr get_family(uint64_t family_id);

	//////////////////////////////////////////////////////////////////////////
public:
	// ����ս��ս����
	struct family_war_info_t
	{
		proto::server::global_data_family_war family_war_data;
		uint32_t family_war_time = 0;	// ս����ʼʱ��
	};

	static family_war_info_t m_family_war_info;	// �����ս״̬

	struct family_against_data_t
	{
		uint64_t m_war_scene_id = 0;
		proto::common::family_against_data m_data;
	};

	typedef std::map<uint32_t, family_against_data_t> against_data_map;
	static against_data_map m_against_data_map;					// ����ս��ս��Ϣ
	static proto::common::family_against_list m_against_list;	// ��ս��Ϣ���л����ٷ���

	typedef std::map<uint64_t, uint32_t> family_against_id_map;
	static family_against_id_map m_family_against_id_map;

	typedef std::deque<proto::common::family_war_history_data> history_data_deque;
	static history_data_deque m_history_deque;					// ��ʷ��Ϣ

	struct score_sort_t
	{
		uint32_t m_id = 0;
		uint64_t m_family_id = 0;
		uint32_t m_score = 0;
		uint32_t m_prestige = 0;

		score_sort_t(uint32_t id, uint64_t family_id, uint32_t score, uint32_t prestige) : m_id(id), m_family_id(family_id), m_score(score), m_prestige(prestige) {};

		friend bool operator< (const score_sort_t& lhs, const score_sort_t& rhs)
		{
			if (lhs.m_score != rhs.m_score)
			{
				return lhs.m_score > rhs.m_score;
			}

			if (lhs.m_prestige != rhs.m_prestige)
			{
				return lhs.m_prestige > rhs.m_prestige;
			}

			return lhs.m_family_id < rhs.m_family_id;
		}
	};

public:
	static uint32_t get_week_id(uint32_t t);
	static uint32_t get_war_time(uint32_t id);

	// ���ɶ�ս�б�
	static void update_against_list();

	// �����µ���ʷ����
	static void update_new_history();

	// ���ɶ�ս�б�
	static bool generate_war_list(uint32_t week_id);

	// �Զ�ս���尴�������򣬲���¼����
	static void rank_war_list();

	// ���ü���ս����
	static void family_war_end();

public:
	// ����ս��������
	static void prepare_family_war_reply(uint32_t game_id, const proto::server::ge_prepare_family_war_reply& reply);

	// �õ���ǰ��ս��Ϣ
	static void get_family_war_info(proto::client::ec_get_family_war_list_reply& reply);

	// �õ���ʷ��ս��Ϣ
	static void get_history_info(uint32_t index, proto::client::ec_get_family_war_history_reply& reply);

	// ����������ս����
	static uint32_t ask_enter_family_war_dungeon(const global_user_ptr& p_user);

	// ����ս��ǰ����
	static void family_war_early_result(const proto::server::family_war_result_data& data);

	// ����ս���
	static void family_war_result(const proto::server::ge_family_war_result_notify& results);

public:
	// �õ�������Ը�б�
	static void get_family_prayer_list(const global_user_ptr& p_user, proto::client::ec_family_prayer_list_reply& reply);
	// �õ���Ը�����б�
	static void get_prayer_gift_reward(const global_user_ptr& p_user, proto::client::ec_prayer_gift_reward_reply& reply);
	// �õ���Ը��¼
	static void get_prayer_record(const global_user_ptr& p_user, proto::client::ec_prayer_record_reply& reply);
	// ������Ը
	static uint32_t family_prayer_request(const global_user_ptr& p_user, uint32_t hero_id, uint32_t num);
	// ��Ը����
	static void family_prayer_gift_request(const global_user_ptr& p_user, 
		uint64_t gift_uid, uint32_t hero_tid, uint32_t hero_material_id, uint32_t rarity, uint32_t hero_num, 
		proto::server::eg_family_prayer_gift_failed_notify& reply);

public:
	// ������û���㽨���ļ���
	static family_ptr is_can_create_country();

public:
	// �����̵�
	static void get_family_shop_data(const global_user_ptr& p_user, proto::client::ec_get_family_shop_reply& reply);
	// ����
	static uint32_t family_shop_buy_request(const global_user_ptr& p_user, uint32_t index, uint32_t shop_item, uint32_t num);
	// game ����ʧ�ܻ��˴���
	static void family_shop_failed_back(const global_user_ptr& p_user, uint32_t refresh_time, uint32_t index, uint32_t num);
};