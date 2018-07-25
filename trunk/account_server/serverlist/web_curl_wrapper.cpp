#include "web_curl_wrapper.hpp"
#include "log.hpp"

USING_NS_COMMON;

#define CURL_TIMEOUT 5 //秒

size_t get_callback(const char* ptr, size_t size, size_t nmemb, std::string* stream)
{
	size_t sz = size * nmemb;
	if (NULL == ptr)
	{
		log_error("NULL == ptr");
		return 0;
	}
	if (NULL == stream)
	{
		log_error("NULL == stream");
		return 0;
	}
	stream->append(ptr, sz);

	return sz;
}

bool web_curl_wapper_t::init(const std::string &url) 
{
	//获得curl的一个句柄
	m_curl = curl_easy_init();
	if (NULL == m_curl) 
	{
		log_error("NULL==m_curl");
		return false;
	}

	CURLcode curl_code;

	// 禁止访问超时的时候抛出超时信号
	curl_code = curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1L);
	if (CURLE_OK != curl_code)
	{
		log_error("curl_easy_setopt CURLOPT_NOSIGNAL, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
		return false;
	}
	
	// 若超过此时间curl_easy_perform会立即返回, 单位秒. 注意此时间为从调用curl_easy_perform开始后的总时间
	curl_code = curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, CURL_TIMEOUT);
	if (curl_code != CURLE_OK)
	{
		log_error("curl_easy_setopt CURLOPT_TIMEOUT, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
		return false;
	}

	// 设置url	
	curl_code = curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	if (CURLE_OK != curl_code) 
	{
		log_error("curl_easy_setopt CURLOPT_URL[%s], error[%d:%s]", url.c_str(), curl_code, curl_easy_strerror(curl_code));
		return false;
	}

	//设置回调函数
	curl_code = curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, get_callback);
	if (CURLE_OK != curl_code)
	{
		log_error("curl_easy_setopt CURLOPT_WRITEFUNCTION, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
		return false;
	}
	return true;
}

void web_curl_wapper_t::clean() 
{
	if (m_curl) 
	{
		curl_easy_cleanup(m_curl);
	}
}

std::string web_curl_wapper_t::get_http_request() 
{
	bool rlt = false;
	std::string result = "";

	do
	{
		if (NULL == m_curl)
		{
			log_error("NULL == m_curl");
			break;
		}

		CURLcode curl_code;

		// 设置接收数据的处理对象或变量
		curl_code = curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &result);
		if (CURLE_OK != curl_code)
		{
			log_error("curl_easy_setopt CURLOPT_WRITEDATA, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
			break;
		}

		// 执行
		curl_code = curl_easy_perform(m_curl);
		if (CURLE_OK != curl_code)
		{
			log_error("curl_easy_perform, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
			break;
		}

		//log_warn("get result success result: size: %d", result.size());
		rlt = true;
	} while (false);

	if (rlt == true)
	{
		return result;
	}
	return "";
}

