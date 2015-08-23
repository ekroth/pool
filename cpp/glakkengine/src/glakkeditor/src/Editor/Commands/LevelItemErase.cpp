/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LevelItemErase.hpp"
#include "Level/Level.hpp"
#include "Level/LevelItem.hpp"

#include <algorithm>

using namespace kke;
using std::find;

LevelItemErase::LevelItemErase(LevelItem* levelItem, Level* level, QUndoCommand* parent) : QUndoCommand(parent),
	item(levelItem), position(position), level(level)
{
	setText("Insert LevelItem");
	LevelItems::iterator it = find(level->Items().begin(), level->Items().end(), item);
	
	if (it != level->Items().begin())
		it--;
	
	position = *it;
}

LevelItemErase::~LevelItemErase()
{
	if (find(level->Items().begin(), level->Items().end(), item) == level->Items().end())
	{
		item->UnloadContent();
		delete item;
	}
}

void LevelItemErase::redo()
{
	level->Items().erase(find(level->Items().begin(), level->Items().end(), item));
}

void LevelItemErase::undo()
{
	level->Items().insert(
		find(level->Items().begin(), level->Items().end(), position),
		item);
}
