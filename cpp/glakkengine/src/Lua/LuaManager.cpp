/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Lua/LuaManager.hpp"
#include "Events/EventManager.hpp"
#include "IO/LogManager.hpp"

#include <algorithm>

using namespace kke;
using std::string;

LuaManager::LuaManager() : state(0)
{
	state = luaL_newstate();
	
//     LOGASSERT(state == 0, "Unable to open Lua");
    luaL_openlibs(state);
}

LuaManager::~LuaManager()
{
	lua_close(state);
	state = 0;
}

lua_State* LuaManager::GetState()
{
	return state;
}

void LuaManager::StackOutput() const
{
	const char* sep = "----------";
	int top = lua_gettop(state);
	
	LOGM() << "Beginning stack dump..";
	
	for (int i = 1; i <= top; i++) 
	{
		if (i == 1)
		{
			LOGM() << sep;
		}
		LOGM() << "Nr " << i << ": " << luaL_typename(state,i);
		
		switch (lua_type(state, i)) 
		{
			case LUA_TNUMBER:
				LOGM() << lua_tonumber(state,i);
				break;
			
			case LUA_TSTRING:
				LOGM() << lua_tostring(state,i);
				break;
			
			case LUA_TBOOLEAN:
				LOGM() << (lua_toboolean(state, i) == 0 ? "true" : "false");
				break;
			
			case LUA_TNIL:
				LOGM() << "nil";
				break;
			
			default:
				LOGM() << lua_topointer(state,i);
				break;
		}
		
		LOGM() << sep;
	}
	
	LOGM() << "..stack dump complete.";
}

bool LuaManager::AddLuaListener (int ref, const EventType& type)
{
	auto it = luaListeners.find(type);
	
	if (it == luaListeners.end())
	{
		// Must register event
// // // // // // // // // // // // 		EventManager::Instance().AddListener(this, type);
		luaListeners[type].push_back(ref);
	}
	else
	{	
		auto it2 = std::find(it->second.begin(), it->second.end(), ref);
		
		// Reference doesn't already exist in vector, add it
		if (it2 == it->second.end())
		{
			it->second.push_back(ref);
		}
		else
			return false;
	}
	
	return true;
}

bool LuaManager::DelLuaListener (int ref, const EventType& type)
{
	auto it = luaListeners.find(type);
	
	if (it != luaListeners.end())
	{
		auto it2  = std::find(it->second.begin(), it->second.end(), ref);
		
		if (it2 != it->second.end())
		{
			it->second.erase(it2);
			return true;
		}
		
		if (it->second.size() == 0)
		{
			// Empty, remove it
			luaListeners.erase(it);
			// Stop listening
			EventManager::Instance().DelListener(this, type);
		}
		
	}
	
	return false;
}

bool LuaManager::DelLuaListener (int ref)
{
	bool result = false;
	
	for (auto it = luaListeners.begin(); it != luaListeners.end(); ++it)
	{
		if (DelLuaListener(ref, it->first))
			result = true;
	}
	
	return result;
}

void LuaManager::ProcessEvent (EventData& event)
{
	auto it = luaListeners.find(event.GetType());
	
	if (it != luaListeners.end())
	{
		// Send GetTime() to lua
	}
}

int LuaManager::GetReference (const string& func) const
{
	auto it = funcs.find(func);
	
	if (it != funcs.end())
		return it->second;
	
	return LUA_NOREF;
}

int LuaManager::RegisterReference (const string& func)
{
	auto it = funcs.find(func);
	
	if (it == funcs.end())
	{
		lua_getglobal(state, func.c_str());
		int ref = luaL_ref(state, LUA_REGISTRYINDEX);
		funcs[func] = ref;
		return ref;
	}
	
	return it->second;
}

void LuaManager::UnregisterReference (const string& func)
{
	auto it = funcs.find(func);
	
	if (it != funcs.end())
	{
		lua_unref(state, it->second);
		funcs.erase(it);
	}
}