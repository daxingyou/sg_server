#ifndef __COMMON_HTTP_REQUEST_HPP__
#define __COMMON_HTTP_REQUEST_HPP__

#include "macros.hpp"
#include "log.hpp"

NS_COMMON_BEGIN

#define MAX_HTTP_REQUEST_LENGTH 1024

struct http_request_header_t
{
    std::string name;
    std::string value;
};

// A request received from a platform.
struct http_request_t
{
    http_request_t()
    {
        reset();
    }

    void reset()
    {
        m_method.clear();
        m_uri.clear();
        m_http_version_major = 0;
        m_http_version_minor = 0;
        m_headers.clear();
        m_content.clear();
    }

    // The request method, e.g. "GET", "POST".
    std::string m_method;

    // The requested URI, such as a path to a file.
    std::string m_uri;

    // Major version number, usually 1.
    int32_t m_http_version_major;

    // Minor version number, usually 0 or 1.
    int32_t m_http_version_minor;

    // The headers included with the request.
    std::vector<http_request_header_t> m_headers;

    // The optional content sent with the request.
    std::string m_content;
};

NS_COMMON_END

#endif
