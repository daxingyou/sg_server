#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:OfflineArenaRobotBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:28 X64
///           �ֶ�����:5
///           �����ļ�:l���߾�����.xlsx �����˷ֲ�
///           ��������:����
#pragma pack(1)
struct OfflineArenaRobotBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510EBA9; ///< I|IVVI
#else
	static const int          FormatCheckCode = 0x05110DA9; ///< I|IvvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ���� ����Ψһ��

	uint32_t                  id;                           ///< ����

	uint32_t                  sublevel;                        ///< �ֶ�
	const char*               rank;                         ///< ����
	const char*               level_range;                  ///< �����˵ȼ�
	uint32_t                  against;                      ///< ������������
};
#pragma pack()

struct OfflineArenaRobot : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  sublevel()                       { return t_sublevel; }
	const std::vector<uint32_t>&        rank()                        { return t_rank; }
	const std::vector<uint32_t>&        level_range()                 { return t_level_range; }
	uint32_t                  against()                     { return t_against; }

private:
	uint32_t                  t_id;                         ///< ����

	uint32_t                  t_sublevel;                      ///< �ֶ�
	std::vector<uint32_t>               t_rank;                       ///< ����
	std::vector<uint32_t>               t_level_range;                ///< �����˵ȼ�
	uint32_t                  t_against;                    ///< ������������

public:
	void reset();
	void fill( const OfflineArenaRobotBase& base );
	const char* getClassName() const { return "l���߾�����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< OfflineArenaRobot, OfflineArenaRobotBase > OfflineArenaRobotManager;

inline void OfflineArenaRobot::reset()
{
	this->t_id = 0;
	this->t_sublevel = 0;
	this->t_rank.clear();
	this->t_level_range.clear();
	this->t_against = 0;
};

inline void OfflineArenaRobot::fill( const OfflineArenaRobotBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_sublevel = base.sublevel;
	//if( base.rank == NULL ) this->t_rank.clear(); else this->t_rank.assign( base.rank );
	Dawn::parseContainer<uint32_t>(this->t_rank, base.rank, "~");
	//if( base.level_range == NULL ) this->t_level_range.clear(); else this->t_level_range.assign( base.level_range );
	Dawn::parseContainer<uint32_t>(this->t_level_range, base.level_range, "~");
	this->t_against = base.against;
};

inline const std::string OfflineArenaRobot::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "OfflineArenaRobot.tbls64";
#else
	return "OfflineArenaRobot.tbls32";
#endif
};
