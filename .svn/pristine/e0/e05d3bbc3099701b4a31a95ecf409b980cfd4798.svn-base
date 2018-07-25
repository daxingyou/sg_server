#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:SysChannelBase
/// @remark   记录大小:28 X32
///           记录大小:28 X64
///           字段数量:7
///           数据文件:系统频道表.xlsx Sheet1
///           排序主键:类型,相关参数
#pragma pack(1)
struct SysChannelBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010DD939; ///< II|IIIII
#else
	static const int          FormatCheckCode = 0x010DD939; ///< II|IIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return common::hash_util_t::hash_to_uint32(type, paramId); }; // TODO: 请用 类型,相关参数 生成唯一键

	uint32_t                  type;                         ///< 类型
	uint32_t                  is_notify;                    ///< 是否广播

	uint32_t                  paramId;                      ///< 相关参数
	uint32_t                  count;                        ///< 广播数量
	uint32_t                  notify_id;                    ///< 系统通知ID
	uint32_t                  language_id;                  ///< 语言表ID
	uint32_t                  label;                        ///< 标签类型
};
#pragma pack()

struct SysChannel : public Entry
{
	uint32_t                  type()                        { return t_type; }
	uint32_t                  is_notify()                   { return t_is_notify; }

	uint32_t                  paramId()                     { return t_paramId; }
	uint32_t                  count()                       { return t_count; }
	uint32_t                  notify_id()                   { return t_notify_id; }
	uint32_t                  language_id()                 { return t_language_id; }
	uint32_t                  label()                       { return t_label; }

private:
	uint32_t                  t_type;                       ///< 类型
	uint32_t                  t_is_notify;                  ///< 是否广播

	uint32_t                  t_paramId;                    ///< 相关参数
	uint32_t                  t_count;                      ///< 广播数量
	uint32_t                  t_notify_id;                  ///< 系统通知ID
	uint32_t                  t_language_id;                ///< 语言表ID
	uint32_t                  t_label;                      ///< 标签类型

public:
	void reset();
	void fill( const SysChannelBase& base );
	const char* getClassName() const { return "系统频道表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SysChannel, SysChannelBase > SysChannelManager;

inline void SysChannel::reset()
{
	this->t_type = 0;
	this->t_is_notify = 0;
	this->t_paramId = 0;
	this->t_count = 0;
	this->t_notify_id = 0;
	this->t_language_id = 0;
	this->t_label = 0;
};

inline void SysChannel::fill( const SysChannelBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_type = base.type;
	this->t_is_notify = base.is_notify;
	this->t_paramId = base.paramId;
	this->t_count = base.count;
	this->t_notify_id = base.notify_id;
	this->t_language_id = base.language_id;
	this->t_label = base.label;
};

inline const std::string SysChannel::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SysChannel.tbls64";
#else
	return "SysChannel.tbls32";
#endif
};
