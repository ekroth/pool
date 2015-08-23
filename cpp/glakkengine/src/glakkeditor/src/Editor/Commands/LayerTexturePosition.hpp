/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYERTEXTUREPOSITION_HPP
#define KKE_LAYERTEXTUREPOSITION_HPP

#include <glm/glm.hpp>
#include <QtGui/QUndoCommand>

namespace kke
{
	class LayerTexture;
	class LayerTexturePosition : public QUndoCommand
	{
	public:
		LayerTexturePosition(LayerTexture* layerTexture, const glm::vec3& position, QUndoCommand* parent = nullptr);
		
		void redo();
		void undo();
	private:
		LayerTexture *layerTexture;
		glm::vec3 oldPosition;
		glm::vec3 newPosition;
	};
}

#endif // KKE_LAYERTEXTUREPOSITION_HPP
