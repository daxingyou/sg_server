#ifndef __COMMON_MSG_REPLY_CODE_HPP__
#define __COMMON_MSG_REPLY_CODE_HPP__

namespace msg_reply_code
{
    enum CHANNEL_CHECK : uint32_t
    {
        CHANNEL_CHECK_SUCCESS = 0,
        CHANNEL_CHECK_FAILED,
    };

    enum LC_LOGIN_REPLY : uint32_t
    {
        LC_LOGIN_REPLY_SUCCESS  = 0,            //成功
        LC_LOGIN_REPLY_FAILED_CHANNEL_CHECK,    //渠道验证失败
        LC_LOGIN_REPLY_FAILED_GATE_DATA,        //获取gateway信息失败
        LC_LOGIN_REPLY_FAILED_USER_NUM_MAX,     //人数已达上限
        LC_LOGIN_REPLY_FAILED_UNKNOW,           //未知错误
    };

    enum AL_LOGIN_REPLY : uint32_t
    {
        AL_LOGIN_REPLY_SUCCESS  = 0,            //成功
        AL_LOGIN_REPLY_FAILED_ADD_USER,
        AL_LOGIN_REPLY_FAILED_UNKNOW,           //未知错误
    };

    enum AC_CONNECT_REPLY : uint32_t
    {
        AC_CONNECT_REPLY_SUCCESS  = 0,              //成功
        AC_CONNECT_REPLY_FAILED_GUID_INVALID,       //guid无效
        AC_CONNECT_REPLY_FAILED_USER_NOT_EXIST,     //user不存在
        AC_CONNECT_REPLY_FAILED_UNKNOW,             //未知错误
    };
}

#endif