#include "assert.hpp"
#include "log.hpp"


NS_COMMON_BEGIN

void assert_t::assert_ex(std::string const& desc, const char* filename, int line)
{
    exception_t ex(desc, filename, line);
#if defined(_DEBUG)
#if defined(COMMON_PLATFORM_WIN32)
    COMMON_ASSERT(false);
#elif defined(COMMON_PLATFORM_LINUX)
    __assert(ex.what(), ex.file(), ex.line());
#endif
#else
    log_error("assert:%s", ex.what());
#endif
}

NS_COMMON_END
