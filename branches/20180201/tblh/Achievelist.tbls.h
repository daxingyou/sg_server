#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:AchievelistBase
/// @remark   ��¼��С:24 X32
///           ��¼��С:32 X64
///           �ֶ�����:6
///           �����ļ�:c�ɾͱ�.xlsx �ɾ��б�
///           ��������:�ɾ�ID
#pragma pack(1)
struct AchievelistBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010EADB6; ///< I|IVIIV
#else
	static const int          FormatCheckCode = 0x0110AE56; ///< I|IvIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �ɾ�ID ����Ψһ��

	uint32_t                  id;                           ///< �ɾ�ID

	uint32_t                  level;                        ///< �ɾ͵ȼ�
	const char*               condition;                    ///< �ɾ��������(�¼�:ID:����)
	uint32_t                  drop_id;                      ///< �������ߣ�����ID)
	uint32_t                  type;                         ///< �ɾ�����(����)
	const char*               des;                          ///< �ɾ�����
};
#pragma pack()

struct Achievelist : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level()                       { return t_level; }
	const Dawn::triple< uint32_t, uint32_t, uint32_t>&        condition()                   { return t_condition; }
	uint32_t                  drop_id()                     { return t_drop_id; }
	uint32_t                  type()                        { return t_type; }
	const std::string&        des()                         { return t_des; }

private:
	uint32_t                  t_id;                         ///< �ɾ�ID

	uint32_t                  t_level;                      ///< �ɾ͵ȼ�
	Dawn::triple< uint32_t,uint32_t, uint32_t>			t_condition;                  ///< �ɾ��������(�¼�:ID:����)
	uint32_t                  t_drop_id;                    ///< �������ߣ�����ID)
	uint32_t                  t_type;                       ///< �ɾ�����(����)
	std::string               t_des;                        ///< �ɾ�����

public:
	void reset();
	void fill( const AchievelistBase& base );
	const char* getClassName() const { return "c�ɾͱ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Achievelist, AchievelistBase > AchievelistManager;

inline void Achievelist::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_condition.clear();
	this->t_drop_id = 0;
	this->t_type = 0;
	this->t_des.clear();
};

inline void Achievelist::fill( const AchievelistBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	//if( base.condition == NULL ) this->t_condition.clear(); else this->t_condition.assign( base.condition );
	Dawn::parseTuple(this->t_condition, base.condition, ":");
	this->t_drop_id = base.drop_id;
	this->t_type = base.type;
	if( base.des == NULL ) this->t_des.clear(); else this->t_des.assign( base.des );
};

inline const std::string Achievelist::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Achievelist.tbls64";
#else
	return "Achievelist.tbls32";
#endif
};
