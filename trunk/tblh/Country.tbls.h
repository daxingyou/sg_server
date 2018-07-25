#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:CountryBase
/// @remark   记录大小:40 X32
///           记录大小:48 X64
///           字段数量:10
///           数据文件:g国家表.xlsx country
///           排序主键:ID
#pragma pack(1)
struct CountryBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0A8CC739; ///< I|VIVIIIIII
#else
	static const int          FormatCheckCode = 0x0A8CE6D9; ///< I|vIvIIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< id

	const char*               name;                         ///< 初始国家名称
	uint32_t                  capital;                      ///< 王城
	const char*               capital_name;                 ///< 王城名称
	uint32_t                  att_area_id;                  ///< 右方区域
	uint32_t                  def_area_id;                  ///< 左方区域
	uint32_t                  att_jump_id;                  ///< 右方出生点
	uint32_t                  def_jump_id;                  ///< 左方出生点
	uint32_t                  att_jump_pos;                 ///< 立国战右方跳转点
	uint32_t                  def_jump_pos;                 ///< 立国战左方跳转点
};
#pragma pack()

struct Country : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        name()						{ return t_name; }
	uint32_t                  capital()						{ return t_capital; }
	const std::string&        capital_name()				{ return t_capital_name; }
	uint32_t                  att_area_id()					{ return t_att_area_id; }
	uint32_t                  def_area_id()					{ return t_def_area_id; }
	uint32_t                  att_jump_id()					{ return t_att_jump_id; }
	uint32_t                  def_jump_id()					{ return t_def_jump_id; }
	uint32_t                  att_jump_pos()				{ return t_att_jump_pos; }
	uint32_t                  def_jump_pos()				{ return t_def_jump_pos; }

private:
	uint32_t                  t_id;                         ///< id

	std::string               t_name;                       ///< 国家名称
	uint32_t                  t_capital;                    ///< 王城
	std::string               t_capital_name;               ///< 王城名称
	uint32_t                  t_att_area_id;                ///< 进攻方区域
	uint32_t                  t_def_area_id;                ///< 防守方区域
	uint32_t                  t_att_jump_id;                ///< 进攻方跳转点
	uint32_t                  t_def_jump_id;                ///< 防守方跳转点
	uint32_t                  t_att_jump_pos;               ///< 立国战右方跳转点
	uint32_t                  t_def_jump_pos;               ///< 立国战左方跳转点

public:
	void reset();
	void fill( const CountryBase& base );
	const char* getClassName() const { return "g国家表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Country, CountryBase > CountryManager;

inline void Country::reset()
{
	this->t_id = 0;
	this->t_name.clear();
	this->t_capital = 0;
	this->t_capital_name.clear();
	this->t_att_area_id = 0;
	this->t_def_area_id = 0;
	this->t_att_jump_id = 0;
	this->t_def_jump_id = 0;
	this->t_att_jump_pos = 0;
	this->t_def_jump_pos = 0;
};

inline void Country::fill( const CountryBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	this->t_capital = base.capital;
	if (base.capital_name == NULL) this->t_capital_name.clear(); else this->t_capital_name.assign(base.capital_name);
	this->t_att_area_id = base.att_area_id;
	this->t_def_area_id = base.def_area_id;
	this->t_att_jump_id = base.att_jump_id;
	this->t_def_jump_id = base.def_jump_id;
	this->t_att_jump_pos = base.att_jump_pos;
	this->t_def_jump_pos = base.def_jump_pos;
};

inline const std::string Country::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Country.tbls64";
#else
	return "Country.tbls32";
#endif
};
