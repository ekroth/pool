/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_RESOURCEFACTORY_HPP
#define KKE_RESOURCEFACTORY_HPP

#include "Utilities/Factory.hpp"
#include "Utilities/Singleton.hpp"
#include "IO/LogManager.hpp"
#include "ResourceType.hpp"

#include <exception>
#include <unordered_map>

#define RESOURCETYPE_DERIVED_STATIC_ASSERT(x) static_assert(std::is_base_of<ResourceType, x>::value, "Object not derived from ResourceType.");

namespace kke
{
	template <typename T>
    using Resource = std::shared_ptr<T>;
	
    /**
     * @brief Manages loading of ResourceType.
     * 
     * In extension to the normal FactoryBase features, it is able to load and manage its own ResourceTypes, that do not have any CTOR-func.
     **/
    class ResourceFactory : public FactoryBase<ResourceType, std::string>, public Singleton<ResourceFactory>
	{
		friend class Singleton<ResourceFactory>;
		
	public:
		/**
		* @brief Reloads all available Resources.
		*
		* @return void
		**/
		void ReloadAll();

		/**
		* @brief Reload all Resources of specified category.
		*
		* @param category Category.
		* @return void
		**/
		void ReloadCategory(const std::string& category);
		
		template<typename T, typename... Args>
		/**
		 * @brief Convenience function.
		 *
         * Will convert pointer into Resource<T>, get name from T::Category().
         * And if there's no ctor-func it will use ResourceType's default initialization model.
         * 
		 * @param args Strings to merge.
		 * @return Resource< T >
		 **/
		Resource<T> Create(Args... args);
	private:
        struct ResourcePair
        {
            uint Users;
            ResourceType* Resource;
        };
        
        typedef std::unordered_map<std::string, std::unordered_map<std::string, ResourcePair>> ResourceMap;
        
    private:
		ResourceFactory();
		virtual ~ResourceFactory();
        
        template<typename T>
        Resource<T> load(const std::string& src);
        
        void noUsersCallback(ResourceType* resource);
        
        ResourceMap resources;
	};
    
    #include "ResourceFactory.inl"
}

#endif // KKE_RESOURCEFACTORY_HPP