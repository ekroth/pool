/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LayerTextureSource.hpp"
#include "Level/LayerTexture.hpp"

using namespace kke;

LayerTextureSource::LayerTextureSource(LayerTexture* layerTexture, const std::string& source, QUndoCommand* parent) : QUndoCommand(parent),
	layerTexture(layerTexture), oldSource(layerTexture->GetSource()), newSource(source)
{
	setText("Change LayerTexture source");
}

void LayerTextureSource::redo()
{
	layerTexture->SetSource(newSource);
}

void LayerTextureSource::undo()
{
	layerTexture->SetSource(oldSource);
}