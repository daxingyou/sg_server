#ifndef __CMD_DESC_HPP__
#define __CMD_DESC_HPP__

#include <string>
#include <map>
#include <string>
#include "common/macros.hpp"
#include "op_cmd.hpp"

namespace op_cmd
{
	typedef std::map<uint32_t, std::string> msg_desc_map;

	class cmd_desc
	{
	public:
		static void init();
		static void close();

	public:
		static const char* get_msg_desc(uint32_t cmd)
		{
			auto it = m_msg_desc.find(cmd);

			if (it != m_msg_desc.end())
			{
				return it->second.c_str();
			}

			return NULL;
		}

	public:
		static msg_desc_map m_msg_desc;
	};
}

#define CMD_DESC(cmd_id) op_cmd::cmd_desc::get_msg_desc(cmd_id)

#define CMD_INIT op_cmd::cmd_desc::init();
#define CMD_CLOSE op_cmd::cmd_desc::close();

#endif