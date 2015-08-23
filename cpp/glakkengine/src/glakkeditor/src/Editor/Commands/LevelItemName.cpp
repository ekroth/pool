/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LevelItemName.hpp"
#include "Level/LevelItem.hpp"

using namespace kke;

using std::string;

LevelItemName::LevelItemName(LevelItem* levelItem, const string& name, QUndoCommand* parent) : QUndoCommand(parent), 
	levelItem(levelItem), oldName(levelItem->GetName()), newName(name)
{
	setText("Change LevelItem name");
}

void LevelItemName::redo()
{
	levelItem->SetName(newName);
}

void LevelItemName::undo()
{
	levelItem->SetName(oldName);
}