#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:OfflineArenaAwardBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:24 X64
///           �ֶ�����:5
///           �����ļ�:l���߾�����.xlsx ����
///           ��������:����
#pragma pack(1)
struct OfflineArenaAwardBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0511AEB6; ///< I|VIVV
#else
	static const int          FormatCheckCode = 0x0513B0D6; ///< I|vIvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ���� ����Ψһ��

	uint32_t                  id;                           ///< ����

	const char*               rank;                         ///< ������
	uint32_t                  sublevel;                     ///< �ֶ�
	const char*               day_award;                    ///< ÿ�ս���
	const char*               up_award;                     ///< ÿǰ��1������
};
#pragma pack()

struct OfflineArenaAward : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<uint32_t>&        rank()                        { return t_rank; }
	uint32_t                  sublevel()                    { return t_sublevel; }
	const std::map<uint32_t,uint32_t>                  day_award()                   { return t_day_award; }
	const std::map<uint32_t, uint32_t>                  up_award()                    { return t_up_award; }

private:
	uint32_t                  t_id;                         ///< ����

	std::vector<uint32_t>               t_rank;                       ///< ������
	uint32_t                  t_sublevel;                   ///< �ֶ�
	std::map<uint32_t, uint32_t>                  t_day_award;                  ///< ÿ�ս���
	std::map<uint32_t, uint32_t>                  t_up_award;                   ///< ÿǰ��1������

public:
	void reset();
	void fill( const OfflineArenaAwardBase& base );
	const char* getClassName() const { return "l���߾�����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< OfflineArenaAward, OfflineArenaAwardBase > OfflineArenaAwardManager;

inline void OfflineArenaAward::reset()
{
	this->t_id = 0;
	this->t_rank.clear();
	this->t_sublevel = 0;
	this->t_day_award.clear();
	this->t_up_award.clear();
};

inline void OfflineArenaAward::fill( const OfflineArenaAwardBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.rank == NULL ) this->t_rank.clear(); else this->t_rank.assign( base.rank );
	Dawn::parseContainer<uint32_t>(this->t_rank, base.rank, "~");
	this->t_sublevel = base.sublevel;
	Dawn::parseMapContainer<uint32_t,uint32_t>(this->t_day_award, base.day_award, ":$");
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_up_award, base.up_award, ":$");
};

inline const std::string OfflineArenaAward::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "OfflineArenaAward.tbls64";
#else
	return "OfflineArenaAward.tbls32";
#endif
};
