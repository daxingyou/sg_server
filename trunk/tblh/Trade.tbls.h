#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:TradeBase
/// @remark   ��¼��С:28 X32
///           ��¼��С:44 X64
///           �ֶ�����:7
///           �����ļ�:t���̱�.xlsx trade
///           ��������:�̻�id
#pragma pack(1)
struct TradeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00DEBEA6; ///< I|IIVVVV
#else
	static const int          FormatCheckCode = 0x00E0D8C6; ///< I|IIvvvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �̻�id ����Ψһ��

	uint32_t                  id;                           ///< �̻�id

	uint32_t                  scene;                        ///< ���ڳ���
	uint32_t                  type;                         ///< ����
	const char*               sell_goods;                   ///< ���ۻ���
	const char*               sell_cnt;                     ///< ���ۻ�������
	const char*               unlock_type;                  ///< �����������
	const char*               unlock_para;                  ///< �����������
};
#pragma pack()

struct Trade : public Entry
{
	uint32_t                  id()                          { return t_id; }

// 	const std::string&        sell_goods()                  { return t_sell_goods; }
// 	const std::string&        sell_cnt()                    { return t_sell_cnt; }
// 	const std::string&        unlock_type()                 { return t_unlock_type; }
// 	const std::string&        unlock_para()                 { return t_unlock_para; }
	uint32_t                  scene()                       { return t_scene; }
	uint32_t                  type()                        { return t_type; }
	uint32_t		sell_goods_size() { return (uint32_t)t_sell_goods_vec.size(); }
	uint32_t		sell_cnt_size() { return (uint32_t)t_sell_cnt_vec.size(); }
	uint32_t		unlock_type_size() { return (uint32_t)t_unlock_type_vec.size(); }
	uint32_t		unlock_para_size() { return (uint32_t)t_unlock_para_vec.size(); }

	uint32_t        sell_goods(uint32_t pos) { return pos >= sell_goods_size() ? 0 : t_sell_goods_vec[pos]; }
	uint32_t        sell_cnt(uint32_t pos) { return pos >= sell_cnt_size() ? 0 : t_sell_cnt_vec[pos]; }
	uint32_t        unlock_type(uint32_t pos) { return pos >= unlock_type_size() ? 0 : t_unlock_type_vec[pos]; }
	uint32_t        unlock_para(uint32_t pos) { return pos >= unlock_para_size() ? 0 : t_unlock_para_vec[pos]; }
private:
	uint32_t                  t_id;                         ///< �̻�id

	uint32_t                  t_scene;                      ///< ���ڳ���
	uint32_t                  t_type;                       ///< ����
	std::string               t_sell_goods;                 ///< ���ۻ���
	std::string               t_sell_cnt;                   ///< ���ۻ�������
	std::string               t_unlock_type;                ///< �����������
	std::string               t_unlock_para;                ///< �����������

	std::vector<uint32_t>	  t_sell_goods_vec;
	std::vector<uint32_t>	  t_sell_cnt_vec;
	std::vector<uint32_t>	  t_unlock_type_vec;
	std::vector<uint32_t>	  t_unlock_para_vec;

public:
	void reset();
	void fill( const TradeBase& base );
	const char* getClassName() const { return "t���̱�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Trade, TradeBase > TradeManager;

inline void Trade::reset()
{
	this->t_id = 0;
	this->t_scene = 0;
	this->t_type = 0;
	this->t_sell_goods.clear();
	this->t_sell_cnt.clear();
	this->t_unlock_type.clear();
	this->t_unlock_para.clear();

	this->t_sell_goods_vec.clear();
	this->t_sell_cnt_vec.clear();
	this->t_unlock_type_vec.clear();
	this->t_unlock_para_vec.clear();
};

inline void Trade::fill( const TradeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_scene = base.scene;
	this->t_type = base.type;
	if( base.sell_goods == NULL ) this->t_sell_goods.clear(); else this->t_sell_goods.assign( base.sell_goods );
	if( base.sell_cnt == NULL ) this->t_sell_cnt.clear(); else this->t_sell_cnt.assign( base.sell_cnt );
	if( base.unlock_type == NULL ) this->t_unlock_type.clear(); else this->t_unlock_type.assign( base.unlock_type );
	if( base.unlock_para == NULL ) this->t_unlock_para.clear(); else this->t_unlock_para.assign( base.unlock_para );
	
	if (base.sell_goods != NULL)
	{
		string_util_t::split<uint32_t>(std::string(base.sell_goods), t_sell_goods_vec, "$");
	}
	if (base.sell_cnt != NULL)
	{
		string_util_t::split<uint32_t>(std::string(base.sell_cnt), t_sell_cnt_vec, "$");
	}
	if (base.unlock_type != NULL)
	{
		string_util_t::split<uint32_t>(std::string(base.unlock_type), t_unlock_type_vec, "$");
	}
	if (base.unlock_para != NULL)
	{
		string_util_t::split<uint32_t>(std::string(base.unlock_para), t_unlock_para_vec, "$");
	}
};

inline const std::string Trade::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Trade.tbls64";
#else
	return "Trade.tbls32";
#endif
};
