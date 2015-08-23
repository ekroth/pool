/* Copyright (c) 2011-2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_EVENTMANAGER_HPP
#define KKE_EVENTMANAGER_HPP

#include "Interface/INoncopyable.hpp"
#include "Events/EventData.hpp"
#include "Utilities/Singleton.hpp"

#include <type_traits>
#include <queue>
#include <map>
#include <vector>
#include <string>
#include <functional>

namespace kke
{
	class EventListener;
	class GameTime;
    
    typedef std::function<void(EventData&)> EventCallback;
	
    /**
     * @brief Handles Events.
     **/
    class EventManagerBase
	{
	public:
		EventManagerBase();
		~EventManagerBase();
		
        bool AddListener(const void* listener, const EventCallback& c, const EventType& type);
        bool DelListener(const void* listener);
        bool DelListener(const void* listener, const EventType& type);
		
		/**
		 * @brief Queue event to be processed next update. Event is now owned by EventManager.
		 **/
		bool QueueEvent(EventData* event);
		
		/**
		 * @brief Abort previously queued event. Event is now owned by EventManager.
		 **/
		bool AbortEvent(EventData* event);
		
		/**
		 * @brief Instantly process event. Won't be owned by EventManager.
		 **/
		bool Trigger(EventData& event);
		
		void Update(GameTime& gametime);
		
	protected:
        typedef std::pair<const void*, EventCallback> EventPair; 
        typedef std::vector<EventPair> CallbackVec;
		typedef std::map<EventType, CallbackVec> ListenerMap;
		typedef std::queue<EventData*> EventQueue;
        
	protected:
        void processEvent(EventData& event);
        CallbackVec::iterator findListener(CallbackVec::iterator begin, CallbackVec::iterator end, const void* listener);
        bool delListener(const void* listener, const std::tuple<bool, const EventType&>& e);
        
		unsigned char activeQueue;
		ListenerMap listeners;
		EventQueue events[2];
	};
	
	class EventManager final : public EventManagerBase, public Singleton<EventManager>
	{
		friend class Singleton<EventManager>;
		
	private:
		EventManager() : EventManagerBase() { }
		~EventManager() { }
	};
    
    template<class EM>
    class ScopedListener;
    typedef ScopedListener<EventManager> ScopedEMListener;
}

#endif // KKE_EVENTMANAGER_HPP