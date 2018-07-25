#include "data_exception.h"
#include <sstream>

namespace Utils{
DataException::DataException(const std::string& file, int line, const std::string& func, 
	const std::string& dataName, const std::string& key)
{
}

DataException::DataException(const std::string& file, int line, const std::string& func, 
	const std::string& dataName, const std::string& key, int key2)
{
}

std::string DataException::GenKeyDesc(const std::string& dataName, const std::string& key)
{
	std::ostringstream oss;
	oss << "Get " << dataName << "(" << key << ") failed!";
	return oss.str();
}

std::string DataException::GenKeyDesc(const std::string& dataName, const std::string& key, int key2)
{
	std::ostringstream oss;
	oss << "Get " << dataName << "(" << key << ", " << key2 << ") failed!";
	return oss.str();
}
}
