#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:RankTypeBase
/// @remark   ��¼��С:28 X32
///           ��¼��С:32 X64
///           �ֶ�����:7
///           �����ļ�:p���а�.xlsx ���а�����
///           ��������:����ID
#pragma pack(1)
struct RankTypeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00DDDBB9; ///< I|IIIIVI
#else
	static const int          FormatCheckCode = 0x00DDE5B9; ///< I|IIIIvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return type_id; }; // TODO: ���� ����ID ����Ψһ��

	uint32_t                  type_id;                      ///< ����ID

	uint32_t                  flush_type;                   ///< ˢ������
	uint32_t                  clear_type;                   ///< �������
	uint32_t                  min_value;                    ///< �ϰ�����
	uint32_t                  count;                        ///< ��ʾ����
	const char*               name;                         ///< ˵��
	uint32_t                  owner_type;                   ///< ����
};
#pragma pack()

struct RankType : public Entry
{
	uint32_t                  type_id()                     { return t_type_id; }

	uint32_t                  flush_type()                  { return t_flush_type; }
	uint32_t                  clear_type()                  { return t_clear_type; }
	uint32_t                  min_value()                   { return t_min_value; }
	uint32_t                  count()                       { return t_count; }
	const std::string&        name()                        { return t_name; }
	uint32_t                  owner_type()                  { return t_owner_type; }

private:
	uint32_t                  t_type_id;                    ///< ����ID

	uint32_t                  t_flush_type;                 ///< ˢ������
	uint32_t                  t_clear_type;                 ///< �������
	uint32_t                  t_min_value;                  ///< �ϰ�����
	uint32_t                  t_count;                      ///< ��ʾ����
	std::string               t_name;                       ///< ˵��
	uint32_t                  t_owner_type;                 ///< ����

public:
	void reset();
	void fill( const RankTypeBase& base );
	const char* getClassName() const { return "p���а�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< RankType, RankTypeBase > RankTypeManager;

inline void RankType::reset()
{
	this->t_type_id = 0;
	this->t_flush_type = 0;
	this->t_clear_type = 0;
	this->t_min_value = 0;
	this->t_count = 0;
	this->t_name.clear();
	this->t_owner_type = 0;
};

inline void RankType::fill( const RankTypeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_type_id = base.type_id;
	this->t_flush_type = base.flush_type;
	this->t_clear_type = base.clear_type;
	this->t_min_value = base.min_value;
	this->t_count = base.count;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	this->t_owner_type = base.owner_type;
};

inline const std::string RankType::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "RankType.tbls64";
#else
	return "RankType.tbls32";
#endif
};
