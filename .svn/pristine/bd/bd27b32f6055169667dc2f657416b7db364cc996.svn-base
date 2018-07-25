#ifndef __WEB_CURL_WRAPPER_HPP__
#define __WEB_CURL_WRAPPER_HPP__

#include <curl/curl.h>
#include <boost/shared_ptr.hpp>

class web_curl_wapper_t 
{
public:
	web_curl_wapper_t() {}
	~web_curl_wapper_t() {}
		
	bool init(const std::string &url);
	void clean();
	
	std::string get_http_request();

private:
	CURL* m_curl = NULL;
};

typedef boost::shared_ptr<web_curl_wapper_t> web_curl_wapper_ptr;

#endif