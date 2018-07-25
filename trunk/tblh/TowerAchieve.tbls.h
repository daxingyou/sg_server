#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:TowerAchieveBase
/// @remark   ��¼��С:12 X32
///           ��¼��С:12 X64
///           �ֶ�����:3
///           �����ļ�:sʱ�ջþ�.xlsx ��ͨ������
///           ��������:id
#pragma pack(1)
struct TowerAchieveBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	uint32_t                  floor;                        ///< ����
	uint32_t                  award;                        ///< ����
};
#pragma pack()

struct TowerAchieve : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  floor()                       { return t_floor; }
	uint32_t                  award()                       { return t_award; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_floor;                      ///< ����
	uint32_t                  t_award;                      ///< ����

public:
	void reset();
	void fill( const TowerAchieveBase& base );
	const char* getClassName() const { return "sʱ�ջþ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TowerAchieve, TowerAchieveBase > TowerAchieveManager;

inline void TowerAchieve::reset()
{
	this->t_id = 0;
	this->t_floor = 0;
	this->t_award = 0;
};

inline void TowerAchieve::fill( const TowerAchieveBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_floor = base.floor;
	this->t_award = base.award;
};

inline const std::string TowerAchieve::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TowerAchieve.tbls64";
#else
	return "TowerAchieve.tbls32";
#endif
};
