#include "game_script_func.h"
#include "main/robot_server.hpp"
#include "main/robot_server_fwd.hpp"
#include "common/log.hpp"
#include "common/ScriptManager.hpp"
#include "robot_common/robot_common_struct.hpp"
#include "common/random_util.hpp"

#include <sstream>
#include <dirent.h>
#include <unistd.h>

/// @brief 输出调试日志
void c_debug( const char* msg )
{
	if( NULL == msg )
		return;

	log_debug( msg );
}

void c_info(const char* msg)
{
	if (NULL == msg)
		return;

	log_info(msg);
}

void c_error(const char* msg)
{
	if (NULL == msg)
		return;

	log_error(msg);
}


/// @brief 断言失败
void c_assert_fail( const char* msg )
{
	if( NULL != msg )
		log_debug( msg );
	assert( false );
}

/// @brief 获得脚本根目录

const char* c_get_root_dir()
{
	///< 防止返回时析构
	static std::string s_root_dir;

	if (s_root_dir.empty())
	{
		s_root_dir += env::robot_config->script_path;
		s_root_dir += "/";
	}

	if (!s_root_dir.empty())
	{
		return s_root_dir.c_str();
	}

	return NULL;
}

/// @brief 加载一个脚本文件
bool c_do_file( const char* filename )
{
	if( filename == NULL )
		return false;
	if (NULL == ScriptManager::getInstancePtr())
	{
		log_error("NULL == ScriptManager::getInstancePtr()");
		return false;
	}
	return ScriptManager::getInstancePtr()->doFile( filename );
}

/// @brief 获得文件夹下的所有文件名
const char* c_get_file_list_by_folder(const char* folder)
{
	///< 防止返回时析构
	static std::string s_dir_files;
	s_dir_files.clear();

	//if (s_dir_files.empty())
	{
		std::string dir(c_get_root_dir());
		std::ostringstream oss;

		dir += folder;
		DIR *d = opendir(dir.c_str());
		if (!d)
		{
			log_error("can not open dir[%s]", dir.c_str());
			return NULL;
		}
		struct dirent* dp;
		while ((dp = readdir(d)) != NULL)
		{
			//if (dp->d_type != DT_REG)
				//continue;

			//c_debug(dp->d_name);

			if (strlen(dp->d_name) > 4 && strcmp(&dp->d_name[strlen(dp->d_name) - 4], ".lua") == 0)
			{
				oss << dp->d_name << " ";
			}
		}

		closedir(d);

		s_dir_files = oss.str();
	}

	return s_dir_files.c_str();
}

int c_rand_between(int a, int b)
{
	return random_util_t::randBetween(a, b);
}

bool c_select_by_tenth(unsigned int tenth)
{
	return random_util_t::selectByTenTh(tenth);
}