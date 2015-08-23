/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/RenderThread.hpp"
#include "Graphics/GfxCache.hpp"
#include "Graphics/GLManager.hpp"
#include "IO/LogManager.hpp"

#define ILUT_USE_OPENGL
#include <IL/ilu.h>
#include <string>

using namespace kke;
using namespace kke::render;

using std::string;
using std::vector;
using std::lock_guard;
using std::mutex;

const EventType& GfxInPassEventData::GfxInPassEventType()
{
    static const EventType type = EventHasher("GfxInPassEventType");
    return type;
}

GfxInPassEventData::GfxInPassEventData(EventData* event, const EventTime& time) : 
    PassEventData(event, GfxInPassEventType(), time)
{
    
}

RenderThread::RenderThread() : 
    initStage(true),
    openGLManager(nullptr)
{

}

RenderThread::~RenderThread()
{
    
}

vector<EventData*> RenderThread::PullEvents()
{
    lock_guard<mutex> lock(outEventsLock);
    std::vector<EventData*> v;
    v.reserve(outEvents.size());
    
    while (!outEvents.empty())
    {
        v.push_back(outEvents.front());
        outEvents.pop();
    }
    
    return v;
}

void RenderThread::PushEvents(std::queue<EventData*>& events)
{
	lock_guard<mutex> lock(inEventsLock);
	
	while (!events.empty())
	{        
		inEvents.push(events.front());
		events.pop();
	}
}

void RenderThread::operator()()
{
    if (initStage && !initialization())
    {
        return;
    }
    
    while (openGLManager->WindowOpen())
    {
        handleEvents();
		
		// Update
		gameTime.Update(openGLManager->GetTime());
		GfxEventManager::Instance().Update(gameTime);
        
        if (!openGLManager->WindowOpen())
            break;
        
        // Setup screen
        GfxCache::Instance().SetFBO(GL_DRAW_FRAMEBUFFER, 0);
        openGLManager->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

        //And draw/render
	

        // Swap buffers
        GfxCache::Instance().SetFBO(GL_DRAW_FRAMEBUFFER, 0);
        openGLManager->SwapBuffers();
    }
    
    while (handleEvents()) { } // Handle all the remaining Events

    cleanup();
}

void RenderThread::ProcessGfxEvent(EventData& e)
{
	// Buffer them
	outEventsBuffer.push(static_cast<GfxInPassEventData&>(e).TakeEvent());
}

void RenderThread::ProcessEvent()
{
	// Blabla
}

bool RenderThread::initialization()
{
    gfxInListener = ScopedGEMListener(this, &RenderThread::ProcessGfxEvent, GfxInPassEventData::GfxInPassEventType());
    openGLManager = new GLManager();
    
#ifdef VERSION
    const string title = string("GlakkeGame - ") + VERSION;
#else
    const string title = "GlakkeGame - Pre-Alpha";
#endif
    
    GLSettings settings;
    settings.Title = title;
    
    if (!openGLManager->Init())
        return false;
    
    openGLManager->SetSettings(settings);
    
    if(!openGLManager->OpenWindow())
        return false;    
    
    if(glewInit() != GLEW_OK)
        return false;
    
    ilInit();

    if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
        return false;
    
    iluInit();
    
    if (iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION)
        return false;
    
    initStage = false;
    return true;
}

void RenderThread::cleanup()
{
    openGLManager->GLFWTerminate();
    GfxCache::Instance().Terminate();
    gfxInListener.Reset();
    if (openGLManager != nullptr)
    {
        delete openGLManager;
        openGLManager = nullptr;
    }
    GfxEventManager::Instance().Terminate();
    
    initStage = true;
}

bool RenderThread::handleEvents()
{
    bool handled = false;
    {
        // Push buffered events to outEvents
        lock_guard<mutex> lock(outEventsLock);
        while (!outEventsBuffer.empty())
        {
            outEvents.push(outEventsBuffer.front());
            outEventsBuffer.pop();
            handled = true;
        }
    }
    
    {
        // Pull new events
        lock_guard<mutex> lock(inEventsLock);
        while (!inEvents.empty())
        {
            GfxEventManager::Instance().QueueEvent(inEvents.front());
            inEvents.pop();
            handled = true;
        }
    }
    
    return handled;
}