#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:LiandanTableBase
/// @remark   ��¼��С:28 X32
///           ��¼��С:36 X64
///           �ֶ�����:7
///           �����ļ�:h�ϳɱ�.xlsx ��ҩ
///           ��������:�ϳ�Id
#pragma pack(1)
struct LiandanTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0EBABBD9; ///< I|IVVIVII
#else
	static const int          FormatCheckCode = 0x00DB5BC9; ///< I|IvvIvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �ϳ�Id ����Ψһ��

	uint32_t                  id;                           ///< �ϳ�Id

	uint32_t                  drop_id;                      ///< �ϳɵ���ID
	const char*               material;                     ///< �ϳɲ���
	const char*               money;                        ///< ���Ļ���
	uint32_t                  time;                         ///< �ϳ�ʱ��
	const char*               done_cost;                    ///< ��������
	uint32_t                  need_level;                   ///< ����ս�ӵȼ�
	uint32_t                  max_count;                    ///< ÿ�����ϳ�
};
#pragma pack()

struct LiandanTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_id()                     { return t_drop_id; }
	const std::map<uint32_t, uint32_t>&        material()                    { return t_material; }
	const std::map<uint32_t, uint32_t>&                  money() { return t_money; }
	uint32_t                  time()                        { return t_time; }
	const Dawn::triple<uint32_t, uint32_t, uint32_t>&        done_cost()                   { return t_done_cost; }
	uint32_t                  need_level()                  { return t_need_level; }
	uint32_t                  max_count()                   { return t_max_count; }

private:
	uint32_t                  t_id;                         ///< �ϳ�Id

	uint32_t                  t_drop_id;                    ///< �ϳɵ���ID
	std::map<uint32_t, uint32_t>               t_material;                   ///< �ϳɲ���
	std::map<uint32_t, uint32_t>               t_money;                     ///< ���Ļ���
	uint32_t                  t_time;                       ///< �ϳ�ʱ��
	Dawn::triple<uint32_t, uint32_t, uint32_t>               t_done_cost;                  ///< ��������
	uint32_t                  t_need_level;                 ///< ����ս�ӵȼ�
	uint32_t                  t_max_count;                  ///< ÿ�����ϳ�

public:
	void reset();
	void fill( const LiandanTableBase& base );
	const char* getClassName() const { return "h�ϳɱ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LiandanTable, LiandanTableBase > LiandanTableManager;

inline void LiandanTable::reset()
{
	this->t_id = 0;
	this->t_drop_id = 0;
	this->t_material.clear();
	this->t_money.clear();
	this->t_time = 0;
	this->t_done_cost.clear();
	this->t_need_level = 0;
	this->t_max_count = 0;
};

inline void LiandanTable::fill( const LiandanTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_id = base.drop_id;
	//if( base.material == NULL ) this->t_material.clear(); else this->t_material.assign( base.material );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_material, base.material, ":$");
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_money, base.money, ":$");
	this->t_time = base.time;
	//if( base.done_cost == NULL ) this->t_done_cost.clear(); else this->t_done_cost.assign( base.done_cost );
	Dawn::parseTuple(this->t_done_cost, base.done_cost, ":");
	this->t_need_level = base.need_level;
	this->t_max_count = base.max_count;
};

inline const std::string LiandanTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LiandanTable.tbls64";
#else
	return "LiandanTable.tbls32";
#endif
};
