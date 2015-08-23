/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/TextureBatch.hpp"
#include "Graphics/Texture2D.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/GLManager.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/FrameBuffer.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/Buffer.hpp"
#include "Graphics/RenderBuffer.hpp"
#include "Graphics/Font.hpp"
#include "Events/EventManager.hpp"

#include "ResourceFactory.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

using namespace kke;

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::mat4;

using std::wstring;
using std::string;

template<typename Vertex>
TextureBatch<Vertex>::TextureBatch(const string& shader) : 
	IRenderer(),
	currBlend(0),
	vertexVbo(new Buffer(true, GL_ARRAY_BUFFER)),
	indexVbo(new Buffer(true, GL_ELEMENT_ARRAY_BUFFER)),
	mainVao(new VertexArray(true)),
	shader(0),
	renderMode(TextureBatchMode::Blend)
		  
{
	this->shader = ResourceFactory::Instance().Create<Shader>(shader);
	
    for (BlendData& data : blendData)
        data.RenderS = RenderStatus::Idle;
	
	setupVao();
}

template<typename Vertex>
TextureBatch<Vertex>::~TextureBatch()
{
}

template<typename Vertex>
void TextureBatch<Vertex>::Begin(RenderMode mode)
{
    LOGASSERT(blendData[currBlend].RenderS == RenderStatus::Idle, "Calling Begin, but status isn't Idle.");
	
	renderMode = (TextureBatchMode)mode;
	
	switch (renderMode)
	{
		case TextureBatchMode::Blend:
		{
			beginBlend();
		}
		break;
	};
	
	blendData[currBlend].RenderS = RenderStatus::Drawing;
}

template<typename Vertex>
void TextureBatch<Vertex>::Draw(const Texture2D& texture, const vec2& position, float rotation, const vec2& scale, 
    const Rectf& texcoords, const vec4* colors, const vec2& origin, TextureBatchEffect effect, TextureBatchFlip flip)
{
	switch (blendData[currBlend].RenderS)
	{
		default:
            LOGASSERT(false, "Calling Draw, but mode is not Drawing.");
			break;
			
		case RenderStatus::Drawing:
			drawBlend(texture, position, rotation, scale, texcoords, colors, origin, effect, flip);
			break;
	};
}

template<typename Vertex>
void TextureBatch<Vertex>::Draw (const Texture2D& texture, const vec2& position, float rotation, const vec2& scale, const Rectf& texcoords, 
    const vec4& color, const vec2& origin, TextureBatchEffect effect, TextureBatchFlip flip)
{
	vec4 colors[4] = { color, color, color, color };
	
	Draw(texture, position, rotation, scale, texcoords, colors, origin, effect, flip);
}

template<typename Vertex>
void TextureBatch<Vertex>::PreEnd()
{
	switch (blendData[currBlend].RenderS)
	{
		default:
            LOGASSERT(false, "Calling PreEnd, but mode is not Drawing.");
			break;
			
		case RenderStatus::Drawing:
		{
			switch (renderMode)
			{
				case TextureBatchMode::Blend:
					preEndBlend();
					break;
			};
			
			blendData[currBlend].RenderS = RenderStatus::PreEnded;
		}
			break;
	};
}

template<typename Vertex>
void TextureBatch<Vertex>::End()
{
	switch (blendData[currBlend].RenderS)
	{
		case RenderStatus::Idle:
            LOGASSERT(false, "Calling End, but mode is Render_Idle.");
			break;
			
		case RenderStatus::Drawing:
		case RenderStatus::PreEnded:
		{
			switch (renderMode)
			{
				case TextureBatchMode::Blend:
					endBlend();
					break;
			};
			
			blendData[currBlend].RenderS = RenderStatus::Idle;
		}
			break;
	};
}

template<typename Vertex>
const Resource<Shader>& TextureBatch<Vertex>::GetShader()
{
	return shader;
}

template<typename Vertex>
void TextureBatch<Vertex>::SetShader (const Resource<Shader>& shader)
{
	this->shader = shader;
	setupVao();
}

template<typename Vertex>
typename TextureBatch<Vertex>::VertexBlendVector& TextureBatch<Vertex>::GetVertexBlend()
{
	return blendData[currBlend].VertexBlendV;
}

template<typename Vertex>
void TextureBatch<Vertex>::SetBlendDataIndex(unsigned char index)
{
    LOGASSERT(index < MaxBlendData, "BlendDataIndex out of bounds.");
	currBlend = index;
}

template<typename Vertex>
unsigned char TextureBatch<Vertex>::GetBlendDataIndex() const
{
	return currBlend;
}

