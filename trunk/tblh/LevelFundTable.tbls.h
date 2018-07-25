#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/utility.hpp"

/// @brief    ���ṹ:LevelFundTableBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:28 X64
///           �ֶ�����:5
///           �����ļ�:y��Ӫ���.xlsx �ɳ�����
///           ��������:ID
#pragma pack(1)
struct LevelFundTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510DEB6; ///< I|IIVV
#else
	static const int          FormatCheckCode = 0x0510E0D6; ///< I|IIvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ID ����Ψһ��

	uint32_t                  id;                           ///< ID

	uint32_t                  level;                        ///< �����ȼ�
	uint32_t                  prize_id;                     ///< ����ID
	const char*               desc;                         ///< ��������
	const char*               show_prize;                   ///< չʾ����
};
#pragma pack()

struct LevelFundTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level()                       { return t_level; }
	uint32_t                  prize_id()                    { return t_prize_id; }
	const std::string&        desc() { return t_desc; }
	const std::string&        show_prize()                  { return t_show_prize; }

private:
	uint32_t                  t_id;                         ///< ID

	uint32_t                  t_level;                      ///< �����ȼ�
	uint32_t                  t_prize_id;                   ///< ����ID
	std::string               t_desc;                       ///< ��������
	std::string               t_show_prize;                 ///< չʾ����

public:
	void reset();
	void fill( const LevelFundTableBase& base );
	const char* getClassName() const { return "y��Ӫ���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LevelFundTable, LevelFundTableBase > LevelFundTableManager;

inline void LevelFundTable::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_prize_id = 0;
	this->t_desc.clear();
	this->t_show_prize.clear();
};

inline void LevelFundTable::fill( const LevelFundTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_prize_id = base.prize_id;
	if( base.desc == NULL ) this->t_desc.clear(); else this->t_desc.assign( base.desc );
	if( base.show_prize == NULL ) this->t_show_prize.clear(); else this->t_show_prize.assign( base.show_prize );
};

inline const std::string LevelFundTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LevelFundTable.tbls64";
#else
	return "LevelFundTable.tbls32";
#endif
};
