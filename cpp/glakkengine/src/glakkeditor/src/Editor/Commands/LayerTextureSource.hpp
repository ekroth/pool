/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYERTEXTURESOURCE_HPP
#define KKE_LAYERTEXTURESOURCE_HPP

#include <string>
#include <QtGui/QUndoCommand>

namespace kke
{
	class LayerTexture;
	class LayerTextureSource : public QUndoCommand
	{
	public:
		LayerTextureSource(LayerTexture* layerTexture, const std::string& source, QUndoCommand* parent = nullptr);
		
		void redo();
		void undo();
	private:
		LayerTexture *layerTexture;
		std::string oldSource;
		std::string newSource;
	};
}

#endif // KKE_LAYERTEXTURESOURCE_HPP
