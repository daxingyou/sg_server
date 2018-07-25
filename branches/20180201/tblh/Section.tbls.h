#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:SectionBase
/// @remark   ��¼��С:12 X32
///           ��¼��С:12 X64
///           �ֶ�����:3
///           �����ļ�:z�½ڱ�.xlsx �½������
///           ��������:id
#pragma pack(1)
struct SectionBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	uint32_t                  task_id;                      ///< ����ID
	uint32_t                  drop_id;                      ///< �ؿ�����
};
#pragma pack()

struct Section : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  task_id()                     { return t_task_id; }
	uint32_t                  drop_id()                     { return t_drop_id; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_task_id;                    ///< ����ID
	uint32_t                  t_drop_id;                    ///< �ؿ�����

public:
	void reset();
	void fill( const SectionBase& base );
	const char* getClassName() const { return "z�½ڱ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Section, SectionBase > SectionManager;

inline void Section::reset()
{
	this->t_id = 0;
	this->t_task_id = 0;
	this->t_drop_id = 0;
};

inline void Section::fill( const SectionBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_task_id = base.task_id;
	this->t_drop_id = base.drop_id;
};

inline const std::string Section::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Section.tbls64";
#else
	return "Section.tbls32";
#endif
};
