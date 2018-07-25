#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:HolidayBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:8 X64
///           �ֶ�����:2
///           �����ļ�:j���ձ�.xlsx ���ձ�
///           ��������:ʱ��id
#pragma pack(1)
struct HolidayBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ʱ��id ����Ψһ��

	uint32_t                  id;                           ///< ʱ��id

	uint32_t                  drop_id;                      ///< �����
};
#pragma pack()

struct Holiday : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_id()                     { return t_drop_id; }

private:
	uint32_t                  t_id;                         ///< ʱ��id

	uint32_t                  t_drop_id;                    ///< �����

public:
	void reset();
	void fill( const HolidayBase& base );
	const char* getClassName() const { return "j���ձ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Holiday, HolidayBase > HolidayManager;

inline void Holiday::reset()
{
	this->t_id = 0;
	this->t_drop_id = 0;
};

inline void Holiday::fill( const HolidayBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_id = base.drop_id;
};

inline const std::string Holiday::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Holiday.tbls64";
#else
	return "Holiday.tbls32";
#endif
};
