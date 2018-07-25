#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:LuckydrawSeqBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:c�鿨��.xlsx �鿨����
///           ��������:�鿨����
#pragma pack(1)
struct LuckydrawSeqBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return type; }; // TODO: ���� �鿨���� ����Ψһ��

	uint32_t                  type;                         ///< �鿨����

	const char*               seq;                          ///< �鿨����
};
#pragma pack()

struct LuckydrawSeq : public Entry
{
	uint32_t                  type()                        { return t_type; }

	const std::string&        seq()                         { return t_seq; }

private:
	uint32_t                  t_type;                       ///< �鿨����

	std::string               t_seq;                        ///< �鿨����

public:
	void reset();
	void fill( const LuckydrawSeqBase& base );
	const char* getClassName() const { return "c�鿨��"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LuckydrawSeq, LuckydrawSeqBase > LuckydrawSeqManager;

inline void LuckydrawSeq::reset()
{
	this->t_type = 0;
	this->t_seq.clear();
};

inline void LuckydrawSeq::fill( const LuckydrawSeqBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_type = base.type;
	if( base.seq == NULL ) this->t_seq.clear(); else this->t_seq.assign( base.seq );
};

inline const std::string LuckydrawSeq::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LuckydrawSeq.tbls64";
#else
	return "LuckydrawSeq.tbls32";
#endif
};
