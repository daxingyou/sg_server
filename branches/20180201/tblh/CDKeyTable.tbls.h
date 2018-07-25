#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:CDKeyTableBase
/// @remark   记录大小:12 X32
///           记录大小:16 X64
///           字段数量:3
///           数据文件:d兑换码.xlsx 兑换码
///           排序主键:礼包类型id
#pragma pack(1)
struct CDKeyTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510E6; ///< I|IV
#else
	static const int          FormatCheckCode = 0x00051106; ///< I|Iv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 礼包类型id 生成唯一键

	uint32_t                  id;                           ///< 礼包类型id

	uint32_t                  mail_id;                      ///< 邮件id
	const char*               drop_list;                    ///< 掉落id
};
#pragma pack()

struct CDKeyTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  mail_id()                     { return t_mail_id; }
	//const std::string&        drop_list()                   { return t_drop_list; }
	const std::map<uint32_t, uint32_t>&        drop_list()  { return t_drop_list; }

private:
	uint32_t                  t_id;                         ///< 礼包类型id

	uint32_t                  t_mail_id;                    ///< 邮件id
	//std::string               t_drop_list;                  ///< 掉落id
	std::map<uint32_t, uint32_t> t_drop_list;				///掉落组
public:
	void reset();
	void fill( const CDKeyTableBase& base );
	const char* getClassName() const { return "d兑换码"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< CDKeyTable, CDKeyTableBase > CDKeyTableManager;

inline void CDKeyTable::reset()
{
	this->t_id = 0;
	this->t_mail_id = 0;
	this->t_drop_list.clear();
};

inline void CDKeyTable::fill( const CDKeyTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_mail_id = base.mail_id;
	//if( base.drop_list == NULL ) this->t_drop_list.clear(); else this->t_drop_list.assign( base.drop_list );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_drop_list, base.drop_list, ":$");
};

inline const std::string CDKeyTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "CDKeyTable.tbls64";
#else
	return "CDKeyTable.tbls32";
#endif
};
