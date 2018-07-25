#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:LevelLimitTableBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:16 X64
///           �ֶ�����:4
///           �����ļ�:t���´���.xlsx ����
///           ��������:�ȼ�
#pragma pack(1)
struct LevelLimitTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �ȼ� ����Ψһ��

	uint32_t                  id;                           ///< �ȼ�

	uint32_t                  finish_task;                  ///< �������
	uint32_t                  max_exp;                      ///< ��������
	uint32_t                  exchange_rate;                ///< ת������
};
#pragma pack()

struct LevelLimitTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  finish_task()                 { return t_finish_task; }
	uint32_t                  max_exp()                     { return t_max_exp; }
	uint32_t                  exchange_rate()               { return t_exchange_rate; }

private:
	uint32_t                  t_id;                         ///< �ȼ�

	uint32_t                  t_finish_task;                ///< �������
	uint32_t                  t_max_exp;                    ///< ��������
	uint32_t                  t_exchange_rate;              ///< ת������

public:
	void reset();
	void fill( const LevelLimitTableBase& base );
	const char* getClassName() const { return "t���´���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LevelLimitTable, LevelLimitTableBase > LevelLimitTableManager;

inline void LevelLimitTable::reset()
{
	this->t_id = 0;
	this->t_finish_task = 0;
	this->t_max_exp = 0;
	this->t_exchange_rate = 0;
};

inline void LevelLimitTable::fill( const LevelLimitTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_finish_task = base.finish_task;
	this->t_max_exp = base.max_exp;
	this->t_exchange_rate = base.exchange_rate;
};

inline const std::string LevelLimitTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LevelLimitTable.tbls64";
#else
	return "LevelLimitTable.tbls32";
#endif
};
