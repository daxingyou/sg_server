#ifndef __NETWORK_PROTOBUF_HPP_
#define __NETWORK_PROTOBUF_HPP_

#include <google/protobuf/io/coded_stream.h> 
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include "tcp_socket.hpp"
#include "log.hpp"
#include "cmd_desc.hpp"
#include <zlib.h>


NS_NETWORK_BEGIN

// typedef google::protobuf::io::CodedOutputStream coded_output_stream_t;
// typedef google::protobuf::io::ArrayOutputStream array_output_stream_t;
// typedef google::protobuf::io::CodedInputStream coded_input_stream_t;

static const uint32_t MAX_UZLIB_CHAR = 400 * 1024;							/**< 未压缩数据缓存 */
static const uint32_t MAX_TCP_PACKET_SIZE = 64 * 1024 - 1;					/**< TCP包最大数据包长度 */
static const uint32_t PH_LEN = sizeof(msg_head_t);							/**< 数据包头 */
static const uint32_t UID_MASK_LEN = sizeof(uint64_t);						/**< 数据UID长度 */
static const uint32_t MAX_TCP_DATA_SIZE = MAX_TCP_PACKET_SIZE - PH_LEN;		/**< 数据包最大长度 */
static const uint32_t PACKET_ZIP_MIN = 1024;								/**< 数据包压缩最小大小 */
#define unzip_size(zip_size) ((zip_size) * 120 / 100 + 12)					/**< 压缩需要的缓存 > compressBound */

// for message
template<typename T_MSG>
static msg_buf_ptr wrap_msg(uint16_t cmd, uint64_t uid, const T_MSG& msg)
{
	uint32_t body_sz = msg.ByteSize();
	if (PH_LEN + UID_MASK_LEN + body_sz <= MAX_TCP_PACKET_SIZE) {
		msg_buf_ptr out(new msg_buf_t(PH_LEN + UID_MASK_LEN + body_sz));
		char* p = out->begin();
		msg_head_t* head = new (out->data()) msg_head_t();//placement new;
		head->cmd = cmd;
		head->len = UID_MASK_LEN + body_sz;
		head->flags = msg_head_t::flag_uid;
		p += PH_LEN;

		*((uint64_t*)p) = uid;
		p += UID_MASK_LEN;

		msg.SerializeToArray(p, body_sz);
		return out;
	} else {
		if (PH_LEN + UID_MASK_LEN + body_sz >= MAX_UZLIB_CHAR) {
			log_error("user[%lu] cmd[%s:%d] msg size[%u] >= MAX_UZLIB_CHAR", uid, CMD_DESC(cmd), cmd, PH_LEN + UID_MASK_LEN + body_sz);
			return NULL;
		}

		char unBuf[MAX_UZLIB_CHAR];
		bzero(unBuf, sizeof(unBuf));
		*((uint64_t*)unBuf) = uid;
		uint32_t uzSize = UID_MASK_LEN;
		msg.SerializeToArray(unBuf + uzSize, body_sz);
		uzSize += body_sz;

		uLong destLen = unzip_size(uzSize);
		char zlibBuf[destLen + PH_LEN];
		char* p = zlibBuf;
		msg_head_t* head = new (p) msg_head_t();//placement new;
		head->cmd = cmd;
		head->flags = msg_head_t::flag_uid;
		head->flags |= msg_head_t::flag_compress; // 压缩标记
		p += PH_LEN;

		int32_t retcode = compress((unsigned char *)p, &destLen, (const Bytef *)unBuf, uzSize);
		if (retcode != Z_OK) {
			log_error("user[%lu] cmd[%s:%d] compress error, reason:%d", uid, CMD_DESC(cmd), cmd, retcode);
			return NULL;
		}

		head->len = destLen;
		// log_info("user[%lu] cmd[%s:%d] uzsize=%u size=%u", uid, CMD_DESC(cmd), cmd, uzSize, destLen);
		if (destLen > MAX_TCP_DATA_SIZE) log_warn("user[%lu] cmd[%s:%d] uzsize=%u size=%u > MAX_TCP_DATA_SIZE", uid, CMD_DESC(cmd), cmd, uzSize, destLen);

		msg_buf_ptr out(new msg_buf_t(zlibBuf, PH_LEN + destLen));
		return out;
	}
}

