/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LayerTextureOrigin.hpp"
#include "Level/LayerTexture.hpp"

using namespace kke;

LayerTextureOrigin::LayerTextureOrigin(LayerTexture* layerTexture, const glm::vec2& origin, QUndoCommand* parent) : QUndoCommand(parent),
	layerTexture(layerTexture), oldOrigin(layerTexture->GetOrigin()), newOrigin(origin)
{
	setText("Change LayerTexture origin");
}

void LayerTextureOrigin::redo()
{
	layerTexture->SetOrigin(newOrigin);
}

void LayerTextureOrigin::undo()
{
	layerTexture->SetOrigin(oldOrigin);
}