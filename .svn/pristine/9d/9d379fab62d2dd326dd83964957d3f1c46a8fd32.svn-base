#ifndef __SCRIPT_MANAGER_H__
#define __SCRIPT_MANAGER_H__

#include "LuaState.h"
#include "Singleton.h"

class ScriptManager : public LuaState , public SingletonBase<ScriptManager>
{

public:
	ScriptManager();
	virtual ~ScriptManager();

public:
	/// @brief 脚本加载初始化
	/// @return 加载是否成功
	bool init(const std::string& path);

	void close();

	// 加载脚本
	bool load(const std::string& path);

};

#endif//__SCRIPT_MANAGER_H__