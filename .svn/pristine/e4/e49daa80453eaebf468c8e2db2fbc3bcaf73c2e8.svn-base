#pragma once
#include "redis_common.hpp"

// 分类字段
static const std::string GROUP_ID = "group_id";
// 分层字段
static const std::string LAYER_ID = "layer";
// 版本字段
static const std::string VERSION_ID = "version";
// 基础字段
static const std::string FIGHT_BASE_ID = "fight_data_base";
// 战斗字段
static const std::string COMBAT_DATA_ID = "combat_data";

// 个人录像数据
static const std::string UID_ID = "uid";
static const std::string VIDEO_ID = "video_id";

// 上榜分类类型
enum video_class_type : uint32_t
{
	class_type_top = 0,
	class_type_first,
	class_type_limit,
	class_type_recent,

	class_type_max,
	class_type_none,
};

static const std::string TOP_ID = "top";	// 上榜字段
static const std::string FIRST_ID = "first";// 首通字段
static const std::string LIMIT_ID = "limit";// 极限字段
static const std::string RECENT_ID = "recent";// 最近字段

static const std::string CLASS_LIST[class_type_max] =
{
	TOP_ID, FIRST_ID, LIMIT_ID, RECENT_ID
};