/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Lua/LuaAddon.hpp"
#include "Lua/LuaManager.hpp"
#include <lua.hpp>

using namespace kke;

using std::string;

LuaAddon::LuaAddon () : 
	LuaScript(), initRef(LUA_NOREF), termRef(LUA_NOREF)
{

}

LuaAddon::~LuaAddon()
{
	Unload();
}

bool LuaAddon::Load(const std::string& path)
{
	bool result = LuaScript::Load(path);
	
	if (result)
	{
		// Initialize functions
		lua_rawgeti(LuaManager::Instance().GetState(), LUA_REGISTRYINDEX, GetReference());
		lua_pcall(LuaManager::Instance().GetState(), 0, 0, 0);
		
		
		// Initialize references
		lua_getglobal(LuaManager::Instance().GetState(), string(name + "_initialize").c_str());
		initRef = luaL_ref(LuaManager::Instance().GetState(), LUA_REGISTRYINDEX);
		
		lua_getglobal(LuaManager::Instance().GetState(), string(name + "_terminate").c_str());
		termRef = luaL_ref(LuaManager::Instance().GetState(), LUA_REGISTRYINDEX);
		
		result = Initialize();
	}

	return result;
}

bool LuaAddon::Unload()
{
	Terminate();
	LuaScript::Unload();
	
	// Remove references
	luaL_unref(LuaManager::Instance().GetState(), LUA_REGISTRYINDEX, initRef);
	luaL_unref(LuaManager::Instance().GetState(), LUA_REGISTRYINDEX, termRef);
	
	initRef = LUA_NOREF;
	termRef = LUA_NOREF;
	
	return true;
}

const std::string& LuaAddon::GetName() const
{
	return name;
}

void LuaAddon::SetName(const std::string& name)
{
	this->name = name;
}

bool LuaAddon::Initialize()
{
	lua_rawgeti(LuaManager::Instance().GetState(), LUA_REGISTRYINDEX, initRef);
	lua_pcall(LuaManager::Instance().GetState(), 0, 0, 0);
	
	// Return result?
	
	return true;
}

bool LuaAddon::Terminate()
{
	lua_rawgeti(LuaManager::Instance().GetState(), LUA_REGISTRYINDEX, termRef);
	lua_pcall(LuaManager::Instance().GetState(), 0, 0, 0);
	
	// Return result?
	
	return true;
}