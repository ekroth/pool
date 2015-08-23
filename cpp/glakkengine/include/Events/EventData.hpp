/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_EVENTDATA_HPP
#define KKE_EVENTDATA_HPP

#include "Utilities/FNV1aHash.hpp"

#include <string>

#define EventHasher(x) KKE_FNV1AHASH(x)

namespace kke
{
	typedef double EventTime;
	typedef FNV1aHash::Hash EventType;
	
	/**
	 * @brief Run-time hash
	 **/
	static EventType EventHasherRT(const char* str)
	{
		return FNV1aHash::hash_rt(str);
	}
	
	/**
	 * @brief Run-time hash
	 **/
	static EventType EventHasherRT(const std::string& str)
	{
		return FNV1aHash::hash_rt(str.c_str());
	}
	
	static const char* const EventWildCharStr = (char*)"WildCharEvent";
	static const EventType EventWildChar = EventHasher(EventWildCharStr);

	class GameTime;
	class EventData
	{
		friend class EventManager;
	public:
		constexpr EventData(const EventType& type, const EventTime& time) : type(type), time(time) { }
		virtual ~EventData();
		
		virtual const EventType& GetType() const;
		virtual const EventTime& GetTime() const;
	protected:
		EventType type;
		EventTime time;
	};
}

#endif // KKE_EVENTDATA_HPP