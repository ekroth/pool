/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYERTEXTURETEXCOORDS_HPP
#define KKE_LAYERTEXTURETEXCOORDS_HPP

#include "Math/Rect.hpp"
#include <QtGui/QUndoCommand>

namespace kke
{
	class LayerTexture;
	class LayerTextureTexcoords : public QUndoCommand
	{
	public:
		LayerTextureTexcoords(LayerTexture* layerTexture, const Rectf& texcoords, QUndoCommand* parent = nullptr);
		
		void redo();
		void undo();
	private:
		LayerTexture *layerTexture;
		Rectf oldTexcoords;
		Rectf newTexcoords;
	};
}

#endif // KKE_LAYERTEXTURETEXCOORDS_HPP
