/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_GLMANAGER_HPP
#define KKE_GLMANAGER_HPP

#include "Interface/INoncopyable.hpp"
#include "Events/EventData.hpp"
#include "Events/ScopedListener.hpp"
#include "Utilities/Singleton.hpp"
#include "Graphics/GfxEventManager.hpp"

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <stack>

#include <GL/glfw.h>
#include <string>
#include <map>
#include <vector>

namespace kke
{
    class EventData;
    
    struct GLSettings
    {
        GLSettings(const glm::ivec2& res = glm::ivec2(640, 480), const glm::vec4& clear = glm::vec4(1.f, 1.f, 1.f, 1.f), bool fullscreen = false, 
                   bool dynFps = true, char activeFps = 60, char inactiveFps = 30, const std::string& title = "Untitled Game");
        
        bool operator==(const GLSettings& s) const;
        bool operator!=(const GLSettings& s) const;
        
        glm::ivec2 Resolution;
        glm::vec4 ClearColor;
        bool Fullscreen;
        bool DynamicFPS;
        char ActiveFPS;
        char InactiveFPS;
        std::string Title;
    };
    
	namespace render
	{
		class GLManager
		{
		public:
			GLManager();
			~GLManager();

			bool Init();
			void GLFWTerminate();

			bool OpenWindow();
			void CloseWindow();
			void IconifyWindow() const;
			void RestoreWindow() const;
            bool WindowOpen() const;
            
            const GLSettings& GetSettings() const;
            void SetSettings(const GLSettings& s);
            bool ApplySettings();

			void SwapBuffers();

			void SetWindowPosition (int x, int y);
			void SetTime (double time);

			void SetOptionRefreshRate (int refresh);
			void SetOptionResize (bool enable);
			void SetOptionMinGLMajor (int version);
			void SetOptionMinGLMinor (int version);
			void SetOptionGLForward (bool enable);
			void SetOptionDebugContext (bool enable);

			void SetOptionRedBits (int bits);
			void SetOptionGreenBits (int bits);
			void SetOptionBlueBits (int bits);
			void SetOptionAlphaBits (int bits);

			void GetResolution (int& width, int& height);
			bool ExtensionSupported (const std::string& ext) const;
			void GetGLVersion (int& major, int& minor, int& rev) const;
			void GetGLFWVersion (int& major, int& minor, int& rev) const;
			std::vector<GLFWvidmode> GetVideoModes (int size = 28) const;
			GLFWvidmode GetDesktopMode() const;
			double GetTime() const;
            int GetNumberOfCPUs() const;

			void Clear (GLbitfield bits);

			bool ErrorCheck() const;

			int GetWindowWidth() const;
			int GetWindowHeight() const;
            
            void ProcessEvent(EventData& e);

		private:
			// Callback
			static int GLFWCALL windowCloseCallback();
			static void GLFWCALL windowSizeCallback (int width, int height);
			static void GLFWCALL keyCallback (int key, int action);
			static void GLFWCALL charCallback (int key, int action);
			static void GLFWCALL mouseButtonCallback (int key, int action);
			static void GLFWCALL mousePositionCallback (int x, int y);
			static void GLFWCALL mouseWheelCallback (int pos);

		private:
			GLSettings settings;
            
            ScopedGEMListener openWinListen, closeWinListen, settingsListen;
		};
	}

	class KeyEventData : public EventData
	{
	public:
		KeyEventData (int key, int action,
		              const EventType& type, const EventTime& time);

		int GetKey() const { return key; }
		int GetAction() const { return action; }

	private:
		int key;
		int action;
	};

	class MousePositionEventData : public EventData
	{
	public:
		MousePositionEventData (int x, int y,
		                        const EventType& type, const EventTime& time);

		int GetX() const { return x; }
		int GetY() const { return y; }

	private:
		int x;
		int y;
	};

	class MouseWheelEventData : public EventData
	{
	public:
		MouseWheelEventData (int value,
		                     const EventType& type, const EventTime& time);

		int GetValue() const { return value; }

	private:
		int value;
	};

	class WindowResizeEventData : public EventData
	{
	public:
		WindowResizeEventData (int width, int height, const EventType& type, const EventTime& time);

		int GetWidth() const { return width; }
		int GetHeight() const { return height; }

	private:
		int width;
		int height;
	};
    
    class GLSettingsEventData : public EventData
    {
    public:
        GLSettingsEventData (GLSettings s, const EventType& type, const EventTime& time);

        const GLSettings& GetSettings() const { return settings; }
        
    private:
        GLSettings settings;
    };
}

#endif // KKE_GLMANAGER_HPP