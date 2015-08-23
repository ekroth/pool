/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_RENDERER_HPP
#define KKE_RENDERER_HPP

#include "Math/Rect.hpp"
#include "Interface/INoncopyable.hpp"
#include "Events/EventListener.hpp"
#include "Events/ScopedListener.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <queue>
#include <GL/glew.h>
#include <string>
#include <memory>

namespace kke
{
	class Texture2D;
	class ArrayObject;
	class BufferObject;
	template<typename T>
	class Resource;
	class Shader;
	class FrameBuffer;
	class Font;
	
	/**
	 * @brief Vertex for rendering.
	 **/
	struct PrimitiveVertex
	{
		glm::vec2 Position;
		glm::vec2 Texcoords;
		glm::vec4 Color;
	};
	
	struct RenderVertex : public PrimitiveVertex
	{
		float Effect;
	};
	
	/**
	 * @brief Type of effect on the shader.
	 * RenderEffect_RGBA = Using RGBA in texture.
	 * RenderEffect_RGB = Using RGB in texture.
	 * RenderEffect_R = Using R in texture as alpha.
	 * RenderEffect_A = Using A in texture as alpha.
	 **/
	enum class RenderEffect
	{
		RGBA,
		RGB,
		R,
		A
	};
	
	enum class RenderFlip
	{
		None,
		Vertical,
		Horizontal,
		Both
	};

	/**
	 * @brief Rendermode.
	 * Render_Idle - Begin() not called.
	 * Render_Blend - Draw textures in correct order.
	 **/
	enum class RenderMode
	{
		Idle,
		Blend
	};
	
	/**
	 * @brief How the FBO size is changed with viewport.
	 * FBO_Static - Same size at all times.
	 * FBO_Relative - Depending on viewport.
	 **/
	enum class FBOSizeMode
	{
		Static,
		Relative
	};

	/**
	 * @brief Class for batch rendering.
	 **/
	class Renderer final : public INoncopyable, public EventListener
	{
	public:
		Renderer();
		~Renderer();
		
		void SetClearColor(const glm::vec4& color);

		/**
		 * @brief Begin drawing. Not all modes support all overloadable Draw functions. 
		 * The renderer won't touch any glEnable calls (blending, depth etc), it'll only
		 * draw textures in a way which matches the mode. See enum RenderMode.
		 *
		 * @param mode RenderMode. Defaults to Render_Blend.
		 * @param clearFbo If FBO should be cleared. Defaults to true.
		 * @return void
		 **/
		void Begin (RenderMode mode = RenderMode::Blend, bool clearFbo = true);
		
		template<typename VertexType>
		void Transform(
			std::vector<unsigned int>& elements,
			std::vector<VertexType>& vertices,
			const Texture2D& texture, 
		    const glm::vec2& position,
		    float rotation = 0.0f,
		    const glm::vec2& scale = glm::vec2 (1, 1),
		    const Rectf& texcoords = Rectf (0, 0, 1, 1),
		    const glm::vec4& color = glm::vec4(1, 1, 1, 1),
		    const glm::vec2& origin = glm::vec2 (0, 0));
		
		void Draw(const Texture2D& texture,
		    const glm::vec2& position,
		    float rotation,
		    const glm::vec2& scale,
		    const Rectf& texcoords,
		    const glm::vec4* colors,
		    const glm::vec2& origin = glm::vec2 (0, 0),
			RenderEffect effect = RenderEffect::RGBA,
			RenderFlip flip = RenderFlip::None);
		
		void Draw(const Texture2D& texture,
		    const glm::vec2& position,
		    float rotation = 0.0f,
		    const glm::vec2& scale = glm::vec2 (1, 1),
		    const Rectf& texcoords = Rectf (0, 0, 1, 1),
		    const glm::vec4& color = glm::vec4(1, 1, 1, 1),
		    const glm::vec2& origin = glm::vec2 (0, 0),
			RenderEffect effect = RenderEffect::RGBA,
			RenderFlip flip = RenderFlip::None);
		
// 		void Draw(const Texture2D& texture,
// 			const VertexBuffer& vertexBuffer,
// 			int start,
// 			int end);
		
		/**
		 * @brief Forces all textures to be drawn to FBO, but keeps Renderer in same mode.
		 *
		 * @return void
		 **/
		void ForceDraw();
		
		/**
		 * @brief End drawing, will push all textures to FBO.
		 *
		 * @param useFbo If objects should be drawn to Renderer FBO. Defaults to true.
		 * @return void
		 **/
		void End(bool useFbo = true);
		
		FrameBuffer& GetFrameBuffer();
		
		/**
		 * @brief Blit internal FBO to another FBO.
		 *
		 * @param targetFbo Other FBO. Defaults to 0.
		 * @return void
		 **/
		void BlitFBO(FrameBuffer* targetFbo = nullptr);
		void BlitFBO(FrameBuffer* targetFbo, GLbitfield mask);
		
		BufferObject& GetVBO();
		void UpdateVBO();

		void ProcessEvent (EventData& event);
        
	private:
		void drawBlend(
			const Texture2D& texture,
		    const glm::vec2& position,
		    float rotation = 0.0f,
		    const glm::vec2& scale = glm::vec2 (1, 1),
		    const Rectf& texcoords = Rectf (0, 0, 1, 1),
		    const glm::vec4* colors = 0,
		    const glm::vec2& origin = glm::vec2 (0, 0),
			RenderEffect effect = RenderEffect::RGBA,
			RenderFlip flip = RenderFlip::None);
		
		// Only used once, might as well be inline
		void endBlend(bool useFbo);
		void refreshFbo(int width, int height);
        
	private:

		struct RenderData
		{
			const Texture2D *Texture;
			const BufferObject *Buffer;
			const BufferObject *Indices;
			size_t VertexStart;
			size_t VertexEnd;
		};

		typedef std::vector<RenderVertex> VertexBlendVector;
		typedef std::queue<RenderData> DrawBlendQueue;
		typedef std::vector<uint> IndexVector;

    private:
		RenderMode renderMode;
		
		// Render_Blend
		VertexBlendVector vertexBlendVec;
		DrawBlendQueue drawBlendQueue;
		
		IndexVector indexVector;
		
		std::shared_ptr<BufferObject> vertexVbo;
		std::shared_ptr<BufferObject> indexVbo;
		std::shared_ptr<ArrayObject> mainVao;
		std::shared_ptr<Resource<Shader>> shader;
		std::shared_ptr<FrameBuffer> frameBuffer;
        
		FBOSizeMode fboSizeMode;
		glm::vec3 fboLocation;
		glm::vec2 fboSize;
		glm::vec4 fboClearColor;
		bool fboRefreshNeeded;
        
        ScopedListener winResize;
	};
}

#include "Graphics/Renderer.inl"

#endif // KKE_RENDERER_HPP