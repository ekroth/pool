/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_PASSEVENTDATA_HPP
#define KKE_PASSEVENTDATA_HPP

#include "Events/EventData.hpp"

namespace kke
{
	/**
	 * @brief EventData used to pass on EventData.
	 **/
	class PassEventData : public EventData
    {       
    public:
        /**
         * @brief Default CTOR
         *
         * @param passType EventType to pass to GfxManager
         * @param time Time
         **/
        PassEventData(EventData* event, const EventType& type, const EventTime& time);
        virtual ~PassEventData();
        
        /**
         * @brief EventData to pass to GfxManager
         *
         * @return const EventType*
         **/
        const EventData* GetEvent() const { return event; }
        void SetEvent(EventData* e);
        
        /**
         * @brief Take over EventData. Will not be owned by PassEventData anymore.
         *
         * @return :EventData*
         **/
        EventData* TakeEvent();
        
    private:
        EventData* event;
	};
}

#endif // KKE_PASSEVENTDATA_HPP