template<typename Vertex>
void TextureBatch<Vertex>::beginBlend()
{

}

template<typename Vertex>
void TextureBatch<Vertex>::drawBlend (const Texture2D& texture, const vec2& position, float rotation, const vec2& scale, 
						  const Rectf& texcoords, const vec4* colors, const vec2& origin, TextureBatchEffect effect, TextureBatchFlip flip)
{
// 	mat4 modelMatrix = 
// 		glm::translate( // Revert origin
// 			glm::scale( // scale
// 				glm::rotate( // Rotate
// 					glm::translate( // Origin
// 						glm::translate(mat4(1), // Position
// 						vec3(position, z)), 
// 					vec3(origin.x, origin.y, 0)), 
// 				rotation, vec3(0, 0, 1)), 
// 			vec3(scale.x, scale.y, 1)), 
// 		-vec3(origin.x, origin.y, 0));


	DrawBlendQueue &drawBlendQueue = blendData[currBlend].DrawBlendQ;
	VertexBlendVector &vertexBlendVec = blendData[currBlend].VertexBlendV;
	IndexVector &indexVector = blendData[currBlend].IndexV;
		
	mat4 modelMatrix = mat4();
	modelMatrix = glm::translate(modelMatrix, vec3(origin.x, origin.y, 0)); // Move to origin
	modelMatrix = glm::translate(mat4(1), vec3(position, 0)); // Move to position
	modelMatrix = glm::rotate(modelMatrix, rotation, vec3(0, 0, 1)); // Rotate
	modelMatrix = glm::scale(modelMatrix, vec3(scale.x, scale.y, 0)); // Scale
	modelMatrix = glm::translate(modelMatrix, -vec3(origin.x, origin.y, 0)); // Revert origin
	
	if (drawBlendQueue.empty() || drawBlendQueue.back().Texture != &texture)
	{
		// New texture!
		drawBlendQueue.push(RenderData(&texture, vertexVbo.get(), indexVbo.get(), indexVector.size(), 0));
		// End (last param) set last
	}
	
	Vertex vertices[4];
	vec4 pos;
	
	const unsigned int 
		bl = 2, 
		br = 1, 
		tl = 3, 
		tr = 0;
	
	// Bottom left
	pos = modelMatrix * vec4(0, 0, 0, 1);
	vertices[bl].Position = vec2(pos.x, pos.y);
	vertices[bl].Color = colors[0];
	vertices[bl].Effect = (float)effect;
	
	// Top left
	pos = modelMatrix * vec4(0, texcoords.Height * texture.GetHeight(), 0, 1);
	vertices[tl].Position = vec2(pos.x, pos.y); 
	vertices[tl].Color = colors[1];
	vertices[tl].Effect = (float)effect;

	// Top right
	pos = modelMatrix * vec4(texcoords.Width * texture.GetWidth(), texcoords.Height * texture.GetHeight(), 0, 1);
	vertices[tr].Position = vec2(pos.x, pos.y);
	vertices[tr].Color = colors[2];
	vertices[tr].Effect = (float)effect;

	// Bottom right
	pos = modelMatrix * vec4(texcoords.Width * texture.GetWidth(), 0, 0, 1);
	vertices[br].Position = vec2(pos.x, pos.y);
	vertices[br].Color = colors[3];
	vertices[br].Effect = (float)effect;
	
	switch (flip)
	{
		case TextureBatchFlip::None:
			vertices[bl].Texcoords = vec2(texcoords.X, texcoords.Y);
			vertices[tl].Texcoords = vec2(texcoords.X, texcoords.Y + texcoords.Height);
			vertices[tr].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y + texcoords.Height);
			vertices[br].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y);
			break;
		
		case TextureBatchFlip::Vertical:
			vertices[br].Texcoords = vec2(texcoords.X, texcoords.Y);
			vertices[tr].Texcoords = vec2(texcoords.X, texcoords.Y + texcoords.Height);
			vertices[tl].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y + texcoords.Height);
			vertices[bl].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y);
			break;
			
		case TextureBatchFlip::Horizontal:
			vertices[tl].Texcoords = vec2(texcoords.X, texcoords.Y);
			vertices[bl].Texcoords = vec2(texcoords.X, texcoords.Y + texcoords.Height);
			vertices[br].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y + texcoords.Height);
			vertices[tr].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y);
			break;
			
		case TextureBatchFlip::Both:
			vertices[bl].Texcoords = vec2(texcoords.X, texcoords.Y);
			vertices[tl].Texcoords = vec2(texcoords.X, texcoords.Y + texcoords.Height);
			vertices[tr].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y + texcoords.Height);
			vertices[br].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y);
			break;
	}
	
    indexVector.reserve(indexVector.size() + 6);
	indexVector.push_back(vertexBlendVec.size() + bl);
	indexVector.push_back(vertexBlendVec.size() + tl);
	indexVector.push_back(vertexBlendVec.size() + br);
	indexVector.push_back(vertexBlendVec.size() + br);
	indexVector.push_back(vertexBlendVec.size() + tl);
	indexVector.push_back(vertexBlendVec.size() + tr);
	
    vertexBlendVec.reserve(vertexBlendVec.size() + 4);
	for (int i = 0; i < 4; i++)
		vertexBlendVec.push_back(vertices[i]);
	
	drawBlendQueue.back().VertexEnd = indexVector.size();
}

