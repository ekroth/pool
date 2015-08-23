/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "States/State.hpp"

#include "States/StateManager.hpp"
#include "ResourceFactory.hpp"
#include "GameTime.hpp"

#include <GL/glew.h>
#include <iostream>

using namespace kke;

using glm::vec4;

State::State(StateManager* stateManager) :
	clearColor(0.37647f, 0.50980f, 0.71372f, 1.0f), wantFocus(true),
		loadingStages(0), currLoadingStage(0), stateManager(stateManager),
		isLoaded(false)
{
}

State::~State()
{

}

void State::LoadContent()
{
	isLoaded = true;
}

void State::UnloadContent()
{
	isLoaded = false;
}

void State::ExitState()
{
	this->stateManager->RemoveState(this);
}

bool State::GetIsLoaded() const
{
	return isLoaded;
}

void State::SetIsLoaded(bool value)
{
	isLoaded = value;
}

bool State::GetWantFocus() const
{
	return wantFocus;
}

const vec4& State::GetClearColor() const
{
	return clearColor;
}

int State::LoadingStage() const
{
	return currLoadingStage;
}

int State::GetLoadStages() const
{
	return loadingStages;
}
