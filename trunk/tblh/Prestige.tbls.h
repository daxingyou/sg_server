#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:PrestigeBase
/// @remark   ��¼��С:12 X32
///           ��¼��С:12 X64
///           �ֶ�����:3
///           �����ļ�:prestige.xlsx prestige
///           ��������:�����ȼ�
#pragma pack(1)
struct PrestigeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �����ȼ� ����Ψһ��

	uint32_t                  id;                           ///< �����ȼ�

	uint32_t                  exp;                          ///< ��������
	uint32_t                  prestige_limit;               ///< ����ÿ������
};
#pragma pack()

struct Prestige : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  exp()                         { return t_exp; }
	uint32_t                  prestige_limit()              { return t_prestige_limit; }

private:
	uint32_t                  t_id;                         ///< �����ȼ�

	uint32_t                  t_exp;                        ///< ��������
	uint32_t                  t_prestige_limit;             ///< ����ÿ������

public:
	void reset();
	void fill( const PrestigeBase& base );
	const char* getClassName() const { return "prestige"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Prestige, PrestigeBase > PrestigeManager;

inline void Prestige::reset()
{
	this->t_id = 0;
	this->t_exp = 0;
	this->t_prestige_limit = 0;
};

inline void Prestige::fill( const PrestigeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_exp = base.exp;
	this->t_prestige_limit = base.prestige_limit;
};

inline const std::string Prestige::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Prestige.tbls64";
#else
	return "Prestige.tbls32";
#endif
};
