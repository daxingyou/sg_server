#include "curl_wrapper.hpp"
#include "log.hpp"

#define BOOST_SPIRIT_THREADSAFE 
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

USING_NS_COMMON;

//#define TEST_POST

#define CURL_TIMEOUT 5 //秒

//针对SSL，跳过验证证书和HOST
#define SKIP_SSL_VERIFICATION 

/**
* ptr是指向存储数据的指针;
* size是每个块的大小;
* nmemb是指块的数目;
* stream是用户参数;
* 所以根据以上这些参数的信息可以知道，ptr中的数据的总长度是(size * nmemb);
*/
size_t post_callback(const char* ptr, size_t size, size_t nmemb, std::string* stream)
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

bool curl_warpper_t::init(const std::string& url)
{
    //获得curl的一个句柄
    m_curl = curl_easy_init();
    if (NULL == m_curl)
    {
        log_error("NULL == m_curl");
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

    // 设置curl_easy_perform成功后打印出额外信息
    //curl_code = curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
    //if (CURLE_OK != curl_code)
    //{
    //    log_error("curl_easy_setopt CURLOPT_VERBOSE, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
    //    return false;
    //}

    // 在完成交互以后强迫断开连接，不能重用，防止太多的CLOSE_WAIT连接导致性能
    // 最好再修改一下TCP参数调低CLOSE_WAIT和TIME_WAIT的超时时间
    //curl_code = curl_easy_setopt(m_curl, CURLOPT_FORBID_REUSE, 1);
    //if (CURLE_OK != curl_code)
    //{
    //    log_error("curl_easy_setopt CURLOPT_FORBID_REUSE, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
    //    return false;
    //}

    // 若超过此时间curl_easy_perform会立即返回, 单位秒. 注意此时间为从调用curl_easy_perform开始后的总时间
    curl_code = curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, CURL_TIMEOUT);
    if (curl_code != CURLE_OK)
    {
        log_error("curl_easy_setopt CURLOPT_TIMEOUT, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
        return false;
    }

    // 设置url，例如http://192.168.1.51/xxxx/xxxxx
#ifdef TEST_POST
    curl_code = curl_easy_setopt(m_curl, CURLOPT_URL, "https://ysdktest.qq.com/mpay/get_balance_m ");
#else
    curl_code = curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
#endif
    if (CURLE_OK != curl_code)
    {
        log_error("curl_easy_setopt CURLOPT_URL[%s], error[%d:%s]", url.c_str(), curl_code, curl_easy_strerror(curl_code));
        return false;
    }

    // 如果是https，是否要跳过证书验证
    if (0 == strncmp(url.c_str(), "https://", 8))
    {
#ifdef SKIP_SSL_VERIFICATION
        curl_code = curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
        if (CURLE_OK != curl_code)
        {
            log_error("curl_easy_setopt CURLOPT_SSL_VERIFYPEER, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
            return false;
        }

        curl_code = curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
        if (CURLE_OK != curl_code)
        {
            log_error("curl_easy_setopt CURLOPT_SSL_VERIFYHOST, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
            return false;
        }
#else
        curl_code = curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 1L);
        if (CURLE_OK != curl_code)
        {
            log_error("curl_easy_setopt CURLOPT_SSL_VERIFYPEER, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
            return false;
        }

        curl_code = curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 1L);
        if (CURLE_OK != curl_code)
        {
            log_error("curl_easy_setopt CURLOPT_SSL_VERIFYHOST, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
            return false;
        }

        // TODO: 设置一个证书文件
        curl_code = curl_easy_setopt(curl, CURLOPT_CAINFO, "ca-cert.pem");
        if (CURLE_OK != curl_code)
        {
            log_error("curl_easy_setopt CURLOPT_CAINFO, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
            return false;
        }
#endif
    }

    // 设置为POST方法
    curl_code = curl_easy_setopt(m_curl, CURLOPT_POST, 1);
    if (CURLE_OK != curl_code)
    {
        log_error("curl_easy_setopt CURLOPT_POST, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
        return false;
    }

    return true;
}

void curl_warpper_t::clean()
{
    if (m_curl)
    {
        curl_easy_cleanup(m_curl);
    }
}

std::string curl_warpper_t::post_http_request(const std::string& post_fields)
{
    bool rlt = false;
    std::string result = "";

    do 
    {
        if (post_fields.empty())
        {
            log_error("post_fields is empty");
            break;
        }

        if (NULL == m_curl)
        {
            log_error("NULL == m_curl");
            break;
        }

        CURLcode curl_code;

        // 设置回调函数，接收的数据会在回调函数里处理
        curl_code = curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, post_callback);
        if (CURLE_OK != curl_code)
        {
            log_error("curl_easy_setopt CURLOPT_WRITEFUNCTION, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
            break;
        }

        // 设置接收数据的处理对象或变量
        curl_code = curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &result);
        if (CURLE_OK != curl_code)
        {
            log_error("curl_easy_setopt CURLOPT_WRITEDATA, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
            break;
        }

#ifdef TEST_POST
        // 设置post data
        curl_code = curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, "test");
        if (CURLE_OK != curl_code)
        {
            log_error("curl_easy_setopt CURLOPT_POSTFIELDS[%s], error[%d:%s]", post_fields.c_str(), curl_code, curl_easy_strerror(curl_code));
            break;
        }
#else
        // 设置post data长度
        curl_code = curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, post_fields.size());
        if (CURLE_OK != curl_code)
        {
            log_error("curl_easy_setopt CURLOPT_POSTFIELDSIZE[%d], error[%d:%s]", (int32_t)post_fields.size(), curl_code, curl_easy_strerror(curl_code));
            break;
        }

        // 设置post data
        curl_code = curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, post_fields.c_str());
        //curl_code = curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, "test");
        if (CURLE_OK != curl_code)
        {
            log_error("curl_easy_setopt CURLOPT_POSTFIELDS[%s], error[%d:%s]", post_fields.c_str(), curl_code, curl_easy_strerror(curl_code));
            break;
        }
#endif
        
        curl_code = curl_easy_perform(m_curl);
        if (CURLE_OK != curl_code)
        {
            log_error("curl_easy_perform, error[%d:%s]", curl_code, curl_easy_strerror(curl_code));
            break;
        }
        log_warn("post result: %s", result.c_str());

        rlt = true;

    } while (0);

    std::string guid = "";
    if (rlt)
    {
        guid = parse_respone_data(result);
    }
    return guid;
}

std::string curl_warpper_t::parse_respone_data(const std::string& result)
{
    std::string guid = "";

    try
    {
        if (result.empty())
        {
            log_error("result is empty");
            return guid;
        }

        std::stringstream ss(result);
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(ss, pt);

#ifdef TEST_POST
        std::string msg = pt.get<std::string>("msg");
        std::string ret = pt.get<std::string>("ret");
        log_warn("msg[%s] ret[%s]", msg.c_str(), ret.c_str());
#else
        std::string status = pt.get<std::string>("status");
        std::string message = pt.get<std::string>("message");
		//int32_t code =	 pt.get<int32_t>("message.code");

        if ( "success" == status )
        {
            guid = pt.get<std::string>("uid");
        }
        else
        {
            log_error("channel login failed: message[%s]", message.c_str());
        }
#endif
    }
    catch (boost::property_tree::ptree_error& e)
    {
        log_error("parse json[%s] failed, error[%s]", result.c_str(), e.what());
    }

    return guid;
}