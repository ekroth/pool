/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Level/LevelItem.hpp"
#include "Level/Layer.hpp"

using namespace kke;

using std::string;

LevelItem::LevelItem (const std::string& name, Level* level) : isLoaded(false), name(name), level(level)
{

}

LevelItem::~LevelItem()
{
	UnloadContent();
}

const string& LevelItem::GetName() const
{
	return name;
}

void LevelItem::SetName (const string& name)
{
	this->name = name;
}

void LevelItem::SetLevel (Level* level)
{
	this->level = level;
}

Level* LevelItem::GetLevel()
{
	return level;
}

void* LevelItem::Copy() const
{
	return new LevelItem(name, level);
}

void LevelItem::LoadContent()
{
	isLoaded = true;
}

void LevelItem::UnloadContent()
{
	isLoaded = false;
}

void LevelItem::Update (GameTime& gameTime)
{

}

void LevelItem::Draw (IRenderer& renderer)
{

}