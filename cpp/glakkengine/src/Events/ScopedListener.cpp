/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Events/ScopedListener.hpp"
#include "Events/EventManager.hpp"

using namespace kke;

// ScopedListener::ScopedListener() :
//     listener(nullptr),
// 	type(0)
// { }
// 
// ScopedListener::ScopedListener(const void* listener, const EventCallback& c, const EventType& type) : 
//     listener(listener),
// 	type(type)
// { 
// 	EventManager::Instance().AddListener(listener, c, type);
// }
// 
// ScopedListener::ScopedListener(ScopedListener&& sl)
// { 
//     listener = sl.listener;
//     type = sl.type;
//     sl.listener = nullptr;
//     sl.type = 0;
// }
// 
// ScopedListener& ScopedListener::operator=(ScopedListener&& sl)
// {
//     if (this != &sl)
//     {
//         listener = sl.listener;
//         type = sl.type;
//         sl.listener = 0;
//         sl.type = 0;
//     }
// 	return *this;
// }
// 
// ScopedListener::operator bool() const
// {
// 	if (listener != nullptr)
//         return true;
//     
//     return false;
// }
// 
// ScopedListener::~ScopedListener()
// {
// 	Clear();
// }
// 
// void ScopedListener::Clear()
// {
//     if (*this)
//     {
//         EventManager::Instance().DelListener(listener, type);
//     }
//     
//     listener = 0;
// }
