#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"
#include "common/ContainerParse.h"
#include "tblh/attr_enum.hpp"

/// @brief    表格结构:BuffTableBase
/// @remark   记录大小:60 X32
///           记录大小:68 X64
///           字段数量:15
///           数据文件:g光环表.xlsx BuffTable
///           排序主键:buffID
#pragma pack(1)
struct BuffTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000657D9; ///< I|IIIIIVIIIIVIII
#else
	static const int          FormatCheckCode = 0x0019B7D9; ///< I|IIIIIvIIIIvIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 buffID 生成唯一键

	uint32_t                  id;                           ///< buffID

	uint32_t                  buff_big_type;                ///< buff大类型
	uint32_t                  buff_small_type;              ///< buff小类型
	uint32_t                  rounds;                       ///< 持续回合
	uint32_t                  trigger_type;                 ///< 触发方式
	uint32_t                  probability;                  ///< 触发概率
	const char*               elements;                     ///< buff元素
	uint32_t                  remove_type;                  ///< 移除方式
	uint32_t                  overlay_type;                 ///< 叠加方式
	uint32_t                  max_layer;                    ///< 最大层数
	uint32_t                  layer_remove_type;            ///< 层数移除方式
	const char*               layer_convert;                ///< 层数转换
	uint32_t                  buff_formula;                 ///< buff公式
	uint32_t                  buff_effect_type;             ///< buff作用类型
	uint32_t                  max_trigger_count;            ///< 触发次数上限
};
#pragma pack()

struct buff_element_data
{
public:
	friend class BuffTable;

	bool operator < (const buff_element_data &other) const
	{
		return this->id < other.id;
	}

	buff_element_data() 
		: id(0)
		, value1(0)
		, value2(0)
		, value3(0)
		, value4(0)
		, value5(0) 
		, value6(0)
		, value7(0)
		, value8(0)
		, value9(0)
	{}

	uint32_t id;
	int32_t value1;
	int32_t value2;
	int32_t value3;
	int32_t value4;
	int32_t value5;
	int32_t value6;
	int32_t value7;
	int32_t value8;
	int32_t value9;
};


struct BuffTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  buff_big_type()               { return t_buff_big_type; }
	uint32_t                  buff_small_type()             { return t_buff_small_type; }
	uint32_t                  rounds()                      { return t_rounds; }
	uint32_t                  trigger_type()                { return t_trigger_type; }
	uint32_t                  probability()                 { return t_probability; }
	//	const buff_element_data&        elements() { return t_elements; }
	const std::vector<buff_element_data>&        elements() { return t_elements; }
	uint32_t                  remove_type() { return t_remove_type; }
	uint32_t                  overlay_type() { return t_overlay_type; }
	uint32_t                  max_layer() { return t_max_layer; }
	uint32_t                  layer_remove_type() { return t_layer_remove_type; }
	const std::vector<uint32_t>&        layer_convert() { return t_layer_convert; }
	uint32_t				  layer_convert(uint32_t pos) { return pos >= t_layer_convert.size() ? 0 : t_layer_convert[pos]; }
	uint32_t                  buff_formula()                { return t_buff_formula; }
	uint32_t                  buff_effect_type()            { return t_buff_effect_type; }
	uint32_t                  max_trigger_count() { return t_max_trigger_count; }

private:
	uint32_t                  t_id;                         ///< buffID

	uint32_t                  t_buff_big_type;              ///< buff大类型
	uint32_t                  t_buff_small_type;            ///< buff小类型
	uint32_t                  t_rounds;                     ///< 持续回合
	uint32_t                  t_trigger_type;               ///< 触发方式
	uint32_t                  t_probability;                ///< 触发概率
//	buff_element_data		  t_elements;					///< buff元素
	std::vector<buff_element_data> t_elements;				///< buff元素
	uint32_t                  t_remove_type;                ///< 移除方式
	uint32_t                  t_overlay_type;               ///< 叠加方式
	uint32_t                  t_max_layer;                  ///< 最大层数
	uint32_t                  t_layer_remove_type;          ///< 层数移除方式
	std::vector<uint32_t>     t_layer_convert;              ///< 层数转换
	uint32_t                  t_buff_formula;               ///< buff公式
	uint32_t                  t_buff_effect_type;           ///< buff作用类型
	uint32_t                  t_max_trigger_count;          ///< 触发次数上限

