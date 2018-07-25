#ifndef _COMMON_MACROS_HPP_

#define _COMMON_MACROS_HPP_

#include <boost/lexical_cast.hpp>

// platform-------------------------------------------------------
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define COMMON_PLATFORM_WIN32 "win32"
#define COMMON_PLATFORM COMMON_PLATFORM_WIN32
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__)
#define COMMON_PLATFORM_LINUX "linux"
#define COMMON_PLATFORM COMMON_PLATFORM_LINUX
#endif

// type---------------------------------------------------------------
#if defined(COMMON_PLATFORM_WIN32)
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
#elif defined(COMMON_PLATFORM_LINUX)
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long int64_t;
typedef long unsigned int uint64_t;
#endif

// format---------------------------------------------------------------
#if defined(COMMON_PLATFORM_WIN32)
#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"
#define snprintf _snprintf
#define atoll __atoi64
#elif defined(COMMON_PLATFORM_LINUX)
#define stricmp strcasecmp
#define strnicmp strncasecmp
#define I64FMT "%016llX"
#define I64FMTD "%llu"
#define SI64FMTD "%lld"
#endif

// safe delete----------------------------------------------------------
#define SAFE_DELETE_P(p) if(NULL != p){ delete (p); (p) = NULL; }
#define SAFE_DELETE_VEC(x) { if (x) { delete [] (x); (x) = NULL; } }

#define COMMON_TOSTRING(s) #s

#define ENUM_NAME(value)   #value

#define NS_SERVER_BEGIN namespace server {
#define NS_SERVER_END }
#define NS_NETWORK_BEGIN namespace network {
#define NS_NETWORK_END }
#define NS_COMMON_BEGIN namespace common {
#define NS_COMMON_END }

#define USING_NS_SERVER using namespace server
#define USING_NS_NETWORK using namespace network
#define USING_NS_COMMON using namespace common
#define USING_NS_STRING_UTIL using namespace string_util
#define USING_NS_CONFIG using namespace Hades::Data

#define IF_PTR_NULL_RETURN(p) if (NULL == (p)) { return; }
#define IF_PTR_NULL_RETURN_FALSE(p) if (NULL == (p)) { return false; }
#define IF_PTR_NULL_RETURN_TRUE(p) if (NULL == (p)) { return true; }
#define IF_BOOL_FALSE_RETURN_FALSE(b) if (!(b)) { return false; }
#define IF_INT_0_RETURN_FALSE(i) if (0 == (i)) { return false; }

#define FIF(val) float(int(val))

#define MAKEUINT32(a, b)      (( ((uint32_t)(a)) & 0xffff)	| (( ((uint32_t)(b)) & 0xffff) << 16))
#define MAKEUINT64(a, b)		(( ((uint64_t)(a)) & 0xFFFFFFFF)	| (( ((uint64_t)(b)) & 0xFFFFFFFF) << 32 ))

#define CHECK_PORT(port) if ((port) <= 1024 || (port) >= 65535) { printf("\033[31mport[%d] error!\033[0m\n", (port)); return false; }

// 64位的位操作
// pos的取值范围是0-63
#define CT_BIT_SET_64(tar_value, pos)      (tar_value |= ((uint64_t)1 << pos))        // 把第pos位置1
#define CT_BIT_CLR_64(tar_value, pos)      (tar_value &= ~((uint64_t)1 << pos))       // 把第pos位置0
#define CT_BIT_CHECK_64(tar_value, pos)     ((tar_value & ((uint64_t)1 << pos)) != 0)  // 判断第pos是不是1

// 32位的位操作
// pos的取值范围是0-31
#define CT_BIT_SET_32(tar_value, pos)      (tar_value |= ((uint32_t)1 << pos))        // 把第pos位置1
#define CT_BIT_CLR_32(tar_value, pos)      (tar_value &= ~((uint32_t)1 << pos))       // 把第pos位置0
#define CT_BIT_CHECK_32(tar_value, pos)     ((tar_value & ((uint32_t)1 << pos)) != 0)  // 判断第pos是不是1


// 浮点型边界值
#define FLOAT_EPSILON      1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0

// 存盘标记 db用 wsy add 2016.10.19
enum save_flag_type
{
	save_flag_type_not_update = 0,
	save_flag_type_time_update = 1,
	save_flag_type_right_now = 2
};

#define FUNC_CHECK_START try {

#define FUNC_CHECK_END      }\
catch (std::exception& ec) \
{ \
log_error("throw exception:%s", ec.what()); \
}

#endif
