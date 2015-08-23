/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LEVELNAME_HPP
#define KKE_LEVELNAME_HPP

#include <string>
#include <QtGui/QUndoCommand>

namespace kke
{
	class Level;
	class LevelName : public QUndoCommand
	{
	public:
		LevelName(Level* level, const std::string& name, QUndoCommand* parent = nullptr);
		
		void redo();
		void undo();
	private:
		Level *level;
		std::string oldName;
		std::string newName;
	};
}

#endif // KKE_LEVELNAME_HPP
