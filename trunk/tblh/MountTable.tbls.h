#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:MountTableBase
/// @remark   ��¼��С:28 X32
///           ��¼��С:28 X64
///           �ֶ�����:7
///           �����ļ�:z�����.xlsx mount
///           ��������:ID
#pragma pack(1)
struct MountTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00FCA8C9; ///< I|IffIII
#else
	static const int          FormatCheckCode = 0x00FCA8C9; ///< I|IffIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ID ����Ψһ��

	uint32_t                  id;                           ///< ����id

	uint32_t                  quality;                      ///< Ʒ��
	float                     mount_speed;                  ///< ����ʱ�ٶ�
	float                     trade_speed;                  ///< ����ʱ�ٶ�
	uint32_t                  space;                        ///< �ػ��ռ�
	uint32_t                  weight;                       ///< �����
	uint32_t                  people;                       ///< �̶ӹ�ģ
};
#pragma pack()

struct MountTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  quality()                     { return t_quality; }
	float                     mount_speed()                 { return t_mount_speed; }
	float                     trade_speed()                 { return t_trade_speed; }
	uint32_t                  space()                       { return t_space; }
	uint32_t                  weight()                      { return t_weight; }
	uint32_t                  people()                      { return t_people; }

private:
	uint32_t                  t_id;                         ///< ����id

	uint32_t                  t_quality;                    ///< Ʒ��
	float                     t_mount_speed;                ///< ����ʱ�ٶ�
	float                     t_trade_speed;                ///< ����ʱ�ٶ�
	uint32_t                  t_space;                      ///< �ػ��ռ�
	uint32_t                  t_weight;                     ///< �����
	uint32_t                  t_people;                     ///< �̶ӹ�ģ

public:
	void reset();
	void fill( const MountTableBase& base );
	const char* getClassName() const { return "z�����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< MountTable, MountTableBase > MountTableManager;

inline void MountTable::reset()
{
	this->t_id = 0;
	this->t_quality = 0;
	this->t_mount_speed = 0;
	this->t_trade_speed = 0;
	this->t_space = 0;
	this->t_weight = 0;
	this->t_people = 0;
};

inline void MountTable::fill( const MountTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_quality = base.quality;
	this->t_mount_speed = base.mount_speed;
	this->t_trade_speed = base.trade_speed;
	this->t_space = base.space;
	this->t_weight = base.weight;
	this->t_people = base.people;
};

inline const std::string MountTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "MountTable.tbls64";
#else
	return "MountTable.tbls32";
#endif
};
