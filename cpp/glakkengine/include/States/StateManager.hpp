/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_STATEMANAGER_HPP
#define KKE_STATEMANAGER_HPP

#include "Graphics/IRenderer.hpp"
#include "Interface/IInputable.hpp"

#include <vector>

namespace kke
{
	class InputManager;
	class State;
	class GameTime;
	
	typedef std::vector<State*> StateVector;

	/**
	* @brief Manages multiple states.
	**/
	class StateManager
	{
	public:
		StateManager();
		~StateManager();

		/**
		* @brief Handle input. Gives input to the first state which wants it.
		*
		* @param inputManager Input manager.
		* @param elapsed Elapsed time in seconds.
		* @return void
		**/
		void HandleInput(InputManager& inputManager, GameTime& gameTime);

		/**
		* @brief Updates all the states, in order.
		*
		* @param elapsed Elapsed time in seconds.
		* @return void
		**/
		void Update(GameTime& gameTime);

		/**
		* @brief Draw states, in order.
		*
		* @return void
		**/
		void Draw(GameTime& gameTime);

		/**
		* @brief Add a state.
		*
		* @param state State to add.
		* @param slowload If a loading screen should be used. Defaults to false.
		* @return void
		**/
		void AddState(State* state, bool slowload = false);
		void AddState(StateVector states, bool slowload = false);

		/**
		* @brief Add a state which has been loaded.
		*
		* @param state State to add.
		* @return bool If action was successful.
		**/
		bool AddReadyState(State* state);

		/**
		* @brief Unloads and removes a state.
		*
		* @param state State to remove.
		* @return bool If action was successful.
		**/
		bool RemoveState(State* state);

		/**
		* @brief Unloads and removes all present states.
		*
		* @return void
		**/
		void RemoveAllStates();

		const IRenderer& GetRenderer() const;
	private:
		// vector of used states.
		StateVector states;
		IRenderer* renderer;
	};
}
#endif // KKE_STATEMANAGER_HPP