template<typename T_MSG>
static msg_buf_ptr wrap_msg(uint16_t cmd, const T_MSG& msg)
{
	uint32_t body_sz = msg.ByteSize();
	if (PH_LEN + body_sz <= MAX_TCP_PACKET_SIZE) {
		msg_buf_ptr out(new msg_buf_t(PH_LEN + body_sz));
		char* p = out->begin();
		msg_head_t* head = new (out->data()) msg_head_t();//placement new;
		head->cmd = cmd;
		head->len = body_sz;
		head->flags = 0;
		p += PH_LEN;

		msg.SerializeToArray(p, body_sz);
		return out;
	} else {
		if (PH_LEN + body_sz >= MAX_UZLIB_CHAR) {
			log_error("cmd[%s:%d] msg size[%u] >= MAX_UZLIB_CHAR", CMD_DESC(cmd), cmd, PH_LEN + body_sz);
			return NULL;
		}

		char unBuf[MAX_UZLIB_CHAR];
		bzero(unBuf, sizeof(unBuf));
		msg.SerializeToArray(unBuf, body_sz);
		uint32_t uzSize = body_sz;

		uLong destLen = unzip_size(uzSize);
		char zlibBuf[destLen + PH_LEN];
		char* p = zlibBuf;
		msg_head_t* head = new (p) msg_head_t();//placement new;
		head->cmd = cmd;
		head->flags = msg_head_t::flag_compress; // 压缩标记
		p += PH_LEN;

		int32_t retcode = compress((unsigned char *)p, &destLen, (const Bytef *)unBuf, uzSize);
		if (retcode != Z_OK) {
			log_error("cmd[%s:%d] compress error, reason:%d", CMD_DESC(cmd), cmd, retcode);
			return NULL;
		}

		head->len = destLen;
		// log_info("cmd[%s:%d] uzsize=%u size=%u", CMD_DESC(cmd), cmd, uzSize, destLen);
		if (destLen > MAX_TCP_DATA_SIZE) log_warn("cmd[%s:%d] uzsize=%u size=%u > MAX_TCP_DATA_SIZE", CMD_DESC(cmd), cmd, uzSize, destLen);

		msg_buf_ptr out(new msg_buf_t(zlibBuf, PH_LEN + destLen));
		return out;
	}
}

// client->server no compress-------------------------------------------------------------------
template<typename T_BUF>
static msg_buf_ptr re_wrap_msg_to_server(uint16_t cmd, uint64_t uid, const T_BUF& buf)
{
	uint32_t body_sz = buf->size();
	if (PH_LEN + UID_MASK_LEN + body_sz > MAX_TCP_PACKET_SIZE) {
		log_error("user[%lu] cmd[%s:%d] from client size[%u] > MAX_TCP_PACKET_SIZE", uid, CMD_DESC(cmd), cmd, PH_LEN + UID_MASK_LEN + body_sz);
		return NULL;
	}

	msg_buf_ptr out(new msg_buf_t(PH_LEN + UID_MASK_LEN + body_sz));
	char* p = out->begin();
	msg_head_t* head = new (out->data()) msg_head_t();//placement new;
	head->cmd = cmd;
	head->len = UID_MASK_LEN + body_sz;
	head->flags = msg_head_t::flag_uid;
	p += PH_LEN;

	*((uint64_t*)p) = uid;
	p += UID_MASK_LEN;

	std::char_traits<char>::copy(p, buf->data(), body_sz);
	return out;
}

// --------------------------------------------------------------------------------------------------

// server->client------------------------------------------------------------------------------------
template<typename T_BUF>
static msg_buf_ptr re_wrap_buf_to_client(uint16_t cmd, const T_BUF& buf)
{
	uint32_t body_sz = buf->size() - UID_MASK_LEN;
#if 1 // 暂时把服务器去客户端的消息的压缩去掉
	if (PH_LEN + body_sz <= PACKET_ZIP_MIN) {
		msg_buf_ptr out(new msg_buf_t(PH_LEN + body_sz));
		char* p = out->begin();
		msg_head_t* head = new (out->data()) msg_head_t();//placement new;
		head->cmd = cmd;
		head->len = body_sz;
		head->flags = 0;
		p += PH_LEN;

		std::char_traits<char>::copy(p, buf->data() + UID_MASK_LEN, body_sz);
		return out;
	} else {
		if (PH_LEN + body_sz >= MAX_UZLIB_CHAR) {
			log_error("cmd[%s:%d] msg size[%u] >= MAX_UZLIB_CHAR", CMD_DESC(cmd), cmd, PH_LEN + body_sz);
			return NULL;
		}

		char* unBuf = buf->data() + UID_MASK_LEN;
		uLong destLen = unzip_size(body_sz);
		char zlibBuf[destLen + PH_LEN];
		char* p = zlibBuf;
		msg_head_t* head = new (p) msg_head_t();//placement new;
		head->cmd = cmd;
		head->flags = msg_head_t::flag_compress; // 压缩标记
		p += PH_LEN;

		int32_t retcode = compress((unsigned char *)p, &destLen, (const Bytef *)unBuf, body_sz);
		if (retcode != Z_OK) {
			log_error("cmd[%s:%d] compress error, reason:%d", CMD_DESC(cmd), cmd, retcode);
			return NULL;
		}

		head->len = destLen;
		// log_info("cmd[%s:%d] uzsize=%u size=%u", CMD_DESC(cmd), cmd, body_sz, destLen);
		if (destLen > MAX_TCP_DATA_SIZE) log_warn("cmd[%s:%d] uzsize=%u size=%u > MAX_TCP_DATA_SIZE", CMD_DESC(cmd), cmd, body_sz, destLen);

		msg_buf_ptr out(new msg_buf_t(zlibBuf, PH_LEN + destLen));
		return out;
	}
#else
	msg_buf_ptr out(new msg_buf_t(PH_LEN + body_sz));
	char* p = out->begin();
	msg_head_t* head = new (out->data()) msg_head_t();//placement new;
	head->cmd = cmd;
	head->len = body_sz;
	head->flags = 0;
	p += PH_LEN;

	std::char_traits<char>::copy(p, buf->data() + UID_MASK_LEN, body_sz);
	return out;
#endif
}

