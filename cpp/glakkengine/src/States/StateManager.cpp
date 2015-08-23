/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "States/StateManager.hpp"

#include "States/State.hpp"
#include "ResourceFactory.hpp"
#include "GameTime.hpp"
#include "Input/InputManager.hpp"

#include "Graphics/Camera.hpp"

#include <algorithm>

using namespace kke;

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
	RemoveAllStates();
}

void StateManager::HandleInput(InputManager& inputManager, GameTime& gameTime)
{
	if (inputManager.KeyNew(GLFW_KEY_F12))
		ResourceFactory::Instance().ReloadAll();
	
	Camera::Instance().HandleInput(inputManager, gameTime);
	
	for (State*& state: states)
	{
		if (state->GetWantFocus())
		{
			// If the state wants focus, give it
			// input. Only one state may have it,
			// so return afterwards.

			state->HandleInput(inputManager, gameTime);
			break;
		}
	}
}

void StateManager::Update(GameTime& gameTime)
{
	// Copy pointers to what states that need to be updated
	// in case a State is removed during update.

	const unsigned int amount = states.size();
	State *statesUpdate[amount];
	
	std::copy(states.begin(), states.end(), statesUpdate);

	// And update the states
	for (unsigned int i = 0; i < amount; i++)
	{
		statesUpdate[i]->Update(gameTime);
	}
}

void StateManager::Draw(GameTime& gameTime)
{
	// Go through every State and Draw it.
	for (State*& state: states)
	{
		state->Draw(gameTime);
	}
}

void StateManager::AddState(State* state, bool slowload)
{
	if (slowload)
	{
// 		AddState(new LoadingState(state, this));
	}
	else
	{
		// Add the State
		states.push_back(state);
		// Load its content, single thread

		for (int i = 0; !state->GetIsLoaded(); i++)
		{
			if (state->LoadingStage() < i)
			{
				// If the loading stage has not changed
				// since last iteration, and it is still not
				// loaded - then it is stuck!
				// We must stop.
				LOGE() << "(StateManager) A State was stuck during loading!\nCheck that the stage sets isLoaded to true when done.";
			}

			state->LoadContent();

			if (state->GetLoadStages() == 0 && !state->GetIsLoaded())
			{
				// State doesn't support multiple loading stages
				// and the state doesn't set IsLoaded to true.
				// Will take for granted that it is actually loaded.

				state->SetIsLoaded(true);
			}
		}
	}

	// TODO: Reset InputManager timers when adding new state?
}

void StateManager::AddState (StateVector states, bool slowload)
{
	for (State*& it: states)
		AddState(it, slowload);
}

bool StateManager::AddReadyState(State* state)
{
	if (!state->GetIsLoaded())
		return false;

	states.push_back(state);

	return true;
}

bool StateManager::RemoveState(State* state)
{
	StateVector::iterator it = std::find(states.begin(), states.end(), state);

	if (it != states.end())
	{
		// Unload its content
		(*it)->UnloadContent();
		// Delete from heap
		delete (*it);
		// Remove from vector
		states.erase(it);

		// Successful
		return true;
	}

	// Did not find State
	return false;
}

void StateManager::RemoveAllStates()
{
	// Loop through every State
	for (State*& state: states)
	{
		// Unload and delete content
		state->UnloadContent();
		delete state;
	}

	states.clear();
}

const IRenderer& StateManager::GetRenderer() const
{
	return *renderer;
}
