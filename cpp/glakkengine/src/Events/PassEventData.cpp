/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Events/PassEventData.hpp"

using namespace kke;

PassEventData::PassEventData(EventData* event, const EventType& type, const EventTime& time) : 
	EventData(type, time),
	event(event)
{

}

PassEventData::~PassEventData()
{
	if (event != nullptr)
		delete event;
}

void PassEventData::SetEvent(EventData* e)
{
	if (e != event)
	{
		if (event != nullptr)
			delete event;
		
		event = e;
	}
}
        
EventData* PassEventData::TakeEvent()
{
	EventData* e = event;
	event = nullptr;
	return e;
}

