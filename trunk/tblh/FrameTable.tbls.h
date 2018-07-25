#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"
#include "common/utility.hpp"
/// @brief    ���ṹ:FrameTableBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:tͷ���.xlsx Frame
///           ��������:�߿�id
#pragma pack(1)
struct FrameTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �߿�id ����Ψһ��

	uint32_t                  id;                           ///< �߿�id

	const char*               cond;                         ///< ��������
};
#pragma pack()

struct FrameTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t, uint32_t>&        cond()       { return t_cond; }

private:
	uint32_t                  t_id;                         ///< �߿�id

	std::map<uint32_t, uint32_t>   t_cond;                  ///< ��������

public:
	void reset();
	void fill( const FrameTableBase& base );
	const char* getClassName() const { return "tͷ���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FrameTable, FrameTableBase > FrameTableManager;

inline void FrameTable::reset()
{
	this->t_id = 0;
	this->t_cond.clear();
};

inline void FrameTable::fill( const FrameTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	///if( base.cond == NULL ) this->t_cond.clear(); else this->t_cond.assign( base.cond );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_cond, base.cond, ":$");
};

inline const std::string FrameTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FrameTable.tbls64";
#else
	return "FrameTable.tbls32";
#endif
};
