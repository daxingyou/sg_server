#include "message.hpp"
#include "log.hpp"
#include "cmd_desc.hpp"
#include <zlib.h>

NS_NETWORK_BEGIN
USING_NS_COMMON;

int32_t message_parser_t::parse(char* _data, const uint32_t bytes)
{
	int32_t rev_size = 0;

	char* data = _data;
	uint32_t left_len = bytes;
	uint32_t tmp = 0;
	while (left_len > 0)
	{
		if (!is_head_readed())
		{
			tmp = read_head(data, left_len);
			left_len -= tmp;
			data += tmp;
			if (is_head_readed())
			{
				const char* cmd_desc = CMD_DESC(head_.cmd);
				if (NULL == cmd_desc)
				{
					log_error("cmd[%d] is invalid !", head_.cmd);
					return -1;
				}

				if (head_.len > MAX_TCP_DATA_SIZE)
				{
					log_error("recv cmd[%s:%d] size[%d] > recv_max_size[%d]", cmd_desc, head_.cmd, head_.len, MAX_TCP_DATA_SIZE);
					// return -1;
				}

				body_.reset(new msg_buf_t(head_.len));
			} 
			else
			{
				break;
			}
		}

		if (NULL == body_)
		{
			log_error("recv cmd[%s:%d] body is NULL!", CMD_DESC(head_.cmd), head_.cmd);
			return -1;
		}

		tmp = read_body(data, left_len);
		left_len -= tmp;
		data += tmp;

		if (is_body_readed())
		{
			if (!(head_.flags & msg_head_t::flag_compress))
			{ // no compress
				packs_.push_back(message_t(head_, body_));
				rev_size++;
			} 
			else
			{ // do uncompress
				char uzBuf[MAX_UZLIB_CHAR];
				bzero(uzBuf, sizeof(uzBuf));
				uLongf bufSize = MAX_UZLIB_CHAR;
				int32_t retcode = uncompress((Bytef*)uzBuf, &bufSize, (const Bytef*)body_->data(), body_->size());
				if (retcode == Z_OK)
				{
					//log_info("cmd[%s:%d] size=%u uzsize=%u", CMD_DESC(head_.cmd), head_.cmd, body_->size(), bufSize);
					head_.len = bufSize;
					body_->reset(uzBuf, bufSize);
					packs_.push_back(message_t(head_, body_));
					rev_size++;
				} 
				else
				{
					log_error("cmd[%s:%d] compress error, reason:%d", CMD_DESC(head_.cmd), head_.cmd, retcode);
					return -1;
				}
			}

			read_size_ = 0;
		}
	}

	return rev_size;
}

uint32_t message_parser_t::read_head(const char* data, uint32_t size)
{
	uint32_t app = PH_LEN - read_size_;
	if (app > size)
		app = size;

	char* dst = (char*)&head_ + read_size_;
	std::char_traits<char>::copy(dst, data, app);
	read_size_ += app;
	return app;
}

uint32_t message_parser_t::read_body(const char* data, uint32_t size)
{
	uint32_t app = head_.len + PH_LEN - read_size_;
	if (app > size)
		app = size;

	uint32_t cursor = read_size_ - PH_LEN;
	std::char_traits<char>::copy(body_->data() + cursor, data, app);
	read_size_ += app;
	return app;
}

NS_NETWORK_END
