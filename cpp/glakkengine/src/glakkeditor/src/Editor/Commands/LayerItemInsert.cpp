/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LayerItemInsert.hpp"
#include "Level/LayerItem.hpp"
#include "Level/Layer.hpp"

#include <algorithm>

using namespace kke;
using std::find;

LayerItemInsert::LayerItemInsert(LayerItem* levelItem, LayerItem *position, Layer* layer, QUndoCommand* parent) : QUndoCommand(parent),
	item(levelItem), position(position), layer(layer)
{
	setText("Insert LayerItem");
}

LayerItemInsert::~LayerItemInsert()
{
	if (find(layer->Items().begin(), layer->Items().end(), item) == layer->Items().end())
	{
		item->UnloadContent();
		delete item;
	}
}

void LayerItemInsert::redo()
{
	layer->Items().insert(
		find(layer->Items().begin(), layer->Items().end(), position),
		item);
}

void LayerItemInsert::undo()
{
	layer->Items().erase(find(layer->Items().begin(), layer->Items().end(), item));
}