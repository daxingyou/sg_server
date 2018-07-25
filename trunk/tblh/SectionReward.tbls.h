#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/utility.hpp"
/// @brief    ���ṹ:SectionRewardBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:24 X64
///           �ֶ�����:4
///           �����ļ�:z�½ڱ�.xlsx �½ڽ�����
///           ��������:�½�id
#pragma pack(1)
struct SectionRewardBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00511AE6; ///< I|VIV
#else
	static const int          FormatCheckCode = 0x00513B06; ///< I|vIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �½�id ����Ψһ��

	uint32_t                  id;                           ///< �½�id

	const char*               page_list;                    ///< �½���������
	uint32_t                  drop_id;                      ///< �½ڽ���
	const char*               page_elite_list;              ///< ��Ӣ��ս����
};
#pragma pack()

struct SectionReward : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        page_list()                   { return t_page_list; }
	uint32_t                  drop_id()                     { return t_drop_id; }
	const std::string&        page_elite_list()             { return t_page_elite_list; }
	const std::vector<uint32_t>&	page_vec() { return t_page_vec; }
	const std::vector<uint32_t>&	page_elite_vec() { return t_page_elite_vec; }

private:
	uint32_t                  t_id;                         ///< �½�id

	std::string               t_page_list;                  ///< �½���������
	uint32_t                  t_drop_id;                    ///< �½ڽ���
	std::string               t_page_elite_list;            ///< ��Ӣ��ս����
	std::vector<uint32_t>	  t_page_vec;
	std::vector<uint32_t>	  t_page_elite_vec;

public:
	void reset();
	void fill( const SectionRewardBase& base );
	const char* getClassName() const { return "z�½ڱ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SectionReward, SectionRewardBase > SectionRewardManager;

inline void SectionReward::reset()
{
	this->t_id = 0;
	this->t_page_list.clear();
	this->t_drop_id = 0;
	this->t_page_elite_list.clear();
	this->t_page_vec.clear();
	this->t_page_elite_vec.clear();
};

inline void SectionReward::fill( const SectionRewardBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.page_list == NULL ) this->t_page_list.clear(); else this->t_page_list.assign( base.page_list );
	this->t_drop_id = base.drop_id;
	if (base.page_elite_list == NULL) this->t_page_elite_list.clear(); else this->t_page_elite_list.assign(base.page_elite_list);
	if (base.page_list != NULL)
		common::string_util_t::split<uint32_t>(std::string(base.page_list), this->t_page_vec, ":");

	if (base.page_elite_list != NULL)
		common::string_util_t::split<uint32_t>(std::string(base.page_elite_list), this->t_page_elite_vec, ":");
};

inline const std::string SectionReward::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SectionReward.tbls64";
#else
	return "SectionReward.tbls32";
#endif
};
