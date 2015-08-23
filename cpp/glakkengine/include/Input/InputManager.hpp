/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "Interface/INoncopyable.hpp"
#include "Events/ScopedListener.hpp"

#include <map>
#include <GL/glfw.h>
#include <vector>


namespace kke
{
	typedef int InputKey;
	
	/**
	 * @brief Manages input.
	 **/
	class InputManager : public INoncopyable
	{
	public:
		InputManager();
		~InputManager();
		
		void ProcessEvent (EventData& event);

		/**
		* @brief Update the input manager.
		*
		* @param elapsed Elapsed time in seconds.
		* @return void
		**/
		void HandleInput(float elapsed);

		/**
		* @brief If key is down.
		*
		* @param key Keyboard key.
		* @return bool
		**/
		bool KeyDown(InputKey key) const;
		
		/**
		* @brief If key is up.
		*
		* @param key Keyboard key.
		* @return bool
		**/
		bool KeyUp(InputKey key) const;
		
		/**
		* @brief If key was pressed this update/frame.
		*
		* @param key Keyboard key.
		* @return bool
		**/
		bool KeyNew(InputKey key) const;

		/**
		* @brief Return value of key.
		* -4.0f = glfw reported up
		* -3.0f = glfw reported down
		* -2.0f = InputManager set up
		* -1.0f = InputManager set down
		* > 0.1f = How long down
		*
		* @param key Keyboard key.
		* @return float Time in seconds.
		**/
		float KeyValue(InputKey key) const;

		/**
		* @brief Mouse position.
		*
		* @return :Vector2&
		**/
		const glm::vec2& MousePosition() const;
		
		/**
		* @brief Previous mouse position.
		*
		* @return :Vector2&
		**/
		const glm::vec2& LastMousePosition() const;
		
		/**
		* @brief Mouse speed (pixels / second).
		*
		* @return :Vector2&
		**/
		const glm::vec2& MouseSpeed() const;
		
		/**
		* @brief Previous mouse speed (pixels / second).
		*
		* @return :Vector2&
		**/
		const glm::vec2& LastMouseSpeed() const;
		
	private:
		// Keyboard 
		typedef std::map<InputKey, float> KeyMap;
		typedef std::pair<InputKey, float> KeyPair;

		void GLFWCALL keyCallback(int key, int action);
		void GLFWCALL charCallback(int key, int action);
		void GLFWCALL mousePCallback(int x, int y);
		void GLFWCALL mouseWCallback(int pos);

		KeyMap keys;

		// Mouse
		glm::vec2 mouseSpeed,
			lastMouseSpeed,
			mousePosition,
			lastMousePosition;

		int mouseWheel,
			lastMouseWheel;
			
		ScopedEMListener events[5];
	};
}

#endif

