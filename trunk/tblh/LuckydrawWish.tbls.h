#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:LuckydrawWishBase
/// @remark   ��¼��С:12 X32
///           ��¼��С:16 X64
///           �ֶ�����:3
///           �����ļ�:c�鿨��.xlsx ��������
///           ��������:����ID
#pragma pack(1)
struct LuckydrawWishBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510E6; ///< I|IV
#else
	static const int          FormatCheckCode = 0x00051106; ///< I|Iv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ����ID ����Ψһ��

	uint32_t                  id;                           ///< ����ID

	uint32_t                  open_time;                    ///< ����ʱ��
	const char*               open_resource;                ///< ������Դ
};
#pragma pack()

struct LuckydrawWish : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  open_time()                   { return t_open_time; }
	const std::pair<uint32_t,uint32_t>&        open_resource()               { return t_open_resource; }

private:
	uint32_t                  t_id;                         ///< ����ID

	uint32_t                  t_open_time;                  ///< ����ʱ��
	std::pair<uint32_t, uint32_t>               t_open_resource;              ///< ������Դ

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
	this->t_open_time = 0;
	//this->t_open_resource.clear();
};

inline void LuckydrawWish::fill( const LuckydrawWishBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_open_time = base.open_time;
	//if( base.open_resource == NULL ) this->t_open_resource.clear(); else this->t_open_resource.assign( base.open_resource );
	Dawn::parseTuple(this->t_open_resource, base.open_resource, ":");
};

inline const std::string LuckydrawWish::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LuckydrawWish.tbls64";
#else
	return "LuckydrawWish.tbls32";
#endif
};
