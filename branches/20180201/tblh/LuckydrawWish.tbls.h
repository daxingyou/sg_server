#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:LuckydrawWishBase
/// @remark   ��¼��С:28 X32
///           ��¼��С:40 X64
///           �ֶ�����:7
///           �����ļ�:c�鿨��.xlsx ��������
///           ��������:����ID
#pragma pack(1)
struct LuckydrawWishBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00DDEEA6; ///< I|IIIVVV
#else
	static const int          FormatCheckCode = 0x00DE08C6; ///< I|IIIvvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ����ID ����Ψһ��

	uint32_t                  id;                           ///< ����ID

	uint32_t                  type;                         ///< �鿨����
	uint32_t                  default_open;                 ///< Ĭ�Ͽ���
	uint32_t                  open_time;                    ///< ����ʱ��
	const char*               open_resource;                ///< ������Դ
	const char*               cost_resource;                ///< ��������
	const char*               item_resource;                ///< ��������
};
#pragma pack()

struct luckydraw_cost 
{
	uint32_t id = 0;
	uint32_t num = 0;
};

struct LuckydrawWish : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	uint32_t                  default_open()                { return t_default_open; }
	uint32_t                  open_time()                   { return t_open_time; }

private:
	uint32_t                  t_id;                         ///< ����ID

	uint32_t                  t_type;                       ///< �鿨����
	uint32_t                  t_default_open;               ///< Ĭ�Ͽ���
	uint32_t                  t_open_time;                  ///< ����ʱ��

public:
	luckydraw_cost			   open_resource;				
	luckydraw_cost			   cost_resource;				
	luckydraw_cost			   item_resource;				

public:
	void reset();
	void fill( const LuckydrawWishBase& base );
	const char* getClassName() const { return "c�鿨��"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LuckydrawWish, LuckydrawWishBase > LuckydrawWishManager;

inline void LuckydrawWish::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_default_open = 0;
	this->t_open_time = 0;
};

inline void LuckydrawWish::fill( const LuckydrawWishBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_default_open = base.default_open;
	this->t_open_time = base.open_time;

	//��������
	if (NULL != base.open_resource)
	{
		std::vector<std::uint32_t> conditions;
		string_util_t::split<uint32_t>(std::string(base.open_resource), conditions, ":");
		if (conditions.size() == 2)
		{
			this->open_resource.id = conditions[0];
			this->open_resource.num = conditions[1];
		}
	}

	//�鿨����
	if (NULL != base.cost_resource)
	{
		std::vector<std::uint32_t> conditions;
		string_util_t::split<uint32_t>(std::string(base.cost_resource), conditions, ":");
		if (conditions.size() == 2)
		{
			this->cost_resource.id = conditions[0];
			this->cost_resource.num = conditions[1];
		}
	}
	
	//��������
	if (NULL != base.item_resource)
	{
		std::vector<std::uint32_t> conditions;
		string_util_t::split<uint32_t>(std::string(base.item_resource), conditions, ":");
		if (conditions.size() == 2)
		{
			this->item_resource.id = conditions[0];
			this->item_resource.num = conditions[1];
		}
	}
};

inline const std::string LuckydrawWish::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LuckydrawWish.tbls64";
#else
	return "LuckydrawWish.tbls32";
#endif
};
