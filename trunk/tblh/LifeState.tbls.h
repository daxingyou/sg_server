#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:LifeStateBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:16 X64
///           �ֶ�����:4
///           �����ļ�:m���������.xlsx �����
///           ��������:����ID
#pragma pack(1)
struct LifeStateBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ����ID ����Ψһ��

	uint32_t                  id;                           ///< ����ID

	uint32_t                  piece_count;                  ///< ��Ƭ����
	uint32_t                  task_id;                      ///< ͻ������
	uint32_t                  finish_task_id;               ///< �������
};
#pragma pack()

struct LifeState : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  piece_count()                 { return t_piece_count; }
	uint32_t                  task_id()                     { return t_task_id; }
	uint32_t                  finish_task_id()              { return t_finish_task_id; }

private:
	uint32_t                  t_id;                         ///< ����ID

	uint32_t                  t_piece_count;                ///< ��Ƭ����
	uint32_t                  t_task_id;                    ///< ͻ������
	uint32_t                  t_finish_task_id;             ///< �������

public:
	void reset();
	void fill( const LifeStateBase& base );
	const char* getClassName() const { return "m���������"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LifeState, LifeStateBase > LifeStateManager;

inline void LifeState::reset()
{
	this->t_id = 0;
	this->t_piece_count = 0;
	this->t_task_id = 0;
	this->t_finish_task_id = 0;
};

inline void LifeState::fill( const LifeStateBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_piece_count = base.piece_count;
	this->t_task_id = base.task_id;
	this->t_finish_task_id = base.finish_task_id;
};

inline const std::string LifeState::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LifeState.tbls64";
#else
	return "LifeState.tbls32";
#endif
};
