/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LevelItemInsert.hpp"
#include "Level/Level.hpp"
#include "Level/LevelItem.hpp"

#include <algorithm>

using namespace kke;
using std::find;

LevelItemInsert::LevelItemInsert (LevelItem* levelItem, LevelItem* position, Level* level, QUndoCommand* parent) : QUndoCommand(parent),
	item(levelItem), position(position), level(level)
{
	setText("Insert LevelItem");
}

LevelItemInsert::~LevelItemInsert()
{
	if (find(level->Items().begin(), level->Items().end(), item) == level->Items().end())
	{
		item->UnloadContent();
		delete item;
	}
}

void LevelItemInsert::redo()
{
	level->Items().insert(
		find(level->Items().begin(), level->Items().end(), position),
		item);
}

void LevelItemInsert::undo()
{
	level->Items().erase(find(level->Items().begin(), level->Items().end(), item));
}
