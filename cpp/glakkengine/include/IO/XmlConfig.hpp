/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_XMLCONFIG_HPP
#define KKE_XMLCONFIG_HPP

#include "ResourceType.hpp"
#include "Interface/INoncopyable.hpp"

#include <pugixml/pugixml.hpp>

namespace kke
{
	/**
	 * @brief Resource handling a pugixml file.
	 **/
	class XmlConfig : public ResourceType, public INoncopyable
	{
		RESOURCETYPE_CATEGORY(XmlConfig);
		
	public:
		XmlConfig();
		virtual ~XmlConfig();
		
		/**
		 * @brief Load file.
		 *
		 * @param path Path.
		 * @return Result
		 **/
		bool Load(const std::string& path);
		/**
		 * @brief Save file.
		 *
		 * @return bool
		 **/
		bool Save();
		/**
		 * @brief Save file to path.
		 *
		 * @param path ...
		 * @return bool
		 **/
		bool Save(const std::string& path);
		bool Unload();
		
		/**
		 * @brief Get pugixml document
		 *
		 * @return :xml_document&
		 **/
		const pugi::xml_document& GetDoc() const;
        pugi::xml_document& GetDoc();
		
	private:
		pugi::xml_document doc;
	};
}

#endif // KKE_XMLCONFIG_HPP
