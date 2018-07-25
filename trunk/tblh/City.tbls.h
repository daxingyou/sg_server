#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:CityBase
/// @remark   ��¼��С:12 X32
///           ��¼��С:12 X64
///           �ֶ�����:3
///           �����ļ�:c���б�.xlsx city
///           ��������:id
#pragma pack(1)
struct CityBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	uint32_t                  type;                         ///< ��������
	uint32_t                  trade_id;                     ///< �̻�id
};
#pragma pack()

struct City : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	uint32_t                  trade_id()                    { return t_trade_id; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_type;                       ///< ��������
	uint32_t                  t_trade_id;                   ///< �̻�id

public:
	void reset();
	void fill( const CityBase& base );
	const char* getClassName() const { return "c���б�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< City, CityBase > CityManager;

inline void City::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_trade_id = 0;
};

inline void City::fill( const CityBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_trade_id = base.trade_id;
};

inline const std::string City::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "City.tbls64";
#else
	return "City.tbls32";
#endif
};
