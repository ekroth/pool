/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_GFXMANAGER_HPP
#define KKE_GFXMANAGER_HPP

#include "Graphics/RenderThread.hpp"
#include "Utilities/Singleton.hpp"
#include "Events/PassEventData.hpp"
#include "Events/ScopedListener.hpp"

#include <tuple>
#include <thread>

namespace kke
{
    /**
     * @brief PassEventData used to pass on events to Renderer from GfxManager.
     **/
    class GfxOutPassEventData final : public PassEventData
    {
    public:
        /**
         * @brief Underlying EventType for all GfxOutPassEventData
         *
         * @return const EventType&
         **/
        static const EventType& GfxOutPassEventType();
        
    public:
        /**
         * @brief Default CTOR
         *
         * @param passType EventType to pass to GfxManager
         * @param time Time
         **/
        GfxOutPassEventData(EventData* event, const EventTime& time);
        ~GfxOutPassEventData() { }
    };
    
	class GfxManager : public Singleton<GfxManager>
	{
        friend class Singleton<GfxManager>;
        
	public:        
        bool Run();
		void Update();
        
        void ProcessGfxEvent(EventData& e);
        void ProcessEvent(EventData& e);
        
    private:
        GfxManager();
        ~GfxManager();
        
        std::thread thread;
        render::RenderThread renderData;
        
        std::queue<EventData*> outEvents;
        
        ScopedEMListener passGfxListener;
	};
}

#endif // GKKE_FXMANAGER_HPP
