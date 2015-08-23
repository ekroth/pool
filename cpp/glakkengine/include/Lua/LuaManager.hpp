/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LUAMANAGER_HPP
#define KKE_LUAMANAGER__HPP

#include "Events/EventData.hpp"
#include "Utilities/Singleton.hpp"

#include <unordered_map>
#include <vector>
#include <lua.hpp>
#include <string>

namespace kke
{
	class LuaManager final : public Singleton<LuaManager>
	{
		friend class Singleton<LuaManager>;
		
	public:
		lua_State* GetState();
		void StackOutput() const;
		
		/**
		 * @brief Add lua listener.
		 *
		 * @param ref Lua registry reference.
		 * @param type Event.
		 * @return bool
		 **/
		bool AddLuaListener(int ref, const EventType& type);
		
		/**
		 * @brief Remove lua listener.
		 *
		 * @param ref Lua registry reference.
		 * @param type Event.
		 * @return bool
		 **/
		bool DelLuaListener(int ref, const EventType& type);
		
		/**
		 * @brief Remove lua listener from all events.
		 *
		 * @param ref Lua registry reference.
		 * @return bool
		 **/
		bool DelLuaListener(int ref);
		
		void ProcessEvent(EventData& event);
		
		/**
		 * @brief Return registered reference.
		 *
		 * @param func Function name.
		 * @return int Returns LUA_NOREF if not found.
		 **/
		int GetReference(const std::string& func) const;
		
		/**
		 * @brief Register reference.
		 *
		 * @param func Function name.
		 * @return int Returns reference.
		 **/
		int RegisterReference(const std::string& func);
		
		/**
		 * @brief Unregisters reference.
		 *
		 * @param func Function name.
		 * @return void
		 **/
		void UnregisterReference(const std::string& func);
		
	private:
		LuaManager();
		~LuaManager();
		
	private:
		typedef std::vector<int> RefV;
		typedef std::unordered_map<EventType, RefV> ListenerMap;
		typedef std::pair<EventType, RefV> ListenerPair;
		typedef std::unordered_map<std::string, int> FuncRefMap;
		
		lua_State* state;
		ListenerMap luaListeners;
		FuncRefMap funcs;
	};
}

#endif // KKE_LUAMANAGER__HPP