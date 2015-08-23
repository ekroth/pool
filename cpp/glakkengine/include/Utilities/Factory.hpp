/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_FACTORY_HPP
#define KKE_FACTORY_HPP

#include "Utilities/Singleton.hpp"

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

namespace kke
{
	template<typename Base, typename... Args>
	/**
	 * @brief Factory of Base with additional CTOR parameters Args.
     * 
	 * A class is registered by a string which is paired with a std::function which acts as a constructor.
	 **/
	class FactoryBase
	{
	public:
		typedef std::function<std::shared_ptr<Base>(Args...)> ConstrFunction;
		
	public:
		/**
		 * @brief Registers a class with a constructor function.
		 *
		 * @param name Name of class.
		 * @param func Constructor function.
		 * @return :shared_ptr< Base >
		 **/
		void Register(const std::string& name, ConstrFunction func);
		
		/**
		 * @brief Creates a std::shared_ptr<Base> by calling the corresponding function.
		 *
		 * @param name Name of class.
		 * @param args Constructor arguments.
		 * @return :shared_ptr< Base > Empty std::shared_ptr if no registered name exists.
		 **/
		std::shared_ptr<Base> Create(const std::string& name, Args... args);
		
	protected:
		std::unordered_map<std::string, ConstrFunction> registered;
        
	protected:
        FactoryBase();
        ~FactoryBase();
	};
	
	template<typename Base, typename... Args>
	/**
	 * @brief Factory of Base with additional CTOR parameters Args.
     * 
	 * Features the same features as FactoryBase, in addition to being a Singleton.
	 * This class is final, use FactoryBase for derived classes.
	 **/
	class Factory final : public FactoryBase<Base, Args...>, public Singleton<Factory<Base, Args...>>
	{
		friend class Singleton<Factory<Base, Args...>>;
	};
	
	#include "Factory.inl"
}

#endif // KKE_FACTORY_HPP
