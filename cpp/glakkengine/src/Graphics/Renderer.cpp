/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/Renderer.hpp"
#include "Graphics/Texture2D.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/GLManager.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Framebuffer.hpp"
#include "Graphics/ArrayObject.hpp"
#include "Graphics/BufferObject.hpp"
#include "Graphics/Renderbuffer.hpp"
#include "Graphics/Framebuffer.hpp"
#include "Graphics/Font.hpp"
#include "Events/EventManager.hpp"

#include "ResourceFactory.hpp"
#include "Resource.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

using namespace kke;

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::mat4;

using std::wstring;

Renderer::Renderer() : 
		  renderMode(RenderMode::Idle),
		  vertexVbo(new BufferObject(true, GL_ARRAY_BUFFER)),
		  indexVbo(new BufferObject(true, GL_ELEMENT_ARRAY_BUFFER)),
		  mainVao(new ArrayObject(true)),
		  shader(ResourceFactory::Instance().Create<Shader>("content/Shaders/RenderTex.vert", "content/Shaders/RenderTex.frag")),
		  frameBuffer(new FrameBuffer(true)),
		  fboSizeMode(FBOSizeMode::Relative),
		  fboLocation(0, 0, 0),
		  fboSize(1, 1),
		  fboClearColor(1, 1, 1, 1),
		  fboRefreshNeeded(true),
		  winResize(this, EventHasher("WindowResize"))
		  
{	
	// Set Vao settings
	mainVao->Bind();
	
	indexVbo->Bind();
	vertexVbo->Bind();
	
	shader->SetVertexAttrib("in_Vertex", true);
	shader->SetVertexAttrib("in_Texcoord", true);
	shader->SetVertexAttrib("in_Color", true);
	shader->SetVertexAttrib("in_Effect", true);
	
	size_t offset = 0;
	shader->VertexAttribPointer("in_Vertex", 	2, GL_FLOAT, false, sizeof(RenderVertex), (const GLvoid*)offset);
	offset += sizeof(vec2);
	shader->VertexAttribPointer("in_Texcoord", 	2, GL_FLOAT, false, sizeof(RenderVertex), (const GLvoid*)offset);
	offset += sizeof(vec2);
	shader->VertexAttribPointer("in_Color", 	4, GL_FLOAT, false, sizeof(RenderVertex), (const GLvoid*)offset);
	offset += sizeof(vec4);
	shader->VertexAttribPointer("in_Effect", 	1, GL_FLOAT, false, sizeof(RenderVertex), (const GLvoid*)offset);
	
	mainVao->Disable();
}

Renderer::~Renderer()
{
	EventManager::Instance().DelListener(this);
}

void Renderer::SetClearColor (const vec4& color)
{
	this->fboClearColor = color;
}

void Renderer::Begin(RenderMode mode, bool clearFbo)
{
    LOGASSERT(renderMode != RenderMode::Idle, "Calling Begin, but mode isn't Render_Idle.");
	
	switch (mode)
	{
		case RenderMode::Blend:
			
			if (clearFbo)
			{
				// Reset FBO
				frameBuffer->Bind();
				
				if (fboRefreshNeeded)
					refreshFbo(GLManager::Instance().GetWindowWidth(), GLManager::Instance().GetWindowHeight());
				
				GLManager::Instance().SetClearColor(fboClearColor);
				GLManager::Instance().Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				frameBuffer->Disable();
			}
			break;
			
		default:
			break;
	};
	
	renderMode = mode;
}

void Renderer::Draw(const Texture2D& texture, const vec2& position, float rotation, const vec2& scale, 
					const Rectf& texcoords, const vec4* colors, const vec2& origin, RenderEffect effect, RenderFlip flip)
{
	switch (renderMode)
	{
		case RenderMode::Idle:
            LOGASSERT(false, "Calling Draw, but mode is Render_Idle.");
			break;
			
		case RenderMode::Blend:
			drawBlend(texture, position, rotation, scale, texcoords, colors, origin, effect, flip);
			break;
	};
}

void Renderer::Draw (const Texture2D& texture, const vec2& position, float rotation, const vec2& scale, const Rectf& texcoords, 
					 const vec4& color, const vec2& origin, RenderEffect effect, RenderFlip flip)
{
	vec4 colors[4] = { color, color, color, color };
	
	Draw(texture, position, rotation, scale, texcoords, colors, origin, effect, flip);
}

void Renderer::ForceDraw()
{
	RenderMode lastMode = renderMode;
	End();
	renderMode = lastMode;
}

