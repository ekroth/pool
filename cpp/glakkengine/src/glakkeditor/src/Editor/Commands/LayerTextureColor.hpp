/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYERTEXTURECOLOR_HPP
#define KKE_LAYERTEXTURECOLOR_HPP

#include <glm/glm.hpp>
#include <QtGui/QUndoCommand>

namespace kke
{
	class LayerTexture;
	class LayerTextureColor : public QUndoCommand
	{
	public:
		LayerTextureColor(LayerTexture* layerTexture, const glm::vec4* color, QUndoCommand* parent = nullptr);
		
		void redo();
		void undo();
	private:
		LayerTexture *layerTexture;
		glm::vec4 oldColor[4];
		glm::vec4 newColor[4];
	};
}

#endif // KKE_LAYERTEXTURECOLOR_HPP
