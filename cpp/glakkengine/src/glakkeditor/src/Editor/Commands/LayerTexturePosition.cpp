/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LayerTexturePosition.hpp"
#include "Level/LayerTexture.hpp"

using namespace kke;

LayerTexturePosition::LayerTexturePosition(LayerTexture* layerTexture, const glm::vec3& position, QUndoCommand* parent) : QUndoCommand(parent),
	layerTexture(layerTexture), oldPosition(layerTexture->GetPosition()), newPosition(position)
{
	setText("Change LayerTexture position");
}

void LayerTexturePosition::redo()
{
	layerTexture->SetPosition(newPosition);
}

void LayerTexturePosition::undo()
{
	layerTexture->SetPosition(oldPosition);
}