void Renderer::End(bool useFbo)
{
	switch (renderMode)
	{
		case RenderMode::Idle:
            LOGASSERT(false, "Calling End, but mode is Render_Idle.");
			break;
			
		case RenderMode::Blend:
			endBlend(useFbo);
			break;
	};

	renderMode = RenderMode::Idle;
}

FrameBuffer& Renderer::GetFrameBuffer()
{
	return *frameBuffer;
}

void Renderer::BlitFBO(FrameBuffer* targetFbo)
{
	BlitFBO(targetFbo, GL_COLOR_BUFFER_BIT);
}

void Renderer::BlitFBO (FrameBuffer* targetFbo, GLbitfield mask)
{	
	frameBuffer->Blit(0, 0, frameBuffer->GetTexture()->GetWidth(), frameBuffer->GetTexture()->GetHeight(), targetFbo,
					  0, 0, frameBuffer->GetTexture()->GetWidth(), frameBuffer->GetTexture()->GetHeight(), mask);
}

void Renderer::ProcessEvent (EventData& event)
{
	WindowResizeEventData& data = (WindowResizeEventData&)event;
	refreshFbo(data.GetWidth(), data.GetHeight());
}

void Renderer::drawBlend (const Texture2D& texture, const vec2& position, float rotation, const vec2& scale, 
						  const Rectf& texcoords, const vec4* colors, const vec2& origin, RenderEffect effect, RenderFlip flip)
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
		
	mat4 modelMatrix = mat4();
	modelMatrix = glm::translate(modelMatrix, vec3(origin.x, origin.y, 0)); // Move to origin
	modelMatrix = glm::translate(mat4(1), vec3(position, 0)); // Move to position
	modelMatrix = glm::rotate(modelMatrix, rotation, vec3(0, 0, 1)); // Rotate
	modelMatrix = glm::scale(modelMatrix, vec3(scale.x, scale.y, 0)); // Scale
	modelMatrix = glm::translate(modelMatrix, -vec3(origin.x, origin.y, 0)); // Revert origin
	
	if (drawBlendQueue.empty() || drawBlendQueue.back().Texture != &texture)
	{
		// New texture!
		drawBlendQueue.push(RenderData(&texture, vertexVbo, indexVbo, indexVector.size(), 0));
		// End (last param) set last
	}
	
	// Reuse for each vertex
	RenderVertex vertices[4];
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
		case RenderFlip::None:
			vertices[bl].Texcoords = vec2(texcoords.X, texcoords.Y);
			vertices[tl].Texcoords = vec2(texcoords.X, texcoords.Y + texcoords.Height);
			vertices[tr].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y + texcoords.Height);
			vertices[br].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y);
			break;
		
		case RenderFlip::Vertical:
			vertices[br].Texcoords = vec2(texcoords.X, texcoords.Y);
			vertices[tr].Texcoords = vec2(texcoords.X, texcoords.Y + texcoords.Height);
			vertices[tl].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y + texcoords.Height);
			vertices[bl].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y);
			break;
			
		case RenderFlip::Horizontal:
			vertices[tl].Texcoords = vec2(texcoords.X, texcoords.Y);
			vertices[bl].Texcoords = vec2(texcoords.X, texcoords.Y + texcoords.Height);
			vertices[br].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y + texcoords.Height);
			vertices[tr].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y);
			break;
			
		case RenderFlip::Both:
			vertices[bl].Texcoords = vec2(texcoords.X, texcoords.Y);
			vertices[tl].Texcoords = vec2(texcoords.X, texcoords.Y + texcoords.Height);
			vertices[tr].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y + texcoords.Height);
			vertices[br].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y);
			break;
	}
	
	vertexBlendVec.reserve(vertexBlendVec.size() + 4);
	
	indexVector.push_back(vertexBlendVec.size() + bl);
	indexVector.push_back(vertexBlendVec.size() + tl);
	indexVector.push_back(vertexBlendVec.size() + br);
	indexVector.push_back(vertexBlendVec.size() + br);
	indexVector.push_back(vertexBlendVec.size() + tl);
	indexVector.push_back(vertexBlendVec.size() + tr);
	
	for (int i = 0; i < 4; i++)
		vertexBlendVec.push_back(vertices[i]);
	
	drawBlendQueue.back().VertexEnd = indexVector.size();
}

