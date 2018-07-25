#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:OfflineArenaMatchBase
/// @remark   ��¼��С:12 X32
///           ��¼��С:20 X64
///           �ֶ�����:3
///           �����ļ�:l���߾�����.xlsx ����ѡȡ
///           ��������:id
#pragma pack(1)
struct OfflineArenaMatchBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000511B6; ///< I|VV
#else
	static const int          FormatCheckCode = 0x000513D6; ///< I|vv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	const char*               rank;                         ///< �ҵ�����
	const char*               enemy;                        ///< ��������
};
#pragma pack()

struct OfflineArenaMatch : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<uint32_t>&        rank()                        { return t_rank; }
	const std::vector<std::vector<uint32_t>>&        enemy()                       { return t_enemy; }

private:
	uint32_t                  t_id;                         ///< id

	std::vector<uint32_t>               t_rank;                       ///< �ҵ�����
	std::vector<std::vector<uint32_t>>               t_enemy;                      ///< ��������

public:
	void reset();
	void fill( const OfflineArenaMatchBase& base );
	const char* getClassName() const { return "l���߾�����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< OfflineArenaMatch, OfflineArenaMatchBase > OfflineArenaMatchManager;

inline void OfflineArenaMatch::reset()
{
	this->t_id = 0;
	this->t_rank.clear();
	this->t_enemy.clear();
};

inline void OfflineArenaMatch::fill( const OfflineArenaMatchBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.rank == NULL ) this->t_rank.clear(); else this->t_rank.assign( base.rank );
	Dawn::parseContainer<uint32_t>(this->t_rank, base.rank, "~");
	//if( base.enemy == NULL ) this->t_enemy.clear(); else this->t_enemy.assign( base.enemy );
	Dawn::parseDoubleContainer<uint32_t, std::vector<uint32_t>, std::vector<std::vector<uint32_t>>>(this->t_enemy, base.enemy, "~$");
};

inline const std::string OfflineArenaMatch::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "OfflineArenaMatch.tbls64";
#else
	return "OfflineArenaMatch.tbls32";
#endif
};
