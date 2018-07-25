#ifndef __COMMON_HTTP_REQUEST_PARSER_HPP__
#define __COMMON_HTTP_REQUEST_PARSER_HPP__

#include "http_request.hpp"
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

NS_COMMON_BEGIN

struct http_request_t;

class http_request_parser_t
{
public:
    http_request_parser_t();

    void reset();

    /*
    * return true:  parse success, need handle then clear
    * return false: parse failed, need clear
    * return indeterminate: need continue to recv
    */
    template <typename InputIterator>
    boost::tuple<boost::tribool, InputIterator> parse(http_request_t& req, InputIterator begin, InputIterator end)
    {
		boost::tribool result = false;
        while (begin != end)
        {
            result = consume(req, *begin++);
            //if (result || !result) break;
			break;
        }

        result = false;
        if (m_content_length > 0 && req.m_content.size() == m_content_length)
        {
            result = true;
        }
        else if (m_content_length > 0 && req.m_content.size() < m_content_length)
        {
            result = boost::indeterminate;
        }

        return boost::make_tuple(result, begin);
    }

    boost::tuple<bool, unsigned int> parse_content_length_from_header(std::vector<http_request_header_t>& headers);

private:
    // Handle the next character of input.
    boost::tribool consume(http_request_t& req, char input);

    // Check if a byte is an HTTP character.
    static bool is_char(int32_t c);

    // Check if a byte is an HTTP control character.
    static bool is_ctl(int32_t c);

    // Check if a byte is defined as an HTTP tspecial character.
    static bool is_tspecial(int32_t c);

    // Check if a byte is a digit.
    static bool is_digit(int32_t c);

    // Check if two characters are equal, without regard to case.
    static bool tolower_compare(char a, char b);

    // Check whether the two request header names match.
    bool headers_equal(const std::string& a, const std::string& b);

private:
    // The name of the content length header.
    static std::string s_content_length_name;

    // Content length as decoded from headers. Defaults to 0.
    uint32_t m_content_length = 0;

    // The current state of the parser.
    enum state
    {
        method_start,
        method,
        uri_start,
        uri,
        http_version_h,
        http_version_t_1,
        http_version_t_2,
        http_version_p,
        http_version_slash,
        http_version_major_start,
        http_version_major,
        http_version_minor_start,
        http_version_minor,
        expecting_newline_1,
        header_line_start,
        header_lws,
        header_name,
        space_before_header_value,
        header_value,
        expecting_newline_2,
        expecting_newline_3,
        content,
        content_end,
    } m_state;
};

NS_COMMON_END

#endif

