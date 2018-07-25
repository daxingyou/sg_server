#include "ByteBuffer.hpp"

NS_NETWORK_BEGIN
template <>
BufferCmdQueue_t::ByteBuffer()
	: _maxSize(TRUNK_SIZE), _offPtr(0), _currPtr(0), _buffer(_maxSize) { }

NS_NETWORK_END