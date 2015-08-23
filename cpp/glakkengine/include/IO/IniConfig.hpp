/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_INICONFIG_HPP
#define KKE_INICONFIG_HPP

#include "ResourceType.hpp"
#include "Interface/INoncopyable.hpp"

#include <simpleini/SimpleIni.h>

namespace kke
{
	typedef CSimpleIniA IniData;
	/**
	 * @brief Resource handling a Ini-file
	 **/
	class IniConfig : public ResourceType, public INoncopyable
	{
		RESOURCETYPE_CATEGORY(IniConfig);
		
	public:
		IniConfig();
		virtual ~IniConfig();
		
		/**
		 * @brief Load ini-file from path.
		 *
		 * @param path Ini file.
		 * @return bool Result
		 **/
		bool Load(const std::string& path);
		/**
		 * @brief Save currently loaded file.
		 *
		 * @return bool
		 **/
		bool Save();
		/**
		 * @brief Save file to path.
		 *
		 * @param path Path.
		 * @return bool
		 **/
		bool Save(const std::string& path);
		/**
		 * @brief Unload file.
		 *
		 * @return bool
		 **/
		bool Unload();
		
		/**
		 * @brief Get Ini.
		 *
		 * @return const CSimpleIniA&
		 **/
		const IniData& GetIni() const;
        IniData& GetIni();
		
	public:
		static bool SimpleIniError(SI_Error err);
		
	private:
		IniData ini;
	};
}

#endif // KKE_INICONFIG_HPP
