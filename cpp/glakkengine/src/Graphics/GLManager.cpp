/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/GLManager.hpp"
#include "IO/LogManager.hpp"
#include "Graphics/GfxEventManager.hpp"
#include "Graphics/RenderThread.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <GameTime.hpp>

using namespace kke;
using namespace kke::render;

using std::string;
using std::stack;
using std::map;
using std::vector;

using glm::vec4;

GLManager::GLManager() : 
  settings()
{ 
}

GLManager::~GLManager()
{ }

bool GLManager::Init()
{
	bool result =  glfwInit() == GL_TRUE;
    
    if (result)
    {
        openWinListen = ScopedGEMListener(this, &GLManager::ProcessEvent, EventHasher("OpenWindow"));
        closeWinListen = ScopedGEMListener(this, &GLManager::ProcessEvent, EventHasher("CloseWindow"));
        settingsListen = ScopedGEMListener(this, &GLManager::ProcessEvent, EventHasher("GLSettings"));
    }
    
    return result;
}

void GLManager::GLFWTerminate()
{
    openWinListen.Reset();
    closeWinListen.Reset();
    settingsListen.Reset();
	glfwTerminate();
}


bool GLManager::OpenWindow()
{
	auto val = glfwOpenWindow(settings.Resolution.x, settings.Resolution.y, 8, 8, 8, 
						  8, 8, 8, settings.Fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW);
	
	if (val == GL_TRUE)
	{
		glfwSetWindowCloseCallback(&GLManager::windowCloseCallback);
		glfwSetWindowSizeCallback(&GLManager::windowSizeCallback);
		glfwSetKeyCallback(&GLManager::keyCallback);
		glfwSetCharCallback(&GLManager::charCallback);
		glfwSetMouseButtonCallback(&GLManager::mouseButtonCallback);
		glfwSetMousePosCallback(&GLManager::mousePositionCallback);
		glfwSetMouseWheelCallback(&GLManager::mouseWheelCallback);
		
		return true;
	}
	
	return false;
}

void GLManager::CloseWindow()
{
	glfwCloseWindow();
    EventData winClosed(EventHasher("WindowClosed"), glfwGetTime());
    GfxEventManager::Instance().Trigger(winClosed);
    GfxInPassEventData passWinClosed(new EventData(EventHasher("WindowClosed"), glfwGetTime()), glfwGetTime());
    GfxEventManager::Instance().Trigger(passWinClosed);
}

void GLManager::IconifyWindow() const
{
	glfwIconifyWindow();
}

void GLManager::RestoreWindow() const
{
	glfwRestoreWindow();
}

bool GLManager::WindowOpen() const
{
    return glfwGetWindowParam(GLFW_OPENED) == GL_TRUE;
}

void GLManager::SwapBuffers()
{
	if (settings.DynamicFPS)
	{
		if (glfwGetWindowParam(GLFW_ACTIVE) == GL_FALSE)
			glfwSwapInterval(settings.InactiveFPS);
		else
			glfwSwapInterval(settings.ActiveFPS);
	}
	
	ErrorCheck();
	glfwSwapBuffers();
}

const GLSettings& GLManager::GetSettings() const
{
    return settings;
}

void GLManager::SetSettings(const GLSettings& s)
{
    settings = s;
}

bool GLManager::ApplySettings()
{
    glfwSetWindowSize(settings.Resolution.x, settings.Resolution.y);
    glfwSwapInterval(settings.ActiveFPS);
    glfwSetWindowTitle(settings.Title.c_str());
    
    return OpenWindow();
}

void GLManager::SetWindowPosition(int x, int y)
{
	glfwSetWindowPos(x, y);
}

void GLManager::SetTime(double time)
{
	glfwSetTime(time);
}

void GLManager::SetOptionRefreshRate(int refresh)
{
	glfwOpenWindowHint(GLFW_REFRESH_RATE, refresh);
}

void GLManager::SetOptionResize(bool enable)
{
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, enable ? GL_FALSE : GL_TRUE);
}

