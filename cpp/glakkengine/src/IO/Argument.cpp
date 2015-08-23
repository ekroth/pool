/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "IO/Argument.hpp"

#include <iostream>

using namespace kke;

using std::string;
using std::cout;
using std::endl;

Argument::Argument() : 
	lName(string()),
	sName(string()),
	info(string()),
	sValue(string()),
	iValue(0),
	type(ArgumentType::Exist)
{

}

Argument::Argument(ArgumentType type, const string& lName, const std::string& sName, const std::string &info) : 
	lName(lName),
	sName(sName),
	info(info),
	sValue(string()),
	iValue(0),
	type(type)
{

}

const std::string& Argument::GetLongName() const
{
	return lName;
}

const std::string& Argument::GetShortName() const
{
	return sName;
}

int Argument::GetInt() const
{
	if (type != ArgumentType::Int)
		cout << "Argument is not int type." << endl;
	
	return iValue;
}

const std::string& Argument::GetString() const
{
	return sValue;
}

const std::string& Argument::GetInfo() const
{
	return info;
}

ArgumentType Argument::GetType() const
{
	return type;
}

bool Argument::IsArg(const std::string& arg, bool caseSense) const
{
	if (!caseSense)
	{
		std::string low = StringToLow(arg);
		return low == "--" + StringToLow(lName) || low == "-" + StringToLow(sName);
	}
	else
		return arg == "--" + lName || arg == "-" + sName;
}

void Argument::SetSvalue(const std::string& value)
{
	sValue = value;
}

void Argument::SetIvalue(int value)
{
	iValue = value;
}

void Argument::SetInfo(const std::string& info)
{
	this->info = info;
}

// Static
string Argument::StringToLow(const string& var)
{
	std::string low;
	low.reserve(var.length());
	
	for (unsigned int i = 0; i < var.length(); i++)
		low.push_back(tolower(var[i]));
	
	return low;
}
