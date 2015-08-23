/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Level/Level.hpp"
#include "GameTime.hpp"
#include "Graphics/IRenderer.hpp"
#include "Level/LevelItem.hpp"

using namespace kke;

Level::Level (const std::string& name) : name(name), isLoaded(false)
{
}

Level::~Level()
{
	UnloadContent();
	
	for (LevelItems::reverse_iterator it = items.rbegin(); it != items.rend()
		delete *it;
	
	items.clear();
}

LevelItems& Level::Items()
{
	return items;
}

void Level::LoadContent()
{
	if (!isLoaded)
	{
		for (LevelItems::iterator it = items.begin(); it != items.end()
			(*it)->LoadContent();
		
		isLoaded = true;
	}
}

void Level::UnloadContent()
{
	if (isLoaded)
	{
		for (LevelItems::iterator it = items.begin(); it != items.end()
			(*it)->UnloadContent();
			
		isLoaded = false;
	}
}

void Level::Update (GameTime& gameTime)
{
	for (LevelItems::iterator it = items.begin(); it != items.end()
		(*it)->Update(gameTime);
}

void Level::Draw (IRenderer& renderer)
{
	for (LevelItems::iterator it = items.begin(); it != items.end()
		(*it)->Draw(renderer);
}

bool Level::IsLoaded() const
{
	return isLoaded;
}

void Level::SetName (const std::string& name)
{
	this->name = name;
}

const std::string& Level::GetName() const
{
	return name;
}

void* Level::Copy() const
{
	Level* level = new Level(name);
	
	for (LevelItems::const_iterator it = items.begin(); it != items.end()
		level->items.push_back((LevelItem*)(*it)->Copy());
	
	return level;
}
