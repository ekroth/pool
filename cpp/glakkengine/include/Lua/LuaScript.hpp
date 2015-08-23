/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LUASCRIPT_HPP
#define KKE_LUASCRIPT_HPP

#include "Interface/INoncopyable.hpp"
#include "ResourceType.hpp"
#include "Events/EventData.hpp"

#include <string>

namespace kke
{
	class LuaScript : public ResourceType, public INoncopyable
	{
	public:
		explicit LuaScript();
		virtual ~LuaScript();
		
		int GetReference() const;
		virtual bool Load(const std::string& path) override;
		virtual bool Unload() override;
		const std::string& GetCategory() const;
		
	private:
		int reference;
	};
}

#endif // KKE_LUASCRIPT_HPP