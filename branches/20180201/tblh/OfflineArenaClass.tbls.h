#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:OfflineArenaClassBase
/// @remark   ��¼��С:28 X32
///           ��¼��С:32 X64
///           �ֶ�����:7
///           �����ļ�:l���߾�����.xlsx �ֶ�
///           ��������:�ֶ�
#pragma pack(1)
struct OfflineArenaClassBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00DDEFC9; ///< I|IIIVII
#else
	static const int          FormatCheckCode = 0x00DE0FC9; ///< I|IIIvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �ֶ� ����Ψһ��

	uint32_t                  id;                           ///< �ֶ�

	uint32_t                  level;                        ///< ����ȼ�
	uint32_t                  class_up_award;               ///< ���׽���
	uint32_t                  min_rank;                     ///< �������
	const char*               name;                         ///< �����ʼ�������
	uint32_t                  win_award;                    ///< ս��ʤ������
	uint32_t                  lose_award;                   ///< ս��ʧ�ܽ���
};
#pragma pack()

struct OfflineArenaClass : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level()                       { return t_level; }
	uint32_t                  class_up_award()              { return t_class_up_award; }
	uint32_t                  min_rank()                    { return t_min_rank; }
	const std::string&        name()                        { return t_name; }
	uint32_t                  win_award()                   { return t_win_award; }
	uint32_t                  lose_award()                  { return t_lose_award; }

private:
	uint32_t                  t_id;                         ///< �ֶ�

	uint32_t                  t_level;                      ///< ����ȼ�
	uint32_t                  t_class_up_award;             ///< ���׽���
	uint32_t                  t_min_rank;                   ///< �������
	std::string               t_name;                       ///< �����ʼ�������
	uint32_t                  t_win_award;                  ///< ս��ʤ������
	uint32_t                  t_lose_award;                 ///< ս��ʧ�ܽ���

public:
	void reset();
	void fill( const OfflineArenaClassBase& base );
	const char* getClassName() const { return "l���߾�����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< OfflineArenaClass, OfflineArenaClassBase > OfflineArenaClassManager;

inline void OfflineArenaClass::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_class_up_award = 0;
	this->t_min_rank = 0;
	this->t_name.clear();
	this->t_win_award = 0;
	this->t_lose_award = 0;
};

inline void OfflineArenaClass::fill( const OfflineArenaClassBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_class_up_award = base.class_up_award;
	this->t_min_rank = base.min_rank;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	this->t_win_award = base.win_award;
	this->t_lose_award = base.lose_award;
};

inline const std::string OfflineArenaClass::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "OfflineArenaClass.tbls64";
#else
	return "OfflineArenaClass.tbls32";
#endif
};
