/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "GlakkeGame.hpp"
#include "States/OrthoState.hpp"

#include <States/StateManager.hpp>
#include <Input/InputManager.hpp>

using namespace kke;

bool GlakkeGame::init()
{
	stateManager->AddState(new OrthoState(stateManager.get()));
	LOGE() << "INITTT\n";
	return true;
}