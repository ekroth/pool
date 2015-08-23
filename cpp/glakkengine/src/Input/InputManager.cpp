/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/GLManager.hpp"
#include "Input/InputManager.hpp"

#include "Events/EventManager.hpp"

#include <map>
#include <algorithm>

using namespace kke;

using glm::vec2;
using std::vector;

InputManager::InputManager() :
	mouseSpeed(0, 0),
	lastMouseSpeed(0, 0),
	mousePosition(0, 0),
	lastMousePosition(0, 0),
	mouseWheel(0),
	lastMouseWheel(0),
	events({ 
        ScopedEMListener(this, &InputManager::ProcessEvent, EventHasher("KeyboardButton")), 
        ScopedEMListener(this, &InputManager::ProcessEvent, EventHasher("KeyboardChar")), 
        ScopedEMListener(this, &InputManager::ProcessEvent, EventHasher("MouseButton")), 
        ScopedEMListener(this, &InputManager::ProcessEvent, EventHasher("MousePosition")), 
        ScopedEMListener(this, &InputManager::ProcessEvent, EventHasher("MouseWheel")) })
{ 
}

InputManager::~InputManager()
{}

void InputManager::ProcessEvent (EventData& event)
{
	if (event.GetType() == EventHasher("KeyboardButton"))
	{
		KeyEventData &data = (KeyEventData&)event;
		keyCallback(data.GetKey(), data.GetAction());
	}
	else if (event.GetType() == EventHasher("KeyboardChar"))
	{
		KeyEventData &data = (KeyEventData&)event;
		charCallback(data.GetKey(), data.GetAction());
	}
	else if (event.GetType() == EventHasher("MousePosition"))
	{
		MousePositionEventData &data = (MousePositionEventData&)event;
		mousePCallback(data.GetX(), data.GetY());
	}
	else if (event.GetType() == EventHasher("MouseWheel"))
	{
		MouseWheelEventData &data = (MouseWheelEventData&)event;
		mouseWCallback(data.GetValue());
	}
}

void InputManager::HandleInput(float elapsed)
{
	for (auto key : keys)
	{
		const int value = static_cast<int>(key.second);
		
		if (value == -2)
			key.second = -1.0f; // New key
		else if (key.second > -1.0 && key.second < 0.0f)
			key.second = 0.0f + elapsed;
		else
			key.second += elapsed;
	}
}

// Keyboard

bool InputManager::KeyDown(InputKey key) const
{
	KeyMap::const_iterator it = keys.find(key);
	return it != keys.end() ? it->second > -1.0f : false;
}

bool InputManager::KeyUp(InputKey key) const
{
	KeyMap::const_iterator it = keys.find(key);
	return it != keys.end();
}

bool InputManager::KeyNew(InputKey key) const
{
	KeyMap::const_iterator it = keys.find(key);
	return it != keys.end() ? (int)(it->second + 0.5f) == -1 : false;
}

float InputManager::KeyValue(InputKey key) const
{
	KeyMap::const_iterator it = keys.find(key);
	return it != keys.end() ? it->second : 0.0f;
}

const vec2& InputManager::MousePosition() const
{
	return mousePosition;
}

const vec2& InputManager::LastMousePosition() const
{
	return lastMousePosition;
}

const vec2& InputManager::MouseSpeed() const
{
	return mouseSpeed;
}

const vec2& InputManager::LastMouseSpeed() const
{
	return lastMouseSpeed;
}

void GLFWCALL InputManager::keyCallback(int key, int action)
{
	const InputKey ikey = (InputKey)key;
	KeyMap::const_iterator it = keys.find(ikey);
	
	if (it != keys.end() && action != GLFW_PRESS)
		keys.erase(ikey);
	else
		keys[ikey] = -2.0f;
}

void GLFWCALL InputManager::charCallback(int key, int action)
{
// 	Input_Key ikey = (Input_Key)key;
// 	if (!Instance()->KeyDown((ikey)))
// 	  Instance()->keys[ikey]  == GLFW_PRESS ? -3.0f : -4.0f;

  // TODO: Implement for text or something?
}

void GLFWCALL InputManager::mousePCallback(int x, int y)
{
	lastMousePosition = mousePosition;
	mousePosition = vec2(x, y);
}

void GLFWCALL InputManager::mouseWCallback(int pos)
{
	lastMouseWheel = mouseWheel;
	mouseWheel = pos;
}