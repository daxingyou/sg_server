#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:EquipStarBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:16 X64
///           �ֶ�����:4
///           �����ļ�:zװ����.xlsx �Ǽ�Ԫ��
///           ��������:װ���Ǽ�
#pragma pack(1)
struct EquipStarBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� װ���Ǽ� ����Ψһ��

	uint32_t                  id;                           ///< װ���Ǽ�

	uint32_t                  level_limit;                  ///< �ȼ�����
	uint32_t                  strengthen_limit;             ///< ��������ǿ����ֱ�
	uint32_t                  remake_limit;                 ///< ��������ϴ����ֱ�
};
#pragma pack()

struct EquipStar : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level_limit()                 { return t_level_limit; }
	uint32_t                  strengthen_limit()            { return t_strengthen_limit; }
	uint32_t                  remake_limit()                { return t_remake_limit; }

private:
	uint32_t                  t_id;                         ///< װ���Ǽ�

	uint32_t                  t_level_limit;                ///< �ȼ�����
	uint32_t                  t_strengthen_limit;           ///< ��������ǿ����ֱ�
	uint32_t                  t_remake_limit;               ///< ��������ϴ����ֱ�

public:
	void reset();
	void fill( const EquipStarBase& base );
	const char* getClassName() const { return "zװ����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipStar, EquipStarBase > EquipStarManager;

inline void EquipStar::reset()
{
	this->t_id = 0;
	this->t_level_limit = 0;
	this->t_strengthen_limit = 0;
	this->t_remake_limit = 0;
};

inline void EquipStar::fill( const EquipStarBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level_limit = base.level_limit;
	this->t_strengthen_limit = base.strengthen_limit;
	this->t_remake_limit = base.remake_limit;
};

inline const std::string EquipStar::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipStar.tbls64";
#else
	return "EquipStar.tbls32";
#endif
};
