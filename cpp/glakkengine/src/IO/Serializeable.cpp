/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "IO/Serializeable.hpp"



#include <cstdlib>
#include <ostream>

using namespace kke;

using std::string;
using std::ostringstream;

using glm::vec2;
using glm::vec3;
using glm::vec4;

const string Serializeable::XmlTypeDouble = "Double";
const string Serializeable::XmlTypeInt = "Int";
const string Serializeable::XmlTypeBool = "Bool";
const string Serializeable::XmlTypeString = "String";

const string Serializeable::XmlTypeVec2 = "Vec2";
const string Serializeable::XmlTypeVec3 = "Vec3";
const string Serializeable::XmlTypeVec4 = "Vec4";

const string Serializeable::XmlAttribType = "Type";
const string Serializeable::XmlAttribId = "Id";
const string Serializeable::XmlAttribLength = "Length";

const string Serializeable::XmlBoolTrue = "True";
const string Serializeable::XmlBoolFalse = "False";

Serializeable::Serializeable ()
{
}

Serializeable::~Serializeable()
{

}

TiXmlElement *Serializeable::Serialize (const string& id)
{
	TiXmlElement *element = Serialize();
	element->SetAttribute(XmlAttribId, id);
	return element;
}


TiXmlElement *Serializeable::SerializeBool (bool value)
{
	TiXmlElement *element = new TiXmlElement(XmlTypeBool);
	string writeVal;
	element->LinkEndChild(new TiXmlText(value ? XmlBoolTrue : XmlBoolFalse));
	return element;
}

TiXmlElement *Serializeable::SerializeBool (bool value, const string& id)
{
	TiXmlElement *element = SerializeBool(value);
	element->SetAttribute(XmlAttribId, id);
	return element;
}

bool Serializeable::DeserializeBool (const TiXmlElement *element)
{
	return element->ValueStr() == XmlBoolTrue;
}



TiXmlElement *Serializeable::SerializeDouble (double value)
{
	TiXmlElement *element = new TiXmlElement(XmlTypeDouble);
	ostringstream stream;
	stream << value;
	element->LinkEndChild(new TiXmlText(stream.str()));
	return element;
}

TiXmlElement *Serializeable::SerializeDouble (double value, const string& id)
{
	TiXmlElement *element = SerializeDouble(value);
	element->SetAttribute(XmlAttribId, id);
	return element;
}

double Serializeable::DeserializeDouble (const TiXmlElement *element)
{
	return atof(element->Value());
}



TiXmlElement *Serializeable::SerializeInt (int value)
{
	TiXmlElement *element = new TiXmlElement(XmlTypeInt);
	ostringstream stream;
	stream << value;
	element->LinkEndChild(new TiXmlText(stream.str()));
	return element;
}

TiXmlElement *Serializeable::SerializeInt (int value, const string& id)
{
	TiXmlElement *element = SerializeInt(value);
	element->SetAttribute(XmlAttribId, id);
	return element;
}

int Serializeable::DeserializeInt (const TiXmlElement *element)
{
	return atoi(element->Value());
}



TiXmlElement *Serializeable::SerializeString (const string &value)
{
	TiXmlElement *element = new TiXmlElement(XmlTypeString);
	element->LinkEndChild(new TiXmlText(value));
	return element;
}

TiXmlElement *Serializeable::SerializeString (const string &value, const string& id)
{
	TiXmlElement *element = SerializeString(value);
	element->SetAttribute(XmlAttribId, id);
	return element;
}

std::string Serializeable::DeserializeString (const TiXmlElement *element)
{
	return element->ValueStr();
}



TiXmlElement *Serializeable::SerializeVec2(const glm::vec2 &value)
{
	TiXmlElement *element = new TiXmlElement(XmlTypeVec2);
	element->LinkEndChild(SerializeInt(value.x, "X"));
	element->LinkEndChild(SerializeInt(value.y, "Y"));
	return element;
}

TiXmlElement *Serializeable::SerializeVec2(const glm::vec2 &value, const std::string &id)
{
	TiXmlElement *element = SerializeVec2(value);
	element->SetAttribute(XmlAttribId, id);
	return element;
}

glm::vec2 Serializeable::DeserializeVec2 (const TiXmlElement *element)
{
	vec2 vec;
	TiXmlElement *ele = (TiXmlElement*)element->FirstChild();
	
	for (int i = 0; i < 2; i++)
	{
		vec[i] = DeserializeDouble(ele);
		ele = (TiXmlElement*)element->IterateChildren(ele);
	}
	
	return vec;
}



TiXmlElement *Serializeable::SerializeVec3(const glm::vec3 &value)
{
	TiXmlElement *element = new TiXmlElement(XmlTypeVec3);
	element->LinkEndChild(SerializeInt(value.x, "X"));
	element->LinkEndChild(SerializeInt(value.y, "Y"));
	element->LinkEndChild(SerializeInt(value.z, "Z"));
	return element;
}

TiXmlElement *Serializeable::SerializeVec3(const glm::vec3 &value, const std::string &id)
{
	TiXmlElement *element = SerializeVec3(value);
	element->SetAttribute(XmlAttribId, id);
	return element;
}

glm::vec3 Serializeable::DeserializeVec3 (const TiXmlElement *element)
{
	vec3 vec;
	TiXmlElement *ele = (TiXmlElement*)element->FirstChild();
	
	for (int i = 0; i < 3; i++)
	{
		vec[i] = DeserializeDouble(ele);
		ele = (TiXmlElement*)element->IterateChildren(ele);
	}
	
	return vec;
}



TiXmlElement *Serializeable::SerializeVec4(const glm::vec4 &value)
{
	TiXmlElement *element = new TiXmlElement(XmlTypeVec4);
	element->LinkEndChild(SerializeInt(value.x, "X"));
	element->LinkEndChild(SerializeInt(value.y, "Y"));
	element->LinkEndChild(SerializeInt(value.z, "Z"));
	element->LinkEndChild(SerializeInt(value.w, "W"));
	return element;
}

TiXmlElement *Serializeable::SerializeVec4(const glm::vec4 &value, const std::string &id)
{
	TiXmlElement *element = SerializeVec4(value);
	element->SetAttribute(XmlAttribId, id);
	return element;
}

glm::vec4 Serializeable::DeserializeVec4 (const TiXmlElement *element)
{
	vec4 vec;
	TiXmlElement *ele = (TiXmlElement*)element->FirstChild();
	
	for (int i = 0; i < 4; i++)
	{
		vec[i] = DeserializeDouble(ele);
		ele = (TiXmlElement*)element->IterateChildren(ele);
	}
	
	return vec;
}