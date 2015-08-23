/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_GAMEAPP_HPP
#define KKE_GAMEAPP_HPP

#include "Events/ScopedListener.hpp"

// GLEW must be included before GL
#include <GL/glew.h>
#include <GL/glfw.h>
#include <memory>

namespace kke
{
	class GameTime;
	class InputManager;
	class StateManager;
	class ArgParser;
	
	class GameApp
	{
	public:
		GameApp();
		~GameApp();

		/**
		* @brief Game application main loop.
		*
		* @param argc Number of arguments.
		* @param argv Arguments.
		* @return int Result.
		**/
		int Execute (int argc, char** argv);
		
		virtual void ProcessEvent(EventData& event);
		
	protected:
		virtual bool registerArgs();
		virtual bool init();	
		virtual void update();
		virtual void draw();
		virtual bool clean();
		
		std::shared_ptr<StateManager> stateManager;
		std::shared_ptr<GameTime> gameTime;
		std::shared_ptr<InputManager> inputManager;
        ScopedEMListener winCloseReqListen, winClosedListen;
		
		bool appRunning;
		
	private:

		bool bRegisterArgs();
		bool bHandleArgs ();

		/**
		* @brief Initialization.
		*
		* @return bool Result.
		**/
		bool bInit();

		/**
		* @brief Clean up.
		*
		* @return void
		**/
		void bClean();
	};
}
#endif // KKE_GAMEAPP_HPP
