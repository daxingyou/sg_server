#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:OfficeBase
/// @remark   记录大小:20 X32
///           记录大小:24 X64
///           字段数量:5
///           数据文件:n逆命师系统.xlsx 逆命师
///           排序主键:称号id
#pragma pack(1)
struct OfficeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510EAD9; ///< I|IVII
#else
	static const int          FormatCheckCode = 0x05110AD9; ///< I|IvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 称号id 生成唯一键

	uint32_t                  id;                           ///< 称号id

	uint32_t                  exp;                          ///< 升级经验
	const char*               tasks;                        ///< 任务
	uint32_t                  day_award;                    ///< 每日奖励
	uint32_t                  need_level;                   ///< 开启等级
};
#pragma pack()

struct Office : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  exp()                         { return t_exp; }
	const std::vector<uint32_t>&        tasks()                       { return t_tasks; }
	uint32_t                  day_award()                   { return t_day_award; }
	uint32_t                  need_level()                  { return t_need_level; }

private:
	uint32_t                  t_id;                         ///< 称号id

	uint32_t                  t_exp;                        ///< 升级经验
	std::vector<uint32_t>               t_tasks;                      ///< 任务
	uint32_t                  t_day_award;                  ///< 每日奖励
	uint32_t                  t_need_level;                 ///< 开启等级

public:
	void reset();
	void fill( const OfficeBase& base );
	const char* getClassName() const { return "n逆命师系统"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Office, OfficeBase > OfficeManager;

inline void Office::reset()
{
	this->t_id = 0;
	this->t_exp = 0;
	this->t_tasks.clear();
	this->t_day_award = 0;
	this->t_need_level = 0;
};

inline void Office::fill( const OfficeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_exp = base.exp;
	//if( base.tasks == NULL ) this->t_tasks.clear(); else this->t_tasks.assign( base.tasks );
	Dawn::parseContainer<uint32_t>(this->t_tasks, base.tasks, "$");
	this->t_day_award = base.day_award;
	this->t_need_level = base.need_level;
};

inline const std::string Office::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Office.tbls64";
#else
	return "Office.tbls32";
#endif
};
