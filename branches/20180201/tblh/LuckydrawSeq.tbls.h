#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LuckydrawSeqBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:c抽卡表.xlsx 抽卡序列
///           排序主键:抽卡类型
#pragma pack(1)
struct LuckydrawSeqBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return type; }; // TODO: 请用 抽卡类型 生成唯一键

	uint32_t                  type;                         ///< 抽卡类型

	const char*               seq;                          ///< 抽卡序列
};
#pragma pack()

struct LuckydrawSeq : public Entry
{
	uint32_t                  type()                        { return t_type; }

	const std::string&        seq()                         { return t_seq; }

private:
	uint32_t                  t_type;                       ///< 抽卡类型

	std::string               t_seq;                        ///< 抽卡序列

public:
	void reset();
	void fill( const LuckydrawSeqBase& base );
	const char* getClassName() const { return "c抽卡表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LuckydrawSeq, LuckydrawSeqBase > LuckydrawSeqManager;

inline void LuckydrawSeq::reset()
{
	this->t_type = 0;
	this->t_seq.clear();
};

inline void LuckydrawSeq::fill( const LuckydrawSeqBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_type = base.type;
	if( base.seq == NULL ) this->t_seq.clear(); else this->t_seq.assign( base.seq );
};

inline const std::string LuckydrawSeq::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LuckydrawSeq.tbls64";
#else
	return "LuckydrawSeq.tbls32";
#endif
};
