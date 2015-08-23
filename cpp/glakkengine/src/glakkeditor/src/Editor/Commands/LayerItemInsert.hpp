/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYERITEMINSERT_HPP
#define KKE_LAYERITEMINSERT_HPP

#include <QtGui/QUndoCommand>

namespace kke
{
	class Layer;
	class LayerItem;
	class LayerItemInsert : public QUndoCommand
	{
	public:
		LayerItemInsert(LayerItem* levelItem, LayerItem* position, Layer* layer, QUndoCommand* parent = nullptr);
		~LayerItemInsert();
		
		virtual void redo() = 0;
		virtual void undo() = 0;
	protected:
		LayerItem *item, *position;
		Layer *layer;
	};
}

#endif // KKE_LAYERITEMINSERT_HPP
