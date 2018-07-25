#ifndef _COMMON_EXCEPTION_HPP_
#define _COMMON_EXCEPTION_HPP_

#include "macros.hpp"
#include <boost/shared_ptr.hpp>
#include <memory>
#include <sstream>
#include <string>


NS_COMMON_BEGIN

class exception_t : public std::exception
{
public:
    exception_t(std::string const& desc, const char* filename, int32_t line);
    ~exception_t() throw() {}

    void format_header(std::string const& desc);
    const char* what() const throw() { return m_oss->str().c_str();  }
    const char* file() const { return m_filename;  }
    int32_t line() const { return m_line;  }

private:
    const char* m_filename;
    int32_t m_line;
    boost::shared_ptr<std::ostringstream> m_oss;
};


NS_COMMON_END

#endif
