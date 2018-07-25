#ifndef __RANK_SORT_H__
#define __RANK_SORT_H__
#include "common/Entry.h"
#include "redis/rank_data_mgr.hpp"
#include "redis/global_user_data_mgr.hpp"
#include "common.pb.h"

static const uint32_t RANK_RECORD_NUM = 20;
static const uint32_t RANK_SAVE_RCORD_NUM = 2000;

enum rank_owner_type
{
	rot_none = 0,
	rot_user = 1,
	rot_family = 2,
};

template< typename T, typename Key, typename Value, typename Compare >
class sort_rank_t : public Entry
{
public:
	sort_rank_t(bool is_max)
		: Entry(0, "Rank")
		, m_record_num(RANK_RECORD_NUM)
		, m_save_record_num(RANK_SAVE_RCORD_NUM)
		, m_is_max(is_max)
		, m_owner_type(rot_none)
		, m_need_save(false)
	{
	}

	~sort_rank_t()
	{
	}

	const char* getClassName()
	{
		return "Rank";
	}

public:
	/// @brief 初始化
	bool init(uint32_t id, uint32_t min_value, const char* name, rank_owner_type owner_type)
	{
		set_rank_id(id);
		set_min_value(min_value);
		strncpy(this->m_name, name, MAX_NAMESIZE);
		set_owner_type(owner_type);
		if (!load_from_db())
		{
			return false;
		}

		return true;
	}
	//设置门槛
	void set_min_value(uint32_t val)
	{
		m_min_value = val;
	}

	/// @brief 设置记录数量
	void set_record_num(uint32_t num)
	{
		m_record_num = num;
	}
	/// @brief 数据库中的最大数量
	void set_save_record_num(uint32_t num)
	{
		m_save_record_num = num;
	}

	void set_owner_type(rank_owner_type type)
	{
		m_owner_type = type;
	}

	rank_owner_type get_owner_type()
	{
		return m_owner_type;
	}

	/// @brief 读取数据库
	bool load_from_db()
	{
		proto::common::rank_data save;
		rank_data_mgr_t::load_rank_data(this->get_id(), save);
		unserialise_rank(save);

		RANK_LOG("load %s %u, data_size %d", this->get_name().c_str(), this->get_id(), save.ByteSize());
		return true;
	}

	/// @brief 存取数据库
	bool do_save()
	{
		if (!m_need_save)
			return false;

		proto::common::rank_data save;
		serialize_rank(save);
		rank_data_mgr_t::save_rank_data(this->get_id(), save);
		m_need_save = false;
		RANK_LOG("save %s %u,data_size %u", this->get_name().c_str(), this->get_id(), save.ByteSize());
		return true;
	}

	bool check_save(bool immediately)
	{
		return do_save();
	}

	bool set_need_save()
	{
		m_need_save = true;
		do_save(); // 现在先立即保存
		return true;
	}

	/// @brief 停机保存
	bool final()
	{
		check_save(true);
		return true;
	}

	/// @brief 零点处理
	bool zero_hour()
	{
		return true;
	}

	/// @brief 反序列化
	bool unserialise_rank(const proto::common::rank_data& save)
	{
		for (int i = 0; i < save.rank_list_size(); ++i)
		{
			if (save.rank_list(i).key() == 0)
				continue;

			Value item;
			item.key.key = save.rank_list(i).key();
			item.key.time = save.rank_list(i).time();
			item.owner = save.rank_list(i).owner();
			for (int32_t j = 0; j < save.rank_list(i).custom_data_size(); ++j)
			{
				item.custom_data.push_back(save.rank_list(i).custom_data(j));
			}
			m_rank.insert(sort_value(item.key, item));
		}

		return true;
	}

	/// @brief 序列化
	bool serialize_rank(proto::common::rank_data& save)
	{
		save.set_count(m_rank.size());

		for (sort_iter it = m_rank.begin(); it != m_rank.end(); ++it)
		{
			proto::common::rank_single* add = save.add_rank_list();
			if (NULL == add)
				continue;

			Value &item = it->second;

			add->set_key(item.key.key);
			add->set_time(item.key.time);
			add->set_owner(item.owner);
			for (auto str : item.custom_data)
			{
				add->add_custom_data(str);
			}
		}

		return true;
	}


	/// @brief 更新榜单
	bool update_rank(uint64_t owner, Value value)
	{
		RANK_LOG("rank[%d] update role[%llu] key[%d]", get_id(), owner, value.key.key);
		if (owner == 0)
			return false;

		if (!can_enter_rank(value.key))
		{
			/// 参与过排行
			delete_by_owner(owner);
			return false;
		}
		RANK_LOG("rank[%d] update role[%llu] key[%d] success", get_id(), owner, value.key.key);
		sort_iter it = m_rank.begin();
		uint32_t need_del = 0;
		for (; it != m_rank.end(); ++it)
		{
			if (it->second.owner == owner)
			{
				need_del = 1;
				break;
			}
		}
		/// 清除榜上玩家名次
		uint32_t index = 1;
		std::map< uint32_t, uint32_t > old_index;
		for (sort_iter del_it = m_rank.begin(); del_it != m_rank.end(); ++del_it, ++index)
		{
			update_owner_rank(del_it->second.owner, 0);
			old_index[del_it->second.owner] = index;
		}

		if (need_del)
		{
			m_rank.erase(it);
		}

		m_rank.insert(sort_value(value.key, value));

		index = 1;
		/// 设置名称
		for (sort_iter del_it = m_rank.begin(); del_it != m_rank.end() && index <= m_save_record_num; ++del_it, ++index)
		{
			update_owner_rank(del_it->second.owner, index);
		}

		if (m_rank.size() > m_save_record_num)
		{
			sort_iter last_it = m_rank.end();
			m_rank.erase(--last_it);
		}

		set_need_save();
		return true;
	}

