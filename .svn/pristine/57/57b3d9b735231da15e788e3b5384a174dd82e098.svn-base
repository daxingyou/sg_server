#pragma once

#include <string>
#include <vector>

#include "macros.hpp"

#include "TblxFileHead.h"

#define TBL_PATH "./tbl/"
#define TBL_FILE ".tbls64"

class tbl_reader_t
{
public:
	void read_tbl(const char* name, uint32_t id = 0);

	bool loadBinary(const std::string& name, uint32_t id = 0);

	bool load(const std::string& filename, uint32_t id = 0);

	uint32_t parseDescribleSymbol(const std::string& descseg, std::vector< uint32_t >& stringoffset);

	bool refresh(
		const TblxFileHead& head,
		const std::string& descseg,
		const uint32_t itemsize,
		const std::vector< uint32_t >& stringoffset,
		std::vector< char >& data_seg,
		const std::vector< char >& str_seg,
		uint32_t id = 0); 
};