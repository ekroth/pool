/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYERTEXTUREROTATION_HPP
#define KKE_LAYERTEXTUREROTATION_HPP

#include <QtGui/QUndoCommand>

namespace kke
{
	class LayerTexture;
	class LayerTextureRotation : public QUndoCommand
	{
	public:
		LayerTextureRotation(LayerTexture* layerTexture, float rotation, QUndoCommand* parent = nullptr);
		
		void redo();
		void undo();
	private:
		LayerTexture *layerTexture;
		float oldRotation, newRotation;
	};
}

#endif // KKE_LAYERTEXTUREROTATION_HPP
