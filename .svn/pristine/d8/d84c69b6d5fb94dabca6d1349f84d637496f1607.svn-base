#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:NpcBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:nnpc.xlsx Sheet1
///           排序主键:NPCID
#pragma pack(1)
struct NpcBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 NPCID 生成唯一键

	uint32_t                  id;                           ///< NPCID

	uint32_t                  type;                         ///< NPC类型
	uint32_t                  fight_id;                     ///< 战斗入口ID
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

	uint32_t                  t_type;                       ///< NPC类型
	uint32_t                  t_fight_id;                   ///< 战斗入口ID
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
