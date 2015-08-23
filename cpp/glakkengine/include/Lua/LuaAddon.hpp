/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LUAADDON_HPP
#define KKE_LUAADDON_HPP

#include "Lua/LuaScript.hpp"

#include <string>

namespace kke
{
	class LuaAddon : public LuaScript
	{
	public:
		explicit LuaAddon();
		~LuaAddon();
		
		bool Load(const std::string& path);
		bool Unload();
		
		const std::string& GetName() const;
		void SetName(const std::string& name);
		
		bool Initialize();
		bool Terminate();
		
	private:
		std::string name;
		int initRef;
		int termRef;
	};
}

#endif // KKE_LUAADDON_HPP