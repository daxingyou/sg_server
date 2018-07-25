#ifndef __ACCOUNT_CURL_WRAPPER_HPP__
#define __ACCOUNT_CURL_WRAPPER_HPP__

#include <curl/curl.h>
#include <boost/shared_ptr.hpp>

class curl_warpper_t
{
public:
    curl_warpper_t() {}
    ~curl_warpper_t() {}

    bool init(const std::string& url);
    void clean();

    std::string post_http_request(const std::string& post_fields);
    
private:
    std::string parse_respone_data(const std::string& result);

private:
    CURL* m_curl = NULL;
};
typedef boost::shared_ptr<curl_warpper_t> curl_warpper_ptr;

#endif
