#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:BuffBigTypeBase
/// @remark   ��¼��С:12 X32
///           ��¼��С:20 X64
///           �ֶ�����:3
///           �����ļ�:g�⻷��.xlsx buff�����͹�ϵ��
///           ��������:buff������id
#pragma pack(1)
struct BuffBigTypeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000511B6; ///< I|VV
#else
	static const int          FormatCheckCode = 0x000513D6; ///< I|vv
#endif

	//------------------------
	uint32_t getUniqueID() const { return buff_big_type; }; // TODO: ���� buff������id ����Ψһ��

	uint32_t                  buff_big_type;                ///< buff������id

	const char*               buff_resist_types;            ///< ���������id
	const char*               buff_replace_types;           ///< �滻������id
};
#pragma pack()

struct BuffBigType : public Entry
{
	uint32_t                  buff_big_type()               { return t_buff_big_type; }

	const std::vector<uint32_t>&        buff_resist_types() { return t_buff_resist_types; }
	const std::vector<uint32_t>&        buff_replace_types() { return t_buff_replace_types; }

	uint32_t				buff_resist_type(uint32_t pos) { return pos >= t_buff_resist_types.size() ? 0 : t_buff_resist_types[pos]; }
	uint32_t				buff_replace_type(uint32_t pos) { return pos >= t_buff_replace_types.size() ? 0 : t_buff_replace_types[pos]; }
private:
	uint32_t                  t_buff_big_type;              ///< buff������id

	std::vector<uint32_t>     t_buff_resist_types;          ///< ���������id
	std::vector<uint32_t>     t_buff_replace_types;         ///< �滻������id

public:
	void reset();
	void fill( const BuffBigTypeBase& base );
	const char* getClassName() const { return "g�⻷��"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< BuffBigType, BuffBigTypeBase > BuffBigTypeManager;

inline void BuffBigType::reset()
{
	this->t_buff_big_type = 0;
	this->t_buff_resist_types.clear();
	this->t_buff_replace_types.clear();
};

inline void BuffBigType::fill( const BuffBigTypeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_buff_big_type = base.buff_big_type;
	//if( base.buff_resist_types == NULL ) this->t_buff_resist_types.clear(); else this->t_buff_resist_types.assign( base.buff_resist_types );
	//if (base.buff_replace_types == NULL) this->t_buff_replace_types.clear(); else this->t_buff_replace_types.assign(base.buff_replace_types);
	Dawn::parseContainer<uint32_t>(this->t_buff_resist_types, base.buff_resist_types, ":");
	Dawn::parseContainer<uint32_t>(this->t_buff_replace_types, base.buff_replace_types, ":");
};

inline const std::string BuffBigType::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "BuffBigType.tbls64";
#else
	return "BuffBigType.tbls32";
#endif
};
