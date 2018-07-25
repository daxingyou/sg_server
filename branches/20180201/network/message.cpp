#include "message.hpp"
#include "../common/log.hpp"
#include "cmd_desc.hpp"
#include "protobuf.hpp"

NS_NETWORK_BEGIN
USING_NS_COMMON;

bool message_parser_t::parse(char* _data, size_t bytes)
{
    bool ret = false;

    char* data = _data;

    size_t left_len = bytes;
    size_t tmp = 0;
    while (left_len > 0)
    {
        if (!is_head_readed())
        {
            tmp = read_head(data, left_len);
            left_len -= tmp;
            data += tmp;
            if (is_head_readed())
            {
                //const char* cmd_desc = CMD_DESC(head_.cmd);
                //if (NULL == cmd_desc)
                //{
                //    log_error("cmd[%s:%d] is invalid !", cmd_desc, head_.cmd);
                //    return false;
                //}
				if (head_.len > MAX_TCP_DATA_SIZE)
				{
					log_warn("recv cmd[%s:%d] size[%d] > recv_max_size[%d]", CMD_DESC(head_.cmd), head_.cmd, head_.len, MAX_TCP_DATA_SIZE);
					//return false;
				}
                 //if (head_.len > MAX_TCP_DATA_SIZE / 2)
                 //{
                 //    log_warn("recv cmd[%s:%d] size[%d] > recv_half_max_size[%d]", CMD_DESC(head_.cmd), head_.cmd, head_.len, MAX_TCP_DATA_SIZE / 2);
                 //}
                body_.reset(new msg_buf_t(head_.len));
            }
            else
            {
                break;
            }
        }

        if (NULL == body_)
        {
            return false;
        }
        
        tmp = read_body(data, left_len);
        left_len -= tmp;
        data += tmp;

        if (is_body_readed())
        {
			if (!(head_.flags & msg_head_t::flag_compress)) { // no compress
				packs_.push_back(message_t(head_, body_));
			} else { // do uncompress
				char uzBuf[MAX_UZLIB_CHAR];
				bzero(uzBuf, sizeof(uzBuf));
				uLongf bufSize = MAX_UZLIB_CHAR;
				int32_t retcode = uncompress((unsigned char *)uzBuf, &bufSize, (Bytef *)body_->data(), body_->size());
				if (retcode == Z_OK) {
					//log_info("cmd[%s:%d] size=%u uzsize=%u", CMD_DESC(head_.cmd), head_.cmd, body_->size(), bufSize);
					head_.len = bufSize;
					body_->reset(uzBuf, bufSize);
					packs_.push_back(message_t(head_, body_));
				} else {
					log_error("cmd[%s:%d] compress error, reason:%d", CMD_DESC(head_.cmd), head_.cmd, retcode);
				}
			}

            ret = true;
            read_size_ = 0;
        }
    }

    return ret;
}

size_t message_parser_t::read_head(const char* data, size_t size)
{
    size_t app = sizeof(head_) - read_size_;
    if (app > size)
        app = size;

    char* dst = (char*)&head_ + read_size_;
    std::char_traits<char>::copy(dst, data, app);
    read_size_ += app;
    return app;
}

size_t message_parser_t::read_body(const char* data, size_t size)
{
    size_t app = head_.len + sizeof(head_) - read_size_;
    if (app > size)
        app = size;

    size_t cursor = read_size_ - sizeof(head_);
    std::char_traits<char>::copy(body_->data()+cursor, data, app);
    read_size_ += app;
    return app;
}

NS_NETWORK_END
