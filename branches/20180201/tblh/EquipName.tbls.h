#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:EquipNameBase
/// @remark   ��¼��С:24 X32
///           ��¼��С:28 X64
///           �ֶ�����:6
///           �����ļ�:zװ����.xlsx ��׺��
///           ��������:��׺id
#pragma pack(1)
struct EquipNameBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010EAD89; ///< I|IVIII
#else
	static const int          FormatCheckCode = 0x0110AD89; ///< I|IvIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ��׺id ����Ψһ��

	uint32_t                  id;                           ///< ��׺id

	uint32_t                  pool_id;                      ///< ��׺��id
	const char*               attr;                         ///< ��׺����
	uint32_t                  level;                        ///< �ɳ��ȼ����
	uint32_t                  prob;                         ///< ��׺����
	uint32_t                  score;                        ///< ��׺����
};
#pragma pack()

struct EquipName : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  pool_id()                     { return t_pool_id; }
	const std::vector<Dawn::triple<uint32_t,uint32_t,uint32_t>>&        attr()                        { return t_attr; }
	uint32_t                  level()                       { return t_level; }
	uint32_t                  prob()                        { return t_prob; }
	uint32_t                  score() { return t_score; }

private:
	uint32_t                  t_id;                         ///< ��׺id

	uint32_t                  t_pool_id;                    ///< ��׺��id
	std::vector<Dawn::triple<uint32_t, uint32_t, uint32_t>>               t_attr;                       ///< ��׺����
	uint32_t                  t_level;                      ///< �ɳ��ȼ����
	uint32_t                  t_prob;                       ///< ��׺����
	uint32_t                  t_score;                      ///< ��׺����

public:
	void reset();
	void fill( const EquipNameBase& base );
	const char* getClassName() const { return "zװ����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipName, EquipNameBase > EquipNameManager;

inline void EquipName::reset()
{
	this->t_id = 0;
	this->t_pool_id = 0;
	this->t_attr.clear();
	this->t_level = 0;
	this->t_prob = 0;
	this->t_score = 0;
};

inline void EquipName::fill( const EquipNameBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_pool_id = base.pool_id;
	//if( base.attr == NULL ) this->t_attr.clear(); else this->t_attr.assign( base.attr );
	Dawn::parseTupleContainer<Dawn::triple<uint32_t, uint32_t, uint32_t>>(this->t_attr, base.attr, ":$");
	this->t_level = base.level;
	this->t_prob = base.prob;
	this->t_score = base.score;
};

inline const std::string EquipName::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipName.tbls64";
#else
	return "EquipName.tbls32";
#endif
};
