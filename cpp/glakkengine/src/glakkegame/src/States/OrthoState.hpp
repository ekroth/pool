/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_ORTHOSTATE_HPP
#define KKE_ORTHOSTATE_HPP

#include "States/State.hpp"

#include <Events/ScopedListener.hpp>
#include <Graphics/TextureBatch.hpp>
#include <glm/glm.hpp>

namespace kke
{
	class Shader;
	class InputManager;
	class BufferObject;
	class FrameBuffer;
	class Font;
	class Texture2D;

	class OrthoState : public State
	{
	public:
		OrthoState(StateManager* stateManager);
		void LoadContent();
		void UnloadContent();
		void HandleInput(InputManager& inputManager, GameTime& gameTime);
		void Update(GameTime& gameTime);
		void Draw(GameTime& gameTime);
		void DrawHeat(GameTime& gameTime);
		void DrawLight(GameTime& gameTime);
		void DrawBlur(GameTime& gameTime);
		
		void ProcessEvent (EventData& event);
	private:	
		Resource<TextureBatch<>> renderer;
		Resource<Texture2D> texture;
		Resource<Shader> renderShader, heatShader, blurShader;
	};
}

#endif // KKE_TEXTSTATE_HPP
