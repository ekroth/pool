/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/Texture2D.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace kke;

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::mat4;

template<typename VertexType>
void Renderer::Transform(
std::vector<unsigned int>& elements,
std::vector<VertexType>& vertices,
const Texture2D& texture, 
const glm::vec2& position,
float rotation,
const glm::vec2& scale,
const Rectf& texcoords,
const glm::vec4& color,
const glm::vec2& origin)
{
	mat4 modelMatrix = mat4();
	modelMatrix = glm::translate(modelMatrix, vec3(origin.x, origin.y, 0)); // Move to origin
	modelMatrix = glm::translate(mat4(1), vec3(position, 0)); // Move to position
	modelMatrix = glm::rotate(modelMatrix, rotation, vec3(0, 0, 1)); // Rotate
	modelMatrix = glm::scale(modelMatrix, vec3(scale.x, scale.y, 0)); // Scale
	modelMatrix = glm::translate(modelMatrix, -vec3(origin.x, origin.y, 0)); // Revert origin
	
	// Reuse for each vertex
	RenderVertex tvertices[4];
	vec4 pos;
	
	const unsigned int 
		bl = 2, 
		br = 1, 
		tl = 3, 
		tr = 0;
	
	// Bottom left
	pos = modelMatrix * vec4(0, 0, 0, 1);
	tvertices[bl].Position = vec2(pos.x, pos.y);
	tvertices[bl].Color = color;
	
	// Top left
	pos = modelMatrix * vec4(0, texcoords.Height * texture.GetHeight(), 0, 1);
	tvertices[tl].Position = vec2(pos.x, pos.y); 
	tvertices[tl].Color = color;

	// Top right
	pos = modelMatrix * vec4(texcoords.Width * texture.GetWidth(), texcoords.Height * texture.GetHeight(), 0, 1);
	tvertices[tr].Position = vec2(pos.x, pos.y);
	tvertices[tr].Color = color;

	// Bottom right
	pos = modelMatrix * vec4(texcoords.Width * texture.GetWidth(), 0, 0, 1);
	tvertices[br].Position = vec2(pos.x, pos.y);
	tvertices[br].Color = color;
	
	tvertices[bl].Texcoords = vec2(texcoords.X, texcoords.Y);
	tvertices[tl].Texcoords = vec2(texcoords.X, texcoords.Y + texcoords.Height);
	tvertices[tr].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y + texcoords.Height);
	tvertices[br].Texcoords = vec2(texcoords.X + texcoords.Width, texcoords.Y);
	
// 	vertexBlendVec.reserve(vertices.size() + 4);
	
	elements.push_back(vertices.size() + bl);
	elements.push_back(vertices.size() + tl);
	elements.push_back(vertices.size() + br);
	elements.push_back(vertices.size() + br);
	elements.push_back(vertices.size() + tl);
	elements.push_back(vertices.size() + tr);
	
	for (int i = 0; i < 4; i++)
		vertices.push_back(tvertices[i]);
}