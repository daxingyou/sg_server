#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:HeroJobTransferBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:hero_job_transfer.xlsx Sheet1
///           排序主键:角色序列
#pragma pack(1)
struct HeroJobTransferBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 角色序列 生成唯一键

	uint32_t                  id;                           ///< 角色序列

	uint32_t                  initial_id;                   ///< 创建角色id
};
#pragma pack()

struct HeroJobTransfer : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  initial_id()                  { return t_initial_id; }

private:
	uint32_t                  t_id;                         ///< 角色序列

	uint32_t                  t_initial_id;                 ///< 创建角色id

public:
	void reset();
	void fill( const HeroJobTransferBase& base );
	const char* getClassName() const { return "hero_job_transfer"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroJobTransfer, HeroJobTransferBase > HeroJobTransferManager;

inline void HeroJobTransfer::reset()
{
	this->t_id = 0;
	this->t_initial_id = 0;
};

inline void HeroJobTransfer::fill( const HeroJobTransferBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_initial_id = base.initial_id;
};

inline const std::string HeroJobTransfer::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroJobTransfer.tbls64";
#else
	return "HeroJobTransfer.tbls32";
#endif
};
