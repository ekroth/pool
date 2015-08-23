/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Editor/Commands/LayerItemErase.hpp"
#include "Level/LayerItem.hpp"
#include "Level/Layer.hpp"

#include <algorithm>

using namespace kke;
using std::find;

LayerItemErase::LayerItemErase(LayerItem* levelItem, Layer* layer, QUndoCommand* parent) : QUndoCommand(parent),
	item(levelItem), layer(layer)
{
	setText("Erase LayerItem");
	LayerItems::iterator it = find(layer->Items().begin(), layer->Items().end(), item);
	
	if (it != layer->Items().begin())
		it--;
	
	position = *it;
}

LayerItemErase::~LayerItemErase()
{
	if (find(layer->Items().begin(), layer->Items().end(), item) == layer->Items().end())
	{
		item->UnloadContent();
		delete item;
	}
}

void LayerItemErase::redo()
{
	layer->Items().erase(find(layer->Items().begin(), layer->Items().end(), item));
}

void LayerItemErase::undo()
{
	layer->Items().insert(
		find(layer->Items().begin(), layer->Items().end(), position),
		item);
}