template<typename Vertex>
void TextureBatch<Vertex>::preEndBlend()
{
	DrawBlendQueue &drawBlendQueue = blendData[currBlend].DrawBlendQ;
	VertexBlendVector &vertexBlendVec = blendData[currBlend].VertexBlendV;
	IndexVector &indexVector = blendData[currBlend].IndexV;
	
	vertexVbo->Bind();
	indexVbo->Bind();
	
	// Here we upload the whole vector
	// Resize if needed
	const size_t vertexVboSize = vertexBlendVec.size() * sizeof(Vertex);
	if (vertexVbo->GetSize() < vertexVboSize)
		vertexVbo->BufferData(vertexVboSize, &vertexBlendVec[0], GL_DYNAMIC_DRAW);
	else
		vertexVbo->BufferSubData(0, vertexVboSize, &vertexBlendVec[0]);
	
	const size_t indexVboSize = indexVector.size() * sizeof(uint);
	if (indexVbo->GetSize() < indexVboSize)
		indexVbo->BufferData(indexVboSize, &indexVector[0], GL_DYNAMIC_DRAW);
	else
		indexVbo->BufferSubData(0, indexVboSize, &indexVector[0]);
	
	shader->Bind();
	
// // // // // // // // // // // 	Texture2D::Activate(GL_TEXTURE0);
			  
	shader->UniformMat("matrix", 1, false, Camera::Instance().GetFullPtr());
	shader->Uniform("tex", 0);
	
	mainVao->Bind();
}

template<typename Vertex>
void TextureBatch<Vertex>::endBlend()
{
	DrawBlendQueue &drawBlendQueue = blendData[currBlend].DrawBlendQ;
	VertexBlendVector &vertexBlendVec = blendData[currBlend].VertexBlendV;
	IndexVector &indexVector = blendData[currBlend].IndexV;
	
	if (blendData[currBlend].RenderS != RenderStatus::PreEnded)
		PreEnd();
		
	Buffer *currBuffer = vertexVbo.get();
	while (!drawBlendQueue.empty())
	{
		// Only change buffer if needed
		if (drawBlendQueue.front().Data != currBuffer)
			drawBlendQueue.front().Data->Bind();
		
		drawBlendQueue.front().Texture->Bind();
		
		glDrawElements(
			GL_TRIANGLES,
			drawBlendQueue.front().VertexEnd,
			GL_UNSIGNED_INT,
			(const GLvoid*)(drawBlendQueue.front().VertexStart * sizeof(uint))
		);

		drawBlendQueue.pop();
	}
	
	mainVao->Unbind();
	
	vertexBlendVec.clear();
	indexVector.clear();
}

template<typename Vertex>
void TextureBatch<Vertex>::setupVao()
{
	// Set Vao settings
	mainVao->Bind();
	
	indexVbo->Bind();
	vertexVbo->Bind();
	
	shader->Bind();
	shader->SetVertexAttrib("in_Vertex", true);
	shader->SetVertexAttrib("in_Texcoord", true);
	shader->SetVertexAttrib("in_Color", true);
	shader->SetVertexAttrib("in_Effect", true);
	
	size_t offset = 0;
	shader->VertexAttribPointer("in_Vertex", 	2, GL_FLOAT, false, sizeof(Vertex), (const GLvoid*)offset);
	offset += sizeof(Vertex::Position);
	shader->VertexAttribPointer("in_Texcoord", 	2, GL_FLOAT, false, sizeof(Vertex), (const GLvoid*)offset);
	offset += sizeof(Vertex::Texcoords);
	shader->VertexAttribPointer("in_Color", 	4, GL_FLOAT, false, sizeof(Vertex), (const GLvoid*)offset);
	offset += sizeof(Vertex::Color);
	shader->VertexAttribPointer("in_Effect", 	1, GL_FLOAT, false, sizeof(Vertex), (const GLvoid*)offset);
	
	mainVao->Unbind();
}

template class TextureBatch<>;