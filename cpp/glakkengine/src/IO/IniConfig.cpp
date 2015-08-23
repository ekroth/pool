/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "IO/IniConfig.hpp"
#include "IO/LogManager.hpp"

using namespace kke;

bool IniConfig::SimpleIniError(SI_Error err)
{
	switch (err)
	{
		case SI_FAIL:
			LOGE() << "Generic error";
			return false;
			
		case SI_NOMEM:
			LOGE() << "Out of memory";
			return false;
			
		case SI_FILE:
			LOGE() << "File error";
			return false;
			
		default:
			return true;
	};
}

IniConfig::IniConfig()
{

}

IniConfig::~IniConfig()
{
	
}

bool IniConfig::Load(const std::string& path)
{
	if (!SimpleIniError(ini.LoadFile(path.c_str())))
	{
		ini.Reset();
		LOGE() << "Error loading file " << path;
		return false;
	}
	
	this->source = path;
	return true;
}

bool IniConfig::Save()
{
	return Save(source);
}

bool IniConfig::Save(const std::string& path)
{
	if (!SimpleIniError(ini.SaveFile(path.c_str())))
	{
		LOGE() << "Error saving file to " << path;
		return false;
	}
	
	return true;
}

bool IniConfig::Unload()
{
	ini.Reset();
	return true;
}

const IniData& IniConfig::GetIni() const
{
	return ini;
}

IniData& IniConfig::GetIni()
{
    return ini;
}