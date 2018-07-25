#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:LevelDemadBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:16 X64
///           �ֶ�����:4
///           �����ļ�:w�佫�������ı�.xlsx Sheet2
///           ��������:���߱��,�佫�ȼ�
#pragma pack(1)
struct LevelDemadBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x004E10D9; ///< II|II
#else
	static const int          FormatCheckCode = 0x004E10D9; ///< II|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, level); }; // TODO: ���� ���߱��,�佫�ȼ� ����Ψһ��

	uint32_t                  id;                           ///< ���߱��
	uint32_t                  level;                        ///< �佫�ȼ�

	uint32_t                  exp;                          ///< ��Ҫ����
	uint32_t                  exp_limit;                    ///< ��������
};
#pragma pack()

struct LevelDemad : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  level()                       { return t_level; }

	uint32_t                  exp()                         { return t_exp; }
	uint32_t                  exp_limit()                   { return t_exp_limit; }

private:
	uint32_t                  t_id;                         ///< ���߱��
	uint32_t                  t_level;                      ///< �佫�ȼ�

	uint32_t                  t_exp;                        ///< ��Ҫ����
	uint32_t                  t_exp_limit;                  ///< ��������

public:
	void reset();
	void fill( const LevelDemadBase& base );
	const char* getClassName() const { return "w�佫�������ı�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LevelDemad, LevelDemadBase > LevelDemadManager;

inline void LevelDemad::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_exp = 0;
	this->t_exp_limit = 0;
};

inline void LevelDemad::fill( const LevelDemadBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_exp = base.exp;
	this->t_exp_limit = base.exp_limit;
};

inline const std::string LevelDemad::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LevelDemad.tbls64";
#else
	return "LevelDemad.tbls32";
#endif
};