	void update_owner_rank(uint64_t owner, uint32_t rank)
	{
		if (m_owner_type == rot_user)
		{
			global_user_data_mgr_t::set_rank(owner, this->get_id(), rank);
		}
		else if (m_owner_type == rot_family)
		{

		}
	}

	uint32_t get_owner_rank(uint64_t owner)
	{
		if (m_owner_type == rot_user)
		{
			return global_user_data_mgr_t::get_rank(owner, this->get_id());
		}
		else if (m_owner_type == rot_family)
		{

		}

		return 0;
	}

	/// @brief 拷贝榜单
	bool copy_from(sort_rank_t* source)
	{
		if (NULL == source)
		{
			return false;
		}

		clear();

		uint32_t index = 1;
		for (sort_iter it = source->m_rank.begin(); it != source->m_rank.end() && index <= m_save_record_num; ++it, ++index)
		{
			update_rank(it->second.owner, it->second);
		}

		return true;
	}

	/// @brief 是否能进入榜单
	bool can_enter_rank(Key key)
	{
		if (key.key == 0)
		{
			return false;
		}
		if (key.key < m_min_value)
		{
			return false;
		}
		if (m_save_record_num <= 0)
		{
			return false;
		}

		if (m_rank.size() <= m_save_record_num)
		{
			return true;
		}
		else
		{
			sort_reverse_iter it = m_rank.rbegin();
			if (m_is_max)
			{
				/// 从大往小
				if (m_compare(key, it->first))
					return true;
			}
			else
			{
				/// 从小到大
				if (m_compare(key, it->first))
					return true;
			}
		}
		return false;
	}


	/// @brief 清除榜单
	void clear()
	{
		uint32_t index = 1;
		for (sort_iter del_it = m_rank.begin(); del_it != m_rank.end() && index <= m_save_record_num; ++del_it, ++index)
		{
			update_owner_rank(del_it->second.owner, 0);
		}

		m_rank.clear();
		set_need_save();
	}

	/// @brief 玩家删号
	void delete_from_rank(uint64_t owner)
	{
		if (get_owner_rank(owner) == 0)
			return;

		sort_iter it = m_rank.begin();
		uint32_t need_del = 0;
		for (; it != m_rank.end(); ++it)
		{
			if (it->second.owner == owner)
			{
				need_del = 1;
				break;
			}
		}

		uint32_t index = 1;
		/// 清除榜上玩家名次
		for (sort_iter del_it = m_rank.begin(); del_it != m_rank.end() && index <= m_save_record_num; ++del_it, ++index)
		{
			update_owner_rank(del_it->second.owner, 0);
		}

		if (need_del)
		{
			m_rank.erase(it);
		}

		index = 1;
		/// 设置名称
		for (sort_iter del_it = m_rank.begin(); del_it != m_rank.end() && index <= m_save_record_num; ++del_it, ++index)
		{
			update_owner_rank(del_it->second.owner, index);
		}
		set_need_save();
	}

	void delete_by_owner(const uint64_t ownerid)
	{
		sort_iter it = m_rank.begin();
		uint32_t need_del = 0;
		for (; it != m_rank.end(); ++it)
		{
			if (it->second.owner == ownerid)
			{
				need_del = 1;
				break;
			}
		}

		uint32_t index = 1;
		/// 清除榜上玩家名次
		for (sort_iter del_it = m_rank.begin(); del_it != m_rank.end() && index <= m_save_record_num; ++del_it, ++index)
		{
			update_owner_rank(del_it->second.owner, 0);
		}

		if (need_del)
		{
			m_rank.erase(it);
		}

		index = 1;
		/// 设置名称
		for (sort_iter del_it = m_rank.begin(); del_it != m_rank.end() && index <= m_save_record_num; ++del_it, ++index)
		{
			update_owner_rank(del_it->second.owner, index);
		}
		set_need_save();
	}

protected:
	/// @brief 设置榜单id
	void set_rank_id(uint32_t id)
	{
		this->m_id = id;
	}
	/// @brief 大小排序
	void set_max_sort(bool is_max)
	{
		m_is_max = is_max;
	}
protected:
	typedef typename T::value_type sort_value;
	typedef typename T::iterator sort_iter;
	typedef typename T::reverse_iterator sort_reverse_iter;
	/// @brief 榜单
	T m_rank;

	/// @brief 比较器
	Compare m_compare;

	/// @brief 榜单中存在的记录数量
	uint32_t m_record_num;
	/// @brief 存放的最大数量
	uint32_t m_save_record_num;
	/// @brief 大小排序
	bool m_is_max;
	/// @brief 排行参与者类型
	rank_owner_type m_owner_type;
	/// @brief 是否需要保存
	bool m_need_save;
	//门槛
	uint32_t m_min_value = 0;

};


template< typename T >
struct max_rank_compare
{
	bool operator()(const T &s1, const T &s2) const
	{
		return s1 > s2;
	}
};

template< typename key, typename value >
struct maxmap : std::multimap< key, value, max_rank_compare< key > >
{};

template< typename key, typename value >
class max_sort_rank : public sort_rank_t< maxmap< key, value >, key, value, max_rank_compare< key > >
{
public:
	max_sort_rank()
		: sort_rank_t< maxmap< key, value >, key, value, max_rank_compare< key > >(true)
	{}
	~max_sort_rank()
	{}
};

#endif//__RANK_SORT_H__
