/* Copyright (c) 2011-2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Events/EventManager.hpp"
#include "Events/EventData.hpp"
#include "IO/LogManager.hpp"
#include "GameTime.hpp"

using namespace kke;

bool EventManagerBase::AddListener(const void* listener, const EventCallback& c, const EventType& type)
{
	CallbackVec& listVec = listeners[type];
	
	// If listeners already isn't registered at this type
	if (listeners.find(type) != listeners.end() && findListener(listVec.begin(), listVec.end(), listener) != listVec.end())
	{
		return false;
	}
	
	listeners[type].push_back(EventPair(listener, c));
	
	return true;
}

bool EventManagerBase::DelListener(const void* listener)
{
    return delListener(listener, std::tuple<bool, const EventType&>(false, EventType(0))); 
}

bool EventManagerBase::DelListener(const void* listener, const EventType& type)
{
	return delListener(listener, std::tuple<bool, const EventType&>(true, type));
}

bool EventManagerBase::QueueEvent (EventData* event)
{
	events[activeQueue].push(event);	
	return true;
}

bool EventManagerBase::AbortEvent (EventData* event)
{
	if (events[activeQueue].front() == event)
	{
		events[activeQueue].pop();
		return true;
	}
	else
	{
		LOGW() << 	"Can't abort events that aren't at front().\n"
											"Is it needed?";
	}
	
	// Right now you can only pop latest.
	return false;
}

bool EventManagerBase::Trigger (EventData& event)
{
	processEvent(event);
	return true;
}

void EventManagerBase::Update (GameTime& gametime)
{
	// Switch active queues. (So that new events aren't added to same.)
	const auto prevActiveQueue = activeQueue;
	activeQueue = activeQueue == 0 ? 1 : 0;
	
	EventQueue &prevActive = events[prevActiveQueue];
	
	// This could be limited to an X amount of events
	while (!prevActive.empty())
	{
		EventData &event = *prevActive.front();
		
		processEvent(event);
		
		delete prevActive.front();
		prevActive.pop();
	}
	
}

void EventManagerBase::processEvent (EventData& event)
{
	{
		CallbackVec &defVec = listeners[EventWildChar];
		
		for (auto& e: defVec)
		{
			e.second(event);
		}
	}
	
	{
		CallbackVec &listVec = listeners[event.GetType()];
			
		for (auto& e: listVec)
        {
            e.second(event);
        }
	}
}

EventManagerBase::CallbackVec::iterator EventManagerBase::findListener(CallbackVec::iterator begin, CallbackVec::iterator end, const void* listener)
{
    for ( ; begin != end; begin++)
        if (begin->first == listener)
            return begin;
        
    return end;
}

bool EventManagerBase::delListener(const void* listener, const std::tuple<bool, const EventType&>& e)
{
    bool found = false;
    
    std::queue<EventType> empty;
    
    for (auto& p : listeners)
    {
        if (std::get<0>(e) && std::get<1>(e) != p.first)
            continue;
        
        CallbackVec::iterator it2 = findListener(p.second.begin(), p.second.end(), listener);
        
        if (it2 != p.second.end())
        {
            found = true;
            p.second.erase(it2);
        }
        
        if (p.second.empty()) // No more listeners
            empty.push(p.first);
    }
    
    // Remove empty EventVectors
    while (!empty.empty())
    {
        listeners.erase(empty.front());
        empty.pop();
    }
    
    return found;
}

EventManagerBase::EventManagerBase() : 
	activeQueue(0)
{
}

EventManagerBase::~EventManagerBase()
{	
	for (uint i = 0; i < 2; i++)
		while (!events[i].empty())
		{
			LOGW() << "Unprocessed event (" << i << ") : " << events[i].front()->GetType();
			delete events[i].front();
			events[i].pop();
		}
	
	if (listeners.size() != 0)
	{
		LOGW() << "Not all listeners were removed.";
	}
	
	listeners.clear();
}
