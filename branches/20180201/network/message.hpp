#ifndef __NETWORK_MESSAGE_HPP_
#define __NETWORK_MESSAGE_HPP_

#include "common/common_fwd.hpp"
#include "boost/shared_ptr.hpp"
#include <vector>
#include <string.h>


NS_NETWORK_BEGIN

struct msg_head_t
{
    msg_head_t()
        : len(0)
        , cmd(0)
        , cmd_ex(0)
        , flags(0)
        , unused(0)
    {
    }

    static const uint8_t flag_uid = (1<<5);
    static const uint8_t flag_callback = (1<<6);
    static const uint8_t flag_compress = (1<<7);

    uint32_t len;
    uint16_t cmd;
    uint16_t cmd_ex;
    uint16_t flags;
    int16_t unused;
};

class msg_buf_t
{
public:
    msg_buf_t()
        : data_(NULL)
        , size_(0)
    {
    }

    msg_buf_t(size_t sz)
        : data_(new char[sz])
        , size_(sz)
    {
    }

    msg_buf_t(const char* data, size_t sz)
		: data_(new char[sz])
		, size_(sz)
    {
		std::char_traits<char>::copy(data_, data, sz);
    }

    msg_buf_t(const msg_buf_t& b)
        : data_(new char[b.size_])
        , size_(b.size_)
    {
        std::char_traits<char>::copy(data_, b.data_, size_);
    }

    ~msg_buf_t()
    {
        if (data_)
        {
            delete[] data_;
            data_ = NULL;
            size_ = 0;
        }
    }

    msg_buf_t& operator=(const msg_buf_t& b)
    {
		if (this == &b)
			return *this;

        if (data_)
        {
            delete[] data_;
        }

        data_ = new char[b.size_];
        size_ = b.size_;
        std::char_traits<char>::copy(data_, b.data_, size_);
        return *this;
    }

    void reset(size_t sz)
    {
        if (sz == size_)
        {
            memset(data_, 0, sz);
        }
        else
        {
            if (data_)
            {
                delete[] data_;
            }
            data_ = new char[sz];
            size_ = sz;
        }
    }

	void reset(const char* data, size_t sz)
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

    size_t size() const
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
    size_t size_;
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
    {
    }

    bool parse(char* data, size_t bytes);

    std::vector<message_t>& packs(){ return packs_; }

private:
    bool is_head_readed() const
    {
        return read_size_ >= sizeof(head_);
    }

    bool is_body_readed() const
    {
        return head_.len == (read_size_ - sizeof(head_));
    }

    size_t read_head(const char* data, size_t size);
    size_t read_body(const char* data, size_t size);

    std::vector<message_t> packs_;
    size_t read_size_;
    msg_head_t head_;
    msg_buf_ptr body_;
};

NS_NETWORK_END
#endif
