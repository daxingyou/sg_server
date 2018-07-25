#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:SectionBase
/// @remark   记录大小:12 X32
///           记录大小:12 X64
///           字段数量:3
///           数据文件:z章节表.xlsx 章节任务表
///           排序主键:id
#pragma pack(1)
struct SectionBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  task_id;                      ///< 任务ID
	uint32_t                  drop_id;                      ///< 关卡奖励
};
#pragma pack()

struct Section : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  task_id()                     { return t_task_id; }
	uint32_t                  drop_id()                     { return t_drop_id; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_task_id;                    ///< 任务ID
	uint32_t                  t_drop_id;                    ///< 关卡奖励

public:
	void reset();
	void fill( const SectionBase& base );
	const char* getClassName() const { return "z章节表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Section, SectionBase > SectionManager;

inline void Section::reset()
{
	this->t_id = 0;
	this->t_task_id = 0;
	this->t_drop_id = 0;
};

inline void Section::fill( const SectionBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_task_id = base.task_id;
	this->t_drop_id = base.drop_id;
};

inline const std::string Section::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Section.tbls64";
#else
	return "Section.tbls32";
#endif
};
