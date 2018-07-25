#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include <set>
/// @brief    表格结构:FamilyOfficialBase
/// @remark   记录大小:16 X32
///           记录大小:20 X64
///           字段数量:4
///           数据文件:j家族表.xlsx legion_official
///           排序主键:ID
#pragma pack(1)
struct FamilyOfficialBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00712FE6; ///< i|iiV
#else
	static const int          FormatCheckCode = 0x00713006; ///< i|iiv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	int32_t                   id;                           ///< id

	int32_t                   type;                         ///< 官职类型
	int32_t                   player_cnt;                   ///< 官职数量上限
	const char*               power;                        ///< 权限范围
};
#pragma pack()

struct FamilyOfficial : public Entry
{
	int32_t                   id()                          { return t_id; }

	int32_t                   type()                        { return t_type; }
	int32_t                   player_cnt()                  { return t_player_cnt; }
	//const std::string&        power()                       { return t_power; }
	bool					  is_power(uint32_t id);

private:
	int32_t                   t_id;                         ///< id

	int32_t                   t_type;                       ///< 官职类型
	int32_t                   t_player_cnt;                 ///< 官职数量上限
	//std::string               t_power;                      ///< 权限范围

	std::set<uint32_t>		  t_power;                      ///< 权限范围	

public:
	void reset();
	void fill( const FamilyOfficialBase& base );
	const char* getClassName() const { return "j家族表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FamilyOfficial, FamilyOfficialBase > FamilyOfficialManager;

inline void FamilyOfficial::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_player_cnt = 0;
	this->t_power.clear();
};

inline void FamilyOfficial::fill( const FamilyOfficialBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_player_cnt = base.player_cnt;
	//if( base.power == NULL ) this->t_power.clear(); else this->t_power.assign( base.power );
	std::vector<uint32_t> vec;
	common::string_util_t::split<uint32_t>(base.power, vec, "$");
	this->t_power.insert(vec.begin(), vec.end());
};

inline bool FamilyOfficial::is_power(uint32_t id)
{
	std::set<uint32_t>::iterator itr = t_power.find(id);
	if (itr != t_power.end())
	{
		return true;
	}

	return  false;
}

inline const std::string FamilyOfficial::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FamilyOfficial.tbls64";
#else
	return "FamilyOfficial.tbls32";
#endif
};
