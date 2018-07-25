#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:GuideFightTableBase
/// @remark   ��¼��С:32 X32
///           ��¼��С:32 X64
///           �ֶ�����:8
///           �����ļ�:x��������.xlsx ս������
///           ��������:����ID
#pragma pack(1)
struct GuideFightTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0DDD8CD9; ///< I|IIIIIII
#else
	static const int          FormatCheckCode = 0x0DDD8CD9; ///< I|IIIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ����ID ����Ψһ��

	uint32_t                  id;                           ///< ID

	uint32_t                  against_id;                   ///< ����ID
	uint32_t                  guide_id;                     ///< ����ID
	uint32_t                  step;                         ///< ����ʱ��
	uint32_t                  round;                        ///< ����
	uint32_t                  type;                         ///< ����
	uint32_t                  state;                        ///< ս���׶η���
	uint32_t                  play_time;                    ///< ����ʱ��
};
#pragma pack()

struct GuideFightTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  against_id()                  { return t_against_id; }
	uint32_t                  guide_id()                    { return t_guide_id; }
	uint32_t                  step()                        { return t_step; }
	uint32_t                  round()                       { return t_round; }
	uint32_t                  type()                        { return t_type; }
	uint32_t                  state()                       { return t_state; }
	uint32_t                  play_time()                   { return t_play_time; }

private:
	uint32_t                  t_id;                         ///< ID

	uint32_t                  t_against_id;                 ///< ����ID
	uint32_t                  t_guide_id;                   ///< ����ID
	uint32_t                  t_step;                       ///< ����ʱ��
	uint32_t                  t_round;                      ///< ����
	uint32_t                  t_type;                       ///< ����
	uint32_t                  t_state;                      ///< ս���׶η���
	uint32_t                  t_play_time;                  ///< ����ʱ��

public:
	void reset();
	void fill( const GuideFightTableBase& base );
	const char* getClassName() const { return "x��������"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< GuideFightTable, GuideFightTableBase > GuideFightTableManager;

inline void GuideFightTable::reset()
{
	this->t_id = 0;
	this->t_against_id = 0;
	this->t_guide_id = 0;
	this->t_step = 0;
	this->t_round = 0;
	this->t_type = 0;
	this->t_state = 0;
	this->t_play_time = 0;
};

inline void GuideFightTable::fill( const GuideFightTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_against_id = base.against_id;
	this->t_guide_id = base.guide_id;
	this->t_step = base.step;
	this->t_round = base.round;
	this->t_type = base.type;
	this->t_state = base.state;
	this->t_play_time = base.play_time;
};

inline const std::string GuideFightTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "GuideFightTable.tbls64";
#else
	return "GuideFightTable.tbls32";
#endif
};
