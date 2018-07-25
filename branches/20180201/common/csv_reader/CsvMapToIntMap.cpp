#include "CsvMapToIntMap.hpp"
#include "CsvParse.hpp"
#include "ParseDropItem.hpp"
#include <cmath>
#ifndef WIN32
#include <arpa/inet.h>
#include<string.h>
#endif

bool CsvMapToIntMap::Open(uint32_t mapId, 
	uint32_t max_size, 
	const char* path, 
	uint32_t& xMax, uint32_t& yMax, 
	uint64_t** array, 
	uint32_t& xOldMax, 
	unsigned char** pMapBuf)
{
	char* pFileBuf = CsvParse::FOpen(path);
	if (NULL == pFileBuf)
		return false;

	xOldMax = ntohl(*(uint32_t*)(pFileBuf + 4));
	uint32_t now_x = std::min<uint32_t>(xOldMax, max_size);	// 设置最大值
	uint32_t oldYMax = ntohl(*(uint32_t*)(pFileBuf + 8));
	oldYMax = std::min<uint32_t>(oldYMax, max_size);			// 设置最大值
	uint32_t xMaxInt = ceil(((float)now_x) / 64);
	xMax = xMaxInt * 64;
	yMax = oldYMax;
	*array = new uint64_t[xMaxInt*yMax];
	memset(*array, 0, xMaxInt*yMax);
	unsigned char* pmap = (unsigned char*)(pFileBuf + 12);
	for (uint32_t y = 0; y < oldYMax; ++y)
	{
		for (uint32_t x = 0; x < now_x; ++x)
		{
			if (pmap[x + y*xOldMax] >= (unsigned char)1)
			{
				int index = x / 64 + y *(xMax / 64);
				int bitIndex = x % 64;
				uint64_t temp = (1ULL << bitIndex);
				(*array)[index] |= temp;
			}
		}
	}
	*pMapBuf = (unsigned char*)pFileBuf;

	return true;
}
