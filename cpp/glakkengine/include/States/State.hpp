/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_STATE_HPP
#define KKE_STATE_HPP

#include "Graphics/IRenderer.hpp"
#include "Interface/ILoadable.hpp"
#include "Interface/IInputable.hpp"
#include "Interface/IUpdatable.hpp"
#include "Interface/IRenderable.hpp"

#include "GameObject.hpp"
#include <glm/glm.hpp>

namespace kke
{
	class StateManager;
	class InputManager;
	class ResourceManager;
	class GameTime;

	class State
	{
	public:
		explicit State(StateManager* stateManager);
		virtual ~State();

		virtual void LoadContent();
		/**
		* @brief Unloading all resources.
		*
		* @return void
		**/
		virtual void UnloadContent();

		/**
		* @brief Manage input.
		*
		* @param inputManager Input manager.
		* @param elapsed Elapsed time in seconds.
		* @return void
		**/
		virtual void HandleInput(InputManager& inputManager, GameTime &gameTime)
		{
		}

		/**
		* @brief Update state.
		*
		* @param elapsed Elapsed time in seconds.
		* @return void
		**/
		virtual void Update(GameTime& gameTime)
		{
		}

		/**
		* @brief Draw state.
		*
		* @param renderer Renderer.
		* @return void
		**/
		virtual void Draw(GameTime& gameTime)
		{
		}

		// If the State has been Loaded.
		bool GetIsLoaded() const;
		// Set the state to loaded.
		void SetIsLoaded(bool value);

		// If the State needs input.
		bool GetWantFocus() const;
		// Color the screen should be cleared with.
		const glm::vec4& GetClearColor() const;
		// Which stage in loading the State currently is.

		int LoadingStage() const;
		// Get amount of loading stages supported.
		int GetLoadStages() const;
	protected:
		// Exits the State, removes it from StateManager.
		void ExitState();

		// Color when clearing
		glm::vec4 clearColor;
		bool wantFocus;
		// Amount of times LoadContent can be called
		// until the loading is complete.
		int loadingStages;
		int currLoadingStage;

		StateManager *stateManager;
		bool isLoaded;
	private:
	};
}

#endif // KKE_STATE_HPP
