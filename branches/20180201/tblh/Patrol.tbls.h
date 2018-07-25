#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:PatrolBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:xѲ�߱�.xlsx Ѳ�߻���
///           ��������:����ID
#pragma pack(1)
struct PatrolBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ����ID ����Ψһ��

	uint32_t                  id;                           ///< ����ID

	const char*               level;                        ///< �Ƽ��ȼ�
};
#pragma pack()

struct Patrol : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::pair<uint32_t, uint32_t>&        level()                       { return t_level; }

private:
	uint32_t                  t_id;                         ///< ����ID

	std::pair<uint32_t, uint32_t>               t_level;                      ///< �Ƽ��ȼ�

public:
	void reset();
	void fill( const PatrolBase& base );
	const char* getClassName() const { return "xѲ�߱�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Patrol, PatrolBase > PatrolManager;

inline void Patrol::reset()
{
	this->t_id = 0;
	//this->t_level.clear();
};

inline void Patrol::fill( const PatrolBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.level == NULL ) this->t_level.clear(); else this->t_level.assign( base.level );
	Dawn::parseTuple<uint32_t, uint32_t>(this->t_level, base.level, "~");
};

inline const std::string Patrol::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Patrol.tbls64";
#else
	return "Patrol.tbls32";
#endif
};
