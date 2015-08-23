/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYERTEXTURESCALE_HPP
#define KKE_LAYERTEXTURESCALE_HPP

#include "glm/glm.hpp"
#include <QtGui/QUndoCommand>

namespace kke
{
	class LayerTexture;
	class LayerTextureScale : public QUndoCommand
	{
	public:
		LayerTextureScale(LayerTexture* layerTexture, const glm::vec2& scale, QUndoCommand* parent = nullptr);
		
		void redo();
		void undo();
	private:
		LayerTexture *layerTexture;
		glm::vec2 oldScale;
		glm::vec2 newScale;
	};
}

#endif // KKE_LAYERTEXTURESCALE_HPP