void GLManager::SetOptionMinGLMajor(int version)
{
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, version);
}

void GLManager::SetOptionMinGLMinor(int version)
{
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, version);
}

void GLManager::SetOptionGLForward(bool enable)
{
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, enable ? GL_TRUE : GL_FALSE);
}

void GLManager::SetOptionDebugContext(bool enable)
{
	glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, enable ? GL_TRUE : GL_FALSE);
}

void GLManager::SetOptionRedBits(int bits)
{
	glfwOpenWindowHint(GLFW_ACCUM_RED_BITS, bits);
}

void GLManager::SetOptionGreenBits(int bits)
{
	glfwOpenWindowHint(GLFW_ACCUM_GREEN_BITS, bits);
}

void GLManager::SetOptionBlueBits(int bits)
{
	glfwOpenWindowHint(GLFW_ACCUM_BLUE_BITS, bits);
}

void GLManager::SetOptionAlphaBits(int bits)
{
	glfwOpenWindowHint(GLFW_ACCUM_ALPHA_BITS, bits);
}

void GLManager::GetResolution(int& width, int& height)
{
	glfwGetWindowSize(&width, &height);
}

bool GLManager::ExtensionSupported(const std::string& ext) const
{
	return glfwExtensionSupported(ext.c_str()) == GL_TRUE;
}

void GLManager::GetGLVersion(int& major, int& minor, int& rev) const
{
	glfwGetGLVersion(&major, &minor, &rev);
}

void GLManager::GetGLFWVersion(int& major, int& minor, int& rev) const
{
	glfwGetVersion(&major, &minor, &rev);
}

std::vector<GLFWvidmode> GLManager::GetVideoModes(int size) const
{
    std::vector<GLFWvidmode> vec;
	GLFWvidmode list[size];
	vec.reserve(size);
	
	glfwGetVideoModes(list, size);
	
	for (int i = 0; i < size; i++)
	  vec.push_back(list[i]);
    
    return vec;
}

GLFWvidmode GLManager::GetDesktopMode() const
{
	GLFWvidmode mode;
	glfwGetDesktopMode(&mode);
	
	return mode;
}

double GLManager::GetTime() const
{
	return glfwGetTime();
}

int GLManager::GetNumberOfCPUs() const
{
    return glfwGetNumberOfProcessors();
}

void GLManager::Clear(GLbitfield bits)
{
	glClear(bits);
}

