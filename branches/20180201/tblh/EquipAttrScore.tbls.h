#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:EquipAttrScoreBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:z战力转换表.xlsx 装备非数值转战力
///           排序主键:属性ID
#pragma pack(1)
struct EquipAttrScoreBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 属性ID 生成唯一键

	uint32_t                  id;                           ///< 属性ID

	uint32_t                  score;                        ///< 提升战力/评分
};
#pragma pack()

struct EquipAttrScore : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  score()                       { return t_score; }

private:
	uint32_t                  t_id;                         ///< 属性ID

	uint32_t                  t_score;                      ///< 提升战力/评分

public:
	void reset();
	void fill( const EquipAttrScoreBase& base );
	const char* getClassName() const { return "z战力转换表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipAttrScore, EquipAttrScoreBase > EquipAttrScoreManager;

inline void EquipAttrScore::reset()
{
	this->t_id = 0;
	this->t_score = 0;
};

inline void EquipAttrScore::fill( const EquipAttrScoreBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_score = base.score;
};

inline const std::string EquipAttrScore::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipAttrScore.tbls64";
#else
	return "EquipAttrScore.tbls32";
#endif
};
