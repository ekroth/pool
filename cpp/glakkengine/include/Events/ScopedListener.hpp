/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_SCOPEDLISTENER_HPP
#define KKE_SCOPEDLISTENER_HPP

#include "Events/EventManager.hpp"

namespace kke
{	
	/**
	 * @brief Works similar to std::scoped_ptr, will register class upon construction, and unregister class upon destruction.
	 * 
	 * For now it only uses EventManager, and not any other derived classes of EventManagerBase.
	 **/
    template<class EM>
	class ScopedListener final
    {
    public:
		ScopedListener();
        
        template<typename L, typename C>
        /**
         * @brief Wraps std::bind.
         *
         * @param listener Listener.
         * @param c Listener callback.
         * @param type Event.
         **/
        ScopedListener(L listener, C c, const EventType& type);
        
        /**
         * @brief Registers listener's callback with EventManager.
         *
         * @param listener Listener.
         * @param c Listener Callback function object.
         * @param type Event.
         **/
        ScopedListener(const void* listener, const EventCallback& c, const EventType& type);
        
        ~ScopedListener();
		
        // No reason to copy?
        ScopedListener(const ScopedListener& sl) = delete;
        ScopedListener& operator=(const ScopedListener& sl) = delete;
        
        // When moving we don't need to register. And moved target should be empty.
        ScopedListener(ScopedListener&& sl);
		ScopedListener& operator=(ScopedListener&& sl);
        
        /**
         * @return bool listener != nullptr.
         **/
        operator bool() const;
        
        /**
         * @brief Unregisters current listener and resets object.
         *
         * @return void
         **/
        void Reset();
        
    private:
        const void* listener;
        EventType type;
    };
    
#include "Events/ScopedListener.inl"
}

#endif // KKE_SCOPEDLISTENER_HPP