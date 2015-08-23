/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Lua/LuaScript.hpp"
#include "Lua/LuaManager.hpp"

#include <lua.hpp>

using namespace kke;

LuaScript::LuaScript () : 
	ResourceType (), reference(LUA_NOREF)
{

}

LuaScript::~LuaScript()
{
	Unload();
}

int LuaScript::GetReference() const
{
	return reference;
}

bool LuaScript::Load(const std::string& path)
{
	if (luaL_loadfile(LuaManager::Instance().GetState(), path.c_str()) != 0)
		return false;
	
	reference = luaL_ref(LuaManager::Instance().GetState(), LUA_REGISTRYINDEX);
	
	source = path;

	return reference != LUA_NOREF && reference != LUA_REFNIL;
}

bool LuaScript::Unload()
{
	if (reference != LUA_NOREF)
	{
		luaL_unref(LuaManager::Instance().GetState(), LUA_REGISTRYINDEX, reference);
		reference = LUA_NOREF;
	}
	
	return true;
}

const std::string& LuaScript::GetCategory() const
{
	static const std::string category = "LuaScript";
	return category;
}
