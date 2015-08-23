/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "IO/XmlConfig.hpp"
#include "IO/LogManager.hpp"

using namespace kke;

XmlConfig::XmlConfig()
{

}

XmlConfig::~XmlConfig()
{

}

bool XmlConfig::Load(const std::string& path)
{
	pugi::xml_parse_result rs = doc.load_file(path.c_str());
	if (rs.status != pugi::xml_parse_status::status_ok)
	{
		LOGE() << "Error loading file: " << path << ". Error: " << rs.description();
		doc.reset();
		return false;
	}
	
	this->source = path;
	return true;
}

bool XmlConfig::Save()
{
	return Save(source);
}

bool XmlConfig::Save(const std::string& path)
{
	if (!doc.save_file(path.c_str()))
	{
		LOGE() << "Unable to save Xml to " << path;
		return false;
	}
	
	return true;
}

bool XmlConfig::Unload()
{
	doc.reset();
	return true;
}

const pugi::xml_document& XmlConfig::GetDoc() const
{
	return doc;
}

pugi::xml_document& XmlConfig::GetDoc()
{
    return doc;
}