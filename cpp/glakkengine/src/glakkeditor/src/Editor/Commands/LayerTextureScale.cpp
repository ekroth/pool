/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LayerTextureScale.hpp"
#include "Level/LayerTexture.hpp"

using namespace kke;

LayerTextureScale::LayerTextureScale(LayerTexture* layerTexture, const glm::vec2& scale, QUndoCommand* parent) : QUndoCommand(parent),
	layerTexture(layerTexture), oldScale(layerTexture->GetScale()), newScale(scale)
{
	setText("Change LayerTexture scale");
}

void LayerTextureScale::redo()
{
	layerTexture->SetOrigin(newScale);
}

void LayerTextureScale::undo()
{
	layerTexture->SetOrigin(oldScale);
}