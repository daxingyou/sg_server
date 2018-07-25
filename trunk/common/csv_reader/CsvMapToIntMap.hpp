#ifndef _CsvMapToIntMap_
#define _CsvMapToIntMap_
#include <stdint.h>

class CsvMapToIntMap
{
public:
	static bool Open(uint32_t mapId, 
		uint32_t max_size, 
		const char* path, 
		uint32_t& xMax, uint32_t& yMax, 
		uint64_t** array, 
		uint32_t& xOldMax, 
		unsigned char** pMapBuf);
};

#endif

