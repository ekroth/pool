/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "IO/ILog.hpp"

using namespace kke;

const std::string ILog::MSG_SHORTS[] = { "ERR", "WRN", "MSG", "DEB" };

void ILog::Begin(const LogLevel& level) 
{
    GetStream() << MSG_SHORTS[(unsigned int)level] << ": ";
}

void ILog::End(const LogLevel& level)
{
    GetStream() << std::endl;
}