/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

/* class SomeSingleton : public Singleton<SomeSingleton>
 * friend class Singleton<SomeSingleton>;
 * 
 * Friend is required in order to keep CTOR and DTOR private.
 */

#ifndef KKE_SINGLETON_HPP
#define KKE_SINGLETON_HPP

#include "Interface/INoncopyable.hpp"

namespace kke
{
    template<typename T>
    /**
	 * @brief Singleton of type T. friend is required in order to keep CTOR and DTOR private.
	 **/
	class Singleton : public INoncopyable
	{ 
    public:
		/**
		 * @brief Returns an instance of T.
		 *
		 * @return T&
		 **/
		static T& Instance();
		
		/**
		 * @brief Forces release of resources. Will do nothing if already terminated.
		 *
		 * @return void
		 **/
		static void Terminate();
        
        /**
         * @brief Explicitly Initialize instance.
         *
         * @return void
         **/
        static void Initialize();
        
    protected:
        Singleton();
        ~Singleton();
        
    protected:
        static T* instance;
	};
    
    #include "Singleton.inl"
}
	
#endif // KKE_SINGLETON_HPP