/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LayerSpeed.hpp"
#include "Level/Layer.hpp"

using namespace kke;

LayerSpeed::LayerSpeed(Layer* layer, const glm::vec2& speed, QUndoCommand* parent) : QUndoCommand(parent), 
	layer(layer), oldSpeed(layer->GetSpeed()), newSpeed(speed)
{
	setText("Change Layer speed");
}

void LayerSpeed::redo()
{
	layer->SetSpeed(newSpeed);
}

void LayerSpeed::undo()
{
	layer->SetSpeed(oldSpeed);
}