void Renderer::endBlend(bool useFbo)
{
	vertexVbo->Bind();
	indexVbo->Bind();
	
	if (useFbo)
	{
		// Bind and set appropriate viewport
		frameBuffer->Bind();
		Camera::Instance().SetupViewport(0, 0, frameBuffer->GetTexture()->GetWidth(), frameBuffer->GetTexture()->GetHeight());
	}
	
	// Here we upload the whole vector
	// Resize if needed
	const size_t vertexVboSize = vertexBlendVec.size() * sizeof(RenderVertex);
// 	if (vertexVbo->GetSize() < vertexVboSize)
		vertexVbo->BufferData(vertexVboSize, &vertexBlendVec[0], GL_DYNAMIC_DRAW);
// 	else
// 		vertexVbo->BufferSubData(0, vertexVboSize, &vertexBlendVec[0]);
	
	const size_t indexVboSize = indexVector.size() * sizeof(uint);
// 	if (indexVbo->GetSize() < indexVboSize)
		indexVbo->BufferData(indexVboSize, &indexVector[0], GL_DYNAMIC_DRAW);
// 	else
// 		indexVbo->BufferSubData(0, indexVboSize, &indexVector[0]);
	
	shader->Bind();
	
	Texture2D::Activate(GL_TEXTURE0);
			  
	shader->UniformMat("matrix", 1, false, Camera::Instance().GetFullPtr());
	shader->Uniform("tex", 0);
	
	mainVao->Bind();
		
	BufferObject *currBuffer = vertexVbo;
	while (!drawBlendQueue.empty())
	{
		// Only change buffer if needed
		if (drawBlendQueue.front().Buffer != currBuffer)
			drawBlendQueue.front().Buffer->Bind();
		
		drawBlendQueue.front().Texture->Bind();
		
		glDrawElements(
			GL_TRIANGLES,
			drawBlendQueue.front().VertexEnd,
			GL_UNSIGNED_INT,
			(const GLvoid*)(drawBlendQueue.front().VertexStart * sizeof(uint))
		);

		drawBlendQueue.pop();
	}
	
	mainVao->Disable();
	
	if (useFbo)
	{
		// Unbind and restore viewport
		frameBuffer->Disable();
		Camera::Instance().SetupViewport();
	}
	
	vertexBlendVec.clear();
	indexVector.clear();
}

