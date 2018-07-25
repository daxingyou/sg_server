#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:NpcBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:16 X64
///           �ֶ�����:4
///           �����ļ�:nnpc.xlsx Sheet1
///           ��������:NPCID
#pragma pack(1)
struct NpcBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� NPCID ����Ψһ��

	uint32_t                  id;                           ///< NPCID

	uint32_t                  type;                         ///< NPC����
	uint32_t                  fight_id;                     ///< ս�����ID
	uint32_t                  ai_id;                        ///< NPCAI
};
#pragma pack()

struct Npc : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	uint32_t                  fight_id()                    { return t_fight_id; }
	uint32_t                  ai_id()                       { return t_ai_id; }

private:
	uint32_t                  t_id;                         ///< NPCID

	uint32_t                  t_type;                       ///< NPC����
	uint32_t                  t_fight_id;                   ///< ս�����ID
	uint32_t                  t_ai_id;                      ///< NPCAI

public:
	void reset();
	void fill( const NpcBase& base );
	const char* getClassName() const { return "nnpc"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Npc, NpcBase > NpcManager;

inline void Npc::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_fight_id = 0;
	this->t_ai_id = 0;
};

inline void Npc::fill( const NpcBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_fight_id = base.fight_id;
	this->t_ai_id = base.ai_id;
};

inline const std::string Npc::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Npc.tbls64";
#else
	return "Npc.tbls32";
#endif
};