template<typename T_MSG>
static msg_buf_ptr wrap_msg_to_client(uint16_t cmd, const T_MSG& msg)
{
	uint32_t body_sz = msg.ByteSize();
#if 1 // 暂时把服务器去客户端的消息的压缩去掉
	if (PH_LEN + body_sz <= PACKET_ZIP_MIN) {
		msg_buf_ptr out(new msg_buf_t(PH_LEN + body_sz));
		char* p = out->begin();
		msg_head_t* head = new (out->data()) msg_head_t();//placement new;
		head->cmd = cmd;
		head->len = body_sz;
		head->flags = 0;
		p += PH_LEN;

		msg.SerializeToArray(p, body_sz);
		return out;
	} else {
		if (PH_LEN + body_sz >= MAX_UZLIB_CHAR) {
			log_error("cmd[%s:%d] msg size[%u] >= MAX_UZLIB_CHAR", CMD_DESC(cmd), cmd, PH_LEN + body_sz);
			return NULL;
		}

		char unBuf[MAX_UZLIB_CHAR];
		bzero(unBuf, sizeof(unBuf));
		msg.SerializeToArray(unBuf, body_sz);
		uint32_t uzSize = body_sz;

		uLong destLen = unzip_size(uzSize);
		char zlibBuf[destLen + PH_LEN];
		char* p = zlibBuf;
		msg_head_t* head = new (p) msg_head_t();//placement new;
		head->cmd = cmd;
		head->flags = msg_head_t::flag_compress; // 压缩标记
		p += PH_LEN;

		int32_t retcode = compress((unsigned char *)p, &destLen, (const Bytef *)unBuf, uzSize);
		if (retcode != Z_OK) {
			log_error("cmd[%s:%d] compress error, reason:%d", CMD_DESC(cmd), cmd, retcode);
			return NULL;
		}

		head->len = destLen;
		// log_info("cmd[%s:%d] uzsize=%u size=%u", CMD_DESC(cmd), cmd, uzSize, destLen);
		if (destLen > MAX_TCP_DATA_SIZE) log_warn("cmd[%s:%d] uzsize=%u size=%u > MAX_TCP_DATA_SIZE", CMD_DESC(cmd), cmd, uzSize, destLen);

		msg_buf_ptr out(new msg_buf_t(zlibBuf, PH_LEN + destLen));
		return out;
	}
#else
	msg_buf_ptr out(new msg_buf_t(PH_LEN + body_sz));
	char* p = out->begin();
	msg_head_t* head = new (out->data()) msg_head_t();//placement new;
	head->cmd = cmd;
	head->len = body_sz;
	head->flags = 0;
	p += PH_LEN;

	msg.SerializeToArray(p, body_sz);
	return out;
#endif
}

template<typename T_CMD>
static msg_buf_ptr wrap_msg_to_client(T_CMD& cmd)
{
	msg_buf_ptr out(new msg_buf_t(sizeof(msg_head_t)));
	// char* p = out->begin();

	msg_head_t* head = new (out->data()) msg_head_t();
	head->cmd = cmd;
	head->len = 0;
	// p += sizeof(msg_head_t);

	return out;
}

