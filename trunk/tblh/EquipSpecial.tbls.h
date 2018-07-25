#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:EquipSpecialBase
/// @remark   记录大小:20 X32
///           记录大小:24 X64
///           字段数量:5
///           数据文件:z装备表.xlsx 特技
///           排序主键:装备特技id
#pragma pack(1)
struct EquipSpecialBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510EAD9; ///< I|IVII
#else
	static const int          FormatCheckCode = 0x05110AD9; ///< I|IvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 装备特技id 生成唯一键

	uint32_t                  id;                           ///< 装备特技id

	uint32_t                  pool_id;                      ///< 装备特技池id
	const char*               attr;                         ///< 三件特技属性
	uint32_t                  prob;                         ///< 装备特技概率
	uint32_t                  score;                        ///< 特技评分
};
#pragma pack()

struct EquipSpecial : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  pool_id()                     { return t_pool_id; }
	const std::vector<uint32_t>&        attr()                        { return t_attr; }
	uint32_t                  prob()                        { return t_prob; }
	uint32_t                  score() { return t_score; }

private:
	uint32_t                  t_id;                         ///< 装备特技id

	uint32_t                  t_pool_id;                    ///< 装备特技池id
	std::vector<uint32_t>               t_attr;                       ///< 三件特技属性
	uint32_t                  t_prob;                       ///< 装备特技概率
	uint32_t                  t_score;                      ///< 特技评分

public:
	void reset();
	void fill( const EquipSpecialBase& base );
	const char* getClassName() const { return "z装备表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipSpecial, EquipSpecialBase > EquipSpecialManager;

inline void EquipSpecial::reset()
{
	this->t_id = 0;
	this->t_pool_id = 0;
	this->t_attr.clear();
	this->t_prob = 0;
	this->t_score = 0;
};

inline void EquipSpecial::fill( const EquipSpecialBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_pool_id = base.pool_id;
	//if( base.attr == NULL ) this->t_attr.clear(); else this->t_attr.assign( base.attr );
	Dawn::parseContainer<uint32_t>(this->t_attr, base.attr, "$");
	this->t_prob = base.prob;
	this->t_score = base.score;
};

inline const std::string EquipSpecial::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipSpecial.tbls64";
#else
	return "EquipSpecial.tbls32";
#endif
};
