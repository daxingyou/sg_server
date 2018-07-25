#ifndef __RHEA_UTIL_DATA_EXCEPTION_H__
#define __RHEA_UTIL_DATA_EXCEPTION_H__

#include <string>
#define UTILS_DATA_EXCEPTION(dataName, key) \
	Utils::DataException(__FILE__, __LINE__, __FUNCTION__, dataName, key)

#define UTILS_DEKEY_DATA_EXCEPTION(dataName, key1, key2) \
	Utils::DataException(__FILE__, __LINE__, __FUNCTION__, dataName, key1, key2)

namespace Utils{

class  DataException
{
public:
	DataException(const std::string& file, int line, const std::string& func, 
		const std::string& dataName, const std::string& key);
	DataException(const std::string& file, int line, const std::string& func, 
		const std::string& dataName, const std::string& key, int key2);
private:
	std::string GenKeyDesc(const std::string& dataName, const std::string& key);
	std::string GenKeyDesc(const std::string& dataName, const std::string& key, int key2);
};
}

#endif