// --------------------------------------------------------------------------------------------------
template<typename T_BUF>
static msg_buf_ptr re_wrap_msg_s(uint16_t cmd, const T_BUF& buf)
{
	uint32_t body_sz = buf->size();
	if (PH_LEN + body_sz <= MAX_TCP_PACKET_SIZE) {
		msg_buf_ptr out(new msg_buf_t(PH_LEN + body_sz));
		char* p = out->begin();
		msg_head_t* head = new (out->data()) msg_head_t();//placement new;
		head->cmd = cmd;
		head->len = body_sz;
		head->flags = msg_head_t::flag_uid;
		p += PH_LEN;

		std::char_traits<char>::copy(p, buf->data(), body_sz);
		return out;
	} else {
		if (PH_LEN + body_sz >= MAX_UZLIB_CHAR) {
			log_error("cmd[%s:%d] msg size[%u] >= MAX_UZLIB_CHAR", CMD_DESC(cmd), cmd, PH_LEN + body_sz);
			return NULL;
		}

		char unBuf[MAX_UZLIB_CHAR];
		bzero(unBuf, sizeof(unBuf));
		std::char_traits<char>::copy(unBuf, buf->data(), body_sz);
		uint32_t uzSize = body_sz;

		uLong destLen = unzip_size(uzSize);
		char zlibBuf[destLen + PH_LEN];
		char* p = zlibBuf;
		msg_head_t* head = new (p) msg_head_t();//placement new;
		head->cmd = cmd;
		head->flags = msg_head_t::flag_uid;
		head->flags |= msg_head_t::flag_compress; // 压缩标记
		p += PH_LEN;

		int32_t retcode = compress((unsigned char *)p, &destLen, (const Bytef *)unBuf, uzSize);
		if (retcode != Z_OK) {
			log_error("cmd[%s:%d] compress error, reason:%d", CMD_DESC(cmd), cmd, retcode);
			return NULL;
		}

		head->len = destLen;
		// log_info("cmd[%s:%d] uzsize=%u size=%u", CMD_DESC(cmd), cmd, uzSize, destLen);
		if (destLen > MAX_TCP_DATA_SIZE) log_warn("cmd[%s:%d] uzsize=%u size=%u > MAX_TCP_DATA_SIZE", CMD_DESC(cmd), cmd, uzSize, destLen);

		msg_buf_ptr out(new msg_buf_t(zlibBuf, PH_LEN + destLen));
		return out;
	}
}

// for log send
template<typename T_MSG>
static void wrap_msg(std::string& out, T_MSG& msg)
{
    size_t body_sz = msg.ByteSize();
    out.resize(body_sz);
    char* p = &*out.begin();

	//modify by fangjian 2016/8/18
    msg.SerializeToArray(p, body_sz);
	//array_output_stream_t arr(p, body_sz);
    //coded_output_stream_t output(&arr);
    //msg.SerializeToCodedStream(&output);
}

//-----------------------------------------------------------------------------------------------
template<typename T_MSG>
static void unwrap_msg(T_MSG& msg, uint64_t& uid, const msg_buf_ptr& buf)
{
    uid = 0;
    if (buf->size() < sizeof(uint64_t))
    {
        return;
    }
    char* p = buf->data();
    uid = *((uint64_t*)p);
    p += sizeof(uint64_t);
	//modify by fangjian 2016/8/18
    msg.ParseFromArray(p, buf->size() - sizeof(uint64_t));

    //coded_input_stream_t input((const google::protobuf::uint8*)p, buf->size()-sizeof(uint64_t));
    //msg.ParseFromCodedStream(&input);
}

template<typename T_MSG>
static void unwrap_msg(T_MSG& msg, const msg_buf_ptr& buf)
{
	//modify by fangjian 2016/8/18
	msg.ParseFromArray(buf->data(), buf->size());
	//coded_input_stream_t input(reinterpret_cast<const uint8_t*>(buf->data()), buf->size());
    //msg.ParseFromCodedStream(&input);
}

template<typename T_MSG>
static void unwrap_msg(T_MSG& msg, std::string const& data)
{
	//modify by fangjian 2016/8/18
	msg.ParseFromArray(data.c_str(), data.length());
    //coded_input_stream_t input(reinterpret_cast<const uint8_t*>(data.data()), data.size());
    //msg.ParseFromCodedStream(&input);
}

//加密解密--------------------------------------------------------------------------------------------------------
#define SG_SERVER_TO_CLIENT_KEY "sg_sever_to_client_key"
#define SG_CLIENT_TO_SERVER_KEY "sg_client_to_sever_key"

static void encrypt (char* data, int32_t len, const char* key, int32_t begin_pos)
{
    if( NULL == data || NULL == key || len <= 0)
    {
        return;
    }
    char* p_buff = data;
    const char* p_key = key;
    int32_t KeyLen = int32_t(strlen(p_key));
    for (int32_t i = 0; i < (len); ++i)
    {
        *p_buff ^= p_key[(i+begin_pos)%KeyLen];
        p_buff++; 
    }
}

