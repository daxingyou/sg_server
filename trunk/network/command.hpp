#ifndef _NETWORK_COMMOND_H_
#define _NETWORK_COMMOND_H_

#include "macros.hpp"

#pragma pack(1)
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

	static const uint8_t flag_uid = (1 << 5);
	static const uint8_t flag_callback = (1 << 6);
	static const uint8_t flag_compress = (1 << 7);

	uint32_t len;
	uint16_t cmd;
	uint16_t cmd_ex;
	uint16_t flags;
	int16_t unused;
};

struct gc_user_move_cmd
{
	uint64_t object_id;
	uint32_t pos_x;
	uint32_t pos_y;
	uint32_t direction;
	uint32_t speed;
	uint32_t high;

	gc_user_move_cmd()
		: object_id(0), pos_x(0), pos_y(0), direction(0), speed(0), high(0) {};
};
#pragma pack()

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif