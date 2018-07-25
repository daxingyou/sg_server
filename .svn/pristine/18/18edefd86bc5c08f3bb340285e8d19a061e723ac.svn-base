/**
* \brief 定义字节缓冲
*/

#ifndef _BYTEBUFFER_H_
#define _BYTEBUFFER_H_

#include <string.h>
#include <vector>

#include "macros.hpp"
#include "message.hpp"

NS_NETWORK_BEGIN

/**
* 字节缓冲，用于套接口接收和发送数据的缓冲
* \param _type 缓冲区数据类型
*/
template <typename _type>
class ByteBuffer
{
public:
	ByteBuffer();

	inline void put(const char *buf, const unsigned int size)
	{
		//首先确认缓冲内存是否足够
		wr_reserve(size);
		bcopy(buf, &_buffer[_currPtr], size);
		_currPtr += size;
	}

	inline char *wr_buf()
	{
		return &_buffer[_currPtr];
	}

	inline char *rd_buf()
	{
		return &_buffer[_offPtr];
	}

	inline bool rd_ready() const
	{
		return _currPtr > _offPtr;
	}

	inline unsigned int rd_size() const
	{
		return _currPtr - _offPtr;
	}

	inline void rd_flip(unsigned int size)
	{
		_offPtr += size;
		if (_currPtr > _offPtr) {
			unsigned int tmp = _currPtr - _offPtr;
			if (_offPtr >= tmp) {
				memmove(&_buffer[0], &_buffer[_offPtr], tmp);
				_offPtr = 0;
				_currPtr = tmp;
			}
		} else {
			_offPtr = 0;
			_currPtr = 0;
		}
	}

	inline void rd_off(unsigned int size)
	{
		_offPtr += size;
	}

	inline void rd_flip()
	{
		if (_currPtr > _offPtr) {
			unsigned int tmp = _currPtr - _offPtr;
			if (_offPtr >= tmp) {
				memmove(&_buffer[0], &_buffer[_offPtr], tmp);
				_offPtr = 0;
				_currPtr = tmp;
			}
		} else {
			_offPtr = 0;
			_currPtr = 0;
		}
	}

	inline unsigned int wr_size() const
	{
		return _maxSize - _currPtr;
	}

	inline void wr_flip(const unsigned int size)
	{
		_currPtr += size;
	}

	inline void reset()
	{
		_offPtr = 0;
		_currPtr = 0;
	}

	inline unsigned int maxSize() const
	{
		return _maxSize;
	}

	inline unsigned int offPtr() const
	{
		return _offPtr;
	}

	inline unsigned int currPtr() const
	{
		return _currPtr;
	}

	inline void wr_reserve(const unsigned int size);

private:

	unsigned int _maxSize;
	unsigned int _offPtr;
	unsigned int _currPtr;
	_type _buffer;
};

//////////////////////////////////////////////////////////////////////////
/**
 * \brief 动态内存的缓冲区，可以动态扩展缓冲区大小
 */
typedef ByteBuffer<std::vector<char>> BufferCmdQueue_t;

template <>
inline void BufferCmdQueue_t::wr_reserve(const unsigned int size)
{
	if (wr_size() < size) {
#define ResetCount(size) (((size) + TRUNK_SIZE - 1) / TRUNK_SIZE)
		_maxSize += (TRUNK_SIZE * ResetCount(size));
		_buffer.resize(_maxSize);
	}
}
//////////////////////////////////////////////////////////////////////////

NS_NETWORK_END

#endif
