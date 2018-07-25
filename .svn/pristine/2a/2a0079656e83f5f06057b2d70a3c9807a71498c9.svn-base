#!/usr/bin/python
# -*- coding: utf-8 -*-

import os,re,sys,time

file_name = "./op_cmd/op_cmd"
out_hpp = "./op_cmd/op_cmd.hpp"
out_cs = "./op_cmd/msg_cmd.cs"
out_lua = "./op_cmd/cmd_lua.lua"
out_msg = "./op_cmd/cmd_desc.cpp"

ANALYSIS_RESULTS = []

#---------------------hpp begin--------------------
hpp_head = '''#ifndef __OP_CMD_HPP__
#define __OP_CMD_HPP__

/*
client      'c'
account     'n'
transfer    'f'
login       'l'
gate        'a'
game        'g'
chat     	'h'
db          'd'
center      'e'
gm          'm'
log         'o'
gmtool      't'
area		'r'
cross 		's'
*/

namespace op_cmd 
{
enum
{
'''

hpp_end = '''};
}

#endif
'''

hpp_line1 = "    {0}\n"
hpp_line2 = "    {0},\n"
#---------------------hpp end--------------------
#---------------------cs begin---------------------
cs_head = '''namespace msg_cmd
{
	public enum cmd
	{
'''

cs_end = '''	}
}
'''

cs_line1 = "		{0} = {1},\n"
cs_line2 = "		{0}\n"
#---------------------cs end---------------------
#---------------------lua begin---------------------
lua_head = '''
MSGCMD = 
{
'''

lua_end = '''}
'''

lua_line1 = "	{0} = {1},\n"
lua_line2 = "	{0}\n"
#---------------------lua end---------------------
#---------------------msg begin---------------------
msg_head = '''#include "cmd_desc.hpp"

namespace op_cmd
{
	#define DESC_MSG(_MSG_) cmd_desc::m_msg_desc.insert(msg_desc_map::value_type(_MSG_, #_MSG_))

	msg_desc_map cmd_desc::m_msg_desc;

	void cmd_desc::init()
	{
		m_msg_desc.clear();
'''

msg_end = '''	}

	void cmd_desc::close()
	{
		m_msg_desc.clear();
	}

}
'''

msg_line = "		DESC_MSG({0});\n"
#---------------------end begin---------------------

def read_file():
	if not os.path.exists(file_name):
		print "Can't not find op_cmd!!"
		return

	with open(file_name, 'r') as result:
		data = result.read().split('\n')
		for line in data:
			l = re.sub(r'[\x00-\x1f]', '', line)	#替换所有控制符
			ANALYSIS_RESULTS.append(l)

def output_hpp():
	output = open(out_hpp, 'w')
	output.write(hpp_head)
	for line in ANALYSIS_RESULTS:
		if len(line) > 0:
			if re.search("//+",line):
				output.write(hpp_line1.format(line))
			elif re.search("_BEGIN",line):
				output.write(hpp_line2.format(line))
			elif re.search("_END",line):
				output.write('\n')
				output.write(hpp_line2.format(line))
				output.write('\n')
			elif re.match('\w+', line):
				output.write(hpp_line2.format(line))
	output.write(hpp_end)
	output.close()

def output_cs():
	output = open(out_cs, 'w')
	output.write(cs_head)
	msg_begin = 0
	is_continue = False	# 只生成带有client
	for line in ANALYSIS_RESULTS:
		if len(line) > 0:
			if re.search("//+",line):
				if re.search('client+', line):
					is_continue = False
					output.write(cs_line2.format(line))
				else:
					is_continue = True
			if is_continue:
				continue
			
			if re.search("_BEGIN",line):
				re_line = re.split(' |,\\r',line)
				msg_begin = int(re_line[2])
			elif re.search("_END",line):
				msg_begin = 0
				output.write('\n')
			elif re.match('\w+', line):
				msg_begin += 1
				output.write(cs_line1.format(line.split(',')[0], msg_begin))
							
	output.write(cs_end)
	output.close()	
	
def output_lua():
	output = open(out_lua, 'w')
	output.write(lua_head)
	msg_begin = 0
	is_continue = False	# 只生成带有client
	for line in ANALYSIS_RESULTS:
		if len(line) > 0:
			if re.search("//+",line):
				if re.search('client+', line):
					is_continue = False
					output.write(lua_line2.format(line[2:]))
				else:
					is_continue = True
			if is_continue:
				continue
			
			if re.search("_BEGIN",line):
				re_line = re.split(' |,\\r',line)
				msg_begin = int(re_line[2])
			elif re.search("_END",line):
				msg_begin = 0
				output.write('\n')
			elif re.match('\w+', line):
				msg_begin += 1
				output.write(lua_line1.format(line.split(',')[0], msg_begin))
							
	output.write(lua_end)
	output.close()	
	
def output_msg():
	output = open(out_msg, 'w')
	output.write(msg_head)
	for line in ANALYSIS_RESULTS:
		if len(line) > 0:
			if re.search("//+",line):
				pass
			elif re.search("_BEGIN",line):
				pass
			elif re.search("_END",line):
				pass
			elif re.match('\w+', line):
				output.write(msg_line.format(line))
	output.write(msg_end)
	output.close()

def main():
	print('begin generate head file...')
	read_file()
	output_hpp()
	output_cs()
	output_lua()
	output_msg()
	print('end generate head file...')
			
if __name__ == '__main__':
    main()
