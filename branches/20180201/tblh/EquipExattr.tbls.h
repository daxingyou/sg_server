#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:EquipExattrBase
/// @remark   ��¼��С:24 X32
///           ��¼��С:24 X64
///           �ֶ�����:6
///           �����ļ�:zװ����.xlsx �д����Ա�
///           ��������:�д�ID
#pragma pack(1)
struct EquipExattrBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010DDD89; ///< I|IIIII
#else
	static const int          FormatCheckCode = 0x010DDD89; ///< I|IIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �д�ID ����Ψһ��

	uint32_t                  id;                           ///< �д�ID

	uint32_t                  exattr_pool;                  ///< �дʳ�ID
	uint32_t                  prob;                         ///< �дʸ���
	uint32_t                  attr_type;                    ///< �д�����ID
	uint32_t                  attr_value;                   ///< �д�������ֵ
	uint32_t                  score;                        ///< �д�����
};
#pragma pack()

struct EquipExattr : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  exattr_pool()                 { return t_exattr_pool; }
	uint32_t                  prob()                        { return t_prob; }
	uint32_t                  attr_type()                   { return t_attr_type; }
	uint32_t                  attr_value()                  { return t_attr_value; }
	uint32_t                  score() { return t_score; }

private:
	uint32_t                  t_id;                         ///< �д�ID

	uint32_t                  t_exattr_pool;                ///< �дʳ�ID
	uint32_t                  t_prob;                       ///< �дʸ���
	uint32_t                  t_attr_type;                  ///< �д�����ID
	uint32_t                  t_attr_value;                 ///< �д�������ֵ
	uint32_t                  t_score;                      ///< �д�����

public:
	void reset();
	void fill( const EquipExattrBase& base );
	const char* getClassName() const { return "zװ����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipExattr, EquipExattrBase > EquipExattrManager;

inline void EquipExattr::reset()
{
	this->t_id = 0;
	this->t_exattr_pool = 0;
	this->t_prob = 0;
	this->t_attr_type = 0;
	this->t_attr_value = 0;
	this->t_score = 0;
};

inline void EquipExattr::fill( const EquipExattrBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_exattr_pool = base.exattr_pool;
	this->t_prob = base.prob;
	this->t_attr_type = base.attr_type;
	this->t_attr_value = base.attr_value;
	this->t_score = base.score;
};

inline const std::string EquipExattr::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipExattr.tbls64";
#else
	return "EquipExattr.tbls32";
#endif
};
