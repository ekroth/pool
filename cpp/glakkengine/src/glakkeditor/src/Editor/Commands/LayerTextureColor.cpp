/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LayerTextureColor.hpp"
#include "Level/LayerTexture.hpp"

using namespace kke;

LayerTextureColor::LayerTextureColor(LayerTexture* layerTexture, const glm::vec4* color, QUndoCommand* parent) : QUndoCommand(parent),
	layerTexture(layerTexture)
{
	setText("Change LayerTexture color");
	
	for (int i = 0; i < 4; i++)
	{
		oldColor[i] = layerTexture->GetColor(i);
		newColor[i] = color[i];
	}
}

void LayerTextureColor::redo()
{
	for (int i = 0; i < 4; i++)
	{
		layerTexture->SetColor(i, newColor[i]);
	}
}

void LayerTextureColor::undo()
{
	for (int i = 0; i < 4; i++)
	{
		layerTexture->SetColor(i, oldColor[i]);
	}
}