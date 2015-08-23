/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYERITEMERASE_HPP
#define KKE_LAYERITEMERASE_HPP

#include <QtGui/QUndoCommand>

namespace kke
{
	class Layer;
	class LayerItem;
	class LayerItemErase : public QUndoCommand
	{
	public:
		LayerItemErase(LayerItem* levelItem, Layer* layer, QUndoCommand* parent = nullptr);
		~LayerItemErase();
		
		virtual void redo() = 0;
		virtual void undo() = 0;
	protected:
		LayerItem *item, *position;
		Layer *layer;
	};
}

#endif // KKE_LAYERITEMERASE_HPP
