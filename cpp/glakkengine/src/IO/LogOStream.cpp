/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "IO/LogOStream.hpp"

#include <iostream>

using namespace kke;

LogOStream::LogOStream(std::ostream* stream, bool owned) : 
    stream(stream), owned(owned)
{
    
}

LogOStream::~LogOStream()
{
    if (owned)
        delete stream;
}

void LogOStream::Begin(const LogLevel& level) 
{
    GetStream() << MSG_SHORTS[(unsigned int)level] << ": ";
}

void LogOStream::End(const LogLevel& level)
{
    GetStream() << std::endl;
}

std::ostream& LogOStream::GetStream()
{ 
    return *stream; 
}
    
    
    
     