/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "IO/ArgParser.hpp"

#include <sstream>
#include <iostream>

using namespace kke;

using std::string;
using std::istringstream;
using std::cout;
using std::endl;

ArgParser::ArgParser() : caseSense(true)
{

}

ArgParser::~ArgParser()
{

}

bool ArgParser::Registered (const ArgumentID& id)
{
	return argMap.find(id) != argMap.end();
}

bool ArgParser::Exist(const ArgumentID& id)
{
	return GetArgument(id) != nullptr;
}

int ArgParser::ValidArgument(const std::string& arg) const
{
	for (auto i : argMap)
	{
		if (i.second.IsArg(arg, caseSense))
		{
			return i.first;
		}
	}
	
	return -1;
}

const Argument* ArgParser::GetArgument (const ArgumentID& id)
{
	ArgMap::const_iterator it = argMap.find(id);
	
	if (it == argMap.end())
		return nullptr;
	
	return &it->second;
}

bool ArgParser::Process(int argc, char **argv)
{
	// Skip file name (0).
	for (int i = 1; i < argc; i++)
	{
		string current = string(argv[i]);
		bool validArg = false;
		ArgumentID argId;
		
		// Find argument with same name
		for (auto arg : argMap)
		{
			if (arg.second.IsArg(current, caseSense))
			{
				validArg = true;
				argId = arg.first;
			}
		}
		
		if (!validArg)
		{
			cout << "Invalid argument: " << current << endl;
			return false;
		}
		
		Argument &arg = argMap.find(argId)->second;
		
		switch(arg.GetType())
		{
			case ArgumentType::Exist:
				// No value to set
				break;
				
			case ArgumentType::Int:
			{
				bool error = false;
				
				if (i == argc - 1)
				{
					error = true;
				}
				else
				{
					istringstream stream(argv[++i]);
					if (stream.good())
					{
						int val;
						stream >> val;
						
						arg.SetIvalue(val);
						arg.SetSvalue(argv[i]); // Also set string
					}
					else
						error = true;
				}
				
				if (error)
					cout << "-" + arg.GetShortName() + ", --" + arg.GetLongName() + " argument requires integer." << endl;
			}
				break;
				
			case ArgumentType::String:
			{
				bool error = false;
				
				if (i == argc - 1)
					error = true;
				else
					arg.SetSvalue(argv[++i]);
				
				if (error)
					cout << "-" + arg.GetShortName() + ", --" + arg.GetLongName() + " argument requires string." << endl;
			}
				
				break;
		}
	}
	
	return true;
}

bool ArgParser::Register(const ArgumentID& id, ArgumentType type, const std::string& lName, const std::string& sName, const std::string &info)
{
	bool isGood = true;
	
// #if DEBUG
	
	for (auto arg : argMap)
	{
		bool duplicate = false;
		
		if (arg.first == id)
		{
			cout << "Duplicate argument IDs exist: " << id << endl;
			duplicate = true;
		}
		
		if ((!caseSense && Argument::StringToLow(arg.second.GetLongName()) == Argument::StringToLow(lName)) || 
			arg.second.GetLongName() == lName)
		{
			cout << "Duplicate long argument names exist: " + lName << endl;
			duplicate = true;
		}
		
		if ((!caseSense && Argument::StringToLow(arg.second.GetShortName()) == Argument::StringToLow(sName)) || 
			arg.second.GetShortName() == sName)
		{
			cout << "Duplicate short argument names exist: " + sName << endl;
			duplicate = true;
		}
		
		if (duplicate)
		{
			cout << "Duplicates:" << endl;
			cout << "Current:" << endl;
			cout << lName << endl << sName << endl << id << endl << info << endl << endl;
			
			cout << "Existing:" << endl;
			cout << arg.second.GetLongName() << endl << arg.second.GetShortName() << endl << arg.first << endl << arg.second.GetInfo() << endl << endl;
			
			isGood = false;
		}
	}
	
// #endif
	
	argMap[id] = Argument(type, lName, sName, info);
	return isGood;
}

void ArgParser::CaseSense(bool value)
{
	caseSense = value;
}