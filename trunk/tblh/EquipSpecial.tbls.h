#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:EquipSpecialBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:24 X64
///           �ֶ�����:5
///           �����ļ�:zװ����.xlsx �ؼ�
///           ��������:װ���ؼ�id
#pragma pack(1)
struct EquipSpecialBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510EAD9; ///< I|IVII
#else
	static const int          FormatCheckCode = 0x05110AD9; ///< I|IvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� װ���ؼ�id ����Ψһ��

	uint32_t                  id;                           ///< װ���ؼ�id

	uint32_t                  pool_id;                      ///< װ���ؼ���id
	const char*               attr;                         ///< �����ؼ�����
	uint32_t                  prob;                         ///< װ���ؼ�����
	uint32_t                  score;                        ///< �ؼ�����
};
#pragma pack()

struct EquipSpecial : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  pool_id()                     { return t_pool_id; }
	const std::vector<uint32_t>&        attr()                        { return t_attr; }
	uint32_t                  prob()                        { return t_prob; }
	uint32_t                  score() { return t_score; }

private:
	uint32_t                  t_id;                         ///< װ���ؼ�id

	uint32_t                  t_pool_id;                    ///< װ���ؼ���id
	std::vector<uint32_t>               t_attr;                       ///< �����ؼ�����
	uint32_t                  t_prob;                       ///< װ���ؼ�����
	uint32_t                  t_score;                      ///< �ؼ�����

public:
	void reset();
	void fill( const EquipSpecialBase& base );
	const char* getClassName() const { return "zװ����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipSpecial, EquipSpecialBase > EquipSpecialManager;

inline void EquipSpecial::reset()
{
	this->t_id = 0;
	this->t_pool_id = 0;
	this->t_attr.clear();
	this->t_prob = 0;
	this->t_score = 0;
};

inline void EquipSpecial::fill( const EquipSpecialBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_pool_id = base.pool_id;
	//if( base.attr == NULL ) this->t_attr.clear(); else this->t_attr.assign( base.attr );
	Dawn::parseContainer<uint32_t>(this->t_attr, base.attr, "$");
	this->t_prob = base.prob;
	this->t_score = base.score;
};

inline const std::string EquipSpecial::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipSpecial.tbls64";
#else
	return "EquipSpecial.tbls32";
#endif
};
