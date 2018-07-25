#ifndef __NETWORK_MESSAGE_HPP_
#define __NETWORK_MESSAGE_HPP_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string.h>

#include "command.hpp"

NS_NETWORK_BEGIN

const uint32_t TRUNK_SIZE = 64 * 1024;
const uint32_t MAX_UZLIB_CHAR = 400 * 1024;							/**< 未压缩数据缓存 */
const uint32_t MAX_TCP_PACKET_SIZE = TRUNK_SIZE - 1;				/**< 数据包掩码 */
const uint32_t PH_LEN = sizeof(msg_head_t);							/**< 数据包头 */
const uint32_t UID_MASK_LEN = sizeof(uint64_t);						/**< 数据UID长度 */
const uint32_t MAX_TCP_DATA_SIZE = MAX_TCP_PACKET_SIZE - PH_LEN;	/**< 数据包最大长度 */
const uint32_t PACKET_ZIP_MIN = 1024;								/**< 数据包压缩最小大小 */
#define unzip_size(zip_size) ((zip_size) * 120 / 100 + 12)			/**< 压缩需要的缓存 > compressBound */

class msg_buf_t
{
public:
    msg_buf_t()
        : data_(NULL)
        , size_(0)
    {
    }

    msg_buf_t(uint32_t sz)
        : data_(new char[sz])
        , size_(sz)
    {
    }

    msg_buf_t(const char* data, uint32_t sz)
		: data_(new char[sz])
		, size_(sz)
    {
		std::char_traits<char>::copy(data_, data, sz);
    }

    ~msg_buf_t()
    {
        if (data_) {
            delete[] data_;
            data_ = NULL;
            size_ = 0;
        }
    }

    void reset(uint32_t sz)
    {
        if (sz == size_) {
            memset(data_, 0, size_);
        } else {
            if (data_) {
                delete[] data_;
            }
            data_ = new char[sz];
            size_ = sz;
        }
    }

	void reset(const char* data, uint32_t sz)
	{
		if (data_) {
			delete[] data_;
		}

		data_ = new char[sz];
		size_ = sz;
		std::char_traits<char>::copy(data_, data, sz);
	}

	char* data() const
    {
        return data_;
    }

	uint32_t size() const
    {
        return size_;
    }

	char* begin() const
    {
        return data();
    }

	char* end() const
    {
        return begin() + size();
    }

private:
	char* data_;
	uint32_t size_;
};

typedef boost::shared_ptr<msg_buf_t> msg_buf_ptr;

struct message_t
{
	message_t(const msg_head_t& h, msg_buf_ptr b)
		: head(h)
		, body(b)
	{
	}

    bool has_uid() const
    {
        return (head.flags & msg_head_t::flag_uid) != 0;
    }

    uint64_t get_uid() const
    {
        return *((uint64_t*)body->begin());
    }

    msg_head_t head;
    msg_buf_ptr body;
};

class message_parser_t
{
public:
	message_parser_t()
		: read_size_(0)
		, body_(NULL)
	{
	}

	int32_t parse(char* _data, const uint32_t bytes);

	std::vector<message_t>& packs() { return packs_; }

private:
	bool is_head_readed() const
	{
		return read_size_ >= PH_LEN;
	}

	bool is_body_readed() const
	{
		return head_.len == (read_size_ - PH_LEN);
	}

	uint32_t read_head(const char* data, uint32_t size);
	uint32_t read_body(const char* data, uint32_t size);

	std::vector<message_t> packs_;
	uint32_t read_size_;
	msg_head_t head_;
	msg_buf_ptr body_;
};

NS_NETWORK_END
#endif
