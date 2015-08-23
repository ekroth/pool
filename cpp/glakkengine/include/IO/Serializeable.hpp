/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_SERIALIZEABLE_HPP
#define KKE_SERIALIZEABLE_HPP


#include "Math/Rect.hpp"

#include "tinyxml/tinyxml.h"
#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace kke
{
	class Serializeable
	{
	public:
		Serializeable();
		virtual ~Serializeable();
		
		/**
		 * @brief Deserialize from element.
		 *
		 * @param element ...
		 * @return bool
		 **/
		virtual void Deserialize(const TiXmlElement* element) = 0;
		/**
		 * @brief Creates a new elements and serializes data to it.
		 *
		 * @param element ...
		 * @return bool
		 **/
		virtual TiXmlElement* Serialize() const = 0;
		/**
		 * @brief Creates a new elements and serializes data to it, will also write id (convenience).
		 *
		 * @param element ...
		 * @param name ...
		 * @return bool
		 **/
		TiXmlElement* Serialize(const std::string& id);
		
		/**
		 * @brief Get name of class.
		 *
		 * @return :string&
		 **/
		virtual std::string GetClassName() const = 0;
		
	protected:
		
	private:
		
	public:
		// Built-in types
		static TiXmlElement *SerializeDouble(double value);
		static TiXmlElement *SerializeDouble(double value, const std::string &id);
		static double DeserializeDouble(const TiXmlElement *element);
		
		static TiXmlElement *SerializeInt(int value);
		static TiXmlElement *SerializeInt(int value, const std::string &id);
		static int DeserializeInt(const TiXmlElement *element);
		
		static TiXmlElement *SerializeBool(bool value);
		static TiXmlElement *SerializeBool(bool value, const std::string &id);
		static bool DeserializeBool(const TiXmlElement *element);
		
		static TiXmlElement *SerializeString(const std::string &value);
		static TiXmlElement *SerializeString(const std::string &value, const std::string &id);
		static std::string DeserializeString(const TiXmlElement *element);
		
		static TiXmlElement *SerializeVec2(const glm::vec2 &value);
		static TiXmlElement *SerializeVec2(const glm::vec2 &value, const std::string &id);
		static glm::vec2 DeserializeVec2(const TiXmlElement *element);
		
		static TiXmlElement *SerializeVec3(const glm::vec3 &value);
		static TiXmlElement *SerializeVec3(const glm::vec3 &value, const std::string &id);
		static glm::vec3 DeserializeVec3(const TiXmlElement *element);
		
		static TiXmlElement *SerializeVec4(const glm::vec4 &value);
		static TiXmlElement *SerializeVec4(const glm::vec4 &value, const std::string &id);
		static glm::vec4 DeserializeVec4(const TiXmlElement *element);
		
		static const std::string XmlTypeDouble;
		static const std::string XmlTypeInt;
		static const std::string XmlTypeBool;
		static const std::string XmlTypeString;
		
		static const std::string XmlTypeVec2;
		static const std::string XmlTypeVec3;
		static const std::string XmlTypeVec4;
		
		static const std::string XmlAttribType;
		static const std::string XmlAttribId;
		static const std::string XmlAttribLength;
		
		static const std::string XmlBoolTrue;
		static const std::string XmlBoolFalse;
	};
}

#endif // KKE_SERIALIZEABLE_HPP
