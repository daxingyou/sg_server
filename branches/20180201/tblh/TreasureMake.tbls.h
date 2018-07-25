#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:TreasureMakeBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:32 X64
///           �ֶ�����:5
///           �����ļ�:c�ر�ͼ��.xlsx treasure_make
///           ��������:id
#pragma pack(1)
struct TreasureMakeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510EBB6; ///< I|IVVV
#else
	static const int          FormatCheckCode = 0x05110DD6; ///< I|Ivvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	uint32_t                  map;                          ///< ����id
	const char*               pos;                          ///< ����
	const char*               event_id;                     ///< �¼�id��
	const char*               event_prob;                   ///< ��Ӧ�ĸ���
};
#pragma pack()

struct TreasureMake : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  map()                         { return t_map; }
	const std::pair<uint32_t, uint32_t>&        pos()                         { return t_pos; }
	const std::vector<uint32_t>&        event_id()                    { return t_event_id; }
	const std::vector<uint32_t>&        event_prob()                  { return t_event_prob; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_map;                        ///< ����id
	std::pair<uint32_t, uint32_t>               t_pos;                        ///< ����
	std::vector<uint32_t>               t_event_id;                   ///< �¼�id��
	std::vector<uint32_t>               t_event_prob;                 ///< ��Ӧ�ĸ���

public:
	void reset();
	void fill( const TreasureMakeBase& base );
	const char* getClassName() const { return "c�ر�ͼ��"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TreasureMake, TreasureMakeBase > TreasureMakeManager;

inline void TreasureMake::reset()
{
	this->t_id = 0;
	this->t_map = 0;
	//this->t_pos.clear();
	this->t_event_id.clear();
	this->t_event_prob.clear();
};

inline void TreasureMake::fill( const TreasureMakeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_map = base.map;
	//if( base.pos == NULL ) this->t_pos.clear(); else this->t_pos.assign( base.pos );
	Dawn::parseTuple(this->t_pos, base.pos, "$");
	//if( base.event_id == NULL ) this->t_event_id.clear(); else this->t_event_id.assign( base.event_id );
	Dawn::parseContainer<uint32_t>(this->t_event_id, base.event_id, "$");
	//if( base.event_prob == NULL ) this->t_event_prob.clear(); else this->t_event_prob.assign( base.event_prob );
	Dawn::parseContainer<uint32_t>(this->t_event_prob, base.event_prob, "$");
};

inline const std::string TreasureMake::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TreasureMake.tbls64";
#else
	return "TreasureMake.tbls32";
#endif
};
