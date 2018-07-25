#ifndef _COMMON_ASSERT_HPP_
#define _COMMON_ASSERT_HPP_

#include "macros.hpp"
#include <assert.h>
#include <sstream>

NS_COMMON_BEGIN
//wsy
class assert_t
{
public:
    static assert_t* get()
    {
        static assert_t g_as;
        return &g_as;
    }

    void assert_ex(std::string const& desc, const char* filename, int line);
};

NS_COMMON_END

#define COMMON_ASSERT(exp, ...) { common::assert_t::get()->assert_ex(#exp,  __FILE__, __LINE__); }


#define COMMON_ASSERT_EX(exp, format, ...) \
{ \
    log_error(format, ##__VA_ARGS__); \
    COMMON_ASSERT(exp); \
}

#endif
