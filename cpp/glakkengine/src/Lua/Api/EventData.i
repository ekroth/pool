%module EventData
%{
#include "Events/EventData.hpp"
%}

namespace kke
{
	typedef double EventTime;
	typedef FNV1aHash::FNV1aHash EventType;
	
	/**
	 * @brief Run-time hash
	 **/
	static EventType EventHasherRT(const char* str);
	
	/**
	 * @brief Run-time hash
	 **/
	static EventType EventHasherRT(const std::string& str);
	
	static const char* const EventWildCharStr = (char*)"WildCharEvent";
	static const EventType EventWildChar = EventHasher(EventWildCharStr);

	class GameTime;
	class EventData
	{
	public:
		EventData(const EventType& type, const EventTime& time);
		virtual ~EventData();
		
		virtual const EventType& GetType() const;
		virtual const EventTime& GetTime() const;
	};
}