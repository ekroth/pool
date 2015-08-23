/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LEVELITEMINSERT_HPP
#define KKE_LEVELITEMINSERT_HPP

#include <QtGui/QUndoCommand>

namespace kke
{
	class Level;
	class LevelItem;
	class LevelItemInsert : public QUndoCommand
	{
	public:
		LevelItemInsert(LevelItem* levelItem, LevelItem* position, Level* level, QUndoCommand* parent = nullptr);
		~LevelItemInsert();
		
		virtual void redo() = 0;
		virtual void undo() = 0;
	protected:
		LevelItem *item, *position;
		Level *level;
	};
}

#endif // KKE_LEVELITEMINSERT_HPP