void Renderer::refreshFbo (int width, int height)
{	
	fboRefreshNeeded = false;
	bool attachRbo = false, attachTex = false;
	// TODO: Depend on FBOMode and FBOSize
	RenderBuffer *rbo = frameBuffer->GetRenderbuffer();
	if (rbo == nullptr)
	{
		// No RBO available, create and attach later
		rbo = new RenderBuffer(true);
		attachRbo = true;
	}
	
	rbo->Bind();
	rbo->Allocate(GL_DEPTH_COMPONENT, width, height);
	rbo->Disable();
	
	Texture2D *texture = frameBuffer->GetTexture();
	if (texture == nullptr)
	{
		// No texture available, create and attach later
		texture = new Texture2D(true);
		attachTex = true;
	}
	
	texture->Bind();
	texture->Allocate(0, GL_RGBA, 
		width, 
		height, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	
	// Set default parameters
	texture->SetParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture->SetParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture->SetParameterf(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	texture->SetParameterf(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	frameBuffer->Bind();
	
	// Attach if needed. And attach to both DRAW and READ to blitting support.
	if (attachRbo)
	{
		frameBuffer->AttachRenderbuffer(rbo, GL_DRAW_FRAMEBUFFER);
// 		frameBuffer->AttachRenderbuffer(rbo, GL_READ_FRAMEBUFFER);
	}
	if (attachTex)
	{
		frameBuffer->AttachTexture(texture, GL_DRAW_FRAMEBUFFER);
// 		frameBuffer->AttachTexture(texture, GL_READ_FRAMEBUFFER);
	}
	
// 	frameBuffer->Disable();
}

/*void Renderer::end3D()
{
	vbo->Bind();
	
	// Instead of looping and couting the size of VBO, use 
	// the pre-calculated value.
	if (vbo->GetSize() < internalVboSize)
	{
		// Need to reallocate VBO.
		// TODO: Decrease size after multiple draws smaller than current.
		
		vbo->BufferData(internalVboSize, 0, GL_STREAM_DRAW);
	}
	
	// Put Vertices on VBO
	size_t currentLocation = 0;
	for (Vertex3DMap::iterator it = vertex3DMap.begin(); it != vertex3DMap.end()
	{
		const size_t theSize = it->second.size() * sizeof(RenderVertex);
		vbo->BufferSubData(currentLocation, theSize, &it->second[0]);
		
		// Push
		draw3DQueue.push(RenderData(it->first, vbo, currentLocation, currentLocation + theSize));
		
		currentLocation += theSize;
	}
	
	shader->Use();
	
	Texture2D::Activate(GL_TEXTURE0);
			  
	shader->UniformMat("projectionMatrix", 	1, false, glm::value_ptr(GLManager::Instance().GetMatrix(kke::Matrix_Perspective)));
	shader->UniformMat("viewMatrix", 		1, false, glm::value_ptr(GLManager::Instance().GetMatrix(kke::Matrix_View)));
	shader->Uniform("tex", 0);
	
	shader->SetVertexAttrib("in_Vertex", true);
	shader->SetVertexAttrib("in_Color", true);
	shader->SetVertexAttrib("in_Texcoord", true);
	
	size_t offset = 0;
	shader->VertexAttribPointer("in_Vertex", 	3, GL_FLOAT, false, sizeof(RenderVertex), (const GLvoid*)offset);
	offset += sizeof(glm::vec3);
	shader->VertexAttribPointer("in_Texcoord", 	2, GL_FLOAT, false, sizeof(RenderVertex), (const GLvoid*)offset);
	offset += sizeof(glm::vec2);
	shader->VertexAttribPointer("in_Color", 	4, GL_FLOAT, false, sizeof(RenderVertex), (const GLvoid*)offset);
	
	while (!draw3DQueue.empty())
	{
		draw3DQueue.front().Texture->Bind();
		
		glDrawArrays(
			GL_QUADS, 
			draw3DQueue.front().Start / sizeof(RenderVertex), 
			(draw3DQueue.front().End - draw3DQueue.front().Start) / sizeof(RenderVertex));

		draw3DQueue.pop();
	}
	
	shader->SetVertexAttrib("in_Vertex", false);
	shader->SetVertexAttrib("in_Color", false);
	shader->SetVertexAttrib("in_Texcoord", false);
	
	internalVboSize = 0;
	vertex3DMap.clear();
}*/

/*void Renderer::draw3D (const Texture2D& texture, const vec3& position, float rotation, const vec2& scale, const Rectf& texcoords, const vec4* colors, const vec2& origin)
{
	// Get the VertexVector of the texture
	Vertex3DVector &vector = vertex3DMap[&texture];

	mat4 modelMatrix = glm::translate(mat4(1), position); // Move to position
	modelMatrix = glm::translate(modelMatrix, vec3(origin.x, origin.y, 0)); // Move to origin
	modelMatrix = glm::rotate(modelMatrix, rotation, vec3(0, 0, 1)); // Rotate
	modelMatrix = glm::scale(modelMatrix, vec3(scale.x, scale.y, 1)); // Scale
	modelMatrix = glm::translate(modelMatrix, -vec3(origin.x, origin.y, 0)); // Revert origin
	
	{
		RenderVertex vertex;	
		vec4 pos = modelMatrix * vec4(0, (texcoords.Height - texcoords.Y) * texture.GetHeight(), 1, 1);
		vertex.Position = vec3(pos.x, pos.y, pos.z); 
		vertex.Texcoords = vec2(texcoords.X, texcoords.Y + texcoords.Height);
		vertex.Color = colors[0];
		vector.push_back(vertex);
	}
	
	{
		RenderVertex vertex;
		vec4 pos = modelMatrix * vec4((texcoords.Width - texcoords.X) * texture.GetWidth(),
								  (texcoords.Height - texcoords.Y) * texture.GetHeight(), 1, 1);
		vertex.Position = vec3(pos.x, pos.y, pos.z);
		vertex.Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y + texcoords.Height);
		vertex.Color = colors[1];
		vector.push_back(vertex);
	}
	
	{
		RenderVertex vertex;
		vec4 pos = modelMatrix * vec4((texcoords.Width - texcoords.X) * texture.GetWidth(), 0, 1, 1);
		vertex.Position = vec3(pos.x, pos.y, pos.z);
		vertex.Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y);
		vertex.Color = colors[2];
		vector.push_back(vertex);
	}
	
	{
		RenderVertex vertex;
		vec4 pos = modelMatrix * vec4(0, 0, 1, 1);
		vertex.Position = vec3(pos.x, pos.y, pos.z);
		vertex.Texcoords = vec2(texcoords.X, texcoords.Y);
		vertex.Color = colors[3];
		vector.push_back(vertex);
	}
	
	// Increase current needed size of VBO
	internalVboSize += sizeof(RenderVertex) * 4;
}*/