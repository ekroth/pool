/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYERTEXTUREORIGIN_HPP
#define KKE_LAYERTEXTUREORIGIN_HPP

#include <glm/glm.hpp>
#include <QtGui/QUndoCommand>

namespace kke
{
	class LayerTexture;
	class LayerTextureOrigin : public QUndoCommand
	{
	public:
		LayerTextureOrigin(LayerTexture* layerTexture, const glm::vec2& origin, QUndoCommand* parent = nullptr);
		
		void redo();
		void undo();
	private:
		LayerTexture *layerTexture;
		glm::vec2 oldOrigin;
		glm::vec2 newOrigin;
	};
}

#endif // KKE_LAYERTEXTUREORIGIN_HPP
