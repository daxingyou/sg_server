#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:RoleBuffBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:20 X64
///           �ֶ�����:4
///           �����ļ�:b���ͼ�����.xlsx �����б�
///           ��������:����ID
#pragma pack(1)
struct RoleBuffBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510EA9; ///< I|IVI
#else
	static const int          FormatCheckCode = 0x005110A9; ///< I|IvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ����ID ����Ψһ��

	uint32_t                  id;                           ///< ����ID

	uint32_t                  type;                         ///< ��������
	const char*               param;                        ///< �������
	uint32_t                  time;                         ///< ����ʱ��
};
#pragma pack()

typedef std::vector<int32_t> child_param_vec;
typedef std::vector<child_param_vec> param_vec;

struct RoleBuff : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	uint32_t                  time()                        { return t_time; }

private:
	uint32_t                  t_id;                         ///< ����ID

	uint32_t                  t_type;                       ///< ��������
	param_vec				  t_param;						///< ����
	uint32_t                  t_time;                       ///< ����ʱ��

public:
	void reset();
	void fill( const RoleBuffBase& base );
	const char* getClassName() const { return "b���ͼ�����"; }

	int32_t get_default_param()
	{
		if (t_param.size() > 0)
		{
			if (t_param[0].size() > 0)
				return t_param[0][0];
		}
		return 0;
	}
	const param_vec& get_all_param() { return t_param; }

	static const std::string getBinaryName();
};

typedef DataTableManager< RoleBuff, RoleBuffBase > RoleBuffManager;

inline void RoleBuff::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_param.clear();
	this->t_time = 0;
};

inline void RoleBuff::fill( const RoleBuffBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	Dawn::parseDoubleContainer<int32_t, child_param_vec, param_vec>(this->t_param, base.param, ":$");
	this->t_time = base.time;
};

inline const std::string RoleBuff::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "RoleBuff.tbls64";
#else
	return "RoleBuff.tbls32";
#endif
};
