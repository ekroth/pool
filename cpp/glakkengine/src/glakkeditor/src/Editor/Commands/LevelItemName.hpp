/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LEVELITEMNAME_HPP
#define KKE_LEVELITEMNAME_HPP

#include <string>
#include <QtGui/QUndoCommand>

namespace kke
{
	class LevelItem;
	class LevelItemName : public QUndoCommand
	{
	public:
		LevelItemName(LevelItem* levelItem, const std::string& name, QUndoCommand* parent = nullptr);
		
		void redo();
		void undo();
	private:
		LevelItem *levelItem;
		std::string oldName;
		std::string newName;
	};
}

#endif // KKE_LEVELITEMNAME_HPP
