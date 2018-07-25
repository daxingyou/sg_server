#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:VIPBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:vVIP信息表.xlsx VIP
///           排序主键:VIP等级
#pragma pack(1)
struct VIPBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return vip_level; }; // TODO: 请用 VIP等级 生成唯一键

	uint32_t                  vip_level;                    ///< VIP等级

	uint32_t                  CNY;                          ///< 充值
};
#pragma pack()

struct VIP : public Entry
{
	uint32_t                  vip_level()                   { return t_vip_level; }

	uint32_t                  CNY()                         { return t_CNY; }

private:
	uint32_t                  t_vip_level;                  ///< VIP等级

	uint32_t                  t_CNY;                        ///< 充值

public:
	void reset();
	void fill( const VIPBase& base );
	const char* getClassName() const { return "vVIP信息表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< VIP, VIPBase > VIPManager;

inline void VIP::reset()
{
	this->t_vip_level = 0;
	this->t_CNY = 0;
};

inline void VIP::fill( const VIPBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_vip_level = base.vip_level;
	this->t_CNY = base.CNY;
};

inline const std::string VIP::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "VIP.tbls64";
#else
	return "VIP.tbls32";
#endif
};
