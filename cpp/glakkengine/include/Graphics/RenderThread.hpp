/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_RENDERTHREAD_HPP
#define KKE_RENDERTHREAD_HPP

#include "Graphics/GLManager.hpp"
#include "Graphics/GfxEventManager.hpp"
#include "Interface/INoncopyable.hpp"
#include "Events/PassEventData.hpp"
#include "Events/ScopedListener.hpp"
#include "GameTime.hpp"

#include <tuple>
#include <mutex>
#include <queue>
#include <vector>

namespace kke
{
    class IRenderable;
    class ILoadable;
    /**
     * @brief PassEventData used to pass on events to GfxManager from Renderer.
     **/
    class GfxInPassEventData final : public PassEventData
    {
    public:
        /**
         * @brief Underlying EventType for all GfxInPassEventData
         *
         * @return const EventType&
         **/
        static const EventType& GfxInPassEventType();
        
    public:
        /**
         * @brief Default CTOR
         *
         * @param passType EventType to pass to GfxManager
         * @param time Time
         **/
        GfxInPassEventData(EventData* event, const EventTime& time);
        ~GfxInPassEventData() { }
    };
	
    namespace render
    {
        class Loadable;
        class RenderThread : public INoncopyable
        {

        public:
            RenderThread();
            ~RenderThread();
            
            void operator()();
			/**
			 * @brief Pull outgoing Events.
			 * 
			 *  [ Called by other thread ]
			 * @return :vector< kke::EventData*, std::allocator< kke::EventData* > >
			 **/
			std::vector<EventData*> PullEvents();
                     
            /**
             * @brief Push incoming events.
             *
             * [ Called by other thread ]
             * @param events ...
             * @return void
             **/
            void PushEvents(std::queue<EventData*>& events);
			
			void ProcessGfxEvent(EventData& e);
			void ProcessEvent();
            
        private:
            /**
             * @brief Handle incoming and outgoing events
             *
             * @return bool Returns true if any Events were sent or received.
             **/
            bool handleEvents();
            
            bool initialization();
            void cleanup();
            
            bool initStage;
            render::GLManager* openGLManager;
            
            std::mutex outEventsLock, inEventsLock;
            std::queue<EventData*> outEvents, outEventsBuffer, inEvents;
			
			ScopedGEMListener gfxInListener;
            GameTime gameTime;
        };
    }
}

#endif // KKE_RENDERTHREAD_HPP