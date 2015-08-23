/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

template<class EM>
template<typename L, typename C>
ScopedListener<EM>::ScopedListener(L listener, C c, const EventType& type) : 
	listener(listener),
	type(type)
{ 
    EM::Instance().AddListener(listener, std::bind(c, listener, std::placeholders::_1), type);
}

template<class EM>
ScopedListener<EM>::ScopedListener() :
    listener(nullptr),
    type(0)
{ }

template<class EM>
ScopedListener<EM>::ScopedListener(const void* listener, const EventCallback& c, const EventType& type) : 
    listener(listener),
    type(type)
{ 
    EM::Instance().AddListener(listener, c, type);
}

template<class EM>
ScopedListener<EM>::ScopedListener(ScopedListener<EM>&& sl)
{ 
    listener = sl.listener;
    type = sl.type;
    sl.listener = nullptr;
    sl.type = 0;
}

template<class EM>
ScopedListener<EM>& ScopedListener<EM>::operator=(ScopedListener<EM>&& sl)
{
    if (this != &sl)
    {
        listener = sl.listener;
        type = sl.type;
        sl.listener = 0;
        sl.type = 0;
    }
    return *this;
}

template<class EM>
ScopedListener<EM>::operator bool() const
{
    if (listener != nullptr)
        return true;
    
    return false;
}

template<class EM>
ScopedListener<EM>::~ScopedListener()
{
    Reset();
}

template<class EM>
void ScopedListener<EM>::Reset()
{
    if (*this)
    {
        EM::Instance().DelListener(listener, type);
    }
    
    listener = 0;
}