/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/Font.hpp"
#include "Graphics/Texture2D.hpp"
#include "Graphics/Glyph.hpp"
#include "Math/PackNode.hpp"
#include "Graphics/FTManager.hpp"
#include "Graphics/Buffer.hpp"
#include "Graphics/IRenderer.hpp"
#include "Graphics/GLManager.hpp"

#include "ResourceFactory.hpp"

#include <tinyxml.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <sstream>

using namespace kke;

using std::string;
using std::wstring;
using std::map;
using std::vector;
using std::stringstream;

using glm::vec2;
using glm::vec3;
using glm::vec4;

Font::Font() :
	ResourceType(),
	fontSize(12),
	dpi(96),
	lineSize(96),
	fontPath(),
	face(0),
	texture(0)
{
}

Font::~Font()
{
	Unload();
}

bool Font::Load(const string& source)
{
	const string cacheFolder = /*ConfigManager::Instance().GetConfig("System")->ReadString("Cache", "Cache") +*/ string("Cache/Font");
	string fileName = source.substr(source.find_last_of('/'));
	this->source = source;

	// Load XML document
	TiXmlDocument doc;
	doc.LoadFile(source);

	TiXmlElement *root = doc.RootElement();

	{
		if (root->FirstChild("Size") != 0)
			fontSize = atoi(static_cast<TiXmlElement*>(root->FirstChild("Size"))->GetText());
		else
		{
			LOGW() << "(" << source << ") Font size not set.";
		}
		
		if (root->FirstChild("DPI") != 0)
			dpi = atoi(static_cast<TiXmlElement*>(root->FirstChild("DPI"))->GetText());
		else
		{
			LOGW() << "(" << source << ") Font DPI not set.";
		}
		
		if (root->FirstChild("Path") != 0)
			fontPath = static_cast<TiXmlElement*>(root->FirstChild("Path"))->GetText();
		else
		{
			LOGW() << "(" << source << ") Font path not set.";
			return false;
		}
	}
	
	// Create face
	if (FT_New_Face(FTManager::Instance().GetLib(), fontPath.c_str(), 0, &face))
	{
		LOGE() << "Error create face.";
	}
	
	FT_Set_Char_Size(face, fontSize * 64, 0, dpi, 0);
	bool cacheValid = false;
	
	// Load cache
	TiXmlDocument cacheDoc;
	
	/* Disable cache for now
	 * if (cacheDoc.LoadFile(cacheFolder + fileName + "-cache.xml"))
	{
		cacheValid = LoadCache(cacheDoc, doc, fileName, cacheFolder);
		if (cacheValid)
		{
			LOGM() << "Loading font from cache.";
		}
	}*/

	if (!cacheValid)
	{
		// All chars available in XML doc
		vector<wchar_t> chars;

		// Get Unicode
		{
			TiXmlElement *ele = static_cast<TiXmlElement*>(root->FirstChild("Unicode"));

			if (ele != 0)
			{
				vector < string > unis = ExtractSources(ele->GetText(), ',');
				

				for (uint i = 0; i < unis.size(); i++)
					if (unis[i].find('-') != string::npos)
					{
						size_t pos = unis[i].find('-');
						size_t start = atoi(unis[i].substr(0, pos).c_str());
						size_t end = start;
						
						if (pos != string::npos)
							end = atoi(unis[i].substr(pos + 1).c_str());

						for (size_t n = start; n <= end; n++)
							chars.push_back(n);
					}
					else
					{
						chars.push_back(atoi(unis[i].c_str()));
					}
			}
		}

		// Start size
		Recti packSize(0, 0, 256, 256);

		// WARNING: This is very slow, should look into it?
		// Higher delta will result in faster loading,
		// but might result in a slightly too large texture (i.e. wasted space).
		const float packDelta(1.25f);

		// We must find a working PackSize
		for (bool success = false; !success;)
		{
			PackNode packNode(packSize);

			for (uint i = 0; i < chars.size(); i++)
			{
				uint charIndex = FT_Get_Char_Index(face, chars[i]);

				if (charIndex == 0)
				{
					// Ignore invalid chars
					success = true;
					continue;
				}

				FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT);
				FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

				Recti output(0, 0, 0, 0);

				if (!packNode.Insert(Recti(0, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows), output))
				{
					// Failed, still to small size, try again
					success = false;
					break;
				}

				success = true;
				
				texcoords[chars[i]].X = 	(float)output.X / packSize.Width;
				texcoords[chars[i]].Y = 	(float)output.Y / packSize.Height;
				texcoords[chars[i]].Width = (float)output.Width / packSize.Width;
				texcoords[chars[i]].Height =(float)output.Height / packSize.Height;
			}
			
			if (!success)
			{
				// Increase size and try again
				packSize.Width *= packDelta;
				packSize.Height *= packDelta;
			}
		}
		
		texture = new Texture2D(true);
		texture->Bind();
		texture->Allocate(0, GL_RED, packSize.Width, packSize.Height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	}

	// TODO: Have multiple textures
	cacheDoc.Clear();
	TiXmlElement *cacheRoot = new TiXmlElement("FontCache");
	TiXmlElement *cacheChars = new TiXmlElement("Characters");
	TiXmlElement *cacheTex = new TiXmlElement("Texture");
	cacheRoot->LinkEndChild(doc.RootElement()->Clone());
	cacheRoot->LinkEndChild(cacheChars);
	cacheRoot->LinkEndChild(cacheTex);
	cacheDoc.LinkEndChild(cacheRoot);
	std::stringstream stream;
	
	if (!cacheValid)
	{
		stream << texture->GetWidth();
		cacheTex->SetAttribute("W", stream.str());
		stream.str("");
		stream << texture->GetHeight();
		cacheTex->SetAttribute("H", stream.str());
	}
	
	// Set alignment to one channel
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	texture->SetParameterf(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	texture->SetParameterf(GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	for (RectMap::iterator it = texcoords.begin(); it != texcoords.end(); ++it)
	{
		uint charIndex = FT_Get_Char_Index(face, it->first);

		// Ignore invalid chars
		if (charIndex == 0)
			continue;

		FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT);
		FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

		Glyph* glyph = new Glyph(it->first, face->glyph->metrics, fontSize, charIndex);
		glyphs[it->first] = glyph;	
		
		const int size = face->glyph->bitmap.width * face->glyph->bitmap.rows;
		unsigned char *tmpbuffer = new unsigned char[size];
		
// 		Flip image
		for (int y = 0; y < face->glyph->bitmap.rows; y++)
			for (int x = 0; x < face->glyph->bitmap.width; x++)
			{
				int rindex  = ((face->glyph->bitmap.rows - y - 1) * face->glyph->bitmap.width) + x;
				tmpbuffer[y * face->glyph->bitmap.width + x] = face->glyph->bitmap.buffer[rindex];
			}

		texture->AllocateSub(0, 
							 (int)(texcoords[it->first].X * texture->GetWidth() + 0.5f),
							 (int)(texcoords[it->first].Y * texture->GetHeight() + 0.5f), 
							 (int)(texcoords[it->first].Width * texture->GetWidth() + 0.5f), 
							 (int)(texcoords[it->first].Height * texture->GetHeight() + 0.5f), 
							 GL_RED, GL_UNSIGNED_BYTE, tmpbuffer);
		delete[] tmpbuffer;
		
		if (!cacheValid)
		{
			// Save to cache
			TiXmlElement *gelement = new TiXmlElement("Glyph");
			stream.str("");
			stream << it->first;
			gelement->SetAttribute("Char", stream.str());
			stream.str("");
			stream << texcoords[it->first].X;
			gelement->SetAttribute("X", stream.str());
			stream.str("");
			stream << texcoords[it->first].Y;
			gelement->SetAttribute("Y", stream.str());
			stream.str("");
			stream << texcoords[it->first].Width;
			gelement->SetAttribute("W", stream.str());
			stream.str("");
			stream << texcoords[it->first].Height;
			gelement->SetAttribute("H", stream.str());
			
			cacheChars->LinkEndChild(gelement);
		}
	}
	
	texture->GenerateMipmaps();

	// Restore alignment
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	
	/*if (!cacheValid)
	{
		LOGM() << "Saving cache..";
		if (!cacheDoc.SaveFile(cacheFolder + fileName + "-cache.xml"))
		{
			LOGE() << "Could not save cache xml file.";
		}
	}*/
	
	return true;
}

bool Font::Unload()
{
	if (!glyphs.empty())
	{
		for (auto it = glyphs.begin(); it != glyphs.end(); ++it)
			delete it->second;

		glyphs.clear();
		texcoords.clear();

		fontSize = 0;
		fontPath = string();
		delete texture;

		FT_Done_Face(face);
	}

	return glyphs.empty();
}

const string& Font::GetCategory() const
{
	static string category = "Font";
	return category;
}

bool Font::LoadCache (const TiXmlDocument& cache, const TiXmlDocument& conf, const std::string& fileName, const std::string& cacheFolder)
{
	// TODO: Make sure xml is the same
	const TiXmlElement *root = cache.RootElement();
	
	const TiXmlElement *fontConf = root->FirstChildElement("Font");
	
	if (conf.RootElement()->FirstChild("DPI")->FirstChild()->ValueStr() != fontConf->FirstChild("DPI")->FirstChild()->ValueStr()
		|| conf.RootElement()->FirstChild("Path")->FirstChild()->ValueStr() != fontConf->FirstChild("Path")->FirstChild()->ValueStr()
		|| conf.RootElement()->FirstChild("Unicode")->FirstChild()->ValueStr() != fontConf->FirstChild("Unicode")->FirstChild()->ValueStr())
	{
		LOGM() << "Font config changed since last cache.";
		return false;
	}
	
	// Load all glyphs
	for (const TiXmlElement *child = root->FirstChildElement("Characters")->FirstChildElement(); child != 0; child = child->NextSiblingElement())
	{
		int tmpI;
		child->Attribute("Char", &tmpI);
		wchar_t theChar = tmpI;
		double tmpD;
		child->Attribute("X", &tmpD);
		texcoords[theChar].X = tmpD;
		child->Attribute("Y", &tmpD);
		texcoords[theChar].Y = tmpD;
		child->Attribute("W", &tmpD);
		texcoords[theChar].Width = tmpD;
		child->Attribute("H", &tmpD);
		texcoords[theChar].Height = tmpD;
	}
	
	int width, height;
	root->FirstChildElement("Texture")->Attribute("W", &width);
	root->FirstChildElement("Texture")->Attribute("H", &height);
	
	// Create texture
	texture = new Texture2D(true);
	texture->Bind();
	texture->Allocate(0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	return true;
}

void Font::Draw(
	IRenderer& renderer,
	const string& text,
	const vec2& position,
	uint maxWidth)
{
	wstring ss;
	ss.assign(text.begin(), text.end());
	
	Draw(renderer, ss, position, maxWidth);
}

void Font::Draw(
	IRenderer& renderer,
	const wstring& text,
	const vec2& position,
	uint maxWidth)
{
// 	vec2 pos = position;
// 	const Glyph *prevGlyph = 0;
// 
// 	for (wstring::const_iterator it = text.begin(); it != text.end()
// 	{
// 		wchar_t ch = *it;
// 
// 		switch (ch)
// 		{
// 			case '\n':
// 				pos.y -= FT_MulFix(face->bbox.yMax, face->size->metrics.y_scale) / 64;
// 				pos.x = position.x;
// 				prevGlyph = 0;
// 				continue;
// 		}
// 
// 		GlyphMap::const_iterator gl = glyphs.find(ch);
// 		if (gl == glyphs.end())
// 			continue;
// 
// 		const Glyph& glyph = *gl->second;
// 
// 		pos.x += glyph.GetMetrics().horiBearingX / 64;
// 		pos.y -= (glyph.GetMetrics().height - glyph.GetMetrics().horiBearingY) / 64;
// 
// 		if (prevGlyph != 0)
// 		{
// 			FT_Vector delta;
// 			delta.x = 0;
// 			FT_Get_Kerning(face, prevGlyph->GetCharIndex(), glyph.GetCharIndex(), FT_KERNING_DEFAULT, &delta);
// 			pos.x += delta.x / 64;
// 		}
// 
// 		renderer.Draw(*texture, pos, 0.0f, vec2(1, 1), texcoords.find(ch)->second, 
// 					  vec4(1, 1, 1, 1), vec2(0, 0), RenderEffect::R, RenderFlip::None);
// 		
// 
// 		pos.x -= glyph.GetMetrics().horiBearingX / 64;
// 		pos.y += (glyph.GetMetrics().height - glyph.GetMetrics().horiBearingY) / 64;
// 		
// 		pos.x += glyph.GetMetrics().horiAdvance / 64;
// 
// 		prevGlyph = &glyph;
// 	}
}

bool Font::HasGlyph(wchar_t glyph) const
{
	return glyphs.find(glyph) != glyphs.end();
}

const Glyph* Font::GetGlyph (wchar_t glyph) const
{
	GlyphMap::const_iterator it = glyphs.find(glyph);
	
	if (it != glyphs.end())
		return it->second;
	
	return nullptr;
}

uint Font::GetSize() const
{
	return fontSize;
}

const Texture2D& Font::GetTexture() const
{
	return *texture;
}

/*
void Font::DrawIndy(
	IRenderer& renderer,
	const wstring& text,
	const vec2& position,
	uint maxWidth)
{
	vec2 pos = position;
	const Glyph *prevGlyph = 0;
	
	indices->Bind();
	if (indices->GetSize() < text.length() * sizeof(int) * 4)
		indices->BufferData(text.length() * sizeof(int) * 4, 0, GL_DYNAMIC_DRAW);
	
	uint *indexMap = (uint*)indices->Map(GL_WRITE_ONLY);

	int index = 0;
	for (wstring::const_iterator it = text.begin(); it != text.end()
	{
		wchar_t ch = *it;

		switch (ch)
		{
			case '\n':
				pos.y -= FT_MulFix(face->bbox.yMax, face->size->metrics.y_scale) / 64;
				pos.x = position.x;
				prevGlyph = 0;
				continue;
		}

		GlyphMap::const_iterator gl = glyphs.find(ch);
		if (gl == glyphs.end())
			continue;

		const Glyph& glyph = *gl->second;

		pos.x += glyph.GetMetrics().horiBearingX / 64;
		pos.y -= glyph.GetMetrics().horiBearingY / 64;

		if (prevGlyph != 0)
		{
			FT_Vector delta;
			delta.x = 0;
			FT_Get_Kerning(face, prevGlyph->GetCharIndex(), glyph.GetCharIndex(), FT_KERNING_DEFAULT, &delta);
			pos.x += delta.x / 64;
		}
		
		int loc = (int)texcoords.find(ch)->second.X;
		indexMap[index++] = loc++;
		indexMap[index++] = loc++;
		indexMap[index++] = loc++;
		indexMap[index++] = loc++;

		pos.x -= glyph.GetMetrics().horiBearingX / 64;
		pos.y += glyph.GetMetrics().horiBearingY / 64;
		pos.x += glyph.GetMetrics().horiAdvance / 64;

		prevGlyph = &glyph;
	}
	
	indices->Unmap();
}
*/
