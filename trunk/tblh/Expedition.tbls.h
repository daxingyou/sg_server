#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:ExpeditionBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:16 X64
///           �ֶ�����:4
///           �����ļ�:yԶ����.xlsx Զ����
///           ��������:�ؿ�ID
#pragma pack(1)
struct ExpeditionBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �ؿ�ID ����Ψһ��

	uint32_t                  id;                           ///< �ؿ�ID

	uint32_t                  fight_level;                  ///< �Ѷ�
	uint32_t                  drop_id;                      ///< ͨ�ؽ���
	uint32_t                  honor;                        ///< ��ҫֵ
};
#pragma pack()

struct Expedition : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  fight_level()                 { return t_fight_level; }
	uint32_t                  drop_id()                     { return t_drop_id; }
	uint32_t                  honor()                       { return t_honor; }

private:
	uint32_t                  t_id;                         ///< �ؿ�ID

	uint32_t                  t_fight_level;                ///< �Ѷ�
	uint32_t                  t_drop_id;                    ///< ͨ�ؽ���
	uint32_t                  t_honor;                      ///< ��ҫֵ

public:
	void reset();
	void fill( const ExpeditionBase& base );
	const char* getClassName() const { return "yԶ����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Expedition, ExpeditionBase > ExpeditionManager;

inline void Expedition::reset()
{
	this->t_id = 0;
	this->t_fight_level = 0;
	this->t_drop_id = 0;
	this->t_honor = 0;
};

inline void Expedition::fill( const ExpeditionBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_fight_level = base.fight_level;
	this->t_drop_id = base.drop_id;
	this->t_honor = base.honor;
};

inline const std::string Expedition::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Expedition.tbls64";
#else
	return "Expedition.tbls32";
#endif
};
