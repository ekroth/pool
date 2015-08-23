/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

template<typename T, typename... Args>
Resource<T> ResourceFactory::Create(Args... args)
{
    const std::string source = ResourceType::MergeSources({ args... });
    auto res = FactoryBase<ResourceType, std::string>::Create(T::Category(), source);
    
    if (!res)
    {
        // No registered function, we have to _try_ to load it ourselves
        return load<T>(source);
    }
    
    return std::static_pointer_cast<T>(res);
}

template<typename T>
Resource<T> ResourceFactory::load(const std::string& source)
{
    auto& resMap = resources[T::Category()];
    auto it = resMap.find(source);
    
    if (it == resMap.end())
    {
        T* res = new T();
        
		try 
		{
			if (!res->Load(source))
			{
				LOGE() << "Failed loading " << res->GetCategory() << ": \"" << source << '"';
				delete res;
				return Resource<T>();
			}
		}
		catch (std::exception& e)
		{
			LOGE() << "An error was thrown when loading resource: " << e.what();
			delete res;
			return Resource<T>();
		}

        LOGD() << "New " << res->GetCategory() << ": \"" << source << "\" " << "UNKNOWN" << 's';
        resMap[source] = { 1, res };
        it = resMap.find(source); // This is stupid.. but lazy
    }
    else
    {
        LOGD() << "Returned " << it->second.Resource->GetCategory() << ": \"" << source << '"';
        it->second = { it->second.Users + 1, it->second.Resource };
    }
    
    return Resource<T>(static_cast<T*>(it->second.Resource), std::bind(&ResourceFactory::noUsersCallback, this, std::placeholders::_1));
}