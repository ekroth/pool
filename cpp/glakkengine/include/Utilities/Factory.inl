/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

template<typename Base, typename... Args>
FactoryBase<Base, Args...>::FactoryBase()
{
}

template<typename Base, typename... Args>
FactoryBase<Base, Args...>::~FactoryBase()
{
	
}

template<typename Base, typename... Args>
void FactoryBase<Base, Args...>::Register(const std::string& name, ConstrFunction func)
{
	registered[name] = func;
}

template<typename Base, typename... Args>
std::shared_ptr<Base> FactoryBase<Base, Args...>::Create(const std::string& name, Args... args)
{
	auto it = registered.find(name);
	
	if (it == registered.end())
	{
		return std::shared_ptr<Base>();
	}
	
	return it->second(args...);
}