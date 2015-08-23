/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Level/LayerItem.hpp"
#include "Level/Layer.hpp"

using namespace kke;

using std::string;

LayerItem::LayerItem(const string& name, Layer* layer) : LevelItem(name), layer(layer)
{
	if (layer != nullptr)
		SetLevel(layer->GetLevel());
}

LayerItem::~LayerItem()
{
	UnloadContent();
}

void LayerItem::SetLayer (Layer* layer)
{
	this->layer = layer;
}

Layer* LayerItem::GetLayer()
{
	return layer;
}

void LayerItem::LoadContent()
{
	isLoaded = true;
}

void LayerItem::UnloadContent()
{
	isLoaded = false;
}

void LayerItem::Update (GameTime& gametime)
{
	kke::LevelItem::Update (gametime);
}

void LayerItem::Draw (IRenderer& renderer)
{
	kke::LevelItem::Draw (renderer);
}