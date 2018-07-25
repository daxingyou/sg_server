#pragma once

#include "macros.hpp"

struct TblxFileHead
{
public:
	TblxFileHead(void);
	~TblxFileHead(void);

	///< tpis
	char head_tips[4];
	///< 版本号
	uint32_t version;
	///< 结构描述字段长度
	uint32_t desc_seg_len;
	///< 数据字段长度
	uint32_t data_seg_len;
	///< 字符串字段长度
	uint32_t str_seg_len;


	uint32_t memorySize() const { return data_seg_len + str_seg_len; }
	uint32_t descSegSize() const { return sizeof( *this ); }
	uint32_t dataSegSize() const { return descSegSize() + desc_seg_len; }
	uint32_t strSegSize() const { return dataSegSize() + data_seg_len; }
	uint32_t allSize() const { return sizeof( *this ) + desc_seg_len + data_seg_len + str_seg_len; }
};
