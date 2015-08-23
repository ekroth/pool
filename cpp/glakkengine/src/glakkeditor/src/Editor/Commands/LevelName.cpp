/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LevelName.hpp"
#include "Level/Level.hpp"

using namespace kke;

using std::string;

LevelName::LevelName(Level* level, const string& name, QUndoCommand* parent) : QUndoCommand(parent), 
	level(level), oldName(level->GetName()), newName(name)
{
	setText("Change level name");
}

void LevelName::redo()
{
	level->SetName(newName);
}

void LevelName::undo()
{
	level->SetName(oldName);
}