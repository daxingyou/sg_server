#ifndef __GM_CURL_WRAPPER_HPP__
#define __GM_CURL_WRAPPER_HPP__

#include <curl/curl.h>
#include <boost/shared_ptr.hpp>

class gm_curl_wapper_t 
{
public:
	gm_curl_wapper_t() {}
	~gm_curl_wapper_t() {}
		
	bool init(const std::string &url);
	void clean();

public:	
	std::string get_http_request();

private:
	CURL* m_curl = NULL;
};

typedef boost::shared_ptr<gm_curl_wapper_t> gm_curl_wapper_ptr;

#endif