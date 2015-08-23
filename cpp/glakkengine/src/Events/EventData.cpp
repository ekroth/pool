/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Events/EventData.hpp"

using namespace kke;

using std::string;

EventData::~EventData()
{

}

const EventTime& EventData::GetTime() const
{
	return time;
}

const EventType& kke::EventData::GetType() const
{
	return type;
}
