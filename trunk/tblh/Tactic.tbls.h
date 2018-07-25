#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:TacticBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:32 X64
///           �ֶ�����:5
///           �����ļ�:z�󷨱�.xlsx Sheet1
///           ��������:��ID
#pragma pack(1)
struct TacticBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510EBB6; ///< I|IVVV
#else
	static const int          FormatCheckCode = 0x05110DD6; ///< I|Ivvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ��ID ����Ψһ��

	uint32_t                  id;                           ///< ��ID

	uint32_t                  level;                        ///< �ȼ�
	const char*               form;                         ///< ����״
	const char*               attr_buff;                         ///< ������
	const char*               eye;                          ///< ����
};
#pragma pack()

struct Tactic : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level()                       { return t_level; }
	const std::vector<uint32_t>&        form()                        { return t_form; }
	const std::map<uint32_t, uint32_t>&        attr_buff()                        { return t_attr_buff; }
	const std::vector<uint32_t>&        eye()                         { return t_eye; }

private:
	uint32_t                  t_id;                         ///< ��ID

	uint32_t                  t_level;                      ///< �ȼ�
	std::vector<uint32_t>               t_form;                       ///< ����״
	std::map<uint32_t, uint32_t>              t_attr_buff;                       ///< ������
	std::vector<uint32_t>               t_eye;                        ///< ����

public:
	void reset();
	void fill( const TacticBase& base );
	const char* getClassName() const { return "z�󷨱�"; }

	static const std::string getBinaryName();

	bool is_eye(uint32_t pos)
	{
		for (auto grid : this->t_eye)
		{
			if (grid == pos)
			{
				return true;
			}
		}

		return false;
	}
};

typedef DataTableManager< Tactic, TacticBase > TacticManager;

inline void Tactic::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_form.clear();
	this->t_attr_buff.clear();
	this->t_eye.clear();
};

inline void Tactic::fill( const TacticBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	//if( base.form == NULL ) this->t_form.clear(); else this->t_form.assign( base.form );
	Dawn::parseContainer<uint32_t>(this->t_form, base.form, "$");
	//if( base.attr_buff == NULL ) this->t_attr_buff.clear(); else this->t_attr_buff.assign( base.attr_buff );
	std::vector<uint32_t> temp;
	Dawn::parseContainer<uint32_t>(temp, base.attr_buff, "$");
	if (temp.size() != this->t_form.size())
	{
		log_error("tactic config error %u", this->t_id);
		return;
	}
	for (size_t i = 0; i < this->t_form.size(); ++i)
	{
		this->t_attr_buff[this->t_form[i]] = temp[i];
	}
	//if( base.eye == NULL ) this->t_eye.clear(); else this->t_eye.assign( base.eye );
	Dawn::parseContainer<uint32_t>(this->t_eye, base.eye, "$");
};

inline const std::string Tactic::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Tactic.tbls64";
#else
	return "Tactic.tbls32";
#endif
};
