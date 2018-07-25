#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:MapTableBase
/// @remark   记录大小:48 X32
///           记录大小:68 X64
///           字段数量:12
///           数据文件:c场景表.xlsx 场景
///           排序主键:id
#pragma pack(1)
struct MapTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0AB67339; ///< I|VVIVVVIIIII
#else
	static const int          FormatCheckCode = 0x0C906D59; ///< I|vvIvvvIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< 场景ID

	const char*               main_game;                    ///< 主服务器组
	const char*               diversion_game;               ///< 分流服务器组
	uint32_t                  scene_type;                   ///< 场景类型
	const char*               scene_attr;                   ///< 场景属性
	const char*               scene_born;                   ///< 出生点
	const char*               scene_safe;                   ///< 安全区
	uint32_t                  revive_point;                 ///< 复活点
	uint32_t                  scene_city;                   ///< 场景对应的城市
	uint32_t                  map_size;                     ///< 地图大小
	uint32_t                  map_group_id;                 ///< 场景组ID
	uint32_t                  city_attr;                    ///< 城市属性
};
#pragma pack()

struct MapTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::pair<uint32_t, uint32_t>& main_game()        { return t_main_game; }
	uint32_t                  scene_type()                  { return t_scene_type; }
	const std::string&        scene_attr()                  { return t_scene_attr; }
	const std::string&        scene_born()                  { return t_scene_born; }
	const std::string&        scene_safe()                  { return t_scene_safe; }
	uint32_t                  revive_point()                { return t_revive_point; }
	uint32_t                  scene_city()                  { return t_scene_city; }
	uint32_t                  map_size()                    { return t_map_size; }
	uint32_t                  map_group_id()				{ return t_map_group_id; }
	uint32_t                  city_attr()                   { return t_city_attr; }

	uint32_t				  get_scene_attr_type()			{ return t_scene_attr_type; }
	uint32_t				  get_scene_attr_type_param()	{ return t_scene_attr_type_param; }
	const std::vector<std::pair<uint32_t, uint32_t>>& get_born_list() { return t_born_list; }

	std::pair<uint32_t, uint32_t> get_diversion_game(uint32_t game_id);

private:
	uint32_t                  t_id;                         ///< 场景ID

	std::pair<uint32_t, uint32_t> t_main_game;              ///< 主服务器组
	std::map<uint32_t, std::pair<uint32_t, uint32_t>> t_diversion_game; ///< 分流服务器组
	uint32_t                  t_scene_type;                 ///< 场景类型
	std::string               t_scene_attr;                 ///< 场景属性
	std::string               t_scene_born;                 ///< 出生点
	std::string               t_scene_safe;                 ///< 安全区
	uint32_t                  t_revive_point;               ///< 复活点
	uint32_t                  t_scene_city;                 ///< 场景对应的城市
	uint32_t                  t_map_size;                   ///< 地图大小
	uint32_t                  t_map_group_id;               ///< 场景组ID
	uint32_t                  t_city_attr;                  ///< 城市属性

	uint32_t				  t_scene_attr_type;			// 场景类型
	uint32_t				  t_scene_attr_type_param;		// 场景类型

	std::vector<std::pair<uint32_t, uint32_t>> t_born_list; // 等级段出生点类别

public:
	void reset();
	void fill( const MapTableBase& base );
	const char* getClassName() const { return "c场景表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< MapTable, MapTableBase > MapTableManager;

inline
std::pair<uint32_t, uint32_t> MapTable::get_diversion_game(uint32_t game_id)
{
	std::map<uint32_t, std::pair<uint32_t, uint32_t>>::const_iterator citr = t_diversion_game.find(game_id);
	if (citr != t_diversion_game.end()) {
		return citr->second;
	}

	return std::pair<uint32_t, uint32_t>(0, 0);
}

inline void MapTable::reset()
{
	this->t_id = 0;
	this->t_main_game = std::pair<uint32_t, uint32_t>(0,0);
	this->t_diversion_game.clear();
	this->t_scene_type = 0;
	this->t_scene_attr.clear();
	this->t_scene_born.clear();
	this->t_scene_safe.clear();
	this->t_revive_point = 0;
	this->t_scene_city = 0;
	this->t_map_size = 0;
	this->t_map_group_id = 0;
	this->t_city_attr = 0;
	this->t_scene_attr_type = 0;
	this->t_scene_attr_type_param = 0;
};

inline void MapTable::fill( const MapTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if (base.main_game == NULL) this->t_main_game.clear(); else this->t_main_game.assign(base.main_game);
	if (base.main_game != NULL) {
		std::vector<uint32_t> str;
		common::string_util_t::split<uint32_t>(std::string(base.main_game), str, ":");
		if (str.size() == 2) {
			this->t_main_game = std::pair<uint32_t, uint32_t>(str[0], str[1]);
		}
	}
	//if (base.diversion_game == NULL) this->t_diversion_game.clear(); else this->t_diversion_game.assign(base.diversion_game);
	if (base.diversion_game != NULL) {
		std::vector<std::string> str_list;
		common::string_util_t::split<std::string>(std::string(base.diversion_game), str_list, "$");
		for (const auto& it : str_list) {
			std::vector<uint32_t> str;
			common::string_util_t::split<uint32_t>(it, str, ":");
			if (str.size() == 3) {
				this->t_diversion_game.insert(std::make_pair(str[0], std::pair<uint32_t, uint32_t>(str[1], str[2])));
			}
		}
	}
	this->t_scene_type = base.scene_type;
	if( base.scene_attr == NULL ) this->t_scene_attr.clear(); else this->t_scene_attr.assign( base.scene_attr );
	if( base.scene_born == NULL ) this->t_scene_born.clear(); else this->t_scene_born.assign( base.scene_born );
	if( base.scene_safe == NULL ) this->t_scene_safe.clear(); else this->t_scene_safe.assign( base.scene_safe );
	this->t_revive_point = base.revive_point;
	this->t_scene_city = base.scene_city;
	this->t_map_size = base.map_size;
	this->t_map_group_id = base.map_group_id;
	this->t_city_attr = base.city_attr;
	if (!this->t_scene_attr.empty())
	{
		std::vector<uint32_t> attr;
		common::string_util_t::split<uint32_t>(this->t_scene_attr, attr, "$");
		if (attr.size() == 2)
		{
			t_scene_attr_type = attr[0];
			t_scene_attr_type_param = attr[1];
		}
	}

	if (!this->t_scene_born.empty())
	{
		std::vector<std::string> born_list;
		common::string_util_t::split<std::string>(this->t_scene_born, born_list, "$");
		for (auto& it: born_list)
		{
			std::vector<uint32_t> level_list;
			common::string_util_t::split<uint32_t>(it, level_list, ":");
			if (level_list.size() == 2)
			{
				t_born_list.push_back(std::make_pair(level_list.at(0),level_list.at(1)));
			}
		}
	}
};

inline const std::string MapTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "MapTable.tbls64";
#else
	return "MapTable.tbls32";
#endif
};