bool GLManager::ErrorCheck() const
{
	bool aError = false;
	for (GLenum error = glGetError(); error != GL_NO_ERROR; error = glGetError())
	{
		bool newError = true;
		switch (error)
		{
			case GL_INVALID_ENUM:      
				LOGE() << "GL_INVALID_ENUM"; 
				break;
			case GL_INVALID_VALUE:
				LOGE() << "GL_INVALID_VALUE"; 
				break;
			case GL_INVALID_OPERATION:
				LOGE() << "GL_INVALID_OPERATION"; 
				break;
			case GL_STACK_OVERFLOW:
				LOGE() << "GL_STACK_OVERFLOW"; 
				break;
			case GL_STACK_UNDERFLOW:
				LOGE() << "GL_STACK_UNDERFLOW"; 
				break;
			case GL_OUT_OF_MEMORY:
				LOGE() << "GL_OUT_OF_MEMORY"; 
				break;
			default:
				newError = false;
				break;
		}
		
		switch (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
		{
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
				LOGE() << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
				LOGE() << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				LOGE() << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				LOGE() << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
				LOGE() << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
				LOGE() << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT";
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
				LOGE() << "GL_FRAMEBUFFER_UNSUPPORTED_EXT";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
				LOGE() << "GL_INVALID_FRAMEBUFFER_OPERATION_EXT";
				break;
			default:
				newError = true;
				break;
		}
		
		aError = aError || newError;
	}
	
	return !aError;
}

void GLManager::ProcessEvent(EventData& e)
{
    switch (e.GetType())
    {
        case EventHasher("OpenWindow"):
            break;
            
        case EventHasher("CloseWindow"):
            CloseWindow();
            break;
            
        case EventHasher("GLSettings"):
        {
            GLSettingsEventData& g = static_cast<GLSettingsEventData&>(e);
            
            settings = g.GetSettings();
            ApplySettings();
        }
            break;
    }
}

// Static
int GLFWCALL GLManager::windowCloseCallback()
{	
    // User wants to close the window, let someone handle the event and explicitly close later, but don't close now
	GfxEventManager::Instance().QueueEvent(new GfxInPassEventData(new EventData(EventHasher("WindowCloseRequest"), glfwGetTime()), glfwGetTime()));
	return GL_FALSE;
}

// Static
void GLFWCALL GLManager::windowSizeCallback(int width, int height)
{
	GfxEventManager::Instance().QueueEvent(new GfxInPassEventData(new WindowResizeEventData(width, height, EventHasher("WindowResize"), glfwGetTime()), glfwGetTime()));
}

void GLManager::charCallback (int key, int action)
{
	GfxEventManager::Instance().QueueEvent(new GfxInPassEventData(new KeyEventData(key, action, EventHasher("KeyboardChar"), glfwGetTime()), glfwGetTime()));
}

void GLManager::keyCallback (int key, int action)
{
	GfxEventManager::Instance().QueueEvent(new GfxInPassEventData(new KeyEventData(key, action, EventHasher("KeyboardButton"), glfwGetTime()), glfwGetTime()));
}

void GLManager::mouseButtonCallback (int key, int action)
{
	GfxEventManager::Instance().QueueEvent(new GfxInPassEventData(new KeyEventData(key, action, EventHasher("MouseButton"), glfwGetTime()), glfwGetTime()));
}

void GLManager::mousePositionCallback (int x, int y)
{
	GfxEventManager::Instance().QueueEvent(new GfxInPassEventData(new MousePositionEventData(x, y, EventHasher("MousePosition"), glfwGetTime()), glfwGetTime()));
}

void GLManager::mouseWheelCallback (int pos)
{
	GfxEventManager::Instance().QueueEvent(new GfxInPassEventData(new MouseWheelEventData(pos, EventHasher("MouseWheel"), glfwGetTime()), glfwGetTime()));
}

GLSettings::GLSettings(const glm::ivec2& res, const glm::vec4& clear, bool fullscreen, 
                   bool dynFps, char activeFps, char inactiveFps, const std::string& title) : 
    Resolution(res),
    ClearColor(clear),
    Fullscreen(fullscreen),
    DynamicFPS(dynFps),
    ActiveFPS(activeFps),
    InactiveFPS(inactiveFps),
    Title(title)
{

}

bool GLSettings::operator==(const GLSettings& s) const
{
    return (s.Resolution == Resolution && s.ClearColor == ClearColor && 
        s.Fullscreen == Fullscreen && s.DynamicFPS == DynamicFPS && s.ActiveFPS == ActiveFPS && s.InactiveFPS == InactiveFPS && s.Title == Title);
}

bool GLSettings::operator!=(const GLSettings& s) const
{
    return !(*this == s);
}

KeyEventData::KeyEventData (int key, int action, const EventType& type, const EventTime& time) : 
    EventData (type, time),
    key(key),
    action(action)
{ }

MousePositionEventData::MousePositionEventData (int x, int y, const EventType& type, const EventTime& time) : 
    EventData(type, time),
    x(x),
    y(y)
{ }

MouseWheelEventData::MouseWheelEventData (int value, const EventType& type, const EventTime& time) : 
    EventData(type, time),
    value(value)
{ }

WindowResizeEventData::WindowResizeEventData (int width, int height, const EventType& type, const EventTime& time) : 
    EventData (type, time),
    width(width),
    height(height)
{ }

GLSettingsEventData::GLSettingsEventData (GLSettings s, const EventType& type, const EventTime& time) : 
    EventData(type, time),
    settings(s)
{ }