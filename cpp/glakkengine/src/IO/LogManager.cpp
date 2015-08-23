/* Copyright (c) 2010-2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "IO/LogManager.hpp"
#include "IO/LogOStream.hpp"

#include <iostream>
#include <type_traits>

using namespace kke;

LogManager::LogManager() : 
	log(new LogOStream(&std::cout, false)),
	lvl(LogLevel::Warning),
	owned(true)
{
	
}

LogManager::~LogManager()
{
	if (owned && log != nullptr)
		delete log;
}

void LogManager::SetLog(ILog* log, bool owned)
{
	if (log != nullptr)
	{
		if (this->owned && this->log != nullptr)
			delete this->log;
		
		this->log = log;
		this->owned = owned;
	}
}

ILog& LogManager::GetLog()
{
	return *log;
}

void LogManager::SetMinLevel(const LogLevel& lvl)
{
	this->lvl = lvl;
}

const LogLevel& LogManager::GetMinLevel() const
{
	return lvl;
}
