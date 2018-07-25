#include "http_request_parser.hpp"
#include "utility.hpp"

NS_COMMON_BEGIN

std::string http_request_parser_t::s_content_length_name = "Content-Length";

http_request_parser_t::http_request_parser_t()
    : m_state(method_start)
{
}

void http_request_parser_t::reset()
{
    m_content_length = 0;
    m_state = method_start;
}

boost::tribool http_request_parser_t::consume(http_request_t& req, char input)
{
    switch (m_state)
    {
    case method_start:
        if (!is_char(input) || is_ctl(input) || is_tspecial(input))
        {
            return false;
        }
        else
        {
            m_state = method;
            req.m_method.push_back(input);
            return boost::indeterminate;
        }
    case method:
        if (input == ' ')
        {
            m_state = uri;
            return boost::indeterminate;
        }
        else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
        {
            return false;
        }
        else
        {
            req.m_method.push_back(input);
            return boost::indeterminate;
        }
    case uri_start:
        if (is_ctl(input))
        {
            return false;
        }
        else
        {
            m_state = uri;
            req.m_uri.push_back(input);
            return boost::indeterminate;
        }
    case uri:
        if (input == ' ')
        {
            m_state = http_version_h;
            return boost::indeterminate;
        }
        else if (is_ctl(input))
        {
            return false;
        }
        else
        {
            req.m_uri.push_back(input);
            return boost::indeterminate;
        }
    case http_version_h:
        if (input == 'H')
        {
            m_state = http_version_t_1;
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case http_version_t_1:
        if (input == 'T')
        {
            m_state = http_version_t_2;
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case http_version_t_2:
        if (input == 'T')
        {
            m_state = http_version_p;
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case http_version_p:
        if (input == 'P')
        {
            m_state = http_version_slash;
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case http_version_slash:
        if (input == '/')
        {
            req.m_http_version_major = 0;
            req.m_http_version_minor = 0;
            m_state = http_version_major_start;
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case http_version_major_start:
        if (is_digit(input))
        {
            req.m_http_version_major = req.m_http_version_major * 10 + input - '0';
            m_state = http_version_major;
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case http_version_major:
        if (input == '.')
        {
            m_state = http_version_minor_start;
            return boost::indeterminate;
        }
        else if (is_digit(input))
        {
            req.m_http_version_major = req.m_http_version_major * 10 + input - '0';
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case http_version_minor_start:
        if (is_digit(input))
        {
            req.m_http_version_minor = req.m_http_version_minor * 10 + input - '0';
            m_state = http_version_minor;
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case http_version_minor:
        if (input == '\r')
        {
            m_state = expecting_newline_1;
            return boost::indeterminate;
        }
        else if (is_digit(input))
        {
            req.m_http_version_minor = req.m_http_version_minor * 10 + input - '0';
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case expecting_newline_1:
        if (input == '\n')
        {
            m_state = header_line_start;
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case header_line_start:
        if (input == '\r')
        {
            m_state = expecting_newline_3;
            return boost::indeterminate;
        }
        else if (!req.m_headers.empty() && (input == ' ' || input == '\t'))
        {
            m_state = header_lws;
            return boost::indeterminate;
        }
        else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
        {
            return false;
        }
        else
        {
            req.m_headers.push_back(http_request_header_t());
            req.m_headers.back().name.push_back(input);
            m_state = header_name;
            return boost::indeterminate;
        }
    case header_lws:
        if (input == '\r')
        {
            m_state = expecting_newline_2;
            return boost::indeterminate;
        }
        else if (input == ' ' || input == '\t')
        {
            return boost::indeterminate;
        }
        else if (is_ctl(input))
        {
            return false;
        }
        else
        {
            m_state = header_value;
            req.m_headers.back().value.push_back(input);
            return boost::indeterminate;
        }
    case header_name:
        if (input == ':')
        {
            m_state = space_before_header_value;
            return boost::indeterminate;
        }
        else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
        {
            return false;
        }
        else
        {
            req.m_headers.back().name.push_back(input);
            return boost::indeterminate;
        }
    case space_before_header_value:
        if (input == ' ')
        {
            m_state = header_value;
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case header_value:
        if (input == '\r')
        {
            m_state = expecting_newline_2;
            return boost::indeterminate;
        }
        else if (is_ctl(input))
        {
            return false;
        }
        else
        {
            req.m_headers.back().value.push_back(input);
            return boost::indeterminate;
        }
    case expecting_newline_2:
        if (input == '\n')
        {
            m_state = header_line_start;
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case expecting_newline_3:
        if (input == '\n')
        {
            bool is_ok = false;
            uint32_t len = 0;
            boost::tie(is_ok, len) = parse_content_length_from_header(req.m_headers);

            m_content_length = 0;
            if (!is_ok)
            {
                return false;
            }

            if (len == 0 || len >= MAX_HTTP_REQUEST_LENGTH)
            {
                return false;
            }
            m_content_length = len;

            m_state = content;
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case content:
        // Content.
        while (req.m_content.size() < m_content_length)
        {
            req.m_content.push_back(input);
            return boost::indeterminate;
        }
        if (input == '\0' || input == '\n')
        {
            m_state = content_end;
            return boost::indeterminate;
        }
        else
        {
            return false;
        }
    case content_end:
        {
            // If here, check length
            if (m_content_length == req.m_content.size())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    default:
        return false;
    }
}

bool http_request_parser_t::is_char(int32_t c)
{
    return c >= 0 && c <= 127;
}

bool http_request_parser_t::is_ctl(int32_t c)
{
    return (c >= 0 && c <= 31) || c == 127;
}

bool http_request_parser_t::is_tspecial(int32_t c)
{
    switch (c)
    {
    case '(': case ')': case '<': case '>': case '@':
    case ',': case ';': case ':': case '\\': case '"':
    case '/': case '[': case ']': case '?': case '=':
    case '{': case '}': case ' ': case '\t':
        return true;
    default:
        return false;
    }
}

bool http_request_parser_t::is_digit(int32_t c)
{
    return c >= '0' && c <= '9';
}

bool http_request_parser_t::tolower_compare(char a, char b)
{
    return std::tolower(a) == std::tolower(b);
}

bool http_request_parser_t::headers_equal(const std::string& a, const std::string& b)
{
    if (a.length() != b.length())
        return false;

    return std::equal(a.begin(), a.end(), b.begin(),
        &http_request_parser_t::tolower_compare);
}

boost::tuple<bool, uint32_t> http_request_parser_t::parse_content_length_from_header( std::vector<http_request_header_t>& headers )
{
    bool is_ok = false;
    uint32_t ret_len = 0;
    for (std::size_t i = 0; i < headers.size(); ++i)
    {
        if (headers_equal(headers[i].name, s_content_length_name))
        {
            try
            {
                ret_len = boost::lexical_cast<uint32_t>(headers[i].value);
                is_ok = true;
                break;
            }
            catch (boost::bad_lexical_cast&)
            {
                log_error("parse_content_length_from_header error!");
            }
        }
    }
    return boost::make_tuple(is_ok, ret_len);
}

NS_COMMON_END