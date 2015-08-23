/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYERSPEED_HPP
#define KKE_LAYERSPEED_HPP

#include "glm/glm.hpp"

#include <QtGui/QUndoCommand>

namespace kke
{
	class Layer;
	class LayerSpeed : public QUndoCommand
	{
	public:
		LayerSpeed(Layer *levelItem, const glm::vec2 &speed, QUndoCommand *parent = nullptr);
		
		void redo();
		void undo();
	private:
		Layer *layer;
		glm::vec2 oldSpeed;
		glm::vec2 newSpeed;
	};
}

#endif // KKE_LAYERSPEED_HPP
