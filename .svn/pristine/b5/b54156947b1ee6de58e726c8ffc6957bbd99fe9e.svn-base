#include "ScriptManager.hpp"

int tolua_ToScript_open(lua_State*);

ScriptManager::ScriptManager()
{
}

ScriptManager::~ScriptManager()
{
}

bool ScriptManager::load(const std::string& path)
{
	if (path.empty())
	{
		log_error("load script failed: path is NULL!");
		return false;
	}
	std::string script(path);
	script += "init.lua";

	if (!doFile(script.c_str()))
	{
		return false;
	}
	return true;
}

bool ScriptManager::init(const std::string& path)
{
	tolua_ToScript_open(getHandle());

    if (!load(path))
    {
        return false;
    }

    log_info("init script-lua[%s] success!", path.c_str());
    return true;
}

void ScriptManager::close()
{
	final();
    log_info("close script-lua success!");
}