/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LayerTextureRotation.hpp"
#include "Level/LayerTexture.hpp"

using namespace kke;

LayerTextureRotation::LayerTextureRotation(LayerTexture* layerTexture, float rotation, QUndoCommand* parent) : QUndoCommand(parent),
	layerTexture(layerTexture), oldRotation(layerTexture->GetRotation()), newRotation(rotation)
{
	setText("Change LayerTexture rotation");
}

void LayerTextureRotation::redo()
{
	layerTexture->SetRotation(newRotation);
}

void LayerTextureRotation::undo()
{
	layerTexture->SetRotation(oldRotation);
}