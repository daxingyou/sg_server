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
        LC_LOGIN_REPLY_SUCCESS  = 0,            //�ɹ�
        LC_LOGIN_REPLY_FAILED_CHANNEL_CHECK,    //������֤ʧ��
        LC_LOGIN_REPLY_FAILED_GATE_DATA,        //��ȡgateway��Ϣʧ��
        LC_LOGIN_REPLY_FAILED_USER_NUM_MAX,     //�����Ѵ�����
        LC_LOGIN_REPLY_FAILED_UNKNOW,           //δ֪����
    };

    enum AL_LOGIN_REPLY : uint32_t
    {
        AL_LOGIN_REPLY_SUCCESS  = 0,            //�ɹ�
        AL_LOGIN_REPLY_FAILED_ADD_USER,
        AL_LOGIN_REPLY_FAILED_UNKNOW,           //δ֪����
    };

    enum AC_CONNECT_REPLY : uint32_t
    {
        AC_CONNECT_REPLY_SUCCESS  = 0,              //�ɹ�
        AC_CONNECT_REPLY_FAILED_GUID_INVALID,       //guid��Ч
        AC_CONNECT_REPLY_FAILED_USER_NOT_EXIST,     //user������
        AC_CONNECT_REPLY_FAILED_UNKNOW,             //δ֪����
    };
}

#endif