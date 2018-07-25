#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:BountyBase
/// @remark   ��¼��С:44 X32
///           ��¼��С:56 X64
///           �ֶ�����:11
///           �����ļ�:g�������ͱ�.xlsx Sheet1
///           ��������:����id
#pragma pack(1)
struct BountyBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0AB73769; ///< I|VIIIVVIIVII
#else
	static const int          FormatCheckCode = 0x0C98D489; ///< I|vIIIvvIIvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ����id ����Ψһ��

	uint32_t                  id;                           ///< ����id

	const char*               level_range;                  ///< �ȼ�����
	uint32_t                  star;                         ///< �Ѷ��Ǽ�
	uint32_t                  prob;                         ///< ���͸���
	uint32_t                  target_id;                    ///< ����Ŀ��id
	const char*               destiny_id;                   ///< ��������id
	const char*               restraint_id;                 ///< ���ƿ���id
	uint32_t                  restraint_target;             ///< ����Ŀ��id
	uint32_t                  circle_id;                    ///< ������id
	const char*               award;                        ///< ��ͨ�����
	uint32_t                  prestige_award;               ///< ��������
	uint32_t                  country_award;                ///< ������Դ����
};
#pragma pack()

struct Bounty : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<uint32_t>&        level_range() { return t_level_range; }
	uint32_t                  star()                        { return t_star; }
	uint32_t                  prob()                        { return t_prob; }
	uint32_t                  target_id()                   { return t_target_id; }
	const std::vector<uint32_t>&        destiny_id()                  { return t_destiny_id; }
	const std::vector<uint32_t>&        restraint_id()                { return t_restraint_id; }
	uint32_t                  restraint_target()            { return t_restraint_target; }
	uint32_t                  circle_id()                   { return t_circle_id; }
	const std::vector<uint32_t>&        award()                       { return t_award; }
	uint32_t                  prestige_award()              { return t_prestige_award; }
	uint32_t                  country_award()               { return t_country_award; }

private:
	uint32_t                  t_id;                         ///< ����id

	std::vector<uint32_t>				t_level_range;                ///< �ȼ�����
	uint32_t                  t_star;                       ///< �Ѷ��Ǽ�
	uint32_t                  t_prob;                       ///< ���͸���
	uint32_t                  t_target_id;                  ///< ����Ŀ��id
	std::vector<uint32_t>               t_destiny_id;                 ///< ��������id
	std::vector<uint32_t>               t_restraint_id;               ///< ���ƿ���id
	uint32_t                  t_restraint_target;           ///< ����Ŀ��id
	uint32_t                  t_circle_id;                  ///< ������id
	std::vector<uint32_t>               t_award;                      ///< ��ͨ�����
	uint32_t                  t_prestige_award;             ///< ��������
	uint32_t                  t_country_award;              ///< ������Դ����

public:
	void reset();
	void fill( const BountyBase& base );
	const char* getClassName() const { return "g�������ͱ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Bounty, BountyBase > BountyManager;

inline void Bounty::reset()
{
	this->t_id = 0;
	this->t_level_range.clear();
	this->t_star = 0;
	this->t_prob = 0;
	this->t_target_id = 0;
	this->t_destiny_id.clear();
	this->t_restraint_id.clear();
	this->t_restraint_target = 0;
	this->t_circle_id = 0;
	this->t_award.clear();
	this->t_prestige_award = 0;
	this->t_country_award = 0;
};

inline void Bounty::fill( const BountyBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	Dawn::parseContainer<uint32_t>(this->t_level_range, base.level_range, "$");
	this->t_star = base.star;
	this->t_prob = base.prob;
	this->t_target_id = base.target_id;
	//if( base.destiny_id == NULL ) this->t_destiny_id.clear(); else this->t_destiny_id.assign( base.destiny_id );
	Dawn::parseContainer<uint32_t>(this->t_destiny_id, base.destiny_id, "$");
	//if( base.restraint_id == NULL ) this->t_restraint_id.clear(); else this->t_restraint_id.assign( base.restraint_id );
	Dawn::parseContainer<uint32_t>(this->t_restraint_id, base.restraint_id, "$");
	this->t_restraint_target = base.restraint_target;
	this->t_circle_id = base.circle_id;
	//if( base.award == NULL ) this->t_award.clear(); else this->t_award.assign( base.award );
	Dawn::parseContainer<uint32_t>(this->t_award, base.award, "$");
	this->t_prestige_award = base.prestige_award;
	this->t_country_award = base.country_award;
};

inline const std::string Bounty::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Bounty.tbls64";
#else
	return "Bounty.tbls32";
#endif
};
