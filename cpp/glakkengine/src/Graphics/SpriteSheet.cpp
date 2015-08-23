/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/SpriteSheet.hpp"
#include "Graphics/Texture2D.hpp"
#include "IO/LogManager.hpp"

#include <tinyxml.h>
#include <vector>

using kke::SpriteSheet;
using kke::Rectf;
using kke::Texture2D;

using std::string;
using std::pair;
using std::map;
using std::vector;

SpriteSheet::SpriteSheet() :
	ResourceType(),
	texture(nullptr)
{
}
SpriteSheet::~SpriteSheet()
{
	Unload();
}

bool SpriteSheet::Load(const string& source)
{
	this->source = source;
	vector<string> vec = ExtractSources(source);

	return Load(vec[0].find(".xml") != string::npos ? vec[0] : vec[1], vec[0].find(".xml")
		!= string::npos ? vec[1] : vec[0]);
}

bool SpriteSheet::Load(const string& map, const string& image)
{
	TiXmlDocument doc;
	doc.LoadFile(map);

// 	this->texture = ResourceFactory::Instance().Create<Texture2D>(image);

	// Run through every child source
	for (TiXmlElement* ele = doc.RootElement()->FirstChildElement(); ele != 0;
		ele = (TiXmlElement*) ele->NextSibling())
	{
		string name;
		Rectf rect;
		TiXmlElement* eleChild = 0;

		// Name
		eleChild = (TiXmlElement*) ele->FirstChild("Name");
		name = eleChild->GetText();

		// Get rectangles and convert to float (percentage of image). Makes more sense
		// for opengl.

		// X
		eleChild = (TiXmlElement*) ele->FirstChild("X");
		rect.X = atoi(eleChild->GetText())
			/ (float) texture->GetWidth();

		// Y
		eleChild = (TiXmlElement*) ele->FirstChild("Y");
		rect.Y = atoi(eleChild->GetText())
			/ (float) texture->GetHeight();

		// Width
		eleChild = (TiXmlElement*) ele->FirstChild("Width");
		rect.Width = atoi(eleChild->GetText())
			/ (float) texture->GetWidth();

		// Height
		eleChild = (TiXmlElement*) ele->FirstChild("Height");
		rect.Height = atoi(eleChild->GetText())
			/ (float) texture->GetHeight();

		// Add Recti
		sheets[name] = rect;
	}

	return true;
}

bool SpriteSheet::Unload()
{
    sheets.clear();
	return true;
}

const Rectf &SpriteSheet::GetSource(const string& name)
{
	// Return a Rectangle source
	return sheets[name];
}

const Texture2D &SpriteSheet::GetTexture() const
{
	return *texture;
}
