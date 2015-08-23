/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

template<typename T>
T* Singleton<T>::instance = nullptr;

template<typename T>
T& Singleton<T>::Instance()
{
	if (instance == nullptr)
		instance = new T();
	
	return *instance;
}

template<typename T>
void Singleton<T>::Initialize()
{
    if (instance == nullptr)
        instance = new T();
}

template<typename T>
void Singleton<T>::Terminate()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

template<typename T>
Singleton<T>::Singleton()
{ }

template<typename T>
Singleton<T>::~Singleton()
{ }
