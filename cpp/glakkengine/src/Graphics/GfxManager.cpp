/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/GfxManager.hpp"
#include "IO/LogManager.hpp"
#include "Events/EventManager.hpp"
#include "ResourceFactory.hpp"

#include <vector>

using std::vector;

using namespace kke;

const EventType& GfxOutPassEventData::GfxOutPassEventType()
{
    static const EventType type = EventHasher("GfxOutPassEventType");
    return type;
}

GfxOutPassEventData::GfxOutPassEventData(EventData* event, const EventTime& time) : 
    PassEventData(event, GfxOutPassEventType(), time)
{
    
}

GfxManager::GfxManager() : 
    passGfxListener(this, &GfxManager::ProcessGfxEvent, GfxOutPassEventData::GfxOutPassEventType())
{
}

GfxManager::~GfxManager()
{
    thread.join();
}

void GfxManager::Update()
{
    {
        // Get new events
        auto v = renderData.PullEvents();
        
        for (auto e : v)
        {
            switch (e->GetType())
            {
                case EventHasher("WindowCloseRequest"):
                    LOGD() << "WindowCloseRequest sent to MAIN";
                    break;
                    
                case EventHasher("WindowClosed"):
                    LOGD() << "WindowClosed sent to MAIN";
                    break;
            }
            EventManager::Instance().QueueEvent(e);
        }
    }
    
    {
        // Pass over new events
        renderData.PushEvents(outEvents);
    }
}

bool GfxManager::Run()
{
    thread = std::thread(std::ref(renderData));
}

void GfxManager::ProcessGfxEvent(EventData& e)
{
    switch (static_cast<GfxOutPassEventData&>(e).GetEvent()->GetType())
    {
        case EventHasher("CloseWindow"):
            LOGD() << "CloseWindow sent to RENDER";
            break;
    }
    outEvents.push(static_cast<GfxOutPassEventData&>(e).TakeEvent());
}

void GfxManager::ProcessEvent(EventData& e)
{
    // Yeah
}