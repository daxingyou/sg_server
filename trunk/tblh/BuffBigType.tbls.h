#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:BuffBigTypeBase
/// @remark   记录大小:12 X32
///           记录大小:20 X64
///           字段数量:3
///           数据文件:g光环表.xlsx buff大类型关系表
///           排序主键:buff大类型id
#pragma pack(1)
struct BuffBigTypeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000511B6; ///< I|VV
#else
	static const int          FormatCheckCode = 0x000513D6; ///< I|vv
#endif

	//------------------------
	uint32_t getUniqueID() const { return buff_big_type; }; // TODO: 请用 buff大类型id 生成唯一键

	uint32_t                  buff_big_type;                ///< buff大类型id

	const char*               buff_resist_types;            ///< 互斥大类型id
	const char*               buff_replace_types;           ///< 替换大类型id
};
#pragma pack()

struct BuffBigType : public Entry
{
	uint32_t                  buff_big_type()               { return t_buff_big_type; }

	const std::vector<uint32_t>&        buff_resist_types() { return t_buff_resist_types; }
	const std::vector<uint32_t>&        buff_replace_types() { return t_buff_replace_types; }

	uint32_t				buff_resist_type(uint32_t pos) { return pos >= t_buff_resist_types.size() ? 0 : t_buff_resist_types[pos]; }
	uint32_t				buff_replace_type(uint32_t pos) { return pos >= t_buff_replace_types.size() ? 0 : t_buff_replace_types[pos]; }
private:
	uint32_t                  t_buff_big_type;              ///< buff大类型id

	std::vector<uint32_t>     t_buff_resist_types;          ///< 互斥大类型id
	std::vector<uint32_t>     t_buff_replace_types;         ///< 替换大类型id

public:
	void reset();
	void fill( const BuffBigTypeBase& base );
	const char* getClassName() const { return "g光环表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< BuffBigType, BuffBigTypeBase > BuffBigTypeManager;

inline void BuffBigType::reset()
{
	this->t_buff_big_type = 0;
	this->t_buff_resist_types.clear();
	this->t_buff_replace_types.clear();
};

inline void BuffBigType::fill( const BuffBigTypeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_buff_big_type = base.buff_big_type;
	//if( base.buff_resist_types == NULL ) this->t_buff_resist_types.clear(); else this->t_buff_resist_types.assign( base.buff_resist_types );
	//if (base.buff_replace_types == NULL) this->t_buff_replace_types.clear(); else this->t_buff_replace_types.assign(base.buff_replace_types);
	Dawn::parseContainer<uint32_t>(this->t_buff_resist_types, base.buff_resist_types, ":");
	Dawn::parseContainer<uint32_t>(this->t_buff_replace_types, base.buff_replace_types, ":");
};

inline const std::string BuffBigType::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "BuffBigType.tbls64";
#else
	return "BuffBigType.tbls32";
#endif
};
