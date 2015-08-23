/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LEVELITEMERASE_HPP
#define KKE_LEVELITEMERASE_HPP

#include <QtGui/QUndoCommand>

namespace kke
{
	class Level;
	class LevelItem;
	class LevelItemErase : public QUndoCommand
	{
	public:
		LevelItemErase(LevelItem* levelItem, Level* level, QUndoCommand* parent = nullptr);
		~LevelItemErase();
		
		virtual void redo() = 0;
		virtual void undo() = 0;
	protected:
		LevelItem *item, *position;
		Level *level;
	};
}

#endif // KKE_LEVELITEMERASE_HPP
