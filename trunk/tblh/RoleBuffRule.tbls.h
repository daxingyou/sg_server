#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:RoleBuffRuleBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:16 X64
///           �ֶ�����:4
///           �����ļ�:b���ͼ�����.xlsx �������
///           ��������:��������
#pragma pack(1)
struct RoleBuffRuleBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �������� ����Ψһ��

	uint32_t                  id;                           ///< ��������

	uint32_t                  mutex;                        ///< ����
	uint32_t                  replace;                      ///< �滻
	uint32_t                  together;                     ///< ����
};
#pragma pack()

struct RoleBuffRule : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  mutex()                       { return t_mutex; }
	uint32_t                  replace()                     { return t_replace; }
	uint32_t                  together()                    { return t_together; }

private:
	uint32_t                  t_id;                         ///< ��������

	uint32_t                  t_mutex;                      ///< ����
	uint32_t                  t_replace;                    ///< �滻
	uint32_t                  t_together;                   ///< ����

public:
	void reset();
	void fill( const RoleBuffRuleBase& base );
	const char* getClassName() const { return "b���ͼ�����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< RoleBuffRule, RoleBuffRuleBase > RoleBuffRuleManager;

inline void RoleBuffRule::reset()
{
	this->t_id = 0;
	this->t_mutex = 0;
	this->t_replace = 0;
	this->t_together = 0;
};

inline void RoleBuffRule::fill( const RoleBuffRuleBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_mutex = base.mutex;
	this->t_replace = base.replace;
	this->t_together = base.together;
};

inline const std::string RoleBuffRule::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "RoleBuffRule.tbls64";
#else
	return "RoleBuffRule.tbls32";
#endif
};
