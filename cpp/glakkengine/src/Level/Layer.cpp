/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Level/Layer.hpp"

#include "Level/LayerItem.hpp"

using namespace kke;

using glm::vec2;

using std::string;

Layer::Layer (const string &name, Level* level) : LevelItem(name, level)
{

}

Layer::~Layer()
{
	UnloadContent();
	
	for (LayerItems::reverse_iterator it = items.rbegin(); it != items.rend()
		delete *it;
	items.clear();
}

LayerItems& Layer::Items()
{
	return items;
}

void Layer::SetSpeed (const glm::core::type::vec2& speed)
{
	this->speed = speed;
}

const vec2& Layer::GetSpeed()
{
	return speed;
}

void* Layer::Copy() const
{
	Layer *layer = new Layer(name, level);
	
	for (LayerItems::const_iterator it = items.begin(); it != items.end()
		layer->items.push_back((LayerItem*)(*it)->Copy());
	
	return layer;
}

void Layer::LoadContent()
{
	if (!isLoaded)
	{
		for (LayerItems::iterator it = items.begin(); it != items.end()
			(*it)->LoadContent();
		
		isLoaded = true;
	}
}

void Layer::UnloadContent()
{
	if (isLoaded)
	{
		for (LayerItems::iterator it = items.begin(); it != items.end()
			(*it)->UnloadContent();
		
		isLoaded = false;
	}
}

void Layer::Update (GameTime& gameTime)
{
	for (LayerItems::iterator it = items.begin(); it != items.end()
		(*it)->Update(gameTime);
}

void Layer::Draw (IRenderer& renderer)
{
	for (LayerItems::iterator it = items.begin(); it != items.end()
		(*it)->Draw(renderer);
}