#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:ExpeditionBoxBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:8 X64
///           �ֶ�����:2
///           �����ļ�:yԶ����.xlsx ��Ծ����
///           ��������:�����Ծֵ
#pragma pack(1)
struct ExpeditionBoxBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return glory; }; // TODO: ���� �����Ծֵ ����Ψһ��

	uint32_t                  glory;                        ///< �����Ծֵ

	uint32_t                  reward;                       ///< ������Ʒ
};
#pragma pack()

struct ExpeditionBox : public Entry
{
	uint32_t                  glory()                       { return t_glory; }

	uint32_t                  reward()                      { return t_reward; }

private:
	uint32_t                  t_glory;                      ///< �����Ծֵ

	uint32_t                  t_reward;                     ///< ������Ʒ

public:
	void reset();
	void fill( const ExpeditionBoxBase& base );
	const char* getClassName() const { return "yԶ����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ExpeditionBox, ExpeditionBoxBase > ExpeditionBoxManager;

inline void ExpeditionBox::reset()
{
	this->t_glory = 0;
	this->t_reward = 0;
};

inline void ExpeditionBox::fill( const ExpeditionBoxBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_glory = base.glory;
	this->t_reward = base.reward;
};

inline const std::string ExpeditionBox::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ExpeditionBox.tbls64";
#else
	return "ExpeditionBox.tbls32";
#endif
};
