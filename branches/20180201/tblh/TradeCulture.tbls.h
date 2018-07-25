#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:TradeCultureBase
/// @remark   ��¼��С:4 X32
///           ��¼��С:4 X64
///           �ֶ�����:1
///           �����ļ�:t���̱�.xlsx trade_culture
///           ��������:�Ļ�Ȧid
#pragma pack(1)
struct TradeCultureBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0000050C; ///< I|
#else
	static const int          FormatCheckCode = 0x0000050C; ///< I|
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �Ļ�Ȧid ����Ψһ��

	uint32_t                  id;                           ///< �Ļ�Ȧid

};
#pragma pack()

struct TradeCulture : public Entry
{
	uint32_t                  id()                          { return t_id; }


private:
	uint32_t                  t_id;                         ///< �Ļ�Ȧid


public:
	void reset();
	void fill( const TradeCultureBase& base );
	const char* getClassName() const { return "t���̱�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TradeCulture, TradeCultureBase > TradeCultureManager;

inline void TradeCulture::reset()
{
	this->t_id = 0;
};

inline void TradeCulture::fill( const TradeCultureBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
};

inline const std::string TradeCulture::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TradeCulture.tbls64";
#else
	return "TradeCulture.tbls32";
#endif
};
