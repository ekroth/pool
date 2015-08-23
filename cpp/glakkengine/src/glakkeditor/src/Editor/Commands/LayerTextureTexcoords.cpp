/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LayerTextureTexcoords.hpp"
#include "Level/LayerTexture.hpp"

using namespace kke;

LayerTextureTexcoords::LayerTextureTexcoords(LayerTexture* layerTexture, const Rectf& texcoords, QUndoCommand* parent) : QUndoCommand(parent),
	layerTexture(layerTexture), oldTexcoords(layerTexture->GetTexcoords()), newTexcoords(texcoords)
{
	setText("Change LayerTexture texcoords");
}

void LayerTextureTexcoords::redo()
{
	layerTexture->SetTexcoords(newTexcoords);
}

void LayerTextureTexcoords::undo()
{
	layerTexture->SetTexcoords(oldTexcoords);
}