/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_TEXTUREBATCH_HPP
#define KKE_TEXTUREBATCH_HPP

#include "Graphics/IRenderer.hpp"
#include "Math/Rect.hpp"
#include "Interface/INoncopyable.hpp"
#include "Graphics/Shader.hpp"
#include "ResourceFactory.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <queue>
#include <GL/glew.h>
#include <string>
#include <array>
#include <memory>

namespace kke
{
	class Texture2D;
	class VertexArray;
	class Buffer;
	class Font;
	
	struct TextureBatchVertex
	{
		glm::vec2 Position;
		glm::vec2 Texcoords;
		glm::vec4 Color;
		float Effect;
	};
	
	/**
	 * @brief Type of effect on the shader.
	 * RGBA = Using RGBA in texture.
	 * RGB = Using RGB in texture.
	 * R = Using R in texture as alpha.
	 * A = Using A in texture as alpha.
	 **/
	enum class TextureBatchEffect
	{
		RGBA,
		RGB,
		R,
		A
	};
	
	enum class TextureBatchFlip
	{
		None,
		Vertical,
		Horizontal,
		Both
	};
	
	enum class TextureBatchMode : RenderMode
	{
		Blend
	};
	
	template<typename Vertex = TextureBatchVertex>
	class TextureBatch : public IRenderer
	{
	public:
		typedef std::vector<Vertex> VertexBlendVector;
		
		static const unsigned char MaxBlendData = 8;
		
	public:
		TextureBatch(const std::string& shader = "content/Shaders/Renderers/TextureBatch/Texture.frag|content/Shaders/Renderers/TextureBatch/Texture.vert");
		~TextureBatch();
		
		void Begin(RenderMode mode);
		
		virtual void Draw(const Texture2D& texture,
		    const glm::vec2& position,
		    float rotation,
		    const glm::vec2& scale,
		    const Rectf& texcoords,
		    const glm::vec4* colors,
		    const glm::vec2& origin = glm::vec2 (0, 0),
			TextureBatchEffect effect = TextureBatchEffect::RGBA,
			TextureBatchFlip flip = TextureBatchFlip::None);
		
		virtual void Draw(const Texture2D& texture,
		    const glm::vec2& position,
		    float rotation = 0.0f,
		    const glm::vec2& scale = glm::vec2 (1, 1),
		    const Rectf& texcoords = Rectf (0, 0, 1, 1),
		    const glm::vec4& color = glm::vec4(1, 1, 1, 1),
		    const glm::vec2& origin = glm::vec2 (0, 0),
			TextureBatchEffect effect = TextureBatchEffect::RGBA,
			TextureBatchFlip flip = TextureBatchFlip::None);
		
		/**
		 * @brief Uploads data to VBO and sends needed data to shader. User can now send additional options to shader,
		 * and then explicitly call End() when done.
		 *
		 * @return void
		 **/
		void PreEnd();
		
		/**
		 * @brief Pushes all textures to framebuffer. Will call PreEnd automatically if not already called.
		 *
		 * @return void
		 **/
		void End();
		
		/**
		 * @brief TextureBatch will delete shader if != nullptr.
		 *
		 * @param shader ...
		 * @return void
		 **/
		void SetShader(const Resource<Shader>& shader);
		const Resource<Shader>& GetShader();
		
		VertexBlendVector& GetVertexBlend();
		void SetBlendDataIndex(unsigned char index);
		unsigned char GetBlendDataIndex() const;
		
	protected:
		void beginBlend();
		void drawBlend (const Texture2D& texture, const glm::vec2& position, float rotation, const glm::vec2& scale, 
						const Rectf& texcoords, const glm::vec4* colors, const glm::vec2& origin, TextureBatchEffect effect, TextureBatchFlip flip);
		void preEndBlend();
		void endBlend();
		
		void setupVao();
		
		struct RenderData
		{
			RenderData (const Texture2D* texture, const Buffer *data, const Buffer *indices, size_t start, size_t end) :
				Texture (texture),
				Data(data),
				Indices(indices),
				VertexStart (start),
				VertexEnd (end)
			{ }
			const Texture2D *Texture;
			const Buffer *Data;
			const Buffer *Indices;
			size_t VertexStart;
			size_t VertexEnd;
		};

		typedef std::queue<RenderData> DrawBlendQueue;
		typedef std::vector<uint> IndexVector;
		
		struct BlendData
		{
			VertexBlendVector VertexBlendV;
			DrawBlendQueue DrawBlendQ;
			IndexVector IndexV;
			RenderStatus RenderS;
		};
		
		int currBlend;
		std::array<BlendData, MaxBlendData> blendData;
		
		// Render_Blend
// 		VertexBlendVector vertexBlendVec;
// 		DrawBlendQueue drawBlendQueue;
// 		IndexVector indexBlendVec;
		
		std::shared_ptr<Buffer> vertexVbo;
		std::shared_ptr<Buffer> indexVbo;
		std::shared_ptr<VertexArray> mainVao;
		Resource<Shader> shader;
		TextureBatchMode renderMode;
	};
}

#endif // KKE_TEXTUREBATCH_HPP