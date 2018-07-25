#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:MountTableBase
/// @remark   记录大小:28 X32
///           记录大小:28 X64
///           字段数量:7
///           数据文件:z坐骑表.xlsx mount
///           排序主键:ID
#pragma pack(1)
struct MountTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00FCA8C9; ///< I|IffIII
#else
	static const int          FormatCheckCode = 0x00FCA8C9; ///< I|IffIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< 坐骑id

	uint32_t                  quality;                      ///< 品质
	float                     mount_speed;                  ///< 常规时速度
	float                     trade_speed;                  ///< 跑商时速度
	uint32_t                  space;                        ///< 载货空间
	uint32_t                  weight;                       ///< 最大负重
	uint32_t                  people;                       ///< 商队规模
};
#pragma pack()

struct MountTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  quality()                     { return t_quality; }
	float                     mount_speed()                 { return t_mount_speed; }
	float                     trade_speed()                 { return t_trade_speed; }
	uint32_t                  space()                       { return t_space; }
	uint32_t                  weight()                      { return t_weight; }
	uint32_t                  people()                      { return t_people; }

private:
	uint32_t                  t_id;                         ///< 坐骑id

	uint32_t                  t_quality;                    ///< 品质
	float                     t_mount_speed;                ///< 常规时速度
	float                     t_trade_speed;                ///< 跑商时速度
	uint32_t                  t_space;                      ///< 载货空间
	uint32_t                  t_weight;                     ///< 最大负重
	uint32_t                  t_people;                     ///< 商队规模

public:
	void reset();
	void fill( const MountTableBase& base );
	const char* getClassName() const { return "z坐骑表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< MountTable, MountTableBase > MountTableManager;

inline void MountTable::reset()
{
	this->t_id = 0;
	this->t_quality = 0;
	this->t_mount_speed = 0;
	this->t_trade_speed = 0;
	this->t_space = 0;
	this->t_weight = 0;
	this->t_people = 0;
};

inline void MountTable::fill( const MountTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_quality = base.quality;
	this->t_mount_speed = base.mount_speed;
	this->t_trade_speed = base.trade_speed;
	this->t_space = base.space;
	this->t_weight = base.weight;
	this->t_people = base.people;
};

inline const std::string MountTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "MountTable.tbls64";
#else
	return "MountTable.tbls32";
#endif
};