public:
	void reset();
	void fill( const BuffTableBase& base );
	const char* getClassName() const { return "g光环表"; }

	static const std::string getBinaryName();

	bool is_control_buff()
	{
		//// 1、9、19、20是指改属性
		//if (t_elements.id != 1 && t_elements.id != 9 && t_elements.id != 19 && t_elements.id != 20)	
		//{
		//	return false;
		//}

		//uint32_t type = t_elements.value1;
		//if (type == common::attrtype_enum::stun || type == common::attrtype_enum::confused)
		//{
		//	return true;
		//}
		std::vector<buff_element_data>::const_iterator it;
		for (it = t_elements.begin(); it != t_elements.end(); ++it)
		{
			const buff_element_data& element = *it;
			if (element.id != 1)
			{
				continue;
			}
			
			uint32_t type = element.value1;
			if (type == common::attrtype_enum::stun || type == common::attrtype_enum::confused)
			{
				return true;
			}
		}
		return false;
	}
};

typedef DataTableManager< BuffTable, BuffTableBase > BuffTableManager;

inline void BuffTable::reset()
{
	this->t_id = 0;
	this->t_buff_big_type = 0;
	this->t_buff_small_type = 0;
	this->t_rounds = 0;
	this->t_trigger_type = 0;
	this->t_probability = 0;
	this->t_elements.clear();
	this->t_remove_type = 0;
	this->t_overlay_type = 0;
	this->t_max_layer = 0;
	this->t_layer_remove_type = 0;
	this->t_layer_convert.clear();
	this->t_buff_formula = 0;
	this->t_buff_effect_type = 0;
	this->t_max_trigger_count = 0;
};

inline void BuffTable::fill( const BuffTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_buff_big_type = base.buff_big_type;
	this->t_buff_small_type = base.buff_small_type;
	this->t_rounds = base.rounds;
	this->t_trigger_type = base.trigger_type;
	this->t_probability = base.probability;
	//if( base.elements == NULL ) this->t_elements.clear(); else this->t_elements.assign( base.elements );
	std::vector<std::vector<int>> temp;
	Dawn::parseDoubleContainer<int, std::vector<int>, std::vector<std::vector<int>>>(temp, base.elements, ":$");
	for (auto element : temp)
	{
		buff_element_data data;

		if (element.size() > 0)
			data.id = element[0];
		if (element.size() > 1)
			data.value1 = element[1];
		if (element.size() > 2)
			data.value2 = element[2];
		if (element.size() > 3)
			data.value3 = element[3];
		if (element.size() > 4)
			data.value4 = element[4];
		if (element.size() > 5)
			data.value5 = element[5];
		if (element.size() > 6)
			data.value6 = element[6];
		if (element.size() > 7)
			data.value7 = element[7];
		if (element.size() > 8)
			data.value8 = element[8];
		if (element.size() > 9)
			data.value9 = element[9];

		t_elements.push_back(data);
	}
// 	this->t_elements.values.clear();
// 	Dawn::parseContainer<uint32_t>(this->t_elements.values, base.elements, ":");
// 	
// 	if(this->t_elements.values.size() > 0)
// 		this->t_elements.id = this->t_elements.values[0];
// 	if (this->t_elements.values.size() > 1)
// 		this->t_elements.value1 = this->t_elements.values[1];
// 	if (this->t_elements.values.size() > 2)
// 		this->t_elements.value2 = this->t_elements.values[2];
// 	if (this->t_elements.values.size() > 3)
// 		this->t_elements.value3 = this->t_elements.values[3];

	this->t_remove_type = base.remove_type;
	this->t_overlay_type = base.overlay_type;
	this->t_max_layer = base.max_layer;
	this->t_layer_remove_type = base.layer_remove_type;
	//if (base.layer_convert == NULL) this->t_layer_convert.clear(); else this->t_layer_convert.assign(base.layer_convert);
	Dawn::parseContainer<uint32_t>(this->t_layer_convert, base.layer_convert, ":");
	this->t_buff_formula = base.buff_formula;
	this->t_buff_effect_type = base.buff_effect_type;
	this->t_max_trigger_count = base.max_trigger_count;
};

inline const std::string BuffTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "BuffTable.tbls64";
#else
	return "BuffTable.tbls32";
#endif
};
