#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:TowerTriggerBase
/// @remark   记录大小:20 X32
///           记录大小:24 X64
///           字段数量:5
///           数据文件:s时空幻境.xlsx 隐藏层
///           排序主键:隐藏层id
#pragma pack(1)
struct TowerTriggerBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0511ADD9; ///< I|VIII
#else
	static const int          FormatCheckCode = 0x0513ADD9; ///< I|vIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 隐藏层id 生成唯一键

	uint32_t                  id;                           ///< 隐藏层id

	const char*               name;                         ///< 私聊中关卡名
	uint32_t                  alive_time;                   ///< 存在时间sec
	uint32_t                  against_id;                   ///< 阵容id
	uint32_t                  awards;                       ///< 奖励
};
#pragma pack()

struct TowerTrigger : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        name()                        { return t_name; }
	uint32_t                  alive_time()                  { return t_alive_time; }
	uint32_t                  against_id()                  { return t_against_id; }
	uint32_t                  awards()                      { return t_awards; }

private:
	uint32_t                  t_id;                         ///< 隐藏层id

	std::string               t_name;                       ///< 私聊中关卡名
	uint32_t                  t_alive_time;                 ///< 存在时间sec
	uint32_t                  t_against_id;                 ///< 阵容id
	uint32_t                  t_awards;                     ///< 奖励

public:
	void reset();
	void fill( const TowerTriggerBase& base );
	const char* getClassName() const { return "s时空幻境"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TowerTrigger, TowerTriggerBase > TowerTriggerManager;

inline void TowerTrigger::reset()
{
	this->t_id = 0;
	this->t_name.clear();
	this->t_alive_time = 0;
	this->t_against_id = 0;
	this->t_awards = 0;
};

inline void TowerTrigger::fill( const TowerTriggerBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	this->t_alive_time = base.alive_time;
	this->t_against_id = base.against_id;
	this->t_awards = base.awards;
};

inline const std::string TowerTrigger::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TowerTrigger.tbls64";
#else
	return "TowerTrigger.tbls32";
#endif
};
