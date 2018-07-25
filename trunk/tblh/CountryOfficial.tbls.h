#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:CountryOfficialBase
/// @remark   记录大小:16 X32
///           记录大小:20 X64
///           字段数量:4
///           数据文件:g国家表.xlsx country_official
///           排序主键:ID
#pragma pack(1)
struct CountryOfficialBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DE6; ///< I|IIV
#else
	static const int          FormatCheckCode = 0x00510E06; ///< I|IIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  type;                         ///< 官职类型
	uint32_t                  need_power;                   ///< 任命该官职需要的权限
	const char*               power;                        ///< 权限范围
};
#pragma pack()

struct CountryOfficial : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	uint32_t                  need_power()					{ return t_need_power; }
	//const std::string&        power()                       { return t_power; }
	bool					  is_power(uint32_t id);

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_type;                       ///< 官职类型
	uint32_t                  t_need_power;                 ///< 任命该官职需要的权限
	//std::string               t_power;                      ///< 权限范围
	std::set<uint32_t>		  t_power;                      ///< 权限范围	

public:
	void reset();
	void fill( const CountryOfficialBase& base );
	const char* getClassName() const { return "g国家表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< CountryOfficial, CountryOfficialBase > CountryOfficialManager;

inline void CountryOfficial::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_need_power = 0;
	this->t_power.clear();
};

inline void CountryOfficial::fill( const CountryOfficialBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_need_power = base.need_power;
	// if( base.power == NULL ) this->t_power.clear(); else this->t_power.assign( base.power );
	std::vector<uint32_t> vec;
	common::string_util_t::split<uint32_t>(base.power, vec, "$");
	this->t_power.insert(vec.begin(), vec.end());
};

inline bool CountryOfficial::is_power(uint32_t id)
{
	std::set<uint32_t>::iterator itr = t_power.find(id);
	if (itr != t_power.end())
	{
		return true;
	}

	return  false;
}

inline const std::string CountryOfficial::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "CountryOfficial.tbls64";
#else
	return "CountryOfficial.tbls32";
#endif
};
