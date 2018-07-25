#pragma once

#include <vector>

#include "macros.hpp"
#include "tblh/rank_common.hpp"

class rank_t
{
public:
	rank_t() = delete;
	rank_t(common::rank_common::rank_type type, const char* name);
	virtual ~rank_t() = default;

	virtual bool init();

public:
	// 获得排行榜类型
	common::rank_common::rank_type get_rank_type() const { return m_rank_type; }
	// 数据库和排序zset名字
	const std::string& get_rank_name() const { return m_rank_name; }

public:
	// 获得排行榜数量
	uint32_t get_rank_count();
	// 获得排行榜
	uint32_t get_rank_count_by_score(uint32_t min, uint32_t max);
	// 更新排行榜数值
	bool update_rank_score(uint64_t uid, uint32_t score);
	// 批量设置排行榜
	bool update_rank_score_batch(const std::vector<std::pair<uint64_t, uint32_t>>& vecs);
	// 是否在榜单中
	bool is_in_rank(uint64_t uid);
	// 删除一个
	bool rem_rank_by_uid(uint64_t uid);
	// 批量删除[start,stop]
	bool batch_rem_rank_by_rank(int32_t start, int32_t stop);
	// 批量删除[min,max]
	bool batch_rem_rank_by_score(uint32_t min, uint32_t max);
	// 获得排序权值
	uint32_t get_rank_score(uint64_t uid);
	// 获得排名(0是第一名 -1 表示不在排行榜)
	uint32_t get_rank_value(uint64_t uid);
	// 获得指定范围的排名(参数一样则为获得指定排名)
	bool get_range_by_rank(int32_t start, int32_t stop, std::vector<uint64_t>& result);
	// 获得指定范围的排名
	bool get_range_by_rank_withscores(int32_t start, int32_t stop, std::vector<std::pair<uint64_t, uint32_t>>& result);
	// 获得指定范围的排名
	bool get_range_by_score(uint32_t max, uint32_t min, std::vector<uint64_t>& result);
	// 设置开关
	void set_open(bool val) { m_open = val; }
	
	// 清空排行榜
	void clear_rank();

protected:
	// 添加或者更新一个新的权值
	bool update_sort(uint64_t uid, uint32_t score);
	// 对于临时排行榜做数据记录
	bool update_sort_add(uint64_t uid, uint32_t score_add);
	// 更新排行榜信息
	void update_rank();
	// 删除一条在榜单中的数据并删除redis中的数据
	virtual void remove_rank(uint64_t uid);

protected:
	static const int32_t RANK_PAGE_COUNT = 10;

protected:
	common::rank_common::rank_type m_rank_type;	// 排行榜类型
	bool m_is_time_temp = false;				// 是否为临时排行榜，不断累加，到时间会清理
	std::string m_rank_name;		// 数据归属引用
	uint32_t m_min_condi = 0;		// 入榜条件
	uint32_t m_max_count = 0;		// 最多上榜个数

	uint32_t m_curr_count = 0;		// 当前排行榜个数
	uint32_t m_min_value = 0;		// 榜单最小的值
	uint64_t m_min_uid = 0;			// 榜单最小uid

	bool m_open = true;			// 是否开启（限时用）
};