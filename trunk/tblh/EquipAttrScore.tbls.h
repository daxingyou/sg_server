#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:EquipAttrScoreBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:8 X64
///           �ֶ�����:2
///           �����ļ�:zս��ת����.xlsx װ������ֵתս��
///           ��������:����ID
#pragma pack(1)
struct EquipAttrScoreBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ����ID ����Ψһ��

	uint32_t                  id;                           ///< ����ID

	uint32_t                  score;                        ///< ����ս��/����
};
#pragma pack()

struct EquipAttrScore : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  score()                       { return t_score; }

private:
	uint32_t                  t_id;                         ///< ����ID

	uint32_t                  t_score;                      ///< ����ս��/����

public:
	void reset();
	void fill( const EquipAttrScoreBase& base );
	const char* getClassName() const { return "zս��ת����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipAttrScore, EquipAttrScoreBase > EquipAttrScoreManager;

inline void EquipAttrScore::reset()
{
	this->t_id = 0;
	this->t_score = 0;
};

inline void EquipAttrScore::fill( const EquipAttrScoreBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_score = base.score;
};

inline const std::string EquipAttrScore::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipAttrScore.tbls64";
#else
	return "EquipAttrScore.tbls32";
#endif
};
