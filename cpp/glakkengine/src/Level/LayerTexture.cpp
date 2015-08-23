/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Level/LayerTexture.hpp"

#include "Graphics/Texture2D.hpp"
#include "ResourceFactory.hpp"

using namespace kke;

using glm::vec2;
using glm::vec3;
using glm::vec4;

using std::string;

LayerTexture::LayerTexture(const string& name, Layer* layer) : LayerItem(name, layer),
	texture(nullptr), position(0, 0, 0), origin(0, 0), scale(1, 1), rotation(0), source(string()), texcoords(0.0f, 0.0f, 1.0f, 1.0f)
{
	for (int i = 0; i < 4; i++)
		color[i] = vec4(1, 1, 1, 1);
}

void LayerTexture::SetPosition (const vec3& position)
{
	this->position = position;
}

const vec3& LayerTexture::GetPosition() const
{
	return position;
}

void LayerTexture::SetOrigin (const vec2& origin)
{
	this->origin = origin;
}

const vec2& LayerTexture::GetOrigin() const
{
	return origin;
}

void LayerTexture::SetScale (const vec2& scale)
{
	this->scale = scale;
}

const vec2& LayerTexture::GetScale() const
{
	return scale;
}

void LayerTexture::SetRotation (float rotation)
{
	this->rotation = rotation;
}

float LayerTexture::GetRotation() const
{
	return rotation;
}

void LayerTexture::SetColor (int index, const vec4& color)
{
	this->color[index] = color;
}

const vec4& LayerTexture::GetColor(int index) const
{
	return color[index];
}

void LayerTexture::SetSource (const string& source)
{
	this->source = source;
}

const string& LayerTexture::GetSource() const
{
	return source;
}

void LayerTexture::SetTexcoords (const Rectf& texcoords)
{
	this->texcoords = texcoords;
}

const Rectf& LayerTexture::GetTexcoords() const
{
	return texcoords;
}

const Texture2D& LayerTexture::GetTexture() const
{
	return texture->GetObject();
}

bool LayerTexture::SourceLoaded() const
{
	if (source == string() || texture == nullptr)
		return false;
	
	return source == texture->GetSource() && isLoaded;
}

void LayerTexture::LoadContent()
{
	if (!isLoaded)
	{
		texture = ResourceFactory::Instance().Create<Texture2D>(source);
		isLoaded = true;
	}
}

void LayerTexture::UnloadContent()
{
	if (isLoaded)
	{
		delete texture;
		texture = nullptr;
		isLoaded = false;
	}
}

void LayerTexture::Update (GameTime& gameTime)
{
	rotation += gameTime.Elapsed() * 0.00025f;
}

void LayerTexture::Draw (IRenderer& renderer)
{
	renderer.Draw(
		texture->GetObject(),
		position,
		rotation,
		scale,
		texcoords,
		color[0]);
}