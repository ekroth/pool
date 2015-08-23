/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/NewFont.hpp"

#include <tinyxml.h>
#include <string>

using namespace kke;

using std::string;

NewFont::NewFont() : ResourceType("Font"),
	font(0)
{

}

NewFont::~NewFont()
{
	Unload();
}

bool NewFont::Load (const string& source)
{
	this->source = source;

	// Load XML document
	TiXmlDocument doc;
	doc.LoadFile(source);

	TiXmlElement *root(doc.RootElement());

	{
		if (root->FirstChild("Size") != 0)
		{
			fontSize = atoi(((TiXmlElement*)root->FirstChild("Size"))->GetText());
		}
		else
		{
			LOGW() << "(" << source << ") Font size not set.";
		}
		
		if (root->FirstChild("Path") != 0)
		{
			fontPath = ((TiXmlElement*)root->FirstChild("Path"))->GetText();
		}
		else
		{
			LOGE() << "(" << source << ") Font path not set.";
			return false;
		}
	}
	
	LOGW() << fontPath;
	
	font = new FTTextureFont(fontPath.c_str());
	
	if (font->Error())
	{
		LOGE() << "(" << source << ") Error loading font.";
		return false;
	}
	
	font->FaceSize(fontSize, 96);
	font->

	return true;
}

bool NewFont::Unload()
{
	if (font == 0)
	{
		delete font;
		fontPath = string();
	}
	
	return true;
}

void NewFont::Draw()
{
	font->Render("WELLasdasdsds", -1, FTPoint(200, 200));
}