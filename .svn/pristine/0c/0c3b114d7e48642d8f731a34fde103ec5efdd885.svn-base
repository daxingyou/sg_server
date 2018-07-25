#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:RechargeTotalBase
/// @remark   记录大小:12 X32
///           记录大小:16 X64
///           字段数量:3
///           数据文件:y运营活动表.xlsx 累充豪礼
///           排序主键:充值额度
#pragma pack(1)
struct RechargeTotalBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510E6; ///< I|IV
#else
	static const int          FormatCheckCode = 0x00051106; ///< I|Iv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 充值额度 生成唯一键

	uint32_t                  id;                           ///< 充值额度

	uint32_t                  award;                        ///< 奖励ID
	const char*               display;                      ///< 展示ID
};
#pragma pack()

struct RechargeTotal : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  award()                       { return t_award; }
	const std::string&        display()                     { return t_display; }

private:
	uint32_t                  t_id;                         ///< 充值额度

	uint32_t                  t_award;                      ///< 奖励ID
	std::string               t_display;                    ///< 展示ID

public:
	void reset();
	void fill( const RechargeTotalBase& base );
	const char* getClassName() const { return "y运营活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< RechargeTotal, RechargeTotalBase > RechargeTotalManager;

inline void RechargeTotal::reset()
{
	this->t_id = 0;
	this->t_award = 0;
	this->t_display.clear();
};

inline void RechargeTotal::fill( const RechargeTotalBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_award = base.award;
	if( base.display == NULL ) this->t_display.clear(); else this->t_display.assign( base.display );
};

inline const std::string RechargeTotal::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "RechargeTotal.tbls64";
#else
	return "RechargeTotal.tbls32";
#endif
};
