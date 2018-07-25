#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:TalentScoreBase
/// @remark   ��¼��С:12 X32
///           ��¼��С:12 X64
///           �ֶ�����:3
///           �����ļ�:zս��ת����.xlsx �츳תս��
///           ��������:�츳id,�츳�ȼ�
#pragma pack(1)
struct TalentScoreBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0004E109; ///< II|I
#else
	static const int          FormatCheckCode = 0x0004E109; ///< II|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, level); }; // TODO: ���� �츳id,�츳�ȼ� ����Ψһ��

	uint32_t                  id;                           ///< �츳id
	uint32_t                  level;                        ///< �츳�ȼ�

	uint32_t                  score;                        ///< ����ս��
};
#pragma pack()

struct TalentScore : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  level()                       { return t_level; }

	uint32_t                  score()                       { return t_score; }

private:
	uint32_t                  t_id;                         ///< �츳id
	uint32_t                  t_level;                      ///< �츳�ȼ�

	uint32_t                  t_score;                      ///< ����ս��

public:
	void reset();
	void fill( const TalentScoreBase& base );
	const char* getClassName() const { return "zս��ת����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TalentScore, TalentScoreBase > TalentScoreManager;

inline void TalentScore::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_score = 0;
};

inline void TalentScore::fill( const TalentScoreBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_score = base.score;
};

inline const std::string TalentScore::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TalentScore.tbls64";
#else
	return "TalentScore.tbls32";
#endif
};