#define ENCRYPT(data, data_size, key) encrypt(data, data_size, key, 0)
#define DECRYPT(data, data_size, key) encrypt(data, data_size, key, 0)

// TODO：加密的没有做压缩解压的处理
template<typename T_MSG>
static msg_buf_ptr wrap_msg_encrypt(uint16_t cmd, const T_MSG& msg, const char* key)
{
    size_t body_size = msg.ByteSize();
    msg_buf_ptr out(new msg_buf_t(sizeof(msg_head_t) + body_size));
    char* p = out->begin();

    msg_head_t* head = new (out->data()) msg_head_t();//placement new;
    head->cmd = cmd;
    head->len = body_size;
    head->flags = 0;
    p += sizeof(msg_head_t);

    //array_output_stream_t arr(p, body_size);
    //coded_output_stream_t output(&arr);
    //msg.SerializeToCodedStream(&output);
	msg.SerializeToArray(p, body_size);
    char* body_data = p;

    ENCRYPT(body_data, body_size, key);

    return out;
}

template<typename T_CMD>
static msg_buf_ptr wrap_msg_encrypt(T_CMD& cmd, const char* key)
{
    size_t body_size = 0;
    msg_buf_ptr out(new msg_buf_t(sizeof(msg_head_t)));
    char* p = out->begin();

    msg_head_t* head = new (out->data()) msg_head_t();
    head->cmd = cmd;
    head->len = body_size;
	head->flags = 0;
    p += sizeof(msg_head_t);

    char* body_data = p;

    ENCRYPT(body_data, body_size, key);

    return out;
}

template<typename T_BUF>
static msg_buf_ptr re_wrap_msg_encrypt(uint16_t cmd, const T_BUF& buf, const char* key)
{
    size_t new_body_len = buf->size() - sizeof(uint64_t);
    msg_buf_ptr out(new msg_buf_t(new_body_len + sizeof(msg_head_t)));
    char* p = out->data();

    msg_head_t* head = new (out->data()) msg_head_t();
    head->cmd = cmd;
    head->len = new_body_len;
    head->flags = 0;
    p += sizeof(msg_head_t);

    char* body_data = buf->data() + sizeof(uint64_t);
    size_t body_size = new_body_len;

    ENCRYPT(body_data, body_size, key);

    std::char_traits<char>::copy(p, body_data, body_size);

    return out;
}

template<typename T_MSG>
static void unwrap_msg_decrypt(T_MSG& msg, const msg_buf_ptr& buf, const char* key)
{
    char* body_data = buf->data();
    size_t body_size = buf->size();

    DECRYPT(body_data, body_size, key);

	msg.ParseFromArray(body_data, body_size);
    //coded_input_stream_t input(reinterpret_cast<const uint8_t*>(body_data), body_size);
    //msg.ParseFromCodedStream(&input);
}

template<typename T_BUFF>
static void unwrap_msg_decrypt(const T_BUFF& buf, const char* key)
{
	char* body_data = buf->data();
	size_t body_size = buf->size();

	ENCRYPT(body_data, body_size, key);
}
//------------------------------------------------------------------------------------------------------------------
#define PRE_S2S_MSG(msg_type) \
    msg_type msg;\
    uint64_t uid = 0;\
    unwrap_msg(msg, uid, msg_buf)

#define PRE_S2S_MSG_NO_UID(msg_type) \
    msg_type msg;\
    unwrap_msg(msg, msg_buf)

#define PRE_S2LOG_MSG(msg_type) \
    msg_type msg;\
    network::unwrap_msg(msg, msg_buf)

#define PRE_S2ACCOUNT_MSG(msg_type) \
    msg_type msg;\
    network::unwrap_msg(msg, msg_buf)

#ifdef __ENCRYPT__
    #define PRE_C2S_MSG(msg_type) \
        msg_type msg;\
        unwrap_msg_decrypt(msg, msg_buf, SG_CLIENT_TO_SERVER_KEY)

    #define PRE_S2C_MSG(msg_type) \
        msg_type msg;\
        unwrap_msg_decrypt(msg, msg_buf, SG_SERVER_TO_CLIENT_KEY)
#else
    #define PRE_C2S_MSG(msg_type) \
        msg_type msg;\
        unwrap_msg(msg, msg_buf)

    #define PRE_S2C_MSG(msg_type) \
        msg_type msg;\
        unwrap_msg(msg, msg_buf)
#endif

NS_NETWORK_END
#endif 
