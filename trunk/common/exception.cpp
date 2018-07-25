#include "exception.hpp"


NS_COMMON_BEGIN

exception_t::exception_t(std::string const& desc, const char* filename, int32_t line)
    : m_filename(filename)
    , m_line(line)
    , m_oss(new std::ostringstream())
{
    format_header(desc);
}

void exception_t::format_header(std::string const& desc)
{
    (*m_oss) << desc << " in " << m_filename << ':' << m_line << '\n';
}

NS_COMMON